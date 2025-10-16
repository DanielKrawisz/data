// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_FRACTION
#define DATA_MATH_FRACTION

#include <data/complex.hpp>
#include <data/math/number/rational.hpp>
#include <data/math/nonnegative.hpp>
#include <data/math/number/extended_euclidian.hpp>
#include <data/math/octonion.hpp>

namespace data::math {

    // fraction is capable of taking any normed integral domain
    // (such as the integers or the gaussian integers)
    // and turning it into its fraction field. We can also use
    // it on the unit quaternions to form the rational quaternions
    // and same with octonions.
    template <typename Z, typename N = decltype (quadrance (std::declval<Z> ()))>
    requires integral_domain<Z> && ImplicitlyConvertible<N, Z>
    struct fraction;
}

namespace data {

    // construct a fraction
    template <typename Z, typename N = decltype (quadrance (std::declval<Z> ()))>
    math::fraction<Z, N> inline over (const Z &numerator, const Z &denominator);

}

namespace data::math {

    template <typename Z, typename N>
    bool operator == (const fraction<Z, N> &, const fraction<Z, N> &);

    template <Ordered Z, Ordered N>
    auto inline operator <=> (const fraction<Z, N> &x, const fraction<Z, N> &y);

    template <typename Z, typename N> fraction<Z, N> operator - (const fraction<Z, N> &);
    template <typename Z, typename N> fraction<Z, N> operator + (const fraction<Z, N> &, const fraction<Z, N> &);
    template <typename Z, typename N> fraction<Z, N> operator - (const fraction<Z, N> &, const fraction<Z, N> &);
    template <typename Z, typename N> fraction<Z, N> operator * (const fraction<Z, N> &, const fraction<Z, N> &);

    template <typename Z, typename N> fraction<Z, N> operator / (const fraction<Z, N> &, const fraction<Z, N> &);

    namespace def {
        template <Ordered Z, Ordered N> struct sign<fraction<Z, N>> {
            math::sign operator () (const fraction<Z, N> &x);
        };

        template <typename Z, typename N>
        struct identity<plus<fraction<Z, N>>, fraction<Z, N>> : identity<plus<Z>, Z> {
            fraction<Z, N> operator () ();
        };

        template <typename Z, typename N>
        struct identity<times<fraction<Z, N>>, fraction<Z, N>> : identity<times<Z>, Z> {
            fraction<Z, N> operator () ();
        };

        template <typename Z, typename N>
        struct inverse<plus<fraction<Z, N>>, fraction<Z, N>> : inverse<plus<Z>, Z> {
            fraction<Z, N> operator () (const fraction<Z, N> &a, const fraction<Z, N> &b);
        };

        template <typename Z, typename N> struct plus<fraction<Z, N>> {
            fraction<Z, N> operator () (const fraction<Z, N> &, const fraction<Z, N> &);
        };

        template <typename Z, typename N> struct times<fraction<Z, N>> {
            fraction<Z, N> operator () (const fraction<Z, N> &, const fraction<Z, N> &);
            nonzero<fraction<Z, N>> operator () (const nonzero<fraction<Z, N>> &, const nonzero<fraction<Z, N>> &);
        };

        template <typename Z, typename N> struct inverse<times<fraction<Z, N>>, fraction<Z, N>>;

        template <typename Z, typename N> struct divide<fraction<Z, N>> {
            fraction<Z, N> operator () (const fraction<Z, N> &, const nonzero<fraction<Z, N>> &);
        };

        template <typename Z, typename N> struct conjugate<fraction<Z, N>> {
            fraction<Z, N> operator () (const fraction<Z, N> &x);
        };

        template <typename Z, typename N> requires requires (const Z &z) {
            { data::abs (z) };
        } struct abs<fraction<Z, N>> {
            auto operator () (const fraction<Z, N> &x);
        };

        template <typename Z> requires requires (const Z &x, const Z &y) {
            { inner<Z> {} (x, y) };
        } struct norm<fraction<Z, N>> {
            auto operator () (const fraction<Z, N> &);
        };

        // a way of constructing fractions.
        template <typename Z, typename N = decltype (quadrance (std::declval<Z> ()))> struct over;
    }

    template <typename Z, typename N>
    std::ostream &operator << (std::ostream &o, const fraction<Z, N> &x);

    template <typename Z, typename N> requires integral_domain<Z> && ImplicitlyConvertible<N, Z>
    struct fraction {

        Z Numerator;
        nonzero<N> Denominator;

        bool valid () const {
            return data::valid (Numerator) && data::valid (Denominator);
        }

        fraction ();

        template <typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
        fraction (ZZ n);

        template <typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
        fraction (ZZ n, ZZ d);

        bool operator == (const Z &z) const;

