// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENDIAN
#define DATA_ENDIAN

#include <data/types.hpp>
#include <boost/endian/buffers.hpp>
#include <boost/endian/conversion.hpp>

namespace data::endian {
    
    using order = boost::endian::order;
    constexpr order big = boost::endian::order::big;
    constexpr order little = boost::endian::order::little;
    
    constexpr inline order opposite(order o) {
        return o == big ? little : big;
    } 
    
    // convert native to and from the given endian ordering. 
    template <typename X, order> struct native;
    
    template <typename X> struct native<X, big> {
        static X from(const X x) {
            return boost::endian::native_to_big<X>(x);
        }
        
        static X to(const X x) {
            return boost::endian::big_to_native<X>(x);
        }
    };
    
    template <typename X> struct native<X, little> {
        static X from(const X x) {
            return boost::endian::native_to_little<X>(x);
        }
        
        static X to(const X x) {
            return boost::endian::little_to_native<X>(x);
        }
    };
    
    template <boost::endian::order Order, class T, std::size_t n_bits>
    struct arithmetic : boost::endian::endian_arithmetic<Order, T, n_bits, boost::endian::align::no> {
        using boost_arith = boost::endian::endian_arithmetic<Order, T, n_bits, boost::endian::align::no>;
        using boost_arith::data;
        using boost_arith::boost_arith;
        using iterator = std::vector<byte>::iterator;
        
        arithmetic(const boost_arith& x) : boost_arith(x) {}
        
        iterator begin() {
            return iterator(data());
        }
        
        const iterator begin() const {
            return iterator(data());
        }
        
        iterator end() {
            return iterator(data() + n_bits);
        }
        
        const iterator end() const {
            return iterator(data() + n_bits);
        }
        
        explicit operator bytes_view() const {
            return bytes_view(data(), n_bits / 8);
        }
    };
    
}

namespace data {

  // big endian signed integer unaligned types
  typedef endian::arithmetic<endian::big, int_least8_t, 8>        int8_big;
  typedef endian::arithmetic<endian::big, int_least16_t, 16>      int16_big;
  typedef endian::arithmetic<endian::big, int_least32_t, 24>      int24_big;
  typedef endian::arithmetic<endian::big, int_least32_t, 32>      int32_big;
  typedef endian::arithmetic<endian::big, int_least64_t, 40>      int40_big;
  typedef endian::arithmetic<endian::big, int_least64_t, 48>      int48_big;
  typedef endian::arithmetic<endian::big, int_least64_t, 56>      int56_big;
  typedef endian::arithmetic<endian::big, int_least64_t, 64>      int64_big;

  // big endian unsigned integer unaligned types
  typedef endian::arithmetic<endian::big, uint_least8_t, 8>       uint8_big;
  typedef endian::arithmetic<endian::big, uint_least16_t, 16>     uint16_big;
  typedef endian::arithmetic<endian::big, uint_least32_t, 24>     uint24_big;
  typedef endian::arithmetic<endian::big, uint_least32_t, 32>     uint32_big;
  typedef endian::arithmetic<endian::big, uint_least64_t, 40>     uint40_big;
  typedef endian::arithmetic<endian::big, uint_least64_t, 48>     uint48_big;
  typedef endian::arithmetic<endian::big, uint_least64_t, 56>     uint56_big;
  typedef endian::arithmetic<endian::big, uint_least64_t, 64>     uint64_big;

  // little endian signed integer unaligned types
  typedef endian::arithmetic<endian::little, int_least8_t, 8>     int8_little;
  typedef endian::arithmetic<endian::little, int_least16_t, 16>   int16_little;
  typedef endian::arithmetic<endian::little, int_least32_t, 24>   int24_little;
  typedef endian::arithmetic<endian::little, int_least32_t, 32>   int32_little;
  typedef endian::arithmetic<endian::little, int_least64_t, 40>   int40_little;
  typedef endian::arithmetic<endian::little, int_least64_t, 48>   int48_little;
  typedef endian::arithmetic<endian::little, int_least64_t, 56>   int56_little;
  typedef endian::arithmetic<endian::little, int_least64_t, 64>   int64_little;

  // little endian unsigned integer unaligned types
  typedef endian::arithmetic<endian::little, uint_least8_t, 8>    uint8_little;
  typedef endian::arithmetic<endian::little, uint_least16_t, 16>  uint16_little;
  typedef endian::arithmetic<endian::little, uint_least32_t, 24>  uint24_little;
  typedef endian::arithmetic<endian::little, uint_least32_t, 32>  uint32_little;
  typedef endian::arithmetic<endian::little, uint_least64_t, 40>  uint40_little;
  typedef endian::arithmetic<endian::little, uint_least64_t, 48>  uint48_little;
  typedef endian::arithmetic<endian::little, uint_least64_t, 56>  uint56_little;
  typedef endian::arithmetic<endian::little, uint_least64_t, 64>  uint64_little;
};

#endif

