// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_COUNTABLE
#define DATA_MATH_COUNTABLE

#include <type_traits>

#include <data/types.hpp>

namespace data::interface {
    
    template <typename X>
    concept has_zero_initializer = requires {
        { X {0} };
    };
    
    template <typename X>
    concept has_zero_method = requires {
        { X::zero () } -> std::same_as<X>;
    };
    
    template <typename X>
    concept has_zero_value = has_zero_method<X> || has_zero_initializer<X>;
    
}

namespace data::meta {
    
    template <typename X> struct zero;
    
    template <interface::has_zero_initializer X>
    struct zero<X> {
        X operator () () const {
            return X {0};
        }
    };
    
    template <interface::has_zero_value X>
    struct zero<X> {
        X operator () () const {
            return X::zero ();
        }
    };
    
}

namespace data {
    
    template <typename X>
    X inline zero () {
        return meta::zero<X> {} ();
    }
    
    template <typename N>
    N next (const N &n) {
        return n + 1;
    };
    
}

namespace data::math {
    
    template <typename L>
    concept countable = interface::has_zero_value<L> && requires (const L n) {
        { next<L> (n) } -> std::same_as<L>;
    };

}

#endif
