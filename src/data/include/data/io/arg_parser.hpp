// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_IO_ARG_PARSER
#define DATA_IO_ARG_PARSER

/*
------------------------------------------------------------------------------
 Command-line argument parsing: design, scope, and rationale
------------------------------------------------------------------------------

Overview
--------

This module provides a *minimal, deterministic* command-line argument parser.
It does not attempt to support every command-line convention in existence
because it is impossible to separate syntax from meaning in command-line
arguments in general. No argument parser can satisfy everybody. Instead we
define a particular format that ought to be ok unless you are very particular
and we leave interpretation and validation to higher layers


Argument model
--------------

The command line is received exactly as provided to
`main(int argc, char** argv)`. Each argument is treated as an opaque string.

Arguments are classified into three categories:

  1. Flags
  2. Options
  3. Positional arguments

The program name (`argv[0]`) is always included and is treated as positional
argument at index 0.


Grammar
-------

The accepted grammar is intentionally simple and unambiguous:

  * Long flag:
        --name
    where `name` matches [A-Za-z][A-Za-z0-9_-]*

  * Long option:
        --name=value
    where `name` matches [A-Za-z][A-Za-z0-9_-]* and `value` is any string

  * Abbreviated flags:
        -abcd
    equivalent to the individual flags `-a`, `-b`, `-c`, `-d`
    (no values are permitted in abbreviated form)

  * Positional argument:
        anything that does not match one of the above forms

No other forms are recognized. In particular:

  * `--name value` is not a key/value pair. Instead it would be interpreted
    as a flag called `name` and a positional argument `value`.
  * `-a=value` is not an option. Again, this would be treated as a
    positional argument.
  * other unrecognized forms are treated as positional arguments

This keeps lexing independent of meaning or schema.


Uniqueness and repetition
-------------------------

Each flag or option name may appear *at most once*.

  * Repeated flags are not allowed
  * Repeated options are not allowed
  * Mixed forms (e.g. `-a` and `--a`) are considered the same name and are not
    allowed together

This restriction aligns with the zero-copy design of the parser (arguments and
values are stored as `string_view` into argv) and avoids policy decisions about
overrides, ordering, or value equality. Programs that require repetition or
aggregation should implement that behavior at a higher semantic layer.


Data representation
-------------------

Parsed arguments are stored in an `io::args::parsed` object, constructed as:

    io::args::parsed args (argc, argv);

Internally, the parsed representation consists of:

  * Flags
        A set of flag names that were present.

  * Options
        A map from option name to its associated value.

  * Arguments
        A vector of positional arguments, in order.
        Index 0 always contains the program name.


Interface
---------

The primary interface exposed to users of this parser consists of:

  * `bool has (std::string_view name) const`
        Returns true if the given flag or option name is present.

  * `get<X> (std::string_view key, maybe<X> value)`
        Search options for key, try to read as an X.
  * `get<X> (size_t index, maybe<X> value)`
        Search for an argument at position index and try to read as an X.

Value interpretation is performed via:

    encoding::read<X> {} (string_view)

This function is responsible for converting a string into type `X` and is
expected to throw on failure. The argument parser itself does not perform type
checking or semantic validation.


Separation of concerns
----------------------

This module performs *lexical classification only*.

It does not:
  * infer user intent
  * validate argument combinations
  * enforce schemas
  * assign meaning to positional arguments
  * interpret quoting or escaping

Those concerns are intentionally deferred to higher-level schema and validation
systems. This keeps the parser predictable, testable, and compatible with
existing programs while providing a solid foundation for future extensions.

------------------------------------------------------------------------------
*/

#include <data/slice.hpp>
#include <data/tools/schema.hpp>
#include <regex>

namespace data::io::args {

    struct parsed {
        parsed (int arg_count, const char *const arg_value[]);

        set<string_view> Flags;
        map<string_view, string_view> Options;
        cross<string_view> Arguments;

        bool has (const std::string &) const;

        template <typename X> void get (const std::string &, maybe<X> &) const;
        template <typename X> void get (size_t, maybe<X> &) const;
    };

    template <typename arg_rule, typename opt_rule>
    struct command {
        set<std::string> Flags;
        schema::rule::list<arg_rule> Arguments;
        schema::rule::map<opt_rule> Options;

        command (
            set<std::string> flags,
            const schema::rule::list<arg_rule> &args,
            const schema::rule::map<opt_rule> &opts):
            Flags {flags}, Arguments {args}, Options {opts} {}
    };

    template<typename arg_rule, typename opt_rule>
    command (set<std::string> flags, const schema::rule::list<arg_rule> &args, const schema::rule::map<opt_rule> &opts) -> command<arg_rule, opt_rule>;

    template <typename arg_rule, typename opt_rule>
    maybe<tuple<map<std::string, bool>,
        typename schema::rule::validate<arg_rule>::result,
        typename schema::rule::validate<opt_rule>::result>>
    validate (const parsed &, const command<arg_rule, opt_rule> &);

    bool inline parsed::has (const std::string &x) const {
        return contains (Flags, x);
    }

    template <typename X> void inline parsed::get (const std::string &z, maybe<X> &x) const {
        auto v = contains (Options, z);
        if (v) x = encoding::read<X> {} (*v);
        else x = {};
    }

    template <typename X> void inline parsed::get (size_t z, maybe<X> &x) const {
        if (Arguments.size () > z) x = encoding::read<X> {} (Arguments[z]);
        else x = {};
    }

    template <typename arg_rule, typename opt_rule>
    maybe<tuple<map<std::string, bool>,
        typename schema::rule::validate<arg_rule>::result,
        typename schema::rule::validate<opt_rule>::result>>
    validate (const parsed &p, const command<arg_rule, opt_rule> &c) {

        // TODO here we convert key to string in order to fit it into the contains
        // function. However, it should be possible to say that contains takes
        // anything equality comparable with contained type.
        for (const auto &key : p.Flags) if (!c.Flags.contains (std::string (key))) return {};
        map<std::string, bool> flags;

        for (const auto &key : c.Flags) flags = flags.insert (key, bool (p.Flags.contains (key)));

        try {
            return {{flags, schema::validate (p.Arguments, c.Arguments), schema::validate (p.Options, c.Options)}};
        } catch (const schema::mismatch) {
            return {};
        }
    }
}

#endif


