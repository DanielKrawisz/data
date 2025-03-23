// Copyright (c) 2019-2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_POWER
#define DATA_MATH_POWER

#include <data/math/division.hpp>
#include <iostream>

namespace data {

    template <typename A, typename Exp> A pow (const A &, const Exp &);
    template <typename A, typename Exp, typename Mod> A pow_mod (const A &, const Exp &, const Mod &);

}

namespace data::math {
    template <typename A, typename Exp = A> class pow;
    template <typename A, typename Exp = A, typename Mod = Exp> class pow_mod;
}

namespace data {

    template <typename A, typename Mod> A inline pow (const A &x, const Mod &y) {
        return math::pow<A, Mod> {} (x, y);
    }

    template <typename A, typename Exp, typename Mod> A inline pow_mod (const A &x, const Exp &y, const Mod &n) {
        return math::pow_mod<A, Exp, Mod> {} (x, y, n);
    }
}

namespace data::math {

    template <typename A, typename Exp = A>
    struct power {
        A Base;
        Exp Exponent;

        bool operator == (const power &x) const {
            return Base == x.Base && Exponent == x.Exponent;
        }
    };

    template <typename A, typename Mod>
    std::ostream inline &operator << (std::ostream &o, const power<A, Mod> &p) {
        o << p.Base;
        if (p.Exponent != 1) o << " ^ " << p.Exponent;
        return o;
    }

    template <typename A, typename Exp>
    class pow {
        static A square (const A &x) {
            return x * x;
        }

        // p is at least 2
        static A loop (A so_far, A pow_2n, Exp p) {
            A next_step = (p & Exp {1}) == 1 ? so_far * pow_2n : so_far;
            Exp n = p >> 1;
            if (n == 0) return next_step;
            return loop (next_step, square (pow_2n), n);
        }
        
    public:
        A operator () (A x, Exp n) const {
            if (n == 0) return A {1};
            return loop (A {1}, x, n);
        }
    };
    
    template <typename A, typename Exp, typename Mod>
    class pow_mod {
        static A square (const Mod &mod, const A &x) {
            return x * x % mod;
        }
        
        static A loop (const Mod &mod, const A &so_far, const A &pow_2n, const Exp &p) {
            if (p == 0) return so_far;
            return loop (mod, (p & Exp {1}) == 1 ? (so_far * pow_2n) % mod : so_far, square (mod, pow_2n), p >> 1);
        }
        
    public:
        A operator () (const A &x, const Exp &P, const Mod &mod) const {
            if (mod == 0) throw math::division_by_zero {};
            if (mod == 1) return 0;
            Exp p = P % (mod - 1);
            if (p == 0) return 1;
            if (p == 1) return x % mod;
            if (p == 2) return square (mod, x);
            return loop (mod, (p & Exp {1}) == 1 ? x : 1, square (mod, x), p >> 1) % mod;
        }
    };

}

#endif

