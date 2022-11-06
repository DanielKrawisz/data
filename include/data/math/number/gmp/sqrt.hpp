// Copyright (c) 2020-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_SQRT
#define DATA_MATH_NUMBER_GMP_SQRT

#include <data/math/number/gmp/N.hpp>
#include <data/math/root.hpp>

namespace data::math::number::GMP {
    
    set<N> root(const N& n, uint32 p);
    set<Z> root(const Z& n, uint32 p);

}

namespace data::math {
    
    template <uint64 pow> struct root<N, pow> {
        set<N> operator()(const N& n) {
            return number::GMP::root(n, pow);
        }
    };
    
    template <uint64 pow> struct root<Z, pow> {
        set<Z> operator()(const Z& z) {
            return number::GMP::root(z, pow);
        }
    };
    
    template <uint64 pow> 
    set<dec_uint> root<dec_uint, pow>::operator()(const dec_uint& n) {
        set<dec_uint> x;
        set<N> roots = root<N, pow>{}(N::read(n));
        for (const N &z : roots.values()) x = insert(x, encoding::decimal::write(z));
        return x;
    }
    
    template <uint64 pow> 
    set<dec_int> root<dec_int, pow>::operator()(const dec_int& n) {
        set<dec_int> x;
        set<Z> roots = root<Z, pow>{}(Z::read(n));
        for (const Z &z : roots.values()) x = insert(x, encoding::signed_decimal::write(z));
        return x;
    }
    
    template <uint64 pow> 
    set<hex_uint> root<hex_uint, pow>::operator()(const hex_uint& n) {
        set<hex_uint> x;
        set<N> roots = root<N, pow>{}(N::read(n));
        for (const N &z : roots.values()) x = insert(x, encoding::hexidecimal::write(z));
        return x;
    }

}

#endif
