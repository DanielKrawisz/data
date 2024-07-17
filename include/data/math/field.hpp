// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_FIELD
#define DATA_MATH_FIELD

#include <data/math/division.hpp>
#include <data/math/ring.hpp>
#include <data/math/nonnegative.hpp>
#include <data/math/commutative.hpp>

namespace data::math {

    template <typename elem, typename plus = math::plus<elem>, typename times = math::times<elem>>
    concept skew_field = skew_integral_domain<elem, plus, times> &&
    requires (const elem &a, const elem &b) {
        {a / b} -> std::same_as<elem>;
    } && requires (const nonzero<elem> &a, const nonzero<elem> &b) {
        {inverse<times, elem> {} (a, b)} -> std::convertible_to<nonzero<elem>>;
    };
    
    template <typename elem, typename plus = math::plus<elem>, typename times = math::times<elem>>
    concept field = skew_field<elem, plus, times> && integral_domain<elem, plus, times>;
    
}

#endif
