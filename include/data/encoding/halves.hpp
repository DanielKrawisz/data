// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_HALVES
#define DATA_HALVES

#include <type_traits>
#include <concepts>
#include <data/encoding/endian/endian.hpp>

namespace data::encoding {
    
    template <typename X> struct count_digits;
    
    template <std::integral X> struct count_digits<X> {
        constexpr static bool is_signed = std::is_signed_v<X>;
        constexpr static size_t value = sizeof(X);
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
    
    template <typename whole> struct half_of;
    
    template <> struct half_of<uint64> {
        using type = uint32;
        static type greater_half(uint64 u) {
            return u >> count_digits<type>::value * 8;
        }
        
        static type lesser_half(uint64 u) {
            return u & halves<type, uint64>::lesser;
        }
    };
    
    template <> struct half_of<int64> {
        using type = int32;
        static type greater_half(int64 u) {
            return u >> count_digits<type>::value * 8;
        }
        
        static type lesser_half(int64 u) {
            return u & halves<type, int64>::lesser;
        };
    };
    
    template <> struct half_of<uint32> {
        using type = uint16;
        static type greater_half(uint32 u) {
            return u >> count_digits<type>::value * 8;
        }
        
        static type lesser_half(uint32 u) {
            return u & halves<type, uint32>::lesser;
        }
    };
    
    template <> struct half_of<int32> {
        using type = int16;
        static type greater_half(int32 u) {
            return u >> count_digits<type>::value * 8;
        }
        
        static type lesser_half(int32 u) {
            return u & halves<type, int32>::lesser;
        }
    };
    
    template <> struct half_of<uint16> {
        using type = byte;
        static type greater_half(uint16 u) {
            return u >> count_digits<type>::value * 8;
        }
        
        static type lesser_half(uint16 u) {
            return u & halves<type, uint16>::lesser;
        }
    };
    
    template <> struct half_of<int16> {
        using type = char;
        static type greater_half(int16 u) {
            return u >> count_digits<type>::value * 8;
        }
        
        static type lesser_half(int16 u) {
            return u;
        }
    };
    
    template <typename whole>
    typename half_of<whole>::type greater_half(whole w) {
        return half_of<whole>::greater_half(w);
    };
    
    template <typename whole>
    typename half_of<whole>::type lesser_half(whole w) {
        return half_of<whole>::lesser_half(w);
    };
    
    template <typename half> struct twice;
    
    template <> struct twice<uint32> {
        using type = uint64;
        static type extend(uint32 x) {
            return (type)(x);
        }
    };
    
    template <> struct twice<int32> {
        using type = int64;
        static type extend(int32 x) {
            return (type)(x);
        }
    };
    
    template <> struct twice<uint16> {
        using type = uint32;
        static type extend(uint16 x) {
            return (type)(x);
        }
    };
    
    template <> struct twice<int16> {
        using type = int32;
        static type extend(int16 x) {
            return (type)(x);
        }
    };
    
    template <> struct twice<byte> {
        using type = uint16;
        static type extend(byte x) {
            return (type)(x);
        }
    };
    
    template <> struct twice<char> {
        using type = int16;
        static type extend(char x) {
            return (type)(x);
        }
    };
    
    template <typename half>
    typename twice<half>::type combine(half greater, half lesser) {
        return ((typename twice<half>::type)(greater) << count_digits<half>::value * 8) + lesser;
    };
    
    template <typename half>
    typename twice<half>::type multiply(half a, half b, half r) {
        return (typename twice<half>::type)(a) * (typename twice<half>::type)(b) + (typename twice<half>::type)(r);
    };
    
    template <typename half>
    typename twice<half>::type add(half a, half b, half r) {
        return (typename twice<half>::type)(a) + b + r;
    };
    
}

#endif
