// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_FUNCTION
#define DATA_FUNCTION

#include <concepts>
#include <data/types.hpp>

namespace data::functional {

    template< typename F, typename output, typename... Args >
    concept function = std::regular_invocable<F, Args...> && requires (F&& f, Args&&... args) {
        {std::invoke (std::forward<F> (f), std::forward<Args> (args)...)} -> std::convertible_to<output>;
    };
}

namespace data {
    
    // It is always possible to construct the identity function. 
    template <typename X>
    struct identity {
        X operator () (const X& x) const {
            return x;
        }
    };

    // and the do nothing function.
    template <typename ... args> void do_nothing (args...) {}
    
    // Given an element of Y, we can construct a function from X to Y. 
    template <typename X, typename Y>
    struct constant {
        Y Constant;
        
        constant(Y c) : Constant {c} {}
        
        Y operator() (const X&) const {
            return Constant;
        }
    };
    
    template <typename F, typename X, typename Y> struct inverse;
    
    template <typename X> struct inverse<identity<X>, X, X> {
        X operator()(const X& x) {
            return identity<X>{}(x);
        }
    };
    
    template <typename F, typename X> struct action {
        F Function;
        X Value;
        X operator()(const X& x) {
            return Function(Value, x);
        }
    };

    template <typename n, typename a, typename... args>
    function<n (args...)> inline curry (function<n (a, args...)> f, a va) {
        return [f, va] (args... z) -> n {
            return f (va, z...);
        };
    }

    template <typename n, typename a, typename b, typename... args>
    function<n (args...)> inline curry (function<n (a, b, args...)> f, a va, b vb) {
        return [f, va, vb] (args... z) -> n {
            return f (va, vb, z...);
        };
    }

    template <typename n, typename a, typename b, typename c, typename... args>
    function<n (args...)> inline curry (function<n (a, b, c, args...)> f, a va, b vb, c vc) {
        return [f, va, vb, vc] (args... z) -> n {
            return f (va, vb, vc, z...);
        };
    }

    template <typename n, typename a, typename b, typename c, typename d, typename... args>
    function<n (args...)> inline curry (function<n (a, b, c, d, args...)> f, a va, b vb, c vc, d vd) {
        return [f, va, vb, vc, vd] (args... z) -> n {
            return f (va, vb, vc, vd, z...);
        };
    }

    template <typename n, typename a, typename b, typename c, typename d, typename e, typename... args>
    function<n (args...)> inline curry (function<n (a, b, c, d, e, args...)> f, a va, b vb, c vc, d vd, e ve) {
        return [f, va, vb, vc, vd, ve] (args... z) -> n {
            return f (va, vb, vc, vd, ve, z...);
        };
    }

}

#endif
