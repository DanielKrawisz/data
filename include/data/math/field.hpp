// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_FIELD
#define DATA_MATH_FIELD

#include <data/math/ring.hpp>
#include <data/math/nonnegative.hpp>
#include <data/math/commutative.hpp>

namespace data::math {
    
    template <typename elem, typename plus = math::plus<elem>, typename times = math::times<elem>>
    concept field = math::ring<elem, plus, times> && abelian<elem, plus> && 
    requires (const elem &a, const math::nonzero<elem> &b) {
        {a / b} -> std::same_as<elem>;
    } && math::abelian<math::nonzero<elem>, times>;
    
}

#endif
