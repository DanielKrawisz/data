// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_DIVIDE
#define DATA_DIVIDE

#include <data/math/nonnegative.hpp>
#include <data/abs.hpp>
#include <data/arithmetic.hpp>
#include <iostream> // required on windows

namespace data {

    template <typename dividend, typename divisor>
    auto divide (const dividend &a, const math::nonzero<divisor> &b);

    template <typename dividend, typename divisor>
    bool divides (const dividend &a, const math::nonzero<divisor> &b);

    template <typename N, typename R = N> struct division {
        N Quotient;
        R Remainder;

        bool valid () const {
            return data::valid (Quotient) && data::valid (Remainder);
        }

        division (const N &q, const R &r) : Quotient {q}, Remainder {r} {}
        division () : Quotient {}, Remainder {} {}

        bool operator == (const division &d) const {
            return Quotient == d.Quotient && Remainder == d.Remainder;
        }

        bool operator != (const division &d) const {
            return !(*this == d);
        }
    };

    // modular arithmetic
    template <typename A, typename Mod = A> auto mod (const A &, const math::nonzero<Mod> &);
    template <typename A, typename Mod = A> auto negate_mod (const A &, const math::nonzero<Mod> &);

    template <typename A, typename B, typename Mod> auto plus_mod (const A &, const B &, const math::nonzero<Mod> &);
    template <typename A, typename B, typename Mod> auto minus_mod (const A &, const B &, const math::nonzero<Mod> &);

    template <typename A, typename B, typename Mod> auto times_mod (const A &, const B &, const math::nonzero<Mod> &);
    template <typename A, typename Exp = A, typename Mod = Exp> auto pow_mod (const A &, const Exp &, const math::nonzero<Mod> &);

    // helper functions for computing pow mod and times mod.
    template <typename A, typename Mod = A> auto mul_2_mod (const A &, const math::nonzero<Mod> &);
    template <typename A, typename Mod = A> auto square_mod (const A &, const math::nonzero<Mod> &);
}

namespace data::math {

    struct division_by_zero : std::logic_error {
        division_by_zero () : std::logic_error {"division by zero"} {}
    };

    template <typename dividend, typename divisor = dividend> struct divide;

    template <std::unsigned_integral X>
    struct divide<X, X> {
        division<X, X> operator () (X dividend, nonzero<X> divisor) {
            return {static_cast<X> (dividend / static_cast<X> (divisor.Value)), static_cast<X> (dividend % divisor.Value)};
        }
    };

    template <std::signed_integral X, std::unsigned_integral Y>
    struct divide<X, Y> {
        division<X, Y> operator () (X dividend, nonzero<Y> divisor) {
            auto quotient = static_cast<X> (dividend / static_cast<X> (divisor.Value));
            auto remainder = static_cast<X> (dividend % divisor.Value);
            if (remainder == 0) return {quotient, 0};
            if (dividend < 0) return {static_cast<X> (quotient - 1), static_cast<Y> (divisor.Value - static_cast<Y> (remainder))};
            return {quotient, static_cast<Y> (remainder)};
        }
    };

    template <std::signed_integral X>
    struct divide<X, X> {
        division<X, std::make_unsigned_t<X>> operator () (X dividend, nonzero<X> divisor) {
            auto quotient = static_cast<X> (dividend / divisor.Value);
            auto remainder = static_cast<X> (dividend % divisor.Value);
            if (remainder == 0) return {quotient, 0};
            else if (dividend < 0) {
                if (divisor.Value < 0) return {static_cast<X> (-quotient + 1),
                    static_cast<std::make_unsigned_t<X>> (-divisor.Value + remainder)};
                else return {static_cast<X> (quotient - 1), static_cast<std::make_unsigned_t<X>> (divisor.Value + remainder)};
            } else return {static_cast<X> (quotient), static_cast<std::make_unsigned_t<X>> (remainder)};
        }
    };

    template <typename A, typename Mod = A> struct mod;
    template <typename A, typename Mod = A> struct negate_mod;
    
    template <typename A, typename Mod = A> struct invert_mod;
    template <typename A, typename B = A, typename Mod = B> struct plus_mod;
    template <typename A, typename B = A, typename Mod = B> struct minus_mod;

    template <typename A, typename B = A, typename Mod = B> struct times_mod;
    template <typename A, typename Exp = A, typename Mod = Exp> struct pow_mod;

