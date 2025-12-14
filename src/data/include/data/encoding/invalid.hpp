// Copyright (c) 2019 Daniel Krawisz
// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_INVALID
#define DATA_ENCODING_INVALID

#include <exception>
#include <string>

#include <data/io/exception.hpp>
#include <data/types.hpp>

namespace data::encoding {
    class invalid : exception::base<invalid> {
    public:
        invalid (const std::string &format, const std::string &str) {
            *this << "Invalid " << format << " string: " << str;
        }
    };
}

#endif
