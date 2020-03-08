// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_RATIONAL
#define DATA_MATH_NUMBER_RATIONAL

#include <data/math/number/abs.hpp>
#include <data/math/division.hpp>
#include <data/math/number/extended_euclidian.hpp>
#include <data/math/field.hpp>
#include <data/math/ordered.hpp>

namespace data::math::number {
    template <typename Q>
    struct rational : interface::field<Q, plus<Q>, times<Q>>, interface::ordered<Q> {};
        
    template <typename N>
    struct positive {
        N Number;
        
        positive(N n) : Number{n} {}
        
        bool valid() const {
            return Number > 0;
        }
        
        bool operator==(const positive& p) const {
            return Number == p.Number;
        }
        
        bool operator!=(const positive& p) const {
            return Number != p.Number;
        }
        
        positive operator*(const positive& p) const {
            return positive{Number * p.Number};
        }
        
        positive operator/(const positive& p) const {
            return positive{Number / p.Number};
        }
    };
    
    // TODO: note that N is the type of abs(Z)
    // I don't have a way of doing that yet. 
    template <typename Z, typename N>
    struct fraction : interface::ring<Z, data::plus<Z>, data::times<Z>>, interface::ordered<Z> {
        Z Numerator;
        positive<N> Denominator;
        
        fraction(Z n, positive<N> d) : Numerator{n}, Denominator{d} {}
        fraction(Z n, N d) : Numerator{n}, Denominator{positive{d}} {}
        fraction() : Numerator{0}, Denominator{1} {}
        fraction(Z n) : Numerator{n}, Denominator{uint64{1}} {}
        fraction(const fraction& f) : Numerator{f.Numerator}, Denominator{f.Denominator} {}
        
        bool valid() const {
            return data::valid(Numerator) && Denominator.valid();
        }
        
        static N gcd(N a, N b) {
            return euclidian::extended<N, Z>::algorithm(a, b).GCD;
        }
        
        static fraction divide(Z n, positive<N> d) {
            if (n == 0) return fraction{Z{0}, N{1}};
            N gcd_ab = gcd(abs<N, Z>{}(n), d.Number);
            return fraction{n / Z{gcd_ab}, positive{d / gcd_ab}};
        }
        
        bool operator==(const fraction& f) const {
            return Numerator == f.Numerator && (Numerator == 0 || Denominator == f.Denominator);
        }
        
        bool operator==(const Z& z) const {
            return operator==(fraction{z});
        }
        
        static fraction inverse(const fraction& f) {
            if (f == 0) throw division_by_zero{};
            return fraction{Z{f.Denominator.Number} * arg<Z>{}(f.Numerator), positive{abs<N, Z>{}(f.Numerator)}};
        }
        
        bool operator!=(const fraction& f) const {
            return !operator==(f);
        }
        
        bool operator<(const fraction& f) const;
        
        bool operator>(const fraction& f) const{
            return Numerator * f.Denominator.Number > Denominator.Number * f.Numerator;
        }
        
        bool operator<=(const fraction& f) const{
            return Numerator * f.Denominator.Number <= Denominator.Number * f.Numerator;
        }
        
        bool operator>=(const fraction& f) const{
            return Numerator * f.Denominator.Number >= Denominator.Number * f.Numerator;
        }
        
        fraction operator-() const {
            return {-Numerator, Denominator};
        }
        
        fraction operator+(const fraction& f) const {
            N gcd_rr = gcd(Denominator.Number, f.Denominator.Number);
            return (fraction{f.Numerator * Z{Denominator.Number} + Numerator * Z{f.Denominator.Number}} / gcd_rr) /  
                (fraction{Denominator.Number * f.Denominator.Number} / gcd_rr);
        }
        
        fraction operator+(const Z& z) const {
            return operator+(fraction{z});
        }
        
        fraction operator-(const fraction& f) const {
            return (*this)+(-f);
        }
        
        fraction operator-(const Z& z) const {
            return operator-(fraction{z});
        }
        
        fraction operator*(const fraction& f) const {
            return divide(Numerator * f.Numerator, Denominator * f.Denominator);
        }
        
        fraction operator*(const Z& z) const {
            return operator*(fraction{z});
        }
        
        fraction operator/(const fraction& f) const {
            return (*this)*inverse(f);
        }
        
        fraction operator/(const Z& z) const {
            return operator/(fraction{z});
        }
        
        fraction operator/(const N& n) const {
            return fraction{Numerator, Denominator * n};
        }
        
        fraction& operator=(const fraction& f) {
            Numerator = f.Numerator;
            Denominator = f.Denominator;
        }
        
        fraction& operator+=(const fraction& f) {
            return operator=(operator+(f));
        }
        
        fraction& operator-=(const fraction& f) {
            return operator=(operator-(f));
        }
        
        fraction& operator*=(const fraction& f) {
            return operator=(operator*(f));
        }
        
        fraction& operator/=(const fraction& f) {
            return operator=(operator/(f));
        }
        
    };
    
}

// TODO fill in these types correctly. 
namespace data::math {
    template <typename Z, typename N> 
    struct commutative<data::plus<number::fraction<Z, N>>, number::fraction<Z, N>>;
    
    template <typename Z, typename N> 
    struct associative<data::plus<number::fraction<Z, N>>, number::fraction<Z, N>>;
    
    template <typename Z, typename N> 
    struct commutative<data::times<number::fraction<Z, N>>, number::fraction<Z, N>>;
    
    template <typename Z, typename N> 
    struct associative<data::times<number::fraction<Z, N>>, number::fraction<Z, N>>;
}

template <typename Z, typename N>
inline std::ostream& operator<<(std::ostream& o, const data::math::number::fraction<N, Z>& x) {
    return o << "fraction{" << x.Numerator << ", " << x.Denominator.Number << "}";
}

namespace data::math::number {
    template <typename Z, typename N>
    inline bool fraction<Z, N>::operator<(const fraction& f) const {
        return Numerator * f.Denominator.Number < Denominator.Number * f.Numerator;
    }
}

#endif
