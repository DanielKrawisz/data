// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_ASCII
#define DATA_ENCODING_ASCII

#include <string>
#include <data/cross.hpp>

namespace data::encoding::ascii {
    const std::string Format{"ascii"};
    
    const byte max = 0x7f;

    using character = byte;

    bool inline valid(character b) {
        return b <= max && b >= 0;
    }

    bool inline valid(char c) {
        return c <= max && c >= 0;
    }
    
    bool inline valid(const std::string &x) {
        for (char c : x) if (!valid(c)) return false;
        return true;
    }

    struct string : std::string {
        using std::string::string;
        
        string(const std::string& x) : std::string{x} {}
        
        bool valid() const {
            return ascii::valid(*this);
        }
        
        explicit operator bytes() const;

    };
}

#endif 
