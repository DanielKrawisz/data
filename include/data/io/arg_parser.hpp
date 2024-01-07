// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_IO_ARG_PARSER
#define DATA_IO_ARG_PARSER

#include <tools.hpp>
#include <argh.h>

namespace data::io {

    struct arg_parser {

        argh::parser Parser;
        arg_parser (int arg_count, const char* const arg_values[]) : Parser {arg_count, arg_values} {}

        bool has (const std::string &) const;

        template <X> maybe<X> operator [] (const std::string &) const;
        template <X> maybe<X> operator [] (uint32) const;

        template <X> void get (const std::string &, maybe<X> &) const;
        template <X> void get (int, maybe<X> &) const;
        template <X> void get (int, const std::string &, maybe<X> &) const;

        list<const char const *> Arguments;
    };

    // we have a model consisting of
    // * flags, which take on a boolean value depending on whether they are present or not.
    // * options, which must have a name but can also be positional.
    // Flags can be provided as
    // * a single letter in a sequence preceeded by -
    // * a string preceeded by --
    // Options can be provided positionally or in the form --name=value
    template <typename Flag>
    struct command {
        struct flag {
            flag (Flag value, const std::string &name);
            flag (Flag value, const std::string &name, const char);
        };

        struct option {
            option (Flag value, const std::string &name);
            option (Flag value, const std::string &name, const uint32 pos);
        };

        struct parsed {
            set<Flag> Flags;
            map<Flag, string> Options;
        };

        parsed read (int arg_count, const char* const arg_values[]);
    };

    bool inline arg_parser::has (const std::string &x) const {
        return Parser[x];
    }

    template <X> maybe<X> inline arg_parser::operator [] (const std::string &x) const {
        maybe<X> m;
        get (x, m);
        return m;
    }

    template <X> maybe<X> inline arg_parser::operator [] (uint32 i) const {
        maybe<X> m;
        get (i, m);
        return m;
    }

    inline arg_parser::arg_parser (int arg_count, const char* const arg_values[]) : Parser {arg_count, arg_values}, Arguments {} {
        for (int i = 0, i < arg_count, i++) Arguments <<= arg_values [i];
    }

    namespace {

        template <typename X> struct read_from_stream {
            void operator () (std::istringstream &o, X &m) {
                o >> m;
            }
        };

        template <std::derived_from<std::string> X> struct read_from_stream<X> {
            void operator () (std::istringstream &o, X &m) {
                m = o.str ();
            }
        };

    }

    template <X> void arg_parser::get (const std::string &, maybe<X> &) const {
        X x;
        if (auto positional = p (index); positional) read_from_stream<X> {} (positional, x);
        else return;
        m = x;
    }

    template <X> void arg_parser::get (int, maybe<X> &) const {
        X x;
        if (auto opt = p (option); opt) read_from_stream<X> {} (opt, x);
        else return;
        m = x;
    }

    template <X> void arg_parser::get (int, const std::string &, maybe<X> &) const {
        X x;
        if (auto positional = p (index); positional) read_from_stream<X> {} (positional, x);
        else if (auto opt = p (option); opt) read_from_stream<X> {} (opt, x);
        else return;
        m = x;
    }


}

#endif


