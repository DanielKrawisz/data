// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_HALVES
#define DATA_HALVES

#include <type_traits>
#include <concepts>
#include <data/encoding/endian.hpp>

namespace data::encoding {
    template <std::size_t S> struct int_by_size;
    template <> struct int_by_size<1> { using type = std::int8_t; };
    template <> struct int_by_size<2> { using type = std::int16_t; };
    template <> struct int_by_size<4> { using type = std::int32_t; };
    template <> struct int_by_size<8> { using type = std::int64_t; };

    template <std::size_t S> struct uint_by_size;
    template <> struct uint_by_size<1> { using type = std::uint8_t; };
    template <> struct uint_by_size<2> { using type = std::uint16_t; };
    template <> struct uint_by_size<4> { using type = std::uint32_t; };
    template <> struct uint_by_size<8> { using type = std::uint64_t; };

#if defined(__SIZEOF_INT128__)
    template <> struct uint_by_size<16> { using type = __uint128_t; };
    template <> struct int_by_size<16> { using type = __int128_t; };
#endif
}

namespace data {
    template <typename T> struct half_of;

    template <std::unsigned_integral T>
    struct half_of<T> : encoding::uint_by_size<sizeof (T) / 2> {
        using type = encoding::uint_by_size<sizeof (T) / 2>::type;

        static type greater_half (T u) {
            return u >> sizeof (type) * 8;
        }

        static type lesser_half (T u) {
            return static_cast<T> (u & std::numeric_limits<type>::max ());
        }
    };

    template <std::signed_integral T>
    struct half_of<T> : encoding::int_by_size<sizeof (T) / 2> {
        using type = encoding::int_by_size<sizeof (T) / 2>::type;

        static type greater_half (T u) {
            return u >> sizeof (type) * 8;
        }

        static type lesser_half (T u) {
            return static_cast<T> (u & std::numeric_limits<type>::max ());
        }
    };

#if defined(__SIZEOF_INT128__)

    template <>
    struct half_of<uint128> : encoding::uint_by_size<8> {
        using type = encoding::uint_by_size<8>::type;

        static type greater_half (uint128 u) {
            return u >> sizeof (type) * 8;
        }

        static type lesser_half (uint128 u) {
            return static_cast<uint128> (u & std::numeric_limits<type>::max ());
        }
    };

    template <>
    struct half_of<int128> : encoding::int_by_size<8> {
        using type = encoding::int_by_size<8>::type;

        static type greater_half (int128 u) {
            return u >> sizeof (type) * 8;
        }

        static type lesser_half (int128 u) {
            return static_cast<int128> (u & std::numeric_limits<type>::max ());
        }
    };
#endif

    template<typename T, typename = void>
    struct twice { }; // leave undefined or empty when invalid

    template<std::unsigned_integral T>
    struct twice<T, std::void_t<typename encoding::uint_by_size<sizeof(T) * 2>::type>> {
        using type = typename encoding::uint_by_size<sizeof(T) * 2>::type;
    };

    template<std::signed_integral T>
    struct twice<T, std::void_t<typename encoding::int_by_size<sizeof(T) * 2>::type>> {
        using type = typename encoding::int_by_size<sizeof(T) * 2>::type;
    };

    // Aliases for ease of use
    template<typename T>
    using half_of_t = typename half_of<T>::type;

    template<typename T>
    using twice_t = typename twice<T>::type;

    namespace {
        template<typename, typename = void>
        struct has_twice : std::false_type { };

        template<typename T>
        struct has_twice<T, std::void_t<typename twice<T>::type>> : std::true_type { };
    }
    
    template <std::integral half, std::integral whole>
    requires (sizeof (whole) == sizeof (half) * 2)
    struct halves {
        constexpr static const whole greater = static_cast<whole> (std::numeric_limits<whole>::max () << (sizeof (half) * 8));
        constexpr static const whole lesser = static_cast<whole> (std::numeric_limits<whole>::max () >> (sizeof (half) * 8));
    };
    
    template <typename whole>
    typename half_of<whole>::type greater_half (whole w) {
        return half_of<whole>::greater_half (w);
    };
    
    template <typename whole>
    typename half_of<whole>::type lesser_half (whole w) {
        return half_of<whole>::lesser_half (w);
    };
    
    template <typename half>
    typename twice<half>::type inline combine (half greater, half lesser) {
        return ((typename twice<half>::type) (greater) << (sizeof (half) * 8)) + lesser;
    };
    
    template <typename half>
    typename twice<half>::type inline multiply (half a, half b, half r) {
        return (typename twice<half>::type) (a) * (typename twice<half>::type) (b) + (typename twice<half>::type) (r);
    };
    
    template <typename half>
    typename twice<half>::type inline subtract (half a, half b, half r) {
        return r + a - b;
    };
    
    template <typename half>
    typename twice<half>::type inline add (half a, half b, half r) {
        return (typename twice<half>::type) (a) + b + r;
    };
    
}

#endif
