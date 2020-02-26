// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_GROUP
#define DATA_MATH_GROUP

#include <data/math/associative.hpp>
#include <data/math/commutative.hpp>
#include <data/math/arithmetic.hpp>

namespace data::interface {
    
    template <typename elem, typename op>
    class group : math::associative<op, elem> {
        using require_binary_identity = typename std::enable_if<meta::has_identity<op, elem>::value, void>::type;

    public:
        static elem identity() {
            return op::Identity;
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
