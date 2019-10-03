// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_HALVES
#define DATA_HALVES

#include <data/encoding/endian.hpp>

namespace data {
    // A number that has a set endian ordering. 
    template <typename X, boost::endian::order> struct ordered;
    
    using order = boost::endian::order;
    constexpr order big = boost::endian::order::big;
    constexpr order little = boost::endian::order::little;
    
    template <typename X> struct digits;
    
    template <> struct digits<uint64> {
        constexpr static bool is_signed = false;
        constexpr static size_t value = 64;
    };
    
    template <> struct digits<uint32> {
        constexpr static bool is_signed = false;
        constexpr static size_t value = 32;
    };
    
    template <> struct digits<uint16> {
        constexpr static bool is_signed = false;
        constexpr static size_t value = 16;
    };
    
    template <> struct digits<int64> {
        constexpr static bool is_signed = false;
        constexpr static size_t value = 64;
    };
    
    template <> struct digits<int32> {
        constexpr static bool is_signed = false;
        constexpr static size_t value = 32;
    };
    
    template <> struct digits<int16> {
        constexpr static bool is_signed = false;
        constexpr static size_t value = 16;
    };
    
    template <typename half, typename whole> struct halves;
    
    template <> struct halves<uint32, uint64> {
        constexpr static uint64 greater = 0xffffffff00000000;
        constexpr static uint64 lesser = 0x00000000ffffffff;
    };
    
    template <> struct halves<int32, int64> {
        constexpr static uint64 greater = 0xffffffff00000000;
        constexpr static uint64 lesser = 0x00000000ffffffff;
    };
    
    template <> struct halves<uint16, uint32> {
        constexpr static uint64 greater = 0xffff0000;
        constexpr static uint64 lesser = 0x0000ffff;
    };
    
    template <> struct halves<int16, int32> {
        constexpr static uint64 greater = 0xffff0000;
        constexpr static uint64 lesser = 0x0000ffff;
    };
    
    template <> struct halves<byte, uint16> {
        constexpr static uint64 greater = 0xff00;
        constexpr static uint64 lesser = 0x00ff;
    };
    
    template <> struct halves<char, int16> {
        constexpr static uint64 greater = 0xff00;
        constexpr static uint64 lesser = 0x00ff;
    };
    
    template <typename whole> struct half_of {
        half_of() = delete;
    };
    
    template <> struct half_of<uint64> {
        using type = uint32;
    };
    
    template <> struct half_of<int64> {
        using type = int32;
    };
    
    template <> struct half_of<uint32> {
        using type = uint16;
    };
    
    template <> struct half_of<int32> {
        using type = int16;
    };
    
    template <> struct half_of<uint16> {
        using type = byte;
    };
    
    template <> struct half_of<int16> {
        using type = char;
    };
    
    template <typename X, endian::order o> struct half_of<ordered<X, o>> {
        using type = ordered<typename half_of<X>::type, o>;
    };
    
    template <typename X, endian::order o> struct halves<typename half_of<ordered<X, o>>::type, ordered<X, o>> {
        constexpr static bool is_signed = halves<typename half_of<X>::type, X>::is_signed;
        constexpr static ordered<X, o> greater = halves<typename half_of<X>::type, X>::greater;
        constexpr static ordered<X, o> lesser = halves<typename half_of<X>::type, X>::lesser;
    };
    
    template<typename whole>
    typename half_of<whole>::type greater(whole w) {
        return w >> digits<typename half_of<whole>::type>::value;
    };
    
    template<typename whole>
    typename half_of<whole>::type lesser(whole w) {
        return (typename half_of<whole>::type)(w);
    };
    
    template <typename half> struct twice;
    
    template <> struct twice<uint32> {
        using type = uint64;
    };
    
    template <> struct twice<int32> {
        using type = int64;
    };
    
    template <> struct twice<uint16> {
        using type = uint32;
    };
    
    template <> struct twice<int16> {
        using type = int32;
    };
    
    template <> struct twice<byte> {
        using type = uint16;
    };
    
    template <> struct twice<char> {
        using type = int16;
    };
    
    template<typename half>
    typename twice<half>::type combine(half greater, half lesser) {
        return ((typename twice<half>::type)(greater) << digits<half>::value) + lesser;
    };
    
}

#endif
