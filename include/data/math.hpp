// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH
#define DATA_MATH

#include <data/math/fraction.hpp>
#include <data/math/permutation.hpp>
#include <data/math/polynomial.hpp>

namespace data {
    
    // fractions
    template <typename Z, typename N> using fraction = math::fraction<Z, N>;
    
    // permutations
    template <typename X> using permutation = math::permutation<X>;
    
    // Polynomials 
    template <typename X, typename N> using polynomial = data::math::polynomial<X, N>;
    
}

#endif
