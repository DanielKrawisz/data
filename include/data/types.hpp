// Copyright (c) 2019 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_TYPES
#define DATA_TYPES

#include <vector>
#include <array>
#include <memory>
#include <string>
#include <stdint.h>
#include <boost/endian/arithmetic.hpp>

namespace data {
    
    template<typename X>
    using vector = std::vector<X>;

    using byte = uint8_t;
    using uint16 = uint16_t;
    using uint32 = uint32_t;
    using uint64 = uint64_t;
    using int8 = int8_t ;
    using int16 = int16_t;
    using int32 = int32_t;
    using int64 = int64_t;
    
    using bytes = std::basic_string<byte>;

    template<typename X>
    using ptr = std::shared_ptr<X>;

    using string = std::string;
    using string_view = std::string_view;
    
    using bytes_view = std::basic_string_view<byte>;
    
    typedef boost::endian::big_uint8_t uint8_big;
    typedef boost::endian::big_uint16_t uint16_big;
    typedef boost::endian::big_uint24_t uint24_big;
    typedef boost::endian::big_uint32_t uint32_big;
    typedef boost::endian::big_uint40_t uint40_big;
    typedef boost::endian::big_uint48_t uint48_big;
    typedef boost::endian::big_uint56_t uint56_big;
    typedef boost::endian::big_uint64_t uint64_big;

    typedef boost::endian::big_int8_t int8_big;
    typedef boost::endian::big_int16_t int16_big;
    typedef boost::endian::big_int24_t int24_big;
    typedef boost::endian::big_int32_t int32_big;
    typedef boost::endian::big_int40_t int40_big;
    typedef boost::endian::big_int48_t int48_big;
    typedef boost::endian::big_int56_t int56_big;
    typedef boost::endian::big_int64_t int64_big;

    typedef boost::endian::little_uint8_t uint8_little;
    typedef boost::endian::little_uint16_t uint16_little;
    typedef boost::endian::little_uint24_t uint24_little;
    typedef boost::endian::little_uint32_t uint32_little;
    typedef boost::endian::little_uint40_t uint40_little;
    typedef boost::endian::little_uint48_t uint48_little;
    typedef boost::endian::little_uint56_t uint56_little;
    typedef boost::endian::little_uint64_t uint64_little;

    typedef boost::endian::little_int8_t int8_little;
    typedef boost::endian::little_int16_t int16_little;
    typedef boost::endian::little_int24_t int24_little;
    typedef boost::endian::little_int32_t int32_little;
    typedef boost::endian::little_int40_t int40_little;
    typedef boost::endian::little_int48_t int48_little;
    typedef boost::endian::little_int56_t int56_little;
    typedef boost::endian::little_int64_t int64_little;
}

#endif
