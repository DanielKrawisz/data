// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_SIGN
#define DATA_MATH_SIGN

#include <data/math/countable.hpp>

namespace data::math {
            
    enum sign : int8_t { zero = 0 , positive = 1 , negative = -1 };

    data::math::sign inline operator*(data::math::sign a, data::math::sign b) {
        return a == data::math::zero || b == data::math::zero ? data::math::zero : 
            a == b ? data::math::positive : data::math::negative;
    }

}

namespace data::interface {
    
    template <typename X> concept has_sign_method = requires (const X& x) {
        { x.sign() } -> std::same_as<math::sign>;
    };
    
}

namespace data::meta {
    
    template <typename X> struct sign;
    
    template <interface::has_sign_method X> 
    struct sign<X> {
        math::sign operator()(const X& x) const {
            return x.sign();
        }
    };
    
    template <typename X> requires std::integral<X> || std::floating_point<X> 
    struct sign<X> {
        math::sign operator()(const X& x) const {
            return x == 0 ? data::math::zero : x > 0 ? data::math::positive : data::math::negative;
        }
    };
    
}

namespace data {
    template <typename X> 
    math::sign inline sign(const X& x) {
        return meta::sign<X>{}(x);
    }
}

#endif
