// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ALGEBRA_DIHEDRAL_GROUP
#define DATA_MATH_ALGEBRA_DIHEDRAL_GROUP

#include <data/valid.hpp>

#include <data/math/number/modular.hpp>
#include <data/math/group.hpp>
#include <data/math/associative.hpp>

namespace data::math::algebra {
    
    template <typename N, auto & natural>
    struct dihedral_group : number::modular<N, natural> {
        bool Flip;
        
        dihedral_group operator*(const dihedral_group& e) const;
        dihedral_group operator/(const dihedral_group& e) const;
        
        dihedral_group inverse() const;
        
        bool operator==(const dihedral_group& e) const;
        bool operator!=(const dihedral_group& e) const;
        
        dihedral_group& operator=(const dihedral_group& e);
        
        dihedral_group() : number::modular<N, natural>(N{0}) {}
    private:
        dihedral_group(N n, bool flip) : number::modular<N, natural>{number::modular<N, natural>{n}}, Flip{flip} {}
        dihedral_group(number::modular<N, natural> m, bool flip) : number::modular<N, natural>{m}, Flip{flip} {}
        
    };
    
}

template <typename N, auto & natural>
inline std::ostream& operator<<(std::ostream& o, const data::math::algebra::dihedral_group<N, natural>& m) {
    return o << "dihedral<"<<natural<<">{"<<m.Value<<", "<<m.Flip<<"}";
}

namespace data::math {
    
    template <typename N, auto & natural>
    struct associative<
        data::times<algebra::dihedral_group<N, natural>>, 
        algebra::dihedral_group<N, natural>>
        : associative<data::plus<N>, N> {};
    
    template <typename N, auto & natural>
    struct identity<
        data::times<algebra::dihedral_group<N, natural>>, 
        algebra::dihedral_group<N, natural>>
        : identity<data::times<N>, N> {
        static const algebra::dihedral_group<N, natural> value() {
            return {identity<data::times<N>, N>::value()};
        }
    };
    
}

namespace data::math::algebra {
}

#endif

