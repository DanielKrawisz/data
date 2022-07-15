// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_POWER
#define DATA_MATH_POWER

#include <data/math/division.hpp>

namespace data::math {
    template <typename X, typename N = X>
    class power {
        static X square(const X &x) {
            return x * x;
        }
        
        static X pow(const X &so_far, const X &pow_2n, const N &p) {
            if (p == 0) return so_far;
            return pow((p & N{1}) > 0 ? so_far + pow_2n : so_far, square(pow_2n), p >> 1);
        }
        
    public:
        X operator()(const X &x, const N &n) const {
            if (n == 0) return 1;
            if (n == 1) return x;
            if (n == 2) return square(x);
            return pow(0, x, n);
        }
    };
    
    template <typename X, typename N = X>
    class power_mod {
        static X square(const X &mod, const X &x) {
            auto n = x * x;
            return n > mod ? n % mod : n;
        }
        
        static X pow(const X &mod, const X &so_far, const X &pow_2n, const N &p) {
            if (p == 0) return so_far;
            return pow(mod, (p & N{1}) > 0 ? so_far + pow_2n : so_far, square(mod, pow_2n), p >> 1);
        }
        
    public:
        X operator()(const X &mod, const X &x, const N &n) const {
            if (n == 0) return 1;
            if (n == 1) return x;
            if (n == 2) return square(x, mod);
            return pow(0, x, mod, n) % mod;
        }
    };

}

#endif

