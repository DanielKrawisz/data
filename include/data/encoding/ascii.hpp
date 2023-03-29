// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_ASCII
#define DATA_ENCODING_ASCII

#include <string>
#include <data/cross.hpp>
#include <data/encoding/unicode.hpp>

namespace data::encoding::ASCII {
    const std::string Format {"ascii"};
    
    const byte max = 0x7f;

    bool inline valid (char c) {
        return c <= max && c >= 0;
    }
    
    bool inline valid (string_view x) {
        for (char c : x) if (!valid (c)) return false;
        return true;
    }

    struct string : std::string {
        using std::string::string;
        
        string (const std::string &x) : std::string {x} {}
        string (std::string &&x) : std::string {x} {}
        
        bool valid () const {
            return ASCII::valid (*this);
        }

        explicit operator bytes () const;

        explicit operator data::UTF8 () const;

    };
    
    // for emails
    string encode (const bytes_view x);
    bytes decode (const string &);
}

namespace data {
    using ASCII = encoding::ASCII::string;
}

#endif 
