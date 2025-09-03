// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_IO_ARG_PARSER
#define DATA_IO_ARG_PARSER

#include <data/maybe.hpp>
#include <data/list.hpp>
#include <data/set.hpp>
#include <data/map.hpp>
#include <data/string.hpp>
#include <argh.h>

namespace data::io {

    struct arg_parser {

        argh::parser Parser;
        arg_parser (int arg_count, const char *const arg_values[]);

        bool has (const std::string &) const;

        template <typename X> maybe<X> operator [] (const std::string &) const;
        template <typename X> maybe<X> operator [] (uint32) const;

        template <typename X> void get (const std::string &, maybe<X> &) const;
        template <typename X> void get (uint32, maybe<X> &) const;
        template <typename X> void get (uint32, const std::string &, maybe<X> &) const;

        template <typename X> maybe<X> get (uint32, const std::string &) const;

        list<char const * const> Arguments;
    };

    // we have a model consisting of
    // * flags, which take on a boolean value depending on whether they are present or not.
    // * options, which must have a name but can also be positional.
    // Flags can be provided as
    // * a single letter in a sequence preceeded by -
    // * a string preceeded by --
    // Options can be provided positionally or in the form --name=value
    template <typename K>
    struct command {
        struct flag {
            flag (K key, const std::string &name);
            flag (K key, const std::string &name, const char);

            bool operator () (const arg_parser &) const;

            K Key;
            std::string Name;
            maybe<char> Short;
        };

        struct option {
            option (K key, const std::string &name);

            maybe<std::string> operator () (const arg_parser &) const;

            K Key;
            std::string Name;
            maybe<uint32> Position;
        };

        struct parsed {
            set<K> Keys;
            map<K, string> Options;
            parsed (): Keys {}, Options {} {}
        };

        command (list<option> positional, list<option> options, list<flag> flags);

        parsed read (int arg_count, const char* const arg_values[]);

        list<flag> Flags;
        list<option> Options;
    };

    template <typename K> inline command<K>::flag::flag (K key, const std::string &name): Key {key}, Name {name}, Short {} {}
    template <typename K> inline command<K>::flag::flag (K key, const std::string &name, const char u): Key {key}, Name {name}, Short {u} {}
    template <typename K> inline command<K>::option::option (K key, const std::string &name): Key {key}, Name {name} {}

    template <typename K> command<K>::command (list<option> positional, list<option> options, list<flag> flags) : Flags {flags} {
        uint32 i = 1;
        for (const option &o : positional) {
            option opt = o;
            opt.Position = i;
            Options <<= opt;
            i++;
        }

        Options = Options + options;
    }

    template <typename K> command<K>::parsed command<K>::read (int arg_count, const char *const arg_values[]) {
        arg_parser p {arg_count, arg_values};
        parsed r {};
        for (const option &o : Options) {
            maybe<std::string> val = o (p);
            if (val) r.Options = r.Options.insert (o.Key, *val);
        }

        for (const flag &f : Flags) if (f (p)) r.Keys = r.Keys.insert (f.Key);
    }

    template <typename K>
    bool inline command<K>::flag::operator () (const arg_parser &p) const {
        if (Short) throw exception {} << "I don't know how to do this yet";
        return p.has (Name);
    }

    template <typename K>
    maybe<std::string> inline command<K>::option::option::operator () (const arg_parser &p) const {
        maybe<std::string> x;
        if (Position) p.get<std::string> (*Position, Name, x);
        return x;
    }

    bool inline arg_parser::has (const std::string &x) const {
        return Parser[x];
    }

    template <typename X> maybe<X> inline arg_parser::operator [] (const std::string &x) const {
        maybe<X> m;
        get (x, m);
        return m;
    }

    template <typename X> maybe<X> inline arg_parser::operator [] (uint32 i) const {
        maybe<X> m;
        get (i, m);
        return m;
    }

    template <typename X> maybe<X> inline arg_parser::get (uint32 i, const std::string &x) const {
        maybe<X> m;
        get (i, x, m);
        return m;
    }

    inline arg_parser::arg_parser (int arg_count, const char* const arg_values[]) : Parser {arg_count, arg_values}, Arguments {} {
        for (int i = 0; i < arg_count; i++) Arguments <<= arg_values [i];
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

    template <typename X> void inline arg_parser::get (const std::string &option, maybe<X> &m) const {
        X x;
        if (auto opt = Parser (option); opt) read_from_stream<X> {} (opt, x);
        else return;
        m = x;
    }

    template <typename X> void inline arg_parser::get (uint32 index, maybe<X> &m) const {
        X x;
        if (auto positional = Parser (index); positional) read_from_stream<X> {} (positional, x);
        else return;
        m = x;
    }

    template <typename X> void inline arg_parser::get (uint32 index, const std::string &option, maybe<X> &m) const {
        X x;
        if (auto positional = Parser (index); positional) read_from_stream<X> {} (positional, x);
        else if (auto opt = Parser (option); opt) read_from_stream<X> {} (opt, x);
        else return;
        m = x;
    }


}

#endif


