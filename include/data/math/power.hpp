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

    template <typename X, typename N>
    class pow {
        static X square (X x) {
            return x * x;
        }

        // p is at least 2
        static X step (X so_far, X pow_2n, N p) {
            X next_step = p & 1 == 1 ? so_far * pow_2n : so_far;
            N n = p >> 1;
            if (n == 0) return next_step;
            return step (next_step, square (pow_2n), n);
        }
        
    public:
        X operator () (X x, N n) const {
            if (n == 0) return X {1};
            return step (X {1}, x, n);
        }
    };

}

namespace data {
    template <typename X, typename N> 
    X inline pow (const X &x, const N &n) {
        return math::pow<X, N> {} (x, n);
    }
}

#endif

