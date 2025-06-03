// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_HALVES
#define DATA_HALVES

#include <type_traits>
#include <concepts>
#include <data/encoding/endian.hpp>

namespace data::encoding {
    template<std::size_t S> struct int_by_size;
    template<> struct int_by_size<1> { using type = std::int8_t; };
    template<> struct int_by_size<2> { using type = std::int16_t; };
    template<> struct int_by_size<4> { using type = std::int32_t; };
    template<> struct int_by_size<8> { using type = std::int64_t; };

    template<std::size_t S> struct uint_by_size;
    template<> struct uint_by_size<1> { using type = std::uint8_t; };
    template<> struct uint_by_size<2> { using type = std::uint16_t; };
    template<> struct uint_by_size<4> { using type = std::uint32_t; };
    template<> struct uint_by_size<8> { using type = std::uint64_t; };

    template<std::unsigned_integral T>
    struct half_of : uint_by_size<sizeof (T) / 2> {
        using type = uint_by_size<sizeof (T) / 2>::type;

        static type greater_half (T u) {
            return u >> sizeof (type) * 8;
        }

        static type lesser_half (T u) {
            return static_cast<T> (u & std::numeric_limits<type>::max ());
        }
    };

    template<typename T, typename = void>
    struct twice { }; // leave undefined or empty when invalid

    template<typename T>
    struct twice<T, std::void_t<typename uint_by_size<sizeof(T) * 2>::type>> {
        using type = typename uint_by_size<sizeof(T) * 2>::type;
    };

    // Aliases for ease of use
    template<typename T>
    using half_of_t = typename half_of<T>::type;

    template<typename T>
    using twice_t = typename twice<T>::type;

    template <typename X> struct count_digits;
    
    template <std::integral X> struct count_digits<X> {
        constexpr static bool is_signed = std::is_signed_v<X>;
        constexpr static size_t value = sizeof (X);
    };
    
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

    namespace {
        template <bool is_next, typename... Y> struct get_next_twice_type;

        template <typename half, typename X, typename... Y> struct get_next_twice_type<true, half, X, Y...> {
            using type = X;
        };

        template <typename half, typename X, typename Y, typename... Z> struct get_next_twice_type<false, half, X, Y, Z...> {
            using type = get_next_twice_type<sizeof (Y) == sizeof (half) * 2, half, Y, Z...>::type;
        };

        template <typename half, typename X, typename... Y> struct find_twice {
            using type = get_next_twice_type<sizeof (X) == sizeof (half) * 2, half, X, Y...>::type;
        };
    }
    
    template <typename half>
    typename twice<half>::type inline combine (half greater, half lesser) {
        return ((typename twice<half>::type) (greater) << count_digits<half>::value * 8) + lesser;
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
