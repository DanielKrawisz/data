// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_FIELD
#define DATA_MATH_FIELD

#include <data/math/ring.hpp>
#include <data/math/nonnegative.hpp>
#include <data/math/commutative.hpp>

namespace data::interface {
    
    template <typename elem, typename plus = data::plus<elem>, typename times = data::times<elem>>
    struct division_algebra : ring<elem, plus, times> {
        using require_divide_operator = typename std::enable_if<meta::has_divide_operator<elem, math::nonzero<elem>, elem>::value, void>::type;
    };
    
    template <typename elem, typename Q, typename plus = data::plus<elem>, typename times = data::times<elem>>
    struct normed_division_algebra : division_algebra<elem, plus, times> {
        using require_quadrance_method = typename std::enable_if<meta::has_quadrance_method<elem, math::nonnegative<Q>>::value>::type;
    };
    
    template <typename elem, typename plus = data::plus<elem>, typename times = data::times<elem>>
    struct field : division_algebra<elem, plus, times>, abelian<math::nonzero<elem>, times> {};
    
}

#endif
