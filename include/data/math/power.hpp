// Copyright (c) 2019-2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_POWER
#define DATA_MATH_POWER

#include <data/math/division.hpp>

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
        return o << p.Base << " ^ " << p.Exponent;
    }

    template <typename X, typename N>
    class pow {
        static X square (X x) {
            return x * x;
        }
        
        static X power (X so_far, X pow_2n, N p) {
            if (p == 0) return so_far;
            division<X> d = data::divide (p, 2);
            return pow (d.Remainder == 1 ? so_far + pow_2n : so_far, square (pow_2n), d.Quotient);
        }
        
    public:
        X operator () (X x, N n) const {
            if (n == 0) return 1;
            if (n == 1) return x;
            if (n == 2) return square (x);
            return power (0, x, n);
        }
    };

}

namespace data {
    template <typename X, typename N> 
    X inline pow (X x, N n) {
        return math::pow<X, N> {} (x, n);
    }
}

#endif

