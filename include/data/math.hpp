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
    template <typename Z, typename N = decltype (abs (std::declval<Z> ()))> using fraction = math::fraction<Z, N>;
    
    // Polynomials 
    template <typename X, typename N, char x = 'x'> using polynomial = math::polynomial<X, N, x>;

    
}

#endif
