// Copyright (c) 2020--2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ALGEBRA_DIHEDRAL_GROUP
#define DATA_MATH_ALGEBRA_DIHEDRAL_GROUP

#include <data/valid.hpp>

#include <data/math/number/modular.hpp>

namespace data::math {
    
    template <auto natural, typename N = decltype (natural)>
    struct dihedral_group : number::modular<natural, N> {
        bool Flip;
        
        dihedral_group operator * (const dihedral_group &e) const;
        dihedral_group operator / (const dihedral_group &e) const;
        
        dihedral_group inverse () const;
        
        bool operator == (const dihedral_group &e) const;
        bool operator != (const dihedral_group &e) const;
        
        dihedral_group &operator = (const dihedral_group &e);
        
        dihedral_group () : number::modular<natural, N> (N {0}) {}
    private:
        dihedral_group (N n, bool flip) : number::modular<natural, N> {number::modular<natural, N> {n}}, Flip {flip} {}
        dihedral_group (number::modular<natural, N> m, bool flip) : number::modular<natural, N> {m}, Flip {flip} {}
        
    };

    template <typename N, auto natural>
    inline std::ostream &operator << (std::ostream &o, const dihedral_group<natural, N> &m) {
        return o << "dihedral<" << natural << ">{" << m.Value << ", " << m.Flip << "}";
    }
    
    template <typename N, auto natural>
    struct is_associative<
        times<dihedral_group<natural, N>>,
        dihedral_group<natural, N>>:
            is_associative<plus<N>, N> {};
    
    template <typename N, auto natural>
    struct identity<
        times<dihedral_group<natural, N>>,
        dihedral_group<natural, N>>:
            identity<times<N>, N> {
        dihedral_group<natural, N> operator () () {
            return {identity<times<N>, N>::value ()};
        }
    };
    
    template <typename N, auto natural>
    struct inverse<times<dihedral_group<natural, N>>, dihedral_group<natural, N>> {
        dihedral_group<natural, N> operator () (const dihedral_group<natural, N> &a, const dihedral_group<natural, N> &b) {
            return b / a;
        }
    };
    
}

#endif

