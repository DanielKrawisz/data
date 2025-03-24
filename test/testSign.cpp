// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/numbers.hpp"
#include "data/math.hpp"
#include "gtest/gtest.h"

namespace data {
    
    typedef int_big<9> b09;
    typedef int_big<10> b10;
    typedef int_big<11> b11;
    typedef int_big<20> b20;
    typedef int_little<9> l09;
    typedef int_little<10> l10;
    typedef int_little<11> l11;
    typedef int_little<20> l20;
    
    typedef uint_big<9> ub09;
    typedef uint_big<10> ub10;
    typedef uint_big<11> ub11;
    typedef uint_big<20> ub20;
    typedef uint_little<9> ul09;
    typedef uint_little<10> ul10;
    typedef uint_little<11> ul11;
    typedef uint_little<20> ul20;
    
    using q = fraction<int64, uint32>;
    
    template <typename N> void test_N_sign () {
        
        EXPECT_EQ (sign (N {uint64 {0u}}),                     math::zero);
        EXPECT_EQ (sign (N {uint64 {1u}}),                     math::positive);
        EXPECT_EQ (sign (N {uint64 {2u}}),                     math::positive);
        EXPECT_EQ (sign (N {uint64 {7076852110923542u}}),      math::positive);
        
    };
    
    template <typename N> void test_Z_sign () {
        test_N_sign<N> ();
        
        EXPECT_EQ (sign (Z {int64 {-1}}),                math::negative);
        EXPECT_EQ (sign (Z {int64 {-2}}),                math::negative);
        EXPECT_EQ (sign (Z {int64 {-7076852110923542}}), math::negative);
        
    };
    
    TEST (SignTest, TestSign) {

        test_Z_sign<double> ();

        test_Z_sign<int64> ();
        test_N_sign<uint64> ();

        test_Z_sign<Z> ();
        test_N_sign<N> ();
        
        test_N_sign<Z_bytes_little> ();
        test_N_sign<Z_bytes_big> ();
        
        test_N_sign<N_bytes_little> ();
        test_N_sign<N_bytes_big> ();

        EXPECT_EQ (sign (Q {0}),                     math::zero);
        EXPECT_EQ (sign (Q {1}),                     math::positive);
        EXPECT_EQ (sign (Q {-1}),                    math::negative);
        EXPECT_EQ (sign (Q {2}),                     math::positive);
        EXPECT_EQ (sign (Q {-2}),                    math::negative);
        EXPECT_EQ (sign (Q {2, 3}),                  math::positive);
        EXPECT_EQ (sign (Q {-2, 3}),                 math::negative);

        EXPECT_EQ (sign (q {0}),                     math::zero);
        EXPECT_EQ (sign (q {1}),                     math::positive);
        EXPECT_EQ (sign (q {-1}),                    math::negative);
        EXPECT_EQ (sign (q {2}),                     math::positive);
        EXPECT_EQ (sign (q {-2}),                    math::negative);
        EXPECT_EQ (sign (q {2, 3}),                  math::positive);
        EXPECT_EQ (sign (q {-2, 3}),                 math::negative);

        test_Z_sign<b09> ();
        test_Z_sign<b10> ();
        test_Z_sign<b11> ();
        test_Z_sign<b20> ();
        
        test_N_sign<ub09> ();
        test_N_sign<ub10> ();
        test_N_sign<ub11> ();
        test_N_sign<ub20> ();
        
        test_Z_sign<l09> ();
        test_Z_sign<l10> ();
        test_Z_sign<l11> ();
        test_Z_sign<l20> ();
        
        test_N_sign<ul09> ();
        test_N_sign<ul10> ();
        test_N_sign<ul11> ();
        test_N_sign<ul20> ();
        
        test_Z_sign<dec_int> ();
        test_N_sign<dec_uint> ();
        test_N_sign<base58_uint> ();
        test_N_sign<hex_uint> ();
        test_Z_sign<hex_int> ();
        test_Z_sign<hex_int_BC> ();
        
    }
}
