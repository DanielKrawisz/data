// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_ASCII
#define DATA_ENCODING_ASCII

#include <string>
#include <data/iterable.hpp>

namespace data::encoding::ascii {
    const byte max = 0x7f;

    using character = byte;

    inline bool valid(character b) {
        return b <= max && b >= 0;
    }

    inline bool valid(char c) {
        return c <= max && c >= 0;
    }


    class string {
        std::string const* innerString= nullptr;

    public:
        string(std::string const* input);
        bool valid() const {
            return innerString != nullptr;
        }

        explicit operator std::string() {
            return *innerString ;
        }

        explicit operator bytes();

    };
}

#endif 
