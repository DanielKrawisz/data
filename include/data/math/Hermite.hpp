// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_HERMITE
#define DATA_MATH_HERMITE

#include <data/math/polynomial.hpp>

namespace data::math {
    
    template <typename Z, typename N> polynomial<Z, N> HermitePhysics(N n) {
        if (n == 0) return 1;
        
        auto h = Hermite(n - 1);
        auto x = polynomial<Z, N>::variable();
        return h * x * 2 - h.derivative();
    }
    
    template <typename Z, typename N> polynomial<Z, N> HermiteStatistics(N n) {
        if (n == 0) return 1;
        
        auto h = Hermite(n - 1);
        auto x = polynomial<Z, N>::variable();
        return h * x - h.derivative() / 2;
    }
    
}

#endif
