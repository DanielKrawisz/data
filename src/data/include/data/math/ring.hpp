// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_RING
#define DATA_MATH_RING

#include <data/math/nonnegative.hpp>
#include <data/math/group.hpp>

namespace data::math {

    template <typename elem, typename plus = plus<elem>, typename times = times<elem>>
    concept ring = group<elem, plus> && requires () {
        {math::identity<times, elem> {} ()} -> ImplicitlyConvertible<elem>;
    } && requires (const elem &a, elem &b) {
        {times {} (a, b)} -> ImplicitlyConvertible<elem>;
    };

    template <typename elem, typename plus = plus<elem>, typename times = times<elem>>
    concept integral_domain = ring<elem, plus, times> &&
    requires (const nonzero<elem> &a, const nonzero<elem> &b) {
        {times {} (a, b)} -> ImplicitlyConvertible<nonzero<elem>>;
    };
    
}

#endif 
