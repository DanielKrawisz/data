// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_LINEAR_INNER
#define DATA_MATH_LINEAR_INNER

#include <data/math/linear/space.hpp>
#include <data/abs.hpp>

namespace data::math::linear {
    template <typename F, typename V> struct inner;
        
    template <typename F, typename V> concept inner_space = space<F, V> && requires (const V &a, const V &b) {
        { inner<F, V> {} (a, b) } -> std::same_as<F>;
    };
    
    template <typename F, typename V> requires inner_space<F, V> 
    struct contract<F, V, V> {
        F operator () (const V &a, const V &b) {
            return inner<F, V> {} (a, b);
        }
    };
    
    template <typename X, size_t... sizes> requires inner_space<X, X> struct inner<X, array<X, sizes...>> {
        X operator () (const array<X, sizes...> &x, const array<X, sizes...> &y) {
            X r {};
            auto ai = x.begin ();
            auto bi = y.begin ();
            while (ai != x.end ()) {
                r += inner {} (*ai, *bi);
                ai++;
                bi++;
            }
            return r;
        }   
    };
    
    template <std::floating_point F> struct inner<F, F> {
        F operator () (F a, F b) {
            return a * b;
        }
    };

    template <std::signed_integral F> struct inner<F, F> {
        F operator () (F a, F b) {
            return a * b;
        }
    };

    template <typename X, size_t size> struct inner<X, array<X, size>> {
        X operator () (const array<X, size> &a, const array<X, size> &b) {
            return a * b;
        }
    };
    
}

#endif
