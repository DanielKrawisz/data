// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENDIAN
#define DATA_ENDIAN

#include <data/types.hpp>
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
    
    template <bool is_signed, std::size_t bytes> struct native_representation;
    
    template <bool is_signed, std::size_t bytes> using to_native = typename native_representation<is_signed, bytes>::type;
    
    template <> struct native_representation<true, 1>{
        using type = int_least8_t;
    };
    
    template <> struct native_representation<true, 2>{
        using type = int_least16_t;
    };
    
    template <> struct native_representation<true, 3> {
        using type = int_least32_t;
    };
    
    template <> struct native_representation<true, 4> {
        using type = int_least32_t;
    };
    
    template <> struct native_representation<true, 5> {
        using type = int_least64_t;
    };
    
    template <> struct native_representation<true, 6> {
        using type = int_least64_t;
    };
    
    template <> struct native_representation<true, 7> {
        using type = int_least64_t;
    };
    
    template <> struct native_representation<true, 8> {
        using type = int_least64_t;
    };
    
    template <> struct native_representation<false, 1>{
        using type = uint_least8_t;
    };
    
    template <> struct native_representation<false, 2>{
        using type = uint_least16_t;
    };
    
    template <> struct native_representation<false, 3> {
        using type = uint_least32_t;
    };
    
    template <> struct native_representation<false, 4> {
        using type = uint_least32_t;
    };
    
    template <> struct native_representation<false, 5> {
        using type = uint_least64_t;
    };
    
    template <> struct native_representation<false, 6> {
        using type = uint_least64_t;
    };
    
    template <> struct native_representation<false, 7> {
        using type = uint_least64_t;
    };
    
    template <> struct native_representation<false, 8> {
        using type = uint_least64_t;
    };
    
    template <boost::endian::order Order, bool is_signed, std::size_t bytes>
    struct arithmetic : boost::endian::endian_arithmetic<Order, to_native<is_signed, bytes>, 8 * bytes, boost::endian::align::no> {
        
        using native_type = to_native<is_signed, bytes>;
        using boost_arith = boost::endian::endian_arithmetic<Order, native_type, 8 * bytes, boost::endian::align::no>;
        using boost_arith::data;
        using boost_arith::boost_arith;
        using iterator = byte*;
        
        arithmetic(const boost_arith& x) : boost_arith(x) {}
        
        iterator begin() {
            return data();
        }
        
        const iterator begin() const {
            return data();
        }
        
        iterator end() {
            return data() + bytes;
        }
        
        const iterator end() const {
            return data() + bytes;
        }
        
        explicit operator bytes_view() const {
            return bytes_view(data(), bytes);
        }
        
        using opposite_endian = arithmetic<opposite(Order), is_signed, bytes>;
        
        explicit operator opposite_endian() const {
            return opposite_endian{
                boost::endian::endian_arithmetic<Order, native_type, 8 * bytes, boost::endian::align::no>::operator native_type()};
        }
        
        explicit arithmetic(const opposite_endian& x) {
            *this = arithmetic(x);
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

