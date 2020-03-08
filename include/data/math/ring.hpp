// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_RING
#define DATA_MATH_RING

#include <data/math/group.hpp>

namespace data::interface {
    
    template <typename elem, typename plus, typename times>
    class ring : math::identity<times, elem>, 
        public abelian<elem, plus>, 
        public math::associative<times, elem> {
        using require_plus_operator = typename std::enable_if<meta::has_plus_operator<elem>::value, void>::type;
        using require_minus_operator = typename std::enable_if<meta::has_minus_operator<elem>::value, void>::type;
        using require_times_operator = typename std::enable_if<meta::has_times_operator<elem>::value, void>::type;
    public:
        static elem zero() {
            return times::Identity;
        }
    };
    
}

#endif 
