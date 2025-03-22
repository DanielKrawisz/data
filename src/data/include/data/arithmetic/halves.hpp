// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_HALVES
#define DATA_HALVES

#include <type_traits>
#include <concepts>
#include <data/encoding/endian.hpp>

namespace data::encoding {

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

    namespace {
        template <bool is_next, typename... Y> struct get_next_half_type;

        template <typename whole, typename X, typename... Y> struct get_next_half_type<true, whole, X, Y...> {
            using type = X;
        };

        template <typename whole, typename X, typename Y, typename... Z> struct get_next_half_type<false, whole, X, Y, Z...> {
            using type = get_next_half_type<sizeof (Y) * 2 == sizeof (whole), whole, Y, Z...>::type;
        };

        template <typename whole, typename X, typename... Y> struct find_half {
            using type = get_next_half_type<sizeof (X) * 2 == sizeof (whole), whole, X, Y...>::type;
        };
    }

    template <typename whole> struct half_of;
    
    template <std::unsigned_integral whole> struct half_of<whole> {
        using type = find_half<whole, unsigned long long, unsigned long, unsigned, unsigned short, byte>::type;

        static type greater_half (whole u) {
            return u >> count_digits<type>::value * 8;
        }

        static type lesser_half (whole u) {
            return u & halves<type, whole>::lesser;
        }
    };

    template <std::signed_integral whole> struct half_of<whole> {
        using type = find_half<whole, long long, long, int, short, char>::type;

        static type greater_half (whole u) {
            return u >> count_digits<type>::value * 8;
        }

        static type lesser_half (whole u) {
            return u & halves<type, whole>::lesser;
        }
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

    
    template <typename half> struct twice;

    template <std::unsigned_integral half> struct twice<half> {
        using type = find_twice<half, unsigned short, unsigned, unsigned long, unsigned long long>::type;

        static type extend (half x) {
            return (type) (x);
        }
    };

    template <std::signed_integral half> struct twice<half> {
        using type = find_twice<half, short, int, long, long long>::type;

        static type extend (half x) {
            return (type) (x);
        }
    };
    
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
