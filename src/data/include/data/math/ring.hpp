// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_RING
#define DATA_MATH_RING

#include <data/math/nonzero.hpp>
#include <data/math/group.hpp>

namespace data::math {

    template <typename elem, typename plus = def::plus<elem>, typename times = def::times<elem>>
    concept ring = group<elem, plus> && requires () {
        { def::identity<times, elem> {} () } -> ImplicitlyConvertible<elem>;
    } && requires (const elem &a, elem &b) {
        { times {} (a, b) } -> ImplicitlyConvertible<elem>;
    };

    template <typename elem, typename plus = def::plus<elem>, typename times = def::times<elem>>
    concept integral_domain = ring<elem, plus, times> &&
    requires {
        elem {0};
        elem {1};
    } && requires (const nonzero<elem> &a, const nonzero<elem> &b) {
        { times {} (a, b) } -> ImplicitlyConvertible<nonzero<elem>>;
    };
    
}

#endif 
