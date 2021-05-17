// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_HALVES
#define DATA_HALVES

#include <data/encoding/endian.hpp>

namespace data {
    
    template <typename X> struct digits;
    
    template <> struct digits<uint64> {
        constexpr static bool is_signed = false;
        constexpr static size_t value = 8;
    };
    
    template <> struct digits<uint32> {
        constexpr static bool is_signed = false;
        constexpr static size_t value = 4;
    };
    
    template <> struct digits<uint16> {
        constexpr static bool is_signed = false;
        constexpr static size_t value = 2;
    };
    
    template <> struct digits<int64> {
        constexpr static bool is_signed = true;
        constexpr static size_t value = 8;
    };
    
    template <> struct digits<int32> {
        constexpr static bool is_signed = true;
        constexpr static size_t value = 4;
    };
    
    template <> struct digits<int16> {
        constexpr static bool is_signed = true;
        constexpr static size_t value = 2;
    };
    
    template <> struct digits<byte> {
        constexpr static bool is_signed = false;
        constexpr static size_t value = 1;
    };
    
    template <> struct digits<char> {
        constexpr static bool is_signed = true;
        constexpr static size_t value = 1;
    };
    
    template <endian::order o, bool z, size_t size> struct digits<endian::arithmetic<o, z, size>> {
        constexpr static bool is_signed = z;
        constexpr static size_t value = size;
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
        static type greater(uint64 u) {
            return u >> digits<type>::value;
        }
        
        static type lesser(uint64 u) {
            return u;
        }
    };
    
    template <> struct half_of<int64> {
        using type = int32;
        static type greater(int64 u) {
            return u >> digits<type>::value;
        }
        
        static type lesser(int64 u) {
            return u;
        };
    };
    
    template <> struct half_of<uint32> {
        using type = uint16;
        static type greater(uint32 u) {
            return u >> digits<type>::value;
        }
        
        static type lesser(uint32 u) {
            return u;
        }
    };
    
    template <> struct half_of<int32> {
        using type = int16;
        static type greater(int32 u) {
            return u >> digits<type>::value;
        }
        
        static type lesser(int32 u) {
            return u;
        }
    };
    
    template <> struct half_of<uint16> {
        using type = byte;
        static type greater(uint16 u) {
            return u >> digits<type>::value;
        }
        
        static type lesser(uint16 u) {
            return u;
        }
    };
    
    template <> struct half_of<int16> {
        using type = char;
        static type greater(int16 u) {
            return u >> digits<type>::value;
        }
        
        static type lesser(int16 u) {
            return u;
        }
    };
    
    template <endian::order o, bool is_signed, size_t size> struct half_of<endian::arithmetic<o, is_signed, size>> {
        using type = endian::arithmetic<o, is_signed, digits<typename half_of<endian::to_native<is_signed, size>>::type>::value>;
        static type greater(endian::arithmetic<o, is_signed, size> u) {
            return type{half_of<endian::to_native<is_signed, size>>::greater((endian::to_native<is_signed, size>)(u))};
        }
        
        static type lesser(endian::arithmetic<o, is_signed, size> u) {
            return type{half_of<endian::to_native<is_signed, size>>::lesser((endian::to_native<is_signed, size>)(u))};
        }
    };
    
    template<typename whole>
    typename half_of<whole>::type greater_half(whole w) {
        return half_of<whole>::greater(w);
    };
    
    template<typename whole>
    typename half_of<whole>::type lesser_half(whole w) {
        return half_of<whole>::lesser(w);
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
    
    template <endian::order o, bool is_signed, size_t size> struct twice<endian::arithmetic<o, is_signed, size>> {
        using type = endian::arithmetic<o, is_signed, 2 * size>;
        static type extend(endian::arithmetic<o, is_signed, size> x) {
            return (typename twice<endian::to_native<is_signed, size>>::type)(x);
        }
    };
    
    template<typename half>
    typename twice<half>::type combine(half greater, half lesser) {
        return ((typename twice<half>::type)(greater) << digits<half>::value) + lesser;
    };
    
    template<typename half>
    typename twice<half>::type mul(half a, half b) {
        return (typename twice<half>::type)(a) * (typename twice<half>::type)(b);
    };
    
    template<typename half>
    typename twice<half>::type add(half a, half b, half r) {
        return (typename twice<half>::type)(a) + b + r;
    };
    
}

#endif
