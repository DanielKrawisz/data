// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_INTEGER
#define DATA_MATH_NUMBER_INTEGER

#include <data/math/number/natural.hpp>
#include <data/math/ring.hpp>

namespace data::math {
    template <typename Z, typename N = decltype(data::abs(std::declval<Z>()))> 
    concept integer = whole_number<Z> && ring<Z> && whole_number<N> && requires(int64 x) {
        {Z{x}};
    } && requires(const N &x) {
        {Z(x)};
        {-x} -> std::convertible_to<Z>;
    } && requires(const Z &x) {
        {data::sign(x)} -> std::convertible_to<math::sign>;
        {data::abs(x)} -> std::same_as<N>;
        {-x} -> std::convertible_to<Z>;
        {x == -3} -> std::convertible_to<bool>;
        {x != -3} -> std::convertible_to<bool>;
        {x <= -3} -> std::convertible_to<bool>;
        {x >= -3} -> std::convertible_to<bool>;
        {x < -3} -> std::convertible_to<bool>;
        {x > -3} -> std::convertible_to<bool>;
        {x + -3} -> std::same_as<Z>;
        {x * -3} -> std::same_as<Z>;
        {x - -3} -> std::same_as<Z>;
    } && requires(const Z &a, const N& b) {
        {a == b};
        {a != b};
        {a > b};
        {a >= b};
        {a < b};
        {a <= b};
        {b == a};
        {b != a};
        {b > a};
        {b >= a};
        {b < a};
        {b <= a};
    } && requires(const N &n) {
        {data::abs(n)} -> std::same_as<N>;
    };

    template <typename Z, typename N = decltype(data::abs(std::declval<Z>()))> requires integer<Z, N>
    division<Z, N> divide(const Z& Dividend, const Z& Divisor) {
        N divisor = data::abs(Divisor);
        division<N> d{divide_unsigned<N>(data::abs(Dividend), divisor)};
    }
    
    template <whole_number N> bool divides(const N& dividend, const N divisor) {
        return divide<N>(dividend, divisor).Remainder == 0;
    }

}

#endif