        fraction operator + (const Z &z) const;
        fraction operator - (const Z &z) const;
        fraction operator * (const Z &z) const;
        fraction operator / (const Z &z) const;

        fraction &operator += (const fraction &f);
        fraction &operator -= (const fraction &f);
        fraction &operator *= (const fraction &f);
        fraction &operator /= (const fraction &f);

        // only use this if your fraction is already in lowest terms.
        fraction (Z n, nonzero<N> d) : Numerator {n}, Denominator {d} {}
        friend struct def::over<Z, N>;
    };

}

namespace data {

    template <typename Z, typename N>
    math::fraction<Z, N> inline over (const Z &numerator, const Z &denominator) {
        return math::def::over<Z, N> {} (numerator, denominator);
    }

}

namespace data::math::def {

    template <Integer Z, typename N> struct over<Z, N> {
        fraction<Z, N> operator () (const Z &numerator, const Z &denominator) {
            if (denominator == 0) throw division_by_zero {};
            if (numerator == 0) return fraction {Z {0}, nonzero<N> {N {1u}}};
            N dabs = data::abs (denominator);
            N gcd_ab = number::euclidian::extended<N>::algorithm (data::abs (numerator), dabs).GCD;
            fraction x {numerator * (denominator < 0 ? -1 : 1) / Z (gcd_ab), nonzero<N> (dabs / gcd_ab)};
            return denominator < 0 ? -x: x;
        }
    };

    template <Integer Z, typename N>
    struct over<complex<Z>, N> {
        fraction<complex<Z>, N> operator () (const complex<Z> &numerator, const N &denominator);
        fraction<complex<Z>, N> operator () (const complex<Z> &numerator, const complex<Z> &denominator);
    };

    template <Integer Z, typename N>
    struct over<quaternion<Z>, N> {
        fraction<quaternion<Z>, N> operator () (const quaternion<Z> &numerator, const N &denominator);
        fraction<quaternion<Z>, N> operator () (const quaternion<Z> &numerator, const quaternion<Z> &denominator);
    };

    template <Integer Z, typename N>
    struct over<octonion<Z>, N> {
        fraction<octonion<Z>, N> operator () (const octonion<Z> &numerator, const N &denominator);
        fraction<octonion<Z>, N> operator () (const octonion<Z> &numerator, const octonion<Z> &denominator);
    };
}

namespace data::math {

    template <typename Z, typename N>
    std::ostream inline &operator << (std::ostream &o, const fraction<Z, N> &x) {
        if (x.Denominator.Value == 1) return o << x.Numerator;
        return o << "(" << x.Numerator << " / " << x.Denominator.Value << ")";
    }

    template <typename Z, typename N> requires integral_domain<Z> && ImplicitlyConvertible<N, Z>
    inline fraction<Z, N>::fraction () : Numerator {0}, Denominator {1u} {}

    template <typename Z, typename N> requires integral_domain<Z> && ImplicitlyConvertible<N, Z>
    template <typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
    inline fraction<Z, N>::fraction (ZZ n, ZZ d) : fraction (data::over<Z> (Z (n), Z (d))) {}

    template <typename Z, typename N> requires integral_domain<Z> && ImplicitlyConvertible<N, Z>
    template <typename ZZ> requires ImplicitlyConvertible<ZZ, Z>
    inline fraction<Z, N>::fraction (ZZ n) : Numerator {Z (n)}, Denominator {1u} {}

    template <Ordered Z, Ordered N>
    auto inline operator <=> (const fraction<Z, N> &x, const fraction<Z, N> &y) {
        return x.Numerator * static_cast<Z> (y.Denominator.Value) <=> static_cast<Z> (y.Numerator * x.Denominator.Value);
    }
}

namespace data::math::def {

    template <Ordered Z, Ordered N>
    math::sign inline sign<fraction<Z, N>>::operator () (const fraction<Z, N> &x) {
        return x == 0 ? zero : x < fraction<Z, N> {0} ? negative : positive;
    }

    template <typename Z, typename N>
    fraction<Z, N> inline identity<plus<fraction<Z, N>>, fraction<Z, N>>::operator () () {
        return fraction<Z, N> {identity<plus<Z>, Z>::operator () ()};
    }

    template <typename Z, typename N>
    fraction<Z, N> inline identity<times<fraction<Z, N>>, fraction<Z, N>>::operator () () {
        return fraction<Z, N> {identity<times<Z>, Z>::operator () ()};
    }

