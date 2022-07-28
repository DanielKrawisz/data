// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/math/number/gmp/sqrt.hpp>

namespace data::math::number::GMP {
    
    set<N> root(const N& n, uint32 p) {
        if (p == 0) return n == 1 ? set<N>{n} : set<N>{};
        
        if (p == 1 || n == 0 || n == 1) return set<N>{n};
        N p_root{};
        if (0 == mpz_root(p_root.Value.MPZ, n.Value.MPZ, p)) return set<N>{};
        return set<N>{p_root};
    }
    
    set<Z> root(const Z& n, uint32 p) {
        if (p == 0) return n == 1 ? set<Z>{n} : set<Z>{};
        
        if (p == 1 || n == 0 || n == 1) return set<Z>{n};
        
        if (p % 2 == 0) {
            if (n < 0) return set<Z>{};
            
            set<N> nr = root(data::abs(n), p);
            
            set<Z> zr;
            
            for (const N &n : nr.values()) zr = zr.insert(Z{n}, -Z{n});
            
            return zr;
        }
        
        set<Z> zr;
        
        set<N> nr = root(data::abs(n), p);
        
        if (n < 0) for (const N &n : nr.values()) zr = zr.insert(-Z{n});
        else for (const N &n : nr.values()) zr = zr.insert(Z{n});
        
        return zr; 
        
    }

}

