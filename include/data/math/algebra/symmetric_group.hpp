// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ALGEBRA_SYMMETRIC_GROUP
#define DATA_MATH_ALGEBRA_SYMMETRIC_GROUP

#include <data/valid.hpp>

#include <data/math/permutation.hpp>
#include <data/math/group.hpp>
#include <data/math/associative.hpp>

namespace data::math::algebra {
    
    template <typename N, auto & natural>
    struct symmetric_group : permutation<number::modular<N, natural>> {
        
        symmetric_group operator*(const symmetric_group& e) const;
        symmetric_group operator/(const symmetric_group& e) const;
        
        symmetric_group inverse() const;
        
        bool operator==(const symmetric_group& e) const;
        bool operator!=(const symmetric_group& e) const;
        
        symmetric_group() : permutation<number::modular<N, natural>>{} {}
        
        static ptr<symmetric_group> make(const permutation<N>& p);
        
    private:
        symmetric_group(const permutation<number::modular<N, natural>>& p) : permutation<number::modular<N, natural>>{p} {}
        
    };

    template <typename N, auto & natural>
    inline std::ostream& operator<<(std::ostream& o, const data::math::algebra::symmetric_group<N, natural>& m) {
        return o << "symmetric<"<<natural<<">{"<<static_cast<const data::math::permutation<data::math::number::modular<N, natural>>&>(m)<<"}";
    }
    
}

namespace data::math {
    
    template <typename N, auto & natural>
    struct associative<
        times<algebra::symmetric_group<N, natural>>, 
        algebra::symmetric_group<N, natural>>
        : associative<plus<N>, N> {};
    
    template <typename N, auto & natural>
    struct identity<
        times<algebra::dihedral_group<N, natural>>, 
        algebra::symmetric_group<N, natural>>
        : identity<times<N>, N> {
        static const algebra::symmetric_group<N, natural> value() {
            return {identity<times<N>, N>::value()};
        }
    };
    
}

#endif
