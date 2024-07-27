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
        set<dec_uint> x;
        set<N> roots = root<N, pow> {} (N {n});
        for (const N &z : roots.values ()) x = insert (x, encoding::decimal::write (z));
        return x;
    }
    
    template <uint64 pow> 
    set<dec_int> root<dec_int, pow>::operator () (const dec_int &n) {
        set<dec_int> x;
        set<Z> roots = root<Z, pow> {} (Z {n});
        for (const Z &z : roots.values ()) x = insert (x, encoding::signed_decimal::write (z));
        return x;
    }
    
    template <hex_case zz, uint64 pow> 
    set<hex::uint<zz>> root<hex::uint<zz>, pow>::operator () (const hex::uint<zz> &n) {
        set<hex::uint<zz>> x;
        set<N> roots = root<N, pow> {} (N {n});
        for (const N &z : roots.values ()) x = insert (x, encoding::hexidecimal::write<zz> (z));
        return x;
    }
    
    template <hex_case zz, uint64 pow> 
    set<hex::int1<zz>> root<hex::int1<zz>, pow>::operator () (const hex::int1<zz> &n) {
        set<hex::int1<zz>> x;
        set<Z> roots = root<Z, pow> {} (Z {n});
        for (const Z &z : roots.values ()) x = insert (x, encoding::hexidecimal::write<number::complement::ones, zz> (z));
        return x;
    }
    
    template <hex_case zz, uint64 pow> 
    set<hex::int2<zz>> root<hex::int2<zz>, pow>::operator () (const hex::int2<zz> &n) {
        set<hex::int2<zz>> x;
        set<Z> roots = root<Z, pow> {} (Z {n});
        for (const Z &z : roots.values ()) x = insert (x, encoding::hexidecimal::write<number::complement::twos, zz> (z));
        return x;
    }

    template <uint64 pow>
    set<base58_uint> root<base58_uint, pow>::operator () (const base58_uint &n) {
        set<base58_uint> x;
        set<N> roots = root<N, pow> {} (N {n});
        for (const N &z : roots.values ()) x = insert (x, encoding::base58::encode<N> (z));
        return x;
    }

}

#endif
