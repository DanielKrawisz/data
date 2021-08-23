// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ARITHMETIC
#define DATA_ARITHMETIC

#include <data/types.hpp>
#include <data/encoding/endian/endian.hpp>
#include <data/encoding/endian/words.hpp>

namespace data::endian {
    
    template <boost::endian::order Order, bool is_signed, std::size_t bytes>
    struct arithmetic : boost::endian::endian_arithmetic<Order, to_native<is_signed, bytes>, 8 * bytes, boost::endian::align::no> {
        
        using native_type = to_native<is_signed, bytes>;
        using boost_arith = boost::endian::endian_arithmetic<Order, native_type, 8 * bytes, boost::endian::align::no>;
        using boost_arith::data;
        using boost_arith::boost_arith;
        using iterator = byte*;
        using const_iterator = byte const*;
        
        arithmetic(const boost_arith& x) : boost_arith(x) {}
        
        iterator begin() {
            return data();
        }
        
        const_iterator begin() const {
            return data();
        }
        
        iterator end() {
            return data() + bytes;
        }
        
        const_iterator end() const {
            return data() + bytes;
        }
        
        explicit operator bytes_view() const {
            return bytes_view(data(), bytes);
        }
        
        using opposite_endian = arithmetic<opposite(Order), is_signed, bytes>;
        
        explicit operator opposite_endian() const {
            return opposite_endian{native_type()};
        }
        
        explicit arithmetic(const opposite_endian& x) : boost_arith{native_type(x)} {}
        
        size_t size() const {
            return bytes;
        }
    };
    
}

namespace data {

  // big endian signed integer unaligned types
  typedef endian::arithmetic<endian::big, true, 1>           int8_big;
  typedef endian::arithmetic<endian::big, true, 2>          int16_big;
  typedef endian::arithmetic<endian::big, true, 3>          int24_big;
  typedef endian::arithmetic<endian::big, true, 4>          int32_big;
  typedef endian::arithmetic<endian::big, true, 5>          int40_big;
  typedef endian::arithmetic<endian::big, true, 6>          int48_big;
  typedef endian::arithmetic<endian::big, true, 7>          int56_big;
  typedef endian::arithmetic<endian::big, true, 8>          int64_big;

  // big endian unsigned integer unaligned types
  typedef endian::arithmetic<endian::big, false, 1>         uint8_big;
  typedef endian::arithmetic<endian::big, false, 2>        uint16_big;
  typedef endian::arithmetic<endian::big, false, 3>        uint24_big;
  typedef endian::arithmetic<endian::big, false, 4>        uint32_big;
  typedef endian::arithmetic<endian::big, false, 5>        uint40_big;
  typedef endian::arithmetic<endian::big, false, 6>        uint48_big;
  typedef endian::arithmetic<endian::big, false, 7>        uint56_big;
  typedef endian::arithmetic<endian::big, false, 8>        uint64_big;

  // little endian signed integer unaligned types
  typedef endian::arithmetic<endian::little, true, 1>     int8_little;
  typedef endian::arithmetic<endian::little, true, 2>    int16_little;
  typedef endian::arithmetic<endian::little, true, 3>    int24_little;
  typedef endian::arithmetic<endian::little, true, 4>    int32_little;
  typedef endian::arithmetic<endian::little, true, 5>    int40_little;
  typedef endian::arithmetic<endian::little, true, 6>    int48_little;
  typedef endian::arithmetic<endian::little, true, 7>    int56_little;
  typedef endian::arithmetic<endian::little, true, 8>    int64_little;

  // little endian unsigned integer unaligned types
  typedef endian::arithmetic<endian::little, false, 1>   uint8_little;
  typedef endian::arithmetic<endian::little, false, 2>  uint16_little;
  typedef endian::arithmetic<endian::little, false, 3>  uint24_little;
  typedef endian::arithmetic<endian::little, false, 4>  uint32_little;
  typedef endian::arithmetic<endian::little, false, 5>  uint40_little;
  typedef endian::arithmetic<endian::little, false, 6>  uint48_little;
  typedef endian::arithmetic<endian::little, false, 7>  uint56_little;
  typedef endian::arithmetic<endian::little, false, 8>  uint64_little;
}

#endif


