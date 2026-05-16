// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_FIELD
#define DATA_MATH_FIELD

#include <data/concepts.hpp>
#include <data/arithmetic.hpp>
#include <data/math/ring.hpp>
#include <data/norm.hpp>

namespace data::math {

    template <typename elem, typename plus = def::plus<elem>, typename times = def::times<elem>>
    concept field = integral_domain<elem, plus, times> &&
    requires (const elem &a, const elem &b) {
        {a / b} -> Same<elem>;
    } && requires (const nonzero<elem> &a, const nonzero<elem> &b) {
        { def::inverse<times, elem> {} (a, b) } -> ImplicitlyConvertible<nonzero<elem>>;
    };

    template <typename elem, typename plus = def::plus<elem>, typename times = def::times<elem>>
    concept normed_ring = ring<elem, plus, times> && Normed<elem>;

    template <typename elem, typename plus = def::plus<elem>, typename times = def::times<elem>>
    concept normed_field = field<elem, plus, times> && normed_ring<elem, plus, times>;
    
}

#endif
