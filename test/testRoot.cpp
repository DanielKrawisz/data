// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include <data/math/number/bytes/sqrt.hpp>
#include "gtest/gtest.h"

namespace data {
    
    template <typename NN> void test_root_N () {
        
        auto root_17_0 = root<0> (NN (17));
        auto root_25_0 = root<0> (NN (25));
        auto root_422_1 = root<1> (NN (422));
        auto root_4_2 = root<2> (NN (4));
        auto root_16_2 = root<2> (NN (16));
        auto root_25_2 = root<2> (NN (25));
        auto root_27_2 = root<2> (NN (27));
        auto root_125_3 = root<3> (NN (125));
        auto root_128_3 = root<3> (NN (128));
        auto root_128_7 = root<7> (NN (128));

        NN ntt1 (N ("1798307508862833999690304313948111955510002315423096853"));
        NN ntt2 (N ("1798307508862833999690304313948111955510002315423096851"));

        auto root_1798307508862833999690304313948111955510002315423096853_19 = root<19> (ntt1);
        auto root_1798307508862833999690304313948111955510002315423096853_18 = root<18> (ntt1);
        auto root_1798307508862833999690304313948111955510002315423096851_19 = root<19> (ntt2);

        EXPECT_TRUE (data::empty (root_17_0));
        EXPECT_TRUE (data::empty (root_25_0));
        EXPECT_FALSE (data::empty (root_422_1));
        EXPECT_FALSE (data::empty (root_4_2));
        EXPECT_FALSE (data::empty (root_16_2));
        EXPECT_FALSE (data::empty (root_25_2));
        EXPECT_TRUE (data::empty (root_27_2));
        EXPECT_FALSE (data::empty (root_125_3));
        EXPECT_TRUE (data::empty (root_128_3));
        EXPECT_FALSE (data::empty (root_128_7));
        EXPECT_FALSE (data::empty (root_1798307508862833999690304313948111955510002315423096853_19));
        EXPECT_TRUE (data::empty (root_1798307508862833999690304313948111955510002315423096853_18));
        EXPECT_TRUE (data::empty (root_1798307508862833999690304313948111955510002315423096851_19));

        bool contains_422_1 = contains (root_422_1, NN (422));
        bool contains_4_2 = contains (root_4_2, NN (2));
        bool contains_16_2 = contains (root_16_2, NN (4));
        bool contains_25_2 = contains (root_25_2, NN (5));
        bool contains_125_3 = contains (root_125_3, NN (5));
        bool contains_128_7 = contains (root_128_7, NN (2));
        bool contains_xxxx = contains (root_1798307508862833999690304313948111955510002315423096853_19, NN (717));
        
        EXPECT_TRUE (contains_422_1);
        EXPECT_TRUE (contains_4_2);
        EXPECT_TRUE (contains_16_2);
        EXPECT_TRUE (contains_25_2);
        EXPECT_TRUE (contains_125_3);
        EXPECT_TRUE (contains_128_7);
        EXPECT_TRUE (contains_xxxx);
        
    };
    
    template <typename Z> void test_root_Z () {
        test_root_N<Z> ();
        
        auto root_9_2 = root<2> (Z (9));
        bool contains_9_2 = contains (root_9_2, Z (3));
        bool contains_9_n2 = contains (root_9_2, Z (-3));
        
        EXPECT_TRUE (contains_9_2);
        EXPECT_TRUE (contains_9_n2);
        
        auto root_8_3 = root<3> (Z (8));
        bool contains_8_3 = contains (root_8_3, Z (2));
        bool contains_8_n3 = contains (root_8_3, Z (-2));
        
        EXPECT_TRUE (contains_8_3);
        EXPECT_FALSE (contains_8_n3);
        
    };
    
    TEST (RootsTest, TestRoots) {

        test_root_N<N> ();
        test_root_N<N_bytes_little> ();
        test_root_N<N_bytes_big> ();
        test_root_N<dec_uint> ();
        test_root_N<hex_uint> ();
        test_root_N<base58_uint> ();

        test_root_Z<Z> ();
        test_root_Z<Z_bytes_little> ();
        test_root_Z<Z_bytes_big> ();
        test_root_Z<dec_int> ();
        test_root_Z<hex_int> ();
        test_root_Z<hex_int_BC> ();
    }
}
