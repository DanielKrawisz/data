// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_STRING
#define DATA_STRING

#include <data/tools.hpp>

namespace data {

    list<string> split (const string &str, const string &split) {
        string s = str;
        data::list<std::string> pieces;

        size_t pos;
        while ((pos = s.find (split)) != string::npos) {
            auto token = s.substr (0, pos);
            pieces = pieces << token;
            s.erase (0, pos + split.length ());
        }

        return pieces;
    }
}

#endif
