// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ALGEBRA_FINITE_FIELD
#define DATA_MATH_ALGEBRA_FINITE_FIELD

#include <data/math/number/prime.hpp>
#include <data/math/number/modular.hpp>
#include <data/math/field.hpp>
#include <data/math/number/extended_euclidian.hpp>
#include <data/math/commutative.hpp>
#include <data/math/associative.hpp>

namespace data::math::algebra {
    // TODO use construction type. 
    template <typename N, typename Z, auto & prime> struct prime_field;
    
    template <typename N, typename Z, auto & prime>
    struct prime_field_element : number::modular<N, prime> {
        
        prime_field_element operator+(const prime_field_element& e) const;
        prime_field_element operator-(const prime_field_element& e) const;
        prime_field_element operator*(const prime_field_element& e) const;
        prime_field_element operator/(const prime_field_element& e) const;
        
        prime_field_element inverse() const;
        
        prime_field_element() : number::modular<N, prime>(prime) {}
    private:
        prime_field_element(N n) : number::modular<N, prime>{number::modular<N, prime>{n}} {}
        prime_field_element(number::modular<N, prime> m) : number::modular<N, prime>{m} {}
        
        friend struct prime_field<N, Z, prime>;
    };
    
    template <typename N, typename Z, auto & prime>
    struct prime_field {
        number::prime<N> Modulus;
        
        prime_field(number::prime<N> p) : Modulus{p.Prime == N(prime) ? p : number::prime<N>{}} {}
        
        bool valid() const {
            return Modulus.valid() && Modulus.Prime == N(prime);
        }
        
        prime_field_element<N, Z, prime> make(N n);
    };
    
}

template <typename N, typename Z, auto & prime>
inline std::ostream& operator<<(std::ostream& o, const data::math::algebra::prime_field_element<N, Z, prime>& m) {
    return o << "f<"<<prime<<">{"<<m.Value<<"}";
}

namespace data::math {
    
    template <typename N, typename Z, auto & prime>
    struct commutative<plus<algebra::prime_field_element<N, Z, prime>>, 
        algebra::prime_field_element<N, Z, prime>>
        : commutative<plus<N>, N> {};
    
    template <typename N, typename Z, auto & prime>
    struct associative<
        data::plus<algebra::prime_field_element<N, Z, prime>>, 
        algebra::prime_field_element<N, Z, prime>>
        : associative<data::plus<N>, N> {};
    
    template <typename N, typename Z, auto & prime>
    struct commutative<
        data::times<algebra::prime_field_element<N, Z, prime>>, 
        algebra::prime_field_element<N, Z, prime>>
        : commutative<data::times<N>, N> {};;
    
    template <typename N, typename Z, auto & prime>
    struct associative<
        data::times<algebra::prime_field_element<N, Z, prime>>, 
        algebra::prime_field_element<N, Z, prime>>
        : associative<data::times<N>, N> {};
    
    template <typename N, typename Z, auto & prime>
    struct identity<
        data::plus<algebra::prime_field_element<N, Z, prime>>, 
        algebra::prime_field_element<N, Z, prime>>
        : identity<data::plus<N>, N> {
        static const algebra::prime_field_element<N, Z, prime> value() {
            return {identity<data::plus<N>, N>::value()};
        }
    };
    
    template <typename N, typename Z, auto & prime>
    struct identity< 
        data::times<algebra::prime_field_element<N, Z, prime>>, 
        algebra::prime_field_element<N, Z, prime>>
        : identity<data::times<N>, N> {
        static const algebra::prime_field_element<N, Z, prime> value() {
            return {identity<data::times<N>, N>::value()};
        }
    };
    
}

namespace data::math::algebra {
    template <typename N, typename Z, auto & prime> 
    inline prime_field_element<N, Z, prime> 
    prime_field_element<N, Z, prime>::operator+(const prime_field_element& e) const {
        return {number::modular<N, prime>::operator+(e)};
    }
    
    template <typename N, typename Z, auto & prime> 
    inline prime_field_element<N, Z, prime> 
    prime_field_element<N, Z, prime>::operator-(const prime_field_element& e) const {
        return {number::modular<N, prime>::operator-(e)};
    }
    
    template <typename N, typename Z, auto & prime> 
    inline prime_field_element<N, Z, prime> 
    prime_field_element<N, Z, prime>::operator*(const prime_field_element& e) const {
        return {number::modular<N, prime>::operator*(e)};
    }
    
    template <typename N, typename Z, auto & prime> 
    prime_field_element<N, Z, prime> 
    prime_field_element<N, Z, prime>::inverse() const {
        if (*this == 0) throw division_by_zero{};
        Z bt = number::euclidian::extended<N, Z>::algorithm(N{prime}, 
            number::modular<N, prime>::Value).BezoutT;
        if (bt < 0) bt += N{prime};
        return prime_field_element{number::abs<N, Z>{}(bt)};
    }
    
    template <typename N, typename Z, auto & prime> 
    inline prime_field_element<N, Z, prime> 
    prime_field_element<N, Z, prime>::operator/(const prime_field_element& e) const {
        return operator*(e.inverse());
    }
    
    template <typename N, typename Z, auto & prime> 
    inline prime_field_element<N, Z, prime> prime_field<N, Z, prime>::make(N n) {
        if (!valid()) return prime_field_element<N, Z, prime>{};
        return prime_field_element<N, Z, prime>{n};
    }
}

#endif

