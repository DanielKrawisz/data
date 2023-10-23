// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_STRING
#define DATA_STRING

#include "types.hpp"

namespace data {

    // the only difference between data::string and std::string is that
    // data::string prints with "" around it.
    struct string : std::string {
        using std::string::string;

        string (const std::string &x) : std::string {x} {}
        string (std::string &&x) : std::string {x} {}
    };

    string drop (const std::string &, int64 begin);
    string take (const std::string &, int64 end);

    string substring (const std::string &, int64 begin);
    string substring (const std::string &, int64 begin, int64 end);

    std::ostream inline &operator << (std::ostream &o, const string &x) {
        return o << "\"" << static_cast<std::string> (x) << "\"";
    }

    string inline drop (const std::string &x, int64 begin) {
        return substring (x, begin);
    }

    string inline take (const std::string &x, int64 end) {
        return substring (x, 0, end);
    }

    string inline substring (const std::string &x, int64 begin) {
        return substring (x, begin, x.size ());
    }

}

#endif

