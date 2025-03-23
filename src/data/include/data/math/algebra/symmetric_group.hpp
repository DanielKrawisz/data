// Copyright (c) 2020-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ALGEBRA_SYMMETRIC_GROUP
#define DATA_MATH_ALGEBRA_SYMMETRIC_GROUP

#include <data/valid.hpp>

#include <data/math/permutation.hpp>
#include <data/math/number/modular.hpp>

namespace data::math {
    
    template <auto natural, typename N = decltype (natural)>
    struct symmetric_group : permutation<number::modular<natural, N>> {
        
        symmetric_group operator * (const symmetric_group &e) const;
        symmetric_group operator / (const symmetric_group &e) const;
        
        symmetric_group inverse () const;
        
        bool operator == (const symmetric_group &e) const;
        bool operator != (const symmetric_group &e) const;
        
        symmetric_group () : permutation<number::modular<natural, N>> {} {}
        
        static ptr<symmetric_group> make (const permutation<N> &p);
        
    private:
        symmetric_group (const permutation<number::modular<natural, N>> &p) : permutation<number::modular<natural, N>> {p} {}
        
    };

    template <auto natural, typename N>
    inline std::ostream &operator << (std::ostream &o, const data::math::symmetric_group<natural, N> &m) {
        return o << "symmetric<" << natural << ">{"<<static_cast<const data::math::permutation<data::math::number::modular<natural, N>> &> (m) <<"}";
    }
    
    template <auto natural, typename N>
    struct identity<
        times<symmetric_group<natural, N>>,
        symmetric_group<natural, N>>
        : identity<times<N>, N> {
        symmetric_group<natural, N> operator () () {
            return {identity<times<N>, N>::value ()};
        }
    };
    
    template <auto natural, typename N>
    struct inverse<times<symmetric_group<natural, N>>, symmetric_group<natural, N>> {
        symmetric_group<natural, N> operator () (const symmetric_group<natural, N> &a, const symmetric_group<natural, N> &b) {
            return b / a;
        }
    };
    
}

#endif
