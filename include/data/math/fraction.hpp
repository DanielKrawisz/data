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
        
        math::sign sign () const {
            return data::sign (Numerator);
        }
        
    private:
        fraction (Z n, nonzero<N> d) : Numerator {n}, Denominator {d} {}
        
        static N gcd (N a, N b) {
            return number::euclidian::extended<Z, N>::algorithm (a, b).GCD;
        }

        static fraction divide (Z n, N d) {
            if (d == 0) return fraction {Z {0}, nonzero<N> {N {0u}}}; // Invalid value.
            if (n == 0) return fraction {Z {0}, nonzero<N> {N {1u}}};
            N gcd_ab = gcd (data::abs (n), d);
            return fraction (n / Z (gcd_ab), nonzero (d / gcd_ab));
        }

    public:
        fraction ();
        fraction (Z n);
        fraction (Z n, N d);
        
        bool operator == (const Z &z) const;
        
        static maybe<fraction> inverse (const fraction &f);
        
        fraction operator - () const;
        fraction operator + (const fraction &f) const;
        fraction operator + (const Z &z) const;
        fraction operator - (const fraction &f) const;
        fraction operator - (const Z &z) const;
        fraction operator * (const fraction &f) const;
        fraction operator * (const Z &z) const;
        fraction operator / (const nonzero<fraction> &f) const;
        fraction operator / (const Z &z) const;
        
        fraction &operator += (const fraction &f);
        fraction &operator -= (const fraction &f);
        fraction &operator *= (const fraction &f);
        fraction &operator /= (const fraction &f);

    };

    template <typename Z, typename N>
    std::ostream inline &operator << (std::ostream &o, const fraction<Z, N> &x) {
        return o << "fraction {" << x.Numerator << ", " << x.Denominator.Value << "}";
    }
    
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
    inline fraction<Z, N>::fraction (Z n, N d) : fraction (divide (n, d)) {}

    template <typename Z, typename N>
    inline fraction<Z, N>::fraction (Z n) : Numerator {n}, Denominator {uint64 {1}} {}

    template <typename Z, typename N>
    std::weak_ordering inline operator <=> (const fraction<Z, N> &a, const fraction<Z, N> &b) {
        return a.Numerator * b.Denominator.Value <=> b.Numerator * a.Denominator.Value;
    }

    template <typename Z, typename N>
    bool inline operator == (const fraction<Z, N> &a, const fraction<Z, N> &b) {
        return a <=> b == 0;
    }

    template <typename Z, typename N>
    maybe<fraction<Z, N>> inline fraction<Z, N>::inverse (const fraction &f) {
        if (f == 0) return nullptr;
        return std::make_shared<fraction> (
            fraction {Z {f.Denominator} * data::sign (f.Numerator), nonzero {data::abs (f.Numerator)}});
    }

    template <typename Z, typename N>
    fraction<Z, N> inline fraction<Z, N>::operator - () const {
        return {-Numerator, Denominator};
    }

    template <typename Z, typename N>
    fraction<Z, N> inline fraction<Z, N>::operator + (const fraction &f) const {
        return divide (f.Numerator * Denominator.Value + Numerator * f.Denominator.Value,
            Denominator.Value * f.Denominator.Value);
    }

    template <typename Z, typename N>
    fraction<Z, N> inline fraction<Z, N>::operator + (const Z &z) const {
        return operator + (fraction {z});
    }

    template <typename Z, typename N>
    fraction<Z, N> inline fraction<Z, N>::operator - (const fraction &f) const {
        return (*this) + (-f);
    }

    template <typename Z, typename N>
    fraction<Z, N> fraction<Z, N>::operator - (const Z &z) const {
        return operator - (fraction {z});
    }

    template <typename Z, typename N>
    fraction<Z, N> inline fraction<Z, N>::operator * (const fraction &f) const {
        return fraction {Numerator * f.Numerator, Denominator * f.Denominator};
    }

    template <typename Z, typename N>
    fraction<Z, N> inline fraction<Z, N>::operator * (const Z &z) const {
        return *this * fraction {z};
    }

    template <typename Z, typename N>
    fraction<Z, N> inline fraction<Z, N>::operator / (const nonzero<fraction> &f) const {
        auto ii = inverse (f);
        if (!bool (ii)) throw division_by_zero {};
        return (*this) * *ii;
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

}

#endif
