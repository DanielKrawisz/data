// Copyright (c) 2019 Daniel Krawisz
// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_INVALID
#define DATA_ENCODING_INVALID

#include <exception>
#include <string>

#include <data/types.hpp>

namespace data::encoding {
    struct invalid : std::runtime_error {
        invalid (const std::string &format, const std::string &msg) : 
            std::runtime_error {std::string {"Invalid "} + format + " string: " + msg} {}
    };
}

#endif
