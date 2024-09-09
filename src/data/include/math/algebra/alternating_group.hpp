// Copyright (c) 2020-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ALGEBRA_ALTERNATING_GROUP
#define DATA_MATH_ALGEBRA_ALTERNATING_GROUP

#include <data/valid.hpp>

#include <data/math/permutation.hpp>
#include <data/math/number/modular.hpp>

namespace data::math {
    
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
                permutation<number::modular<N, natural>>::signature() == positive;
        }
        
    private:
        alternating_group(const permutation<number::modular<N, natural>>& p) : permutation<number::modular<N, natural>>{p} {}
        
    };

    template <typename N, auto & natural>
    inline std::ostream& operator<<(std::ostream& o, const alternating_group<N, natural>& m) {
        return o << "alternating<"<<natural<<">{"<<static_cast<const permutation<number::modular<N, natural>>&>(m)<<"}";
    }
    
}

namespace data::math {
    
    template <typename N, auto & natural>
    struct associative<
        times<alternating_group<N, natural>>, 
        alternating_group<N, natural>>
        : associative<plus<N>, N> {};
    
    template <typename N, auto & natural>
    struct identity<
        times<alternating_group<N, natural>>, 
        alternating_group<N, natural>>
        : identity<times<N>, N> {
        alternating_group<N, natural> operator()() {
            return {identity<times<N>, N>::value()};
        }
    };
    
    template <typename N, auto & natural>
    struct inverse<times<alternating_group<N, natural>>, alternating_group<N, natural>> {
        alternating_group<N, natural> operator()(const alternating_group<N, natural>& a, const alternating_group<N, natural>& b) {
            return b / a;
        }
    };
    
}

#endif
