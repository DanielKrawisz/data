// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_FRACTION
#define DATA_MATH_FRACTION

#include <data/norm.hpp>
#include <data/math/number/rational.hpp>
#include <data/math/nonnegative.hpp>
#include <data/math/number/extended_euclidian.hpp>
#include <compare>

namespace data::math {

    // fraction is capable of taking any normed integral domain
    // (such as the integers or the gaussian integers)
    // and turning it into its fraction field. We can also use
    // it on the unit quaternions to form the rational quaternions
    // and same with octonions.
    template <typename Z> requires integral_domain<Z> && normed<Z> struct fraction;

    template <typename Z>
    bool operator == (const fraction<Z> &, const fraction<Z> &);

    template <ordered Z>
    auto operator <=> (const fraction<Z> &x, const fraction<Z> &y);

    template <ordered Z> struct sign<fraction<Z>> {
        signature operator () (const fraction<Z> &x) {
            return x == 0 ? zero : x < fraction<Z> {0} ? negative : positive;
        }
    };

    template <typename Z> fraction<Z> operator - (const fraction<Z> &);
    template <typename Z> fraction<Z> operator + (const fraction<Z> &, const fraction<Z> &);
    template <typename Z> fraction<Z> operator - (const fraction<Z> &, const fraction<Z> &);
    template <typename Z> fraction<Z> operator * (const fraction<Z> &, const fraction<Z> &);

    template <typename Z> fraction<Z> operator / (const fraction<Z> &, const nonzero<fraction<Z>> &);

    template <typename Z>
    struct identity<plus<fraction<Z>>, fraction<Z>> : identity<plus<Z>, Z> {
        fraction<Z> operator () ();
    };

    template <typename Z>
    struct identity<times<fraction<Z>>, fraction<Z>> : identity<times<Z>, Z> {
        fraction<Z> operator () ();
    };

    template <typename Z>
    struct inverse<plus<fraction<Z>>, fraction<Z>> : inverse<plus<Z>, Z> {
        fraction<Z> operator () (const fraction<Z> &a, const fraction<Z> &b);
    };

    template <typename Z> struct times<fraction<Z>> {
        fraction<Z> operator () (const fraction<Z> &, const fraction<Z> &);
        nonzero<fraction<Z>> operator () (const nonzero<fraction<Z>> &, const nonzero<fraction<Z>> &);
    };

    template <typename Z> struct inverse<times<fraction<Z>>, fraction<Z>>;

    template <typename Z> struct divide<fraction<Z>> {
        fraction<Z> operator () (const fraction<Z> &, const nonzero<fraction<Z>> &);
    };

    template <typename Z> struct conjugate<fraction<Z>> {
        fraction<Z> operator () (const fraction<Z> &x);
    };

    template <typename Z> requires requires (const Z &z) {
        { abs<Z> {} (z) };
    } struct abs<fraction<Z>> {
        auto operator () (const fraction<Z> &x);
    };

    template <typename Z> requires requires (const Z &z) {
        { norm<Z> {} (z) };
    } struct norm<fraction<Z>> {
        fraction<decltype (quadrance<Z> {} (std::declval<Z> ()))> operator () (const fraction<Z> &);
    };

    template <typename Z>
    std::ostream &operator << (std::ostream &o, const fraction<Z> &x);
    
    template <typename Z>
    requires integral_domain<Z> && normed<Z>
    struct fraction {
        using N = decltype (data::norm (std::declval<Z> ()));

        Z Numerator;
        nonzero<N> Denominator;
        
        bool valid () const {
            return data::valid (Numerator) && data::valid (Denominator);
        }
        
    private:
        
        static N gcd (N a, N b) {
            return number::euclidian::extended<N, Z>::algorithm (a, b).GCD;
        }

    public:
        // reduce to lowest terms.
        static fraction divide (Z n, N d) {
            if (d == 0) return fraction {Z {0}, nonzero<N> {N {0u}}}; // Invalid value.
            if (n == Z (0)) return fraction {Z {0}, nonzero<N> {N {1u}}};
            N gcd_ab = gcd (data::abs (n), d);
            return fraction (n / Z (gcd_ab), nonzero<N> (d / gcd_ab));
        }

        fraction ();

        template <typename ZZ> requires implicitly_convertible_to<ZZ, Z>
        fraction (ZZ n);

        template <typename ZZ, typename NN> requires implicitly_convertible_to<ZZ, Z> && implicitly_convertible_to<NN, N>
        fraction (ZZ n, NN d);
        
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

    };

    template <typename Z> struct inverse<times<fraction<Z>>, fraction<Z>> {
        nonzero<fraction<Z>> operator () (const nonzero<fraction<Z>> &x) const {
            if (x.Value.Numerator == 0) throw division_by_zero {};
            return nonzero {fraction<Z> {Z (x.Value.Denominator.Value) * data::sign (x.Value.Numerator), nonzero {data::abs (x.Value.Numerator)}}};
        }

        nonzero<fraction<Z>> operator () (const nonzero<fraction<Z>> &a, const nonzero<fraction<Z>> &b) const {
            return nonzero {b.Value / a.Value};
        }
    };

    template <typename Z>
    std::ostream inline &operator << (std::ostream &o, const fraction<Z> &x) {
        if (x.Denominator.Value == 1) return o << x.Numerator;
        return o << "(" << x.Numerator << " / " << x.Denominator.Value << ")";
    }

    template <typename Z>
    requires integral_domain<Z> && normed<Z>
    inline fraction<Z>::fraction () : Numerator {0}, Denominator {1u} {}