    template <typename Z, typename N> struct inverse<times<fraction<Z, N>>, fraction<Z, N>> {
        nonzero<fraction<Z, N>> operator () (const nonzero<fraction<Z, N>> &x) const {
            if (x.Value.Numerator == 0) throw division_by_zero {};
            return nonzero {fraction<Z, N> {
                Z (x.Value.Denominator.Value) * static_cast<int> (data::sign (x.Value.Numerator)),
                nonzero {data::abs (x.Value.Numerator)}}};
        }

        nonzero<fraction<Z, N>> operator () (const nonzero<fraction<Z, N>> &a, const nonzero<fraction<Z, N>> &b) const {
            return nonzero {b.Value / a.Value};
        }
    };

    template <typename Z, typename N>
    fraction<Z, N> inline inverse<plus<fraction<Z, N>>, fraction<Z, N>>::operator () (const fraction<Z, N> &a, const fraction<Z, N> &b) {
        return b - a;
    }

    template <typename Z, typename N> requires requires (const Z &z) {
        { data::abs (z) };
    } auto inline abs<fraction<Z, N>>::operator () (const fraction<Z, N> &x) {
        return fraction<Z, N> {Z (data::abs (x.Numerator)), math::nonzero<N> {N (data::abs (x.Denominator.Value))}};
    }

    template <typename Z, typename N>
    fraction<Z, N> inline conjugate<fraction<Z, N>>::operator () (const fraction<Z, N> &x) {
        return x;
    }

}

namespace data::math {

    template <typename Z, typename N>
    bool inline operator == (const fraction<Z, N> &a, const fraction<Z, N> &b) {
        return a <=> b == 0;
    }

    template <typename Z, typename N> requires integral_domain<Z> && ImplicitlyConvertible<N, Z>
    bool inline fraction<Z, N>::operator == (const Z &z) const {
        return *this == fraction {z};
    }

    template <typename Z, typename N>
    fraction<Z, N> inline operator - (const fraction<Z, N> &x) {
        auto z = x;
        z.Numerator = -z.Numerator;
        return z;
    }

    template <typename Z, typename N>
    fraction<Z, N> inline operator + (const fraction<Z, N> &a, const fraction<Z, N> &b) {
        return def::over<Z, N> {} (b.Numerator * a.Denominator.Value + a.Numerator * b.Denominator.Value,
            Z (a.Denominator.Value * b.Denominator.Value));
    }

    template <typename Z, typename N>
    fraction<Z, N> inline operator - (const fraction<Z, N> &a, const fraction<Z, N> &b) {
        return a + (-b);
    }

    template <typename Z, typename N>
    fraction<Z, N> inline operator * (const fraction<Z, N> &a, const fraction<Z, N> &b) {
        return fraction<Z> {a.Numerator * b.Numerator, Z (a.Denominator.Value * b.Denominator.Value)};
    }

    template <typename Z, typename N> fraction<Z, N> operator / (const fraction<Z, N> &a, const fraction<Z, N> &b) {
        return a * def::inverse<def::times<fraction<Z>>, fraction<Z>> {} (nonzero {b}).Value;
    }

    template <typename Z, typename N> requires integral_domain<Z> && ImplicitlyConvertible<N, Z>
    fraction<Z, N> inline fraction<Z, N>::operator + (const Z &z) const {
        return operator + (fraction {z});
    }

    template <typename Z, typename N> requires integral_domain<Z> && ImplicitlyConvertible<N, Z>
    fraction<Z, N> fraction<Z, N>::operator - (const Z &z) const {
        return operator - (fraction {z});
    }

    template <typename Z, typename N> requires integral_domain<Z> && ImplicitlyConvertible<N, Z>
    fraction<Z, N> inline fraction<Z, N>::operator * (const Z &z) const {
        return *this * fraction {z};
    }

    template <typename Z, typename N> requires integral_domain<Z> && ImplicitlyConvertible<N, Z>
    fraction<Z, N> inline fraction<Z, N>::operator / (const Z &z) const {
        return operator / (fraction {z});
    }

    template <typename Z, typename N> requires integral_domain<Z> && ImplicitlyConvertible<N, Z>
    fraction<Z, N> inline &fraction<Z, N>::operator += (const fraction &f) {
        return *this = (operator + (f));
    }

    template <typename Z, typename N> requires integral_domain<Z> && ImplicitlyConvertible<N, Z>
    fraction<Z, N> inline &fraction<Z, N>::operator -= (const fraction &f) {
        return *this = (operator - (f));
    }

    template <typename Z, typename N> requires integral_domain<Z> && ImplicitlyConvertible<N, Z>
    fraction<Z, N> inline &fraction<Z, N>::operator *= (const fraction &f) {
        return *this = (operator * (f));
    }

    template <typename Z, typename N> requires integral_domain<Z> && ImplicitlyConvertible<N, Z>
    fraction<Z, N> inline &fraction<Z, N>::operator /= (const fraction &f) {
        return *this = (operator / (f));
    }
}

#endif
