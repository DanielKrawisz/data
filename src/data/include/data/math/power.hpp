// Copyright (c) 2019-2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_POWER
#define DATA_MATH_POWER

#include <data/math/division.hpp>
#include <iostream>

namespace data::math {

    template <typename X, typename N>
    struct power {
        X Base;
        N Exponent;

        bool operator == (const power &x) const {
            return Base == x.Base && Exponent == x.Exponent;
        }
    };

    template <typename X, typename N>
    std::ostream inline &operator << (std::ostream &o, const power<X, N> &p) {
        o << p.Base;
        if (p.Exponent != 1) o << " ^ " << p.Exponent;
        return o;
    }

    template <typename X, typename N = X>
    class pow {
        static X square (const X &x) {
            return x * x;
        }

        // p is at least 2
        static X loop (X so_far, X pow_2n, N p) {
            X next_step = (p & N {1}) == 1 ? so_far * pow_2n : so_far;
            N n = p >> 1;
            if (n == 0) return next_step;
            return loop (next_step, square (pow_2n), n);
        }
        
    public:
        X operator () (X x, N n) const {
            if (n == 0) return X {1};
            return loop (X {1}, x, n);
        }
    };
    
    template <typename X, typename N = X>
    class pow_mod {
        static X square (const X &mod, const X &x) {
            auto n = x * x;
            return n > mod ? n % mod : n;
        }
        
        static X loop (const X &mod, const X &so_far, const X &pow_2n, const N &p) {
            if (p == 0) return so_far;
            return loop (mod, (p & N {1}) == 1 ? so_far * pow_2n : so_far, square (mod, pow_2n), p >> 1);
        }
        
    public:
        X operator () (const X &mod, const X &x, const N &p) const {
            if (p == 0) return 1;
            if (p == 1) return x % mod;
            if (p == 2) return square (mod, x);
            return loop (mod, (p & N {1}) == 1 ? x : 1, square (mod, x), p >> 1) % mod;
        }
    };

}

namespace data {
    template <typename X, typename N> 
    X inline pow (const X &x, const N &n) {
        return math::pow<X, N> {} (x, n);
    }

    template <typename X, typename N>
    X inline pow_mod (const X &mod, const X &x, const N &n) {
        return math::pow_mod<X, N> {} (mod, x, n);
    }
}

#endif

