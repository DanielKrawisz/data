// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_GROUP
#define DATA_MATH_GROUP

#include <data/math/associative.hpp>
#include <data/math/commutative.hpp>
#include <data/math/arithmetic.hpp>

namespace data::math {
    
    template <typename F, typename x> struct identity;
}

namespace data::interface {
    
    template <typename elem, typename op>
    struct group : math::associative<op, elem>, math::identity<op, elem> {
        static elem identity() {
            return math::identity<op, elem>::Identity;
        }
        
        using invert = data::inverse<action<op, elem>, elem, elem>;
        
        static elem inverse(const elem& e) {
            return invert{op{}, identity()}(e);
        }
    };
    
    template <typename elem, typename op>
    struct abelian : group<elem, op>, math::commutative<op, elem> {};
    
}

#endif 
