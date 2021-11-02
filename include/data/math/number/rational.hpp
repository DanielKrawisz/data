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
            return data::valid(Number) && Number > 0;
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
}

namespace data::math {
    
    // TODO: note that N is the type of abs(Z)
    // I don't have a way of doing that yet. 
    template <typename Z, typename N>
    struct fraction : interface::ring<Z, data::plus<Z>, data::times<Z>>, interface::ordered<Z> {
        Z Numerator;
        number::positive<N> Denominator;
        
        bool valid() const {
            return data::valid(Numerator) && Denominator.valid();
        }
        
        fraction() : Numerator{0}, Denominator{1} {}
        
    private:
        fraction(Z n, number::positive<N> d) : Numerator{n}, Denominator{d} {}
        
        static N gcd(N a, N b) {
            return number::euclidian::extended<N, Z>::algorithm(a, b).GCD;
        }
        
        static fraction divide(Z n, N d) {
            if (d == 0) return fraction{Z{0}, number::positive<N>{N{0}}}; // Invalid value. 
            if (n == 0) return fraction{Z{0}, number::positive<N>{N{1}}};
            N gcd_ab = gcd(number::abs<N, Z>{}(n), d);
            return fraction(n / Z(gcd_ab), number::positive(d / gcd_ab));
        }
        
    public:
        fraction(Z n, N d) : fraction(divide(n, d)) {}
        fraction(Z n) : Numerator{n}, Denominator{uint64{1}} {}
        
        bool operator==(const fraction& f) const {
            return Numerator == f.Numerator && (Numerator == 0 || Denominator == f.Denominator);
        }
        
        bool operator==(const Z& z) const {
            return operator==(fraction{z});
        }
        
        static ptr<fraction> inverse(const fraction& f) {
            if (f == 0) return nullptr;
            return std::make_shared<fraction>(
                fraction{Z{f.Denominator.Number} * number::arg<Z>{}(f.Numerator), number::positive{number::abs<N, Z>{}(f.Numerator)}});
        }
        
        bool operator!=(const fraction& f) const {
            return !operator==(f);
        }
        
        bool operator<(const fraction& f) const;
        
        bool operator>(const fraction& f) const;
        
        bool operator<=(const fraction& f) const;
        
        bool operator>=(const fraction& f) const;
        
        fraction operator-() const {
            return {-Numerator, Denominator};
        }
        
        fraction operator+(const fraction& f) const {
            return divide(f.Numerator * Denominator.Number + Numerator * f.Denominator.Number,   
                Denominator.Number * f.Denominator.Number);
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
        
        fraction operator*(const fraction& f) const;
        
        fraction operator*(const Z& z) const;
        
        fraction operator/(const nonzero<fraction>& f) const {
            if (f.Value == fraction{0}) throw division_by_zero{};
            return (*this) * *inverse(f);
        }
        
        fraction operator/(const Z& z) const {
            return operator/(fraction{z});
        }
        
        fraction operator/(const N& n) const {
            return fraction{Numerator, Denominator * n};
        }
        
        nonnegative<fraction> quadrance() const {
            return fraction{Numerator * Numerator, Denominator * Denominator};
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
    struct commutative<data::plus<fraction<Z, N>>, fraction<Z, N>> : commutative<data::plus<Z>, Z> {};
    
    template <typename Z, typename N> 
    struct associative<data::plus<fraction<Z, N>>, fraction<Z, N>> : associative<data::plus<Z>, Z> {};
    
    template <typename Z, typename N> 
    struct commutative<data::times<fraction<Z, N>>, fraction<Z, N>> : commutative<data::times<Z>, Z>{};
    
    template <typename Z, typename N> 
    struct associative<data::times<fraction<Z, N>>, fraction<Z, N>> : associative<data::times<Z>, Z>{};
    
    template <typename Z, typename N> 
    struct identity<data::plus<fraction<Z, N>>, fraction<Z, N>> {
        static const math::number::gmp::Z value() {
            return {identity<data::plus<Z>, Z>::value()};
        }
    };
    
    template <typename Z, typename N> 
    struct identity<data::times<fraction<Z, N>>, fraction<Z, N>> {
        static const math::number::gmp::Z value() {
            return {identity<data::times<Z>, Z>::value()};
        }
    };
}

template <typename Z, typename N>
inline std::ostream& operator<<(std::ostream& o, const data::math::fraction<N, Z>& x) {
    return o << "fraction{" << x.Numerator << ", " << x.Denominator.Number << "}";
}

namespace data::math {
    template <typename Z, typename N>
    inline bool fraction<Z, N>::operator<(const fraction& f) const {
        return Numerator * f.Denominator.Number < Denominator.Number * f.Numerator;
    }
    
    template <typename Z, typename N>
    inline bool fraction<Z, N>::operator>(const fraction& f) const{
        return Numerator * f.Denominator.Number > Denominator.Number * f.Numerator;
    }
    
    template <typename Z, typename N>
    inline bool fraction<Z, N>::operator<=(const fraction& f) const{
        return Numerator * f.Denominator.Number <= Denominator.Number * f.Numerator;
    }
    
    template <typename Z, typename N>
    inline bool fraction<Z, N>::operator>=(const fraction& f) const{
        return Numerator * f.Denominator.Number >= Denominator.Number * f.Numerator;
    }
    
    template <typename Z, typename N>
    inline fraction<Z, N> fraction<Z, N>::operator*(const fraction& f) const {
        return divide(Numerator * f.Numerator, Denominator.Number * f.Denominator.Number);
    }
    
    template <typename Z, typename N>
    inline fraction<Z, N> fraction<Z, N>::operator*(const Z& z) const {
        return operator*(fraction{z});
    }
}

#endif
