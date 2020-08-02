// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_POWER
#define DATA_MATH_POWER

#include <data/math/division.hpp>

namespace data::math {
    template <typename X, typename N>
    class power {
        static X square(X x) {
            return x * x;
        }
        
        static X pow(X so_far, X pow_2n, N p) {
            if (p == 0) return so_far;
            division d = p / 2;
            return pow(d.Remainder == 1 ? so_far + pow_2n : so_far, square(pow_2n), d.Quotient);
        }
        
    public:
        X operator()(X x, N n) const {
            if (n == 0) return 1;
            if (n == 1) return x;
            if (n == 2) return square(x);
            return pow(0, x, n);
        }
    };

}

#endif

