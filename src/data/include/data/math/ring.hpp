// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_RING
#define DATA_MATH_RING

#include <data/math/nonnegative.hpp>
#include <data/math/group.hpp>

namespace data::math {

    template <typename elem, typename plus = plus<elem>, typename times = times<elem>>
    concept non_associative_ring = abelian<elem, plus> && requires () {
        {math::identity<times, elem> {} ()} -> implicitly_convertible_to<elem>;
    } && requires (const elem &a, elem &b) {
        {times {} (a, b)} -> implicitly_convertible_to<elem>;
    };
    
    template <typename elem, typename plus = plus<elem>, typename times = times<elem>>
    concept ring = non_associative_ring<elem, plus, times> && associative<times, elem>;

    template <typename elem, typename plus = plus<elem>, typename times = times<elem>>
    concept commutative_ring = ring<elem, plus, times> && abelian<elem, times>;

    template <typename elem, typename plus = plus<elem>, typename times = times<elem>>
    concept skew_integral_domain = ring<elem, plus, times> &&
    requires (const nonzero<elem> &a, const nonzero<elem> &b) {
        {times {} (a, b)} -> implicitly_convertible_to<nonzero<elem>>;
    };

    template <typename elem, typename plus = plus<elem>, typename times = times<elem>>
    concept integral_domain = skew_integral_domain<elem, plus, times> && commutative<times, elem>;
    
}

#endif 