    template <typename Z>
    requires integral_domain<Z> && normed<Z>
    template <typename ZZ, typename NN>
    requires implicitly_convertible_to<ZZ, Z> && implicitly_convertible_to<NN, typename fraction<Z>::N>
    inline fraction<Z>::fraction (ZZ n, NN d) : fraction (divide (Z (n), N (d))) {}

    template <typename Z>
    requires integral_domain<Z> && normed<Z>
    template <typename ZZ> requires implicitly_convertible_to<ZZ, Z>
    inline fraction<Z>::fraction (ZZ n) : Numerator {Z (n)}, Denominator {1u} {}

    template <typename Z>
    bool inline operator == (const fraction<Z> &a, const fraction<Z> &b) {
        return a <=> b == 0;
    }

    template <typename Z>
    std::weak_ordering inline operator <=> (const fraction<Z> &a, const fraction<Z> &b) {
        return a.Numerator * b.Denominator.Value <=> b.Numerator * a.Denominator.Value;
    }

    template <typename Z>
    requires integral_domain<Z> && normed<Z>
    bool inline fraction<Z>::operator == (const Z &z) const {
        return *this == fraction {z};
    }

    template <typename Z>
    fraction<Z> inline operator - (const fraction<Z> &x) {
        auto z = x;
        z.Numerator = -z.Numerator;
        return z;
    }

    template <typename Z>
    fraction<Z> inline operator + (const fraction<Z> &a, const fraction<Z> &b) {
        return fraction<Z>::divide (b.Numerator * a.Denominator.Value + a.Numerator * b.Denominator.Value,
            a.Denominator.Value * b.Denominator.Value);
    }

    template <typename Z>
    fraction<Z> inline operator - (const fraction<Z> &a, const fraction<Z> &b) {
        return a + (-b);
    }

    template <typename Z>
    fraction<Z> inline operator * (const fraction<Z> &a, const fraction<Z> &b) {
        return fraction<Z> {a.Numerator * b.Numerator, a.Denominator.Value * b.Denominator.Value};
    }

    template <integral_domain Z> fraction<Z> operator / (const fraction<Z> &a, const fraction<Z> &b) {
        return a * inverse<times<fraction<Z>>, fraction<Z>> {} (nonzero {b}).Value;
    }

    template <typename Z>
    requires integral_domain<Z> && normed<Z>
    fraction<Z> inline fraction<Z>::operator + (const Z &z) const {
        return operator + (fraction {z});
    }

    template <typename Z>
    requires integral_domain<Z> && normed<Z>
    fraction<Z> fraction<Z>::operator - (const Z &z) const {
        return operator - (fraction {z});
    }

    template <typename Z>
    requires integral_domain<Z> && normed<Z>
    fraction<Z> inline fraction<Z>::operator * (const Z &z) const {
        return *this * fraction {z};
    }

    template <typename Z>
    requires integral_domain<Z> && normed<Z>
    fraction<Z> inline fraction<Z>::operator / (const Z &z) const {
        return operator / (fraction {z});
    }

    template <typename Z>
    requires integral_domain<Z> && normed<Z>
    fraction<Z> inline &fraction<Z>::operator += (const fraction &f) {
        return *this = (operator + (f));
    }

    template <typename Z>
    requires integral_domain<Z> && normed<Z>
    fraction<Z> inline &fraction<Z>::operator -= (const fraction &f) {
        return *this = (operator - (f));
    }

    template <typename Z>
    requires integral_domain<Z> && normed<Z>
    fraction<Z> inline &fraction<Z>::operator *= (const fraction &f) {
        return *this = (operator * (f));
    }

    template <typename Z>
    requires integral_domain<Z> && normed<Z>
    fraction<Z> inline &fraction<Z>::operator /= (const fraction &f) {
        return *this = (operator / (f));
    }

    template <typename Z>
    fraction<Z> inline identity<plus<fraction<Z>>, fraction<Z>>::operator () () {
        return fraction<Z> {identity<plus<Z>, Z>::operator () ()};
    }

    template <typename Z>
    fraction<Z> inline identity<times<fraction<Z>>, fraction<Z>>::operator () () {
        return fraction<Z> {identity<times<Z>, Z>::operator () ()};
    }

    template <typename Z>
    fraction<Z> inline inverse<plus<fraction<Z>>, fraction<Z>>::operator () (const fraction<Z> &a, const fraction<Z> &b) {
        return b - a;
    }

    template <typename Z> requires requires (const Z &z) {
        { abs<Z> {} (z) };
    } auto inline abs<fraction<Z>>::operator () (const fraction<Z> &x) {
        using N = typename fraction<Z>::N;
        return fraction<Z> {Z (data::abs (x.Numerator)), math::nonzero<N> {N (data::abs (x.Denominator.Value))}};
    }

    template <typename Z>
    fraction<Z> inline conjugate<fraction<Z>>::operator () (const fraction<Z> &x) {
        return x;
    }

    template <typename Z> requires requires (const Z &z) {
        { norm<Z> {} (z) };
    } fraction<decltype (quadrance<Z> {} (std::declval<Z> ()))> inline norm<fraction<Z>>::operator () (const fraction<Z> &z) {
        return fraction<decltype (quadrance<Z> {} (std::declval<Z> ()))> {data::quadrance (z.Numerator), z.Denominator};
    }

    template <ordered Z>
    auto inline operator <=> (const fraction<Z> &x, const fraction<Z> &y) {
        return x.Numerator * static_cast<Z> (y.Denominator.Value) <=> static_cast<Z> (y.Numerator * x.Denominator.Value);
    }

}

#endif