    template <typename A, typename Mod = A> struct mul_2_mod;
    template <typename A, typename Mod = A> struct square_mod;

}

namespace data {
    template <typename dividend, typename divisor>
    auto inline divide (const dividend &a, const math::nonzero<divisor> &b) {
        return math::divide<dividend, divisor> {} (a, b);
    }
    
    template <typename dividend, typename divisor>
    bool inline divides (const dividend &a, const math::nonzero<divisor> &b) {
        return b == 0 ? true : math::divide<dividend, divisor> {} (a, b).Remainder == 0;
    }

    template <typename A, typename Mod> auto mod (const A &x, const math::nonzero<Mod> &n) {
        return math::mod<A, Mod> {} (x, n);
    }

    template <typename A, typename Mod> auto negate_mod (const A &x, const math::nonzero<Mod> &n) {
        return math::negate_mod<A, Mod> {} (x, n);
    }

    template <typename A, typename B, typename Mod> auto inline plus_mod (const A &x, const B &y, const math::nonzero<Mod> &n) {
        return math::plus_mod<A, B, Mod> {} (x, y, n);
    }

    template <typename A, typename B, typename Mod> auto inline minus_mod (const A &x, const B &y, const math::nonzero<Mod> &n) {
        return math::minus_mod<A, B, Mod> {} (x, y, n);
    }

    template <typename A, typename Exp, typename Mod> auto inline times_mod (const A &x, const Exp &y, const math::nonzero<Mod> &n) {
        return math::times_mod<A, Exp, Mod> {} (x, y, n);
    }

    template <typename A, typename Exp, typename Mod> auto inline pow_mod (const A &x, const Exp &y, const math::nonzero<Mod> &n) {
        return math::pow_mod<A, Exp, Mod> {} (x, y, n);
    }

    template <typename A, typename Mod> auto invert_mod (const A &x, const math::nonzero<Mod> &n) {
        return math::invert_mod<A, Mod> {} (x, n);
    }

    template <typename A, typename Mod> auto inline mul_2_mod (const A &x, const math::nonzero<Mod> &z) {
        return math::mul_2_mod<A, Mod> {} (x, z);
    }

    template <typename A, typename Mod> auto inline square_mod (const A &x, const math::nonzero<Mod> &z) {
        return math::square_mod<A, Mod> {} (x, z);
    }

    template <typename N, typename R>
    std::ostream inline &operator << (std::ostream &o, const division<N, R> x) {
        return o << "division {Quotient: " << x.Quotient << ", Remainder: " << x.Remainder << "}";
    }

}

namespace data::math {

    // default definition of mod in terms of divide.
    template <typename A, typename Mod> struct mod {
        auto operator () (const A &x, const nonzero<Mod> &n) const {
            return divide<A, Mod> {} (x, n).Remainder;
        }
    };

    template <std::integral A, std::integral Mod> struct mod<A, Mod> {
        auto operator () (const A &x, const nonzero<Mod> &n) const {
            return data::abs (x) % data::abs (n.Value);
        }
    };

    template <typename A, typename Mod> struct negate_mod {
        auto operator () (const A &x, const nonzero<Mod> &n) const {
            return x < n.Value ?
            data::mod (n.Value - x, n):
            data::mod (n.Value - data::mod (x, n), n);
        }
    };

    template <typename A, typename Mod> struct mul_2_mod {
        auto operator () (const A &x, const nonzero<Mod> &n) const {
            return data::mod (data::mul_2 (x), n);
        }
    };

    template <typename A, typename B, typename Mod> struct plus_mod {
        auto operator () (const A &x, const B &y, const nonzero<Mod> &n) const {
            return data::mod (data::plus (x, y), n);
        }
    };

    template <typename A, typename B, typename Mod> struct minus_mod {
        auto operator () (const A &x, const B &y, const nonzero<Mod> &n) const {
            return data::mod (data::minus (x, y), n);
        }
    };

    template <typename A, typename Mod> struct square_mod {
        auto operator () (const A &x, const nonzero<Mod> &n) const {
            return data::mod (data::square (x), n);
        }
    };

    // note: should have a special case for integral that uses twice.
    template <typename A, typename B, typename Mod> struct times_mod {
        auto operator () (A a, B b, nonzero<Mod> n) {
            return data::mod ((data::abs (a) * data::abs (b)), n);
        }
    };
}

#endif
