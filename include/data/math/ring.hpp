// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_RING
#define DATA_MATH_RING

#include <data/math/group.hpp>

namespace data::math {
    
    template <typename elem, typename plus = plus<elem>, typename times = times<elem>>
    concept ring = abelian<elem, plus> && requires () {
        {math::identity<times, elem> {} ()} -> std::convertible_to<elem>;
        typename math::associative<times, elem>;
    } && requires (const elem &a, elem &b) {
        {times {} (a, b)} -> std::convertible_to<elem>;
    };

    template <typename elem, typename plus = plus<elem>, typename times = times<elem>>
    concept skew_integral_domain = ring<elem, plus, times> &&
    requires (const nonzero<elem> &a, const nonzero<elem> &b) {
        {times {} (a, b)} -> std::convertible_to<nonzero<elem>>;
    };

    template <typename elem, typename plus = plus<elem>, typename times = times<elem>>
    concept integral_domain = skew_integral_domain<elem, plus, times> && requires () {
        typename math::commutative<times, elem>;
    };
    
}

#endif 
