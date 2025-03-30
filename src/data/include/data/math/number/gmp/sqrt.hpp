// Copyright (c) 2020-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_SQRT
#define DATA_MATH_NUMBER_GMP_SQRT

#include <data/encoding/integer.hpp>
#include <data/encoding/base58.hpp>
#include <data/math/number/gmp/Z.hpp>

namespace data::math::number::GMP {
    
    set<N> root (const N &n, uint32 p);
    set<Z> root (const Z &n, uint32 p);

}

namespace data::math {
    
    template <uint64 pow> struct root<N, pow> {
        set<N> operator () (const N &n) {
            return number::GMP::root (n, pow);
        }
    };
    
    template <uint64 pow> struct root<Z, pow> {
        set<Z> operator () (const Z &z) {
            return number::GMP::root (z, pow);
        }
    };
    
    template <uint64 pow> 
    set<dec_uint> root<dec_uint, pow>::operator () (const dec_uint &n) {
        return set<dec_uint> (root<N, pow> {} (N {n}));
    }
    
    template <uint64 pow> 
    set<dec_int> root<dec_int, pow>::operator () (const dec_int &n) {
        return set<dec_int> (root<Z, pow> {} (Z {n}));
    }
    
    template <hex_case zz, uint64 pow> 
    set<hex::uint<zz>> root<hex::uint<zz>, pow>::operator () (const hex::uint<zz> &n) {
        return set<hex::uint<zz>> (root<N, pow> {} (N {n}));
    }
    
    template <hex_case zz, uint64 pow> 
    set<hex::int2<zz>> root<hex::int2<zz>, pow>::operator () (const hex::int2<zz> &n) {
        return set<hex::int2<zz>> (root<Z, pow> {} (Z {n}));
    }
    
    template <hex_case zz, uint64 pow> 
    set<hex::intBC<zz>> root<hex::intBC<zz>, pow>::operator () (const hex::intBC<zz> &n) {
        return set<hex::intBC<zz>> (root<Z, pow> {} (Z {n}));
    }

    template <uint64 pow>
    set<base58_uint> root<base58_uint, pow>::operator () (const base58_uint &n) {
        return set<base58_uint> (root<N, pow> {} (N {n}));
    }

}

#endif
