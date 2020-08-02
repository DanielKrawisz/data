// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ALGEBRA_CYCLIC_GROUP
#define DATA_MATH_ALGEBRA_CYCLIC_GROUP

#include <data/valid.hpp>

#include <data/math/number/modular.hpp>
#include <data/math/group.hpp>
#include <data/math/commutative.hpp>
#include <data/math/associative.hpp>

namespace data::math::algebra {
    
    template <typename N, auto & natural>
    struct cyclic_group : number::modular<N, natural> {
        
        cyclic_group operator+(const cyclic_group& e) const;
        cyclic_group operator-(const cyclic_group& e) const;
        
        cyclic_group inverse() const;
        
        bool operator==(const cyclic_group& e) const;
        bool operator!=(const cyclic_group& e) const;
        
        cyclic_group& operator=(const cyclic_group& e);
        
        cyclic_group() : number::modular<N, natural>(0) {}
        cyclic_group(number::modular<N, natural> m) : number::modular<N, natural>{m} {}

    private:
        cyclic_group(N n) : number::modular<N, natural>{number::modular<N, natural>{n}} {}
    };
    
}

template <typename N, auto & natural>
inline std::ostream& operator<<(std::ostream& o, const data::math::algebra::cyclic_group<N, natural>& m) {
    return o << "cyclic<"<<natural<<">{"<<m.Value<<"}";
}

namespace data::math {
    
    template <typename N, auto & natural>
    struct commutative<plus<algebra::cyclic_group<N, natural>>, 
        algebra::cyclic_group<N, natural>>
        : commutative<plus<N>, N> {};
    
    template <typename N, auto & natural>
    struct associative<
        data::plus<algebra::cyclic_group<N, natural>>, 
        algebra::cyclic_group<N, natural>>
        : associative<data::plus<N>, N> {};
    
    template <typename N, auto & natural>
    struct identity<
        data::plus<algebra::cyclic_group<N, natural>>, 
        algebra::cyclic_group<N, natural>>
        : identity<data::plus<N>, N> {
        static const algebra::cyclic_group<N, natural> value() {
            return {identity<data::plus<N>, N>::value()};
        }
    };
    
}

namespace data::math::algebra {
    template <typename N, auto & natural> 
    inline cyclic_group<N, natural> 
    cyclic_group<N, natural>::operator+(const cyclic_group& e) const {
        return {number::modular<N, natural>::operator+(e)};
    }
    
    template <typename N, auto & natural> 
    inline cyclic_group<N, natural> 
    cyclic_group<N, natural>::operator-(const cyclic_group& e) const {
        return {number::modular<N, natural>::operator-(e)};
    }
    
    template <typename N, auto & natural> 
    cyclic_group<N, natural> 
    cyclic_group<N, natural>::inverse() const {
        return {number::modular<N, natural>::operator-()};
    }
}

#endif


