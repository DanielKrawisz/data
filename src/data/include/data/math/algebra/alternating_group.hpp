// Copyright (c) 2020-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ALGEBRA_ALTERNATING_GROUP
#define DATA_MATH_ALGEBRA_ALTERNATING_GROUP

#include <data/valid.hpp>

#include <data/math/permutation.hpp>
#include <data/math/number/modular.hpp>

namespace data::math {
    
    template <auto natural, typename N = decltype (natural)>
    struct alternating_group : permutation<number::modular<natural, N>> {
        
        alternating_group operator * (const alternating_group &e) const;
        alternating_group operator / (const alternating_group &e) const;
        
        alternating_group inverse () const;
        
        bool operator == (const alternating_group &e) const;
        bool operator != (const alternating_group &e) const;
        
        alternating_group () : permutation<number::modular<natural, N>> {} {}
        
        static ptr<alternating_group> make (const permutation<number::modular<natural, N>> &p);
        
        bool valid () const {
            return permutation<number::modular<natural, N>>::valid () &&
                permutation<number::modular<natural, N>>::signature () == positive;
        }
        
    private:
        alternating_group (const permutation<number::modular<natural, N>> &p) : permutation<number::modular<natural, N>> {p} {}
        
    };

    template <typename N, auto & natural>
    inline std::ostream &operator << (std::ostream &o, const alternating_group<natural, N> &m) {
        return o << "alternating<" << natural << ">{" << static_cast<const permutation<number::modular<natural, N>> &> (m) <<"}";
    }
    
}

namespace data::math {
    
    template <auto natural, typename N>
    struct associative<
        times<alternating_group<natural, N>>,
        alternating_group<natural, N>>
        : associative<plus<N>, N> {};
    
    template <auto natural, typename N>
    struct identity<
        times<alternating_group<natural, N>>,
        alternating_group<natural, N>>
        : identity<times<N>, N> {
        alternating_group<natural, N> operator () () {
            return {identity<times<N>, N>::value ()};
        }
    };
    
    template <auto natural, typename N>
    struct inverse<times<alternating_group<natural, N>>, alternating_group<natural, N>> {
        alternating_group<natural, N> operator () (const alternating_group<natural, N> &a, const alternating_group<natural, N> &b) {
            return b / a;
        }
    };
    
}

#endif
