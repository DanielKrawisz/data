// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/concepts.hpp>
#include <data/arithmetic.hpp>
#include <data/math/algebra.hpp>

namespace data::math {
    
    template <typename elem, typename op = def::plus<elem>>
    concept Group = std::default_initializable<elem> && requires () {
        { def::identity<op, elem> {} () } -> ImplicitlyConvertible<elem>;
    } && requires (const elem &a, const elem &b) {
        { op {} (a, b) } -> ImplicitlyConvertible<elem>;
        { def::inverse<op, elem> {} (a, b) } -> ImplicitlyConvertible<elem>;
    };
    
}
