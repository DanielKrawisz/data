// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_FRACTION
#define DATA_MATH_FRACTION

#include <data/math/number/rational.hpp>
#include <data/math/nonnegative.hpp>
#include <data/math/number/extended_euclidian.hpp>

namespace data::math {

    template <typename Z, typename N = decltype (data::abs (std::declval<Z> ()))>
    requires ring<Z, plus<Z>, times<Z>> && ordered<Z> struct fraction;

    template <typename Z, typename N>
    bool operator == (const fraction<Z, N> &, const fraction<Z, N> &);

    template <typename Z, typename N>
    std::weak_ordering operator <=> (const fraction<Z, N> &, const fraction<Z, N> &);

    template <typename Z> fraction<Z> operator - (const fraction<Z> &);
    template <typename Z> fraction<Z> operator + (const fraction<Z> &, const fraction<Z> &);
    template <typename Z> fraction<Z> operator - (const fraction<Z> &, const fraction<Z> &);
    template <typename Z> fraction<Z> operator * (const fraction<Z> &, const fraction<Z> &);

    template <typename Z, typename N>
    struct commutative<plus<fraction<Z, N>>, fraction<Z, N>> : commutative<plus<Z>, Z> {};

    template <typename Z, typename N>
    struct associative<plus<fraction<Z, N>>, fraction<Z, N>> : associative<plus<Z>, Z> {};

    template <typename Z, typename N>
    struct commutative<times<fraction<Z, N>>, fraction<Z, N>> : commutative<times<Z>, Z> {};

    template <typename Z, typename N>
    struct associative<times<fraction<Z, N>>, fraction<Z, N>> : associative<times<Z>, Z> {};

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

    template <ring Z, typename N> struct inverse<plus<fraction<Z, N>>, fraction<Z, N>> : inverse<plus<Z>, Z> {
        fraction<Z, N> operator () (const fraction<Z, N> &a, const fraction<Z, N> &b);
    };

    template <integral_domain Z> struct times<fraction<Z>> {
        fraction<Z> operator () (const fraction<Z> &, const fraction<Z> &);
        nonzero<fraction<Z>> operator () (const nonzero<fraction<Z>> &, const nonzero<fraction<Z>> &);
    };

    template <integral_domain Z> struct inverse<times<fraction<Z>>, fraction<Z>> {
        nonzero<fraction<Z>> operator () (const nonzero<fraction<Z>> &);
        nonzero<fraction<Z>> operator () (const nonzero<fraction<Z>> &, const nonzero<fraction<Z>> &);
    };

    template <integral_domain Z> struct divide<fraction<Z>> {
        fraction<Z> operator () (const fraction<Z> &, const nonzero<fraction<Z>> &);
    };

    template <integral_domain Z> fraction<Z> operator / (const fraction<Z> &, const nonzero<fraction<Z>> &);

    template <typename Z, typename N> struct conjugate<fraction<Z, N>> {
        fraction<Z, N> operator () (const fraction<Z, N> &x);
    };

    template <typename Z, typename N>
    std::ostream &operator << (std::ostream &o, const fraction<Z, N> &x);
    
    template <typename Z, typename N>
    requires ring<Z, plus<Z>, times<Z>> && ordered<Z>
    struct fraction {
        Z Numerator;
        nonzero<N> Denominator;
        
        bool valid () const {
            return data::valid (Numerator) && data::valid (Denominator);
        }
        
        signature sign () const {
            return data::sign (Numerator);
        }
        
    private:
        fraction (Z n, nonzero<N> d) : Numerator {n}, Denominator {d} {}
        
        static N gcd (N a, N b) {
            return number::euclidian::extended<Z, N>::algorithm (a, b).GCD;
        }

    public:
        // reduce to lowest terms.
        static fraction divide (Z n, N d) {
            if (d == 0) return fraction {Z {0}, nonzero<N> {N {0u}}}; // Invalid value.
            if (n == 0) return fraction {Z {0}, nonzero<N> {N {1u}}};
            N gcd_ab = gcd (data::abs (n), d);
            return fraction (n / Z (gcd_ab), nonzero (d / gcd_ab));
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

        friend class inverse<times<fraction<Z>>, fraction<Z>>;
        friend struct abs<fraction<Z, N>>;

    };

    template <typename Z, typename N>
    std::ostream inline &operator << (std::ostream &o, const fraction<Z, N> &x) {
        if (x.Denominator.Value == 1) return o << x.Numerator;
        return o << "(" << x.Numerator << " / " << x.Denominator.Value << ")";
    }

    template <typename Z, typename N>
    struct abs<fraction<Z, N>> {
        nonnegative<fraction<Z, N>> operator () (const fraction<Z, N> &x);
    };

    template <typename Z, typename N>
    struct quadrance<fraction<Z, N>> {
        nonnegative<fraction<Z, N>> operator () (const fraction<Z, N> &x);
    };

