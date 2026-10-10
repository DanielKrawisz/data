// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/concepts.hpp>
#include <data/arithmetic.hpp>
#include <data/math/ring.hpp>
#include <data/norm.hpp>

namespace data::math {

    template <typename elem, typename plus = def::plus<elem>, typename times = def::times<elem>>
    concept Field = IntegralDomain<elem, plus, times> &&
    requires (const elem &a, const elem &b) {
        { a / b } -> Same<elem>;
    } && requires (const nonzero<elem> &a, const nonzero<elem> &b) {
        { def::inverse<times, elem> {} (a, b) } -> ImplicitlyConvertible<nonzero<elem>>;
    };

    template <typename elem, typename plus = def::plus<elem>, typename times = def::times<elem>>
    concept NormedRing = Ring<elem, plus, times> && Normed<elem>;

    template <typename elem, typename plus = def::plus<elem>, typename times = def::times<elem>>
    concept RormedField = Field<elem, plus, times> && NormedRing<elem, plus, times>;
    
}
