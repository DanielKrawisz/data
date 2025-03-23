// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_FIELD
#define DATA_MATH_FIELD

#include <data/concepts.hpp>
#include <data/math/division.hpp>
#include <data/math/ring.hpp>

namespace data::math {

    template <typename elem, typename plus = math::plus<elem>, typename times = math::times<elem>>
    concept field = integral_domain<elem, plus, times> &&
    requires (const elem &a, const elem &b) {
        {a / b} -> std::same_as<elem>;
    } && requires (const nonzero<elem> &a, const nonzero<elem> &b) {
        {inverse<times, elem> {} (a, b)} -> implicitly_convertible_to<nonzero<elem>>;
    };
    
}

#endif
