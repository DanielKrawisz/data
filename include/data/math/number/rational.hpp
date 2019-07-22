// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_FRACTION
#define DATA_MATH_NUMBER_FRACTION

#include <data/types.hpp>
#include <data/math/number/division.hpp>
#include <data/math/number/extended_euclidian.hpp>
#include <data/math/field.hpp>
#include <data/math/ordered.hpp>

namespace data::math {
    struct divide_by_zero : std::exception {};
    namespace number {
        template <typename Q>
        struct rational : field<Q>, ordered<Q> {};
        
        template <typename N>
        struct positive {
            N Number;
            bool valid() const {
                return Number > 0;
            }
            
            positive operator*(const positive<N>& p) const {
                return positive{Number * p.Number};
            }
            
            positive operator/(const positive<N>& p) const {
                return positive{Number / p.Number};
            }
        };
            
        template <typename Z, typename N>
        struct fraction {
            Z Numerator;
            positive<N> Denominator;
            
            fraction(Z& n, positive<N>& d) : Numerator{n}, Denominator{d} {}
            fraction() : Numerator{0}, Denominator{1} {}
            fraction(Z& n) : Numerator{n}, Denominator{1} {}
            fraction(N& n) : fraction{Z{n}} {}
            
            static N gcd(N& a, N& b) {
                return euclidian::extended<N&, Z&>::algorithm(a, b).GCD;
            }
            
            static fraction divide(Z& n, positive<N>& d) {
                if (n == 0) return fraction{0, 1};
                N gcd_ab = gcd(abs(n), d.Number);
                return fraction{n / gcd_ab, {d / gcd_ab}};
            }
            
            bool operator==(const fraction& f) const {
                return Numerator == f.Numerator && (Numerator == 0 || Denominator == f.Denominator);
            }
            
            static fraction inverse(fraction& f) {
                if (f == 0) throw divide_by_zero{};
                return fraction{f.Denominator.Number, {f.Numerator}};
            }
            
            bool operator!=(const fraction& f) const {
                return !*this == f;
            }
            
            bool operator<(const fraction& f) const {
                return f.Numerator * Denominator.Number < Numerator * f.Denominator.Number;
            }
            
            bool operator>(const fraction& f) const{
                return f.Numerator * Denominator.Number > Numerator * f.Denominator.Number;
            }
            
            bool operator<=(const fraction& f) const{
                return f.Numerator * Denominator.Number <= Numerator * f.Denominator.Number;
            }
            
            bool operator>=(const fraction& f) const{
                return f.Numerator * Denominator.Number >= Numerator * f.Denominator.Number;
            }
            
            fraction operator-() const {
                return {-Numerator, Denominator};
            }
            
            fraction operator+(const fraction& f) const {
                N gcd_rr = gcd(Denominator.Number, f.Denominator.Number);
                return fraction{
                    (f.Numerator * Denominator.Number + Numerator * f.Denominator.Number) / gcd_rr, 
                    (Denominator * f.Denominator) / gcd_rr};
            }
            
            fraction operator-(const fraction& f) const {
                return (*this)+(-f);
            }
            
            fraction operator*(const fraction& f) const {
                return divide(Numerator * f.Numerator, Denominator * f.Denominator);
            }
            
            fraction operator/(const fraction& f) const {
                return (*this)*inverse(f);
            }
            
            fraction(fraction& f) : Numerator{f.Numerator}, Denominator{f.Denominator} {}
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
    
}

#endif