    template <typename Z, typename N>
    inline fraction<Z, N>::fraction () : Numerator {0}, Denominator {1u} {}

    template <typename Z, typename N>
    template <typename ZZ, typename NN> requires implicitly_convertible_to<ZZ, Z> && implicitly_convertible_to<NN, N>
    inline fraction<Z, N>::fraction (ZZ n, NN d) : fraction (divide (Z (n), N (d))) {}

    template <typename Z, typename N>
    template <typename ZZ> requires implicitly_convertible_to<ZZ, Z>
    inline fraction<Z, N>::fraction (ZZ n) : Numerator {Z (n)}, Denominator {1u} {}

    template <typename Z, typename N>
    bool inline operator == (const fraction<Z, N> &a, const fraction<Z, N> &b) {
        return a <=> b == 0;
    }

    template <typename Z, typename N>
    std::weak_ordering inline operator <=> (const fraction<Z, N> &a, const fraction<Z, N> &b) {
        return a.Numerator * b.Denominator.Value <=> b.Numerator * a.Denominator.Value;
    }

    template <typename Z, typename N>
    bool inline fraction<Z, N>::operator == (const Z &z) const {
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

    template <integral_domain Z>
    nonzero<fraction<Z>> inverse<times<fraction<Z>>, fraction<Z>>::operator () (const nonzero<fraction<Z>> &x) {
        if (x.Value.Numerator == 0) throw division_by_zero {};
        return nonzero {fraction<Z> {Z (x.Value.Denominator.Value) * data::sign (x.Value.Numerator), nonzero {data::abs (x.Value.Numerator)}}};
    }

    template <integral_domain Z>
    nonzero<fraction<Z>> inverse<times<fraction<Z>>, fraction<Z>>::operator () (const nonzero<fraction<Z>> &a, const nonzero<fraction<Z>> &b) {
        return nonzero {b.Value / a.Value};
    }

    template <integral_domain Z> fraction<Z> operator / (const fraction<Z> &a, const fraction<Z> &b) {
        return a * inverse<times<fraction<Z>>, fraction<Z>> {} (nonzero {b}).Value;
    }

    template <typename Z, typename N>
    fraction<Z, N> inline fraction<Z, N>::operator + (const Z &z) const {
        return operator + (fraction {z});
    }

    template <typename Z, typename N>
    fraction<Z, N> fraction<Z, N>::operator - (const Z &z) const {
        return operator - (fraction {z});
    }

    template <typename Z, typename N>
    fraction<Z, N> inline fraction<Z, N>::operator * (const Z &z) const {
        return *this * fraction {z};
    }

    template <typename Z, typename N>
    fraction<Z, N> inline fraction<Z, N>::operator / (const Z &z) const {
        return operator / (fraction {z});
    }

    template <typename Z, typename N>
    fraction<Z, N> inline &fraction<Z, N>::operator += (const fraction &f) {
        return *this = (operator + (f));
    }

    template <typename Z, typename N>
    fraction<Z, N> inline &fraction<Z, N>::operator -= (const fraction &f) {
        return *this = (operator - (f));
    }

    template <typename Z, typename N>
    fraction<Z, N> inline &fraction<Z, N>::operator *= (const fraction &f) {
        return *this = (operator * (f));
    }

    template <typename Z, typename N>
    fraction<Z, N> inline &fraction<Z, N>::operator /= (const fraction &f) {
        return *this = (operator / (f));
    }

    template <typename Z, typename N>
    fraction<Z, N> inline identity<plus<fraction<Z, N>>, fraction<Z, N>>::operator () () {
        return fraction<Z, N> {identity<plus<Z>, Z>::operator () ()};
    }

    template <typename Z, typename N>
    fraction<Z, N> inline identity<times<fraction<Z, N>>, fraction<Z, N>>::operator () () {
        return fraction<Z, N> {identity<times<Z>, Z>::operator () ()};
    }

    template <typename Z, typename N>
    fraction<Z, N> inline inverse<plus<fraction<Z, N>>, fraction<Z, N>>::operator () (const fraction<Z, N> &a, const fraction<Z, N> &b) {
        return b - a;
    }

    template <typename Z, typename N>
    nonnegative<fraction<Z, N>> inline abs<fraction<Z, N>>::operator () (const fraction<Z, N> &x) {
        return fraction {x.Numerator < 0 ? -x.Numerator : x.Numerator, x.Denominator };
    }

    template <typename Z, typename N>
    nonnegative<fraction<Z, N>> inline quadrance<fraction<Z, N>>::operator () (const fraction<Z, N> &x) {
        return fraction {x.Numerator * x.Numerator, x.Denominator * x.Denominator};
    }

    template <typename Z, typename N>
    fraction<Z, N> inline conjugate<fraction<Z, N>>::operator () (const fraction<Z, N> &x) {
        return x;
    }

}

#endif
