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
#include <data/encoding/read.hpp>
#include <argh.h>

namespace data::io {

    struct arg_parser {

        argh::parser Parser;
        arg_parser (int arg_count, const char *const arg_values[]);

        bool has (const std::string &) const;

        template <typename X> void get (const std::string &, maybe<X> &) const;
        template <typename X> void get (uint32, maybe<X> &) const;
        template <typename X> void get (uint32, const std::string &, maybe<X> &) const;

        list<char const * const> Arguments;
    };

    inline arg_parser::arg_parser (int arg_count, const char* const arg_values[]) : Parser {arg_count, arg_values}, Arguments {} {
        for (int i = 0; i < arg_count; i++) Arguments <<= arg_values [i];
    }

    bool inline arg_parser::has (const std::string &x) const {
        return Parser[x];
    }

    template <typename X> void inline arg_parser::get (const std::string &option, maybe<X> &m) const {
        if (auto opt = Parser (option); opt) m = encoding::read<X> {} (opt);
        return;
    }

    template <typename X> void inline arg_parser::get (uint32 index, maybe<X> &m) const {
        if (auto positional = Parser (index); positional) m = encoding::read<X> {} (positional);
        return;
    }

    template <typename X> void inline arg_parser::get (uint32 index, const std::string &option, maybe<X> &m) const {
        if (auto positional = Parser (index); positional) m = encoding::read<X> {} (positional);
        else if (auto opt = Parser (option); opt) m = encoding::read<X> {} (opt);
    }


}

#endif


