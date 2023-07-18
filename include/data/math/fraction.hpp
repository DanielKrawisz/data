// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_FRACTION
#define DATA_MATH_FRACTION

#include <data/math/number/rational.hpp>
#include <data/math/nonnegative.hpp>
#include <data/math/number/extended_euclidian.hpp>

namespace data::math {
    
    // TODO: note that N is the type of abs(Z)
    // I don't have a way of doing that yet. 
    template <typename Z, typename N = Z>
    requires ring<Z, plus<Z>, times<Z>> && ordered<Z>
    struct fraction {
        Z Numerator;
        nonzero<N> Denominator;
        
        bool valid() const {
            return data::valid (Numerator) && Denominator.valid ();
        }
        
        fraction() : Numerator {0}, Denominator {1} {}
        
        math::sign sign () const {
            return data::sign (Numerator);
        }
        
    private:
        fraction (Z n, nonzero<N> d) : Numerator {n}, Denominator {d} {}
        
        static N gcd (N a, N b) {
            return number::euclidian::extended<Z, N>::algorithm (a, b).GCD;
        }
        
        static fraction divide(Z n, N d) {
            if (d == 0) return fraction {Z {0}, nonzero<N> {N {0}}}; // Invalid value.
            if (n == 0) return fraction {Z {0}, nonzero<N> {N {1}}};
            N gcd_ab = gcd (data::abs(n), d);
            return fraction (n / Z (gcd_ab), nonzero (d / gcd_ab));
        }
        
    public:
        fraction (Z n, N d) : fraction (divide (n, d)) {}
        fraction (Z n) : Numerator {n}, Denominator {uint64 {1}} {}
        
        bool operator == (const fraction &f) const {
            return Numerator == f.Numerator && (Numerator == 0 || Denominator == f.Denominator);
        }
        
        std::weak_ordering operator <=> (const fraction &f) const {
            return Numerator * f.Denominator.Value <=> f.Numerator * Denominator.Value;
        }
        
        bool operator == (const Z &z) const {
            return operator == (fraction {z});
        }
        
        static ptr<fraction> inverse (const fraction &f) {
            if (f == 0) return nullptr;
            return std::make_shared<fraction> (
                fraction {Z {f.Denominator} * data::sign (f.Numerator), nonzero {data::abs (f.Numerator)}});
        }
        
        fraction operator - () const {
            return {-Numerator, Denominator};
        }
        
        fraction operator + (const fraction &f) const {
            return divide (f.Numerator * Denominator.Value + Numerator * f.Denominator.Value,
                Denominator.Value * f.Denominator.Value);
        }
        
        fraction operator + (const Z &z) const {
            return operator + (fraction {z});
        }
        
        fraction operator - (const fraction& f) const {
            return (*this)+(-f);
        }
        
        fraction operator - (const Z &z) const {
            return operator - (fraction {z});
        }
        
        fraction operator * (const fraction &f) const {
            return fraction {Numerator * f.Numerator, Denominator * f.Denominator};
        }
        
        fraction operator * (const Z &z) const {
            return *this * fraction {z};
        }
        
        fraction operator / (const nonzero<fraction> &f) const {
            if (f.Value == fraction {0}) throw division_by_zero {};
            return (*this) * *inverse (f);
        }
        
        fraction operator / (const Z &z) const {
            return operator/(fraction {z});
        }
        
        fraction operator / (const N &n) const {
            return fraction {Numerator, Denominator * n};
        }
        
        nonnegative<fraction> quadrance () const {
            return fraction {Numerator * Numerator, Denominator * Denominator};
        }
        
        fraction& operator = (const fraction& f) {
            Numerator = f.Numerator;
            Denominator = f.Denominator;
        }
        
        fraction& operator += (const fraction &f) {
            return operator = (operator + (f));
        }
        
        fraction& operator -= (const fraction &f) {
            return operator = (operator-(f));
        }
        
        fraction& operator *= (const fraction &f) {
            return operator = (operator * (f));
        }
        
        fraction& operator /= (const fraction &f) {
            return operator = (operator / (f));
        }
    };

    template <typename Z, typename N>
    std::ostream inline &operator << (std::ostream& o, const fraction<N, Z>& x) {
        return o << "fraction {" << x.Numerator << ", " << x.Denominator.Value << "}";
    }
    
}

namespace data::math {
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
        fraction<Z, N> operator () () {
            return fraction<Z, N> {identity<plus<Z>, Z>::operator () ()};
        }
    };
    
    template <typename Z, typename N> 
    struct identity<times<fraction<Z, N>>, fraction<Z, N>> : identity<times<Z>, Z> {
        fraction<Z, N> operator () () {
            return fraction<Z, N> {identity<times<Z>, Z>::operator () ()};
        }
    };
    
    template <typename Z, typename N> 
    struct inverse<plus<fraction<Z, N>>, fraction<Z, N>> : inverse<plus<Z>, Z> {
        fraction<Z, N> operator () (const fraction<Z, N> &a, const fraction<Z, N> &b) {
            return b - a;
        }
    };
}

#endif
