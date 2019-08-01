// Copyright (c) 2019 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_TYPES
#define DATA_TYPES

#include <vector>
#include <array>
#include <memory>
#include <string>
#include <stdint.h>

namespace data {
    
    template<typename X>
    using vector = const std::vector<X>;

    using byte = uint8_t;
    using uint16 = uint16_t;
    using uint32 = uint32_t;
    using uint64 = uint64_t;
    using int8 = int8_t ;
    using int16 = int16_t;
    using int32 = int32_t;
    using int64 = int64_t;
    
    using bytes = const vector<byte>;

    template<typename X>
    using ptr = std::shared_ptr<X>;

    using string = const std::string;
    
    template <typename X, typename Y>
    inline X plus(X a, Y b) {
        return a + b;
    }

}

#endif
