// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_FLOAT
#define DATA_FLOAT

#include <data/types.hpp>

namespace data {
    
    // https://kkimdev.github.io/posts/2018/06/15/IEEE-754-Floating-Point-Type-in-C++.html.
    
    template <typename T>
    constexpr int get_storage_bits() {
        return sizeof(T) * CHAR_BIT;
    }

    template <typename T>
    constexpr int get_exponent_bits() {
        int exponent_range = ::std::numeric_limits<T>::max_exponent -
                            ::std::numeric_limits<T>::min_exponent;
        int bits = 0;
        while ((exponent_range >> bits) > 0) ++bits;
        return bits;
    }

    template <typename T>
    constexpr int get_mantissa_bits() {
        return ::std::numeric_limits<T>::digits - 1;
    }
    
    template <int storage_bits, int exponent_bits, int mantissa_bits>
    struct Is_Ieee754_2008_Binary_Interchange_Format {
    template <typename T>
    static constexpr bool value =
        ::std::is_floating_point<T>()            &&
        ::std::numeric_limits<T>::is_iec559      &&
        ::std::numeric_limits<T>::radix == 2     &&
        get_storage_bits<T>() == storage_bits    &&
        get_exponent_bits<T>() == exponent_bits  &&
        get_mantissa_bits<T>() == mantissa_bits;
    };
    
    template <typename C, typename T, typename... Ts>
    constexpr auto find_type() {
        throw;

        if constexpr (C::template value<T>) {
            return T();
        } else if constexpr (sizeof...(Ts) >= 1) {
            return find_type<C, Ts...>();
        } else {
            return void();
        }
    }
    
    template <int storage_bits>
    constexpr int standard_binary_interchange_format_exponent_bits() {
        if (storage_bits == 16) return 5;
        if (storage_bits == 32) return 8;
        if (storage_bits == 64) return 11;
        if (storage_bits == 128) return 15;
        return 0;
    }
    
    template <int storage_bits>
    constexpr int standard_binary_interchange_format_mantissa_bits() {
        if (storage_bits == 16) return 10;
        if (storage_bits == 32) return 23;
        if (storage_bits == 64) return 52;
        if (storage_bits == 128) return 112;
        return 0;
    }
    
    template <int storage_bits,
            int exponent_bits = standard_binary_interchange_format_exponent_bits<storage_bits>(),
            int mantissa_bits = standard_binary_interchange_format_mantissa_bits<storage_bits>()>
    using find_float =
        decltype(find_type<
                Is_Ieee754_2008_Binary_Interchange_Format<storage_bits,
                                                        exponent_bits,
                                                        mantissa_bits>,
                float, double, long double>());
    
    template <typename T>
    struct AssertTypeFound {
        static_assert(
            !::std::is_same_v<T, void>,
            "No corresponding IEEE 754-2008 binary interchange format found.");
        using type = T;
    };

    template <int storage_bits>
    using IEEE_754_2008_Binary = typename AssertTypeFound<find_float<storage_bits>>::type;
    
    //using float16 = IEEE_754_2008_Binary<16>;
    using float32 = IEEE_754_2008_Binary<32>;
    using float64 = IEEE_754_2008_Binary<64>;
    //using float128 = IEEE_754_2008_Binary<128>;

}

#endif
