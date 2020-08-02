// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ALGEBRA_ALTERNATING_GROUP
#define DATA_MATH_ALGEBRA_ALTERNATING_GROUP

#include <data/valid.hpp>

#include <data/math/permutation.hpp>
#include <data/math/group.hpp>
#include <data/math/associative.hpp>

namespace data::math::algebra {
    
    template <typename N, auto & natural>
    struct alternating_group : permutation<number::modular<N, natural>> {
        
        alternating_group operator*(const alternating_group& e) const;
        alternating_group operator/(const alternating_group& e) const;
        
        alternating_group inverse() const;
        
        bool operator==(const alternating_group& e) const;
        bool operator!=(const alternating_group& e) const;
        
        alternating_group() : permutation<number::modular<N, natural>>{} {}
        
        static ptr<alternating_group> make(const permutation<number::modular<N, natural>>& p);
        
        bool valid() const {
            return permutation<number::modular<N, natural>>::valid() && 
                permutation<number::modular<N, natural>>::signature() == math::positive;
        }
        
    private:
        alternating_group(const permutation<number::modular<N, natural>>& p) : permutation<number::modular<N, natural>>{p} {}
        
    };
    
}

template <typename N, auto & natural>
inline std::ostream& operator<<(std::ostream& o, const data::math::algebra::alternating_group<N, natural>& m) {
    return o << "alternating<"<<natural<<">{"<<static_cast<const data::math::permutation<data::math::number::modular<N, natural>>&>(m)<<"}";
}

namespace data::math {
    
    template <typename N, auto & natural>
    struct associative<
        data::times<algebra::alternating_group<N, natural>>, 
        algebra::alternating_group<N, natural>>
        : associative<data::plus<N>, N> {};
    
    template <typename N, auto & natural>
    struct identity<
        data::times<algebra::dihedral_group<N, natural>>, 
        algebra::alternating_group<N, natural>>
        : identity<data::times<N>, N> {
        static const algebra::alternating_group<N, natural> value() {
            return {identity<data::times<N>, N>::value()};
        }
    };
    
}

#endif
