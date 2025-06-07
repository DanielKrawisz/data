// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/data.hpp"
#include "data/math/algebra/finite_field.hpp"
#include "gtest/gtest.h"

namespace data {
    
    template <typename N>
    void test_prime_field () {

        // make the zero element for each prime field.
        prime_field<N {2}> f2_0 {0};
        prime_field<N {3}> f3_0 {0};
        prime_field<N {5}> f5_0 {0};
        prime_field<N {7}> f7_0 {0};
        prime_field<N {11}> f11_0 {0};
        prime_field<N {13}> f13_0 {0};
        prime_field<N {17}> f17_0 {0};
        prime_field<N {19}> f19_0 {0};
        
        EXPECT_TRUE (f2_0.valid ());
        EXPECT_TRUE (f3_0.valid ());
        EXPECT_TRUE (f5_0.valid ());
        EXPECT_TRUE (f7_0.valid ());
        EXPECT_TRUE (f11_0.valid ());
        EXPECT_TRUE (f13_0.valid ());
        EXPECT_TRUE (f17_0.valid ());
        EXPECT_TRUE (f19_0.valid ());
        
        EXPECT_EQ (f2_0 + f2_0, f2_0);
        EXPECT_EQ (f3_0 + f3_0, f3_0);
        EXPECT_EQ (f5_0 + f5_0, f5_0);
        EXPECT_EQ (f7_0 + f7_0, f7_0);
        EXPECT_EQ (f11_0 + f11_0, f11_0);
        EXPECT_EQ (f13_0 + f13_0, f13_0);
        EXPECT_EQ (f17_0 + f17_0, f17_0);
        EXPECT_EQ (f19_0 + f19_0, f19_0);
        
        EXPECT_EQ (f2_0 - f2_0, f2_0);
        EXPECT_EQ (f3_0 - f3_0, f3_0);
        EXPECT_EQ (f5_0 - f5_0, f5_0);
        EXPECT_EQ (f7_0 - f7_0, f7_0);
        EXPECT_EQ (f11_0 - f11_0, f11_0);
        EXPECT_EQ (f13_0 - f13_0, f13_0);
        EXPECT_EQ (f17_0 - f17_0, f17_0);
        EXPECT_EQ (f19_0 - f19_0, f19_0);
        
        EXPECT_EQ (f2_0 * f2_0, f2_0);
        EXPECT_EQ (f3_0 * f3_0, f3_0);
        EXPECT_EQ (f5_0 * f5_0, f5_0);
        EXPECT_EQ (f7_0 * f7_0, f7_0);
        EXPECT_EQ (f11_0 * f11_0, f11_0);
        EXPECT_EQ (f13_0 * f13_0, f13_0);
        EXPECT_EQ (f17_0 * f17_0, f17_0);
        EXPECT_EQ (f19_0 * f19_0, f19_0);
        
        EXPECT_EQ (f2_0.inverse (), nullptr);
        EXPECT_EQ (f3_0.inverse (), nullptr);
        EXPECT_EQ (f5_0.inverse (), nullptr);
        EXPECT_EQ (f7_0.inverse (), nullptr);
        EXPECT_EQ (f11_0.inverse (), nullptr);
        EXPECT_EQ (f13_0.inverse (), nullptr);
        EXPECT_EQ (f17_0.inverse (), nullptr);
        EXPECT_EQ (f19_0.inverse (), nullptr);
        
        // make the 1 element for each prime field. 
        prime_field<N {2}> f2_1 {1};
        prime_field<N {3}> f3_1 {1};
        prime_field<N {5}> f5_1 {1};
        prime_field<N {7}> f7_1 {1};
        prime_field<N {11}> f11_1 {1};
        prime_field<N {13}> f13_1 {1};
        prime_field<N {17}> f17_1 {1};
        prime_field<N {19}> f19_1 {1};
        
        EXPECT_TRUE (f2_1.valid ());
        EXPECT_TRUE (f3_1.valid ());
        EXPECT_TRUE (f5_1.valid ());
        EXPECT_TRUE (f7_1.valid ());
        EXPECT_TRUE (f11_1.valid ());
        EXPECT_TRUE (f13_1.valid ());
        EXPECT_TRUE (f17_1.valid ());
        EXPECT_TRUE (f19_1.valid ());
        
        EXPECT_EQ (f2_0 + f2_1, f2_1);
        EXPECT_EQ (f3_0 + f3_1, f3_1);
        EXPECT_EQ (f5_0 + f5_1, f5_1);
        EXPECT_EQ (f7_0 + f7_1, f7_1);
        EXPECT_EQ (f11_0 + f11_1, f11_1);
        EXPECT_EQ (f13_0 + f13_1, f13_1);
        EXPECT_EQ (f17_0 + f17_1, f17_1);
        EXPECT_EQ (f19_0 + f19_1, f19_1);
        
        EXPECT_EQ (f2_0 * f2_1, f2_0);
        EXPECT_EQ (f3_0 * f3_1, f3_0);
        EXPECT_EQ (f5_0 * f5_1, f5_0);
        EXPECT_EQ (f7_0 * f7_1, f7_0);
        EXPECT_EQ (f11_0 * f11_1, f11_0);
        EXPECT_EQ (f13_0 * f13_1, f13_0);
        EXPECT_EQ (f17_0 * f17_1, f17_0);
        EXPECT_EQ (f19_0 * f19_1, f19_0);
        
        EXPECT_EQ (f2_0 / f2_1, f2_0);
        EXPECT_EQ (f3_0 / f3_1, f3_0);
        EXPECT_EQ (f5_0 / f5_1, f5_0);
        EXPECT_EQ (f7_0 / f7_1, f7_0);
        EXPECT_EQ (f11_0 / f11_1, f11_0);
        EXPECT_EQ (f13_0 / f13_1, f13_0);
        EXPECT_EQ (f17_0 / f17_1, f17_0);
        EXPECT_EQ (f19_0 / f19_1, f19_0);
        
        EXPECT_EQ (f2_1.inverse (), f2_1);
        EXPECT_EQ (f3_1.inverse (), f3_1);
        EXPECT_EQ (f5_1.inverse (), f5_1);
        EXPECT_EQ (f7_1.inverse (), f7_1);
        EXPECT_EQ (f11_1.inverse (), f11_1);
        EXPECT_EQ (f13_1.inverse (), f13_1);
        EXPECT_EQ (f17_1.inverse (), f17_1);
        EXPECT_EQ (f19_1.inverse (), f19_1);
        
        EXPECT_EQ (f2_1 * f2_1, f2_1);
        EXPECT_EQ (f3_1 * f3_1, f3_1);
        EXPECT_EQ (f5_1 * f5_1, f5_1);
        EXPECT_EQ (f7_1 * f7_1, f7_1);
        EXPECT_EQ (f11_1 * f11_1, f11_1);
        EXPECT_EQ (f13_1 * f13_1, f13_1);
        EXPECT_EQ (f17_1 * f17_1, f17_1);
        EXPECT_EQ (f19_1 * f19_1, f19_1);
        
        prime_field<N {3}> f3_2 {2};
        prime_field<N {5}> f5_2 {2};
        prime_field<N {7}> f7_2 {2};
        prime_field<N {11}> f11_2 {2};
        prime_field<N {13}> f13_2 {2};
        prime_field<N {17}> f17_2 {2};
        prime_field<N {19}> f19_2 {2};
        
        EXPECT_EQ (f2_1 + f2_1, f2_0);
        EXPECT_EQ (f3_1 + f3_1, f3_2);
        EXPECT_EQ (f5_1 + f5_1, f5_2);
        EXPECT_EQ (f7_1 + f7_1, f7_2);
        EXPECT_EQ (f11_1 + f11_1, f11_2);
        EXPECT_EQ (f13_1 + f13_1, f13_2);
        EXPECT_EQ (f17_1 + f17_1, f17_2);
        EXPECT_EQ (f19_1 + f19_1, f19_2);
        
        EXPECT_EQ (f2_0 - f2_1, f2_1);
        EXPECT_EQ (f3_0 - f3_1, f3_2);
        EXPECT_EQ (f5_0 - f5_1, prime_field<N {5}> (4));
        EXPECT_EQ (f7_0 - f7_1, prime_field<N {7}> (6));
        EXPECT_EQ (f11_0 - f11_1, prime_field<N {11}> (10));
        EXPECT_EQ (f13_0 - f13_1, f13.make (12));
        EXPECT_EQ (f17_0 - f17_1, f17.make (16));
        EXPECT_EQ (f19_0 - f19_1, f19.make (18));
        
        EXPECT_EQ (f3_2.inverse (), f3_2);
        EXPECT_EQ (f5_2.inverse (), prime_field<N {5}> (3));
        EXPECT_EQ (f7_2.inverse (), prime_field<N {7}> (4));
        EXPECT_EQ (f11_2.inverse (), prime_field<N {11}> (6));
        EXPECT_EQ (f13_2.inverse (), prime_field<N {13}> (7));
        EXPECT_EQ (f17_2.inverse (), prime_field<N {17}> (9));
        EXPECT_EQ (f19_2.inverse (), prime_field<N {19}> (10));
        
        EXPECT_EQ (prime_field<N {5}> (3).inverse (), f5_2);
        EXPECT_EQ (prime_field<N {7}> (3).inverse (), prime_field<N {7}> (5));
        EXPECT_EQ (prime_field<N {11}> (3).inverse (), f11.make (4));
        EXPECT_EQ (prime_field<N {13}> (3).inverse (), prime_field<N {13}> (9));
        EXPECT_EQ (prime_field<N {17}> (3).inverse (), prime_field<N {17}> (6));
        EXPECT_EQ (prime_field<N {19}> (3).inverse (), prime_field<N {19}> (13));
        
        EXPECT_EQ (f3_2 * f3_2, f3_1);
        EXPECT_EQ (f5_2 * prime_field<N {5}> (3), f5_1);
        EXPECT_EQ (f7_2 * prime_field<N {7}> (4), f7_1);
        EXPECT_EQ (f11_2 * prime_field<N {11}> (6), f11_1);
        EXPECT_EQ (f13_2 * prime_field<N {13}> (7), f13_1);
        EXPECT_EQ (f17_2 * prime_field<N {17}> (9), f17_1);
        EXPECT_EQ (f19_2 * prime_field<N {19}> (10), f19_1);
        
        EXPECT_EQ (prime_field<N {5}> (3) * f5_2, f5_1);
        EXPECT_EQ (prime_field<N {7}> (3) * prime_field<N {7}> (5), f7_1);
        EXPECT_EQ (prime_field<N {11}> (3) * prime_field<N {11}> (4), f11_1);
        EXPECT_EQ (prime_field<N {13}> (3) * prime_field<N {13}> (9), f13_1);
        EXPECT_EQ (prime_field<N {17}> (3) * prime_field<N {17}> (6), f17_1);
        EXPECT_EQ (prime_field<N {19}> (3) * prime_field<N {19}> (13), f19_1);
        
    }

    TEST (PrimeFieldTest, TestPrimeField) {

        test_prime_field<byte> ();
        test_prime_field<uint16> ();
        test_prime_field<uint32> ();
        test_prime_field<uint64> ();

        test_prime_field<int8> ();
        test_prime_field<int16> ();
        test_prime_field<int32> ();
        test_prime_field<int64> ();

        test_prime_field<uint8_little> ();
        test_prime_field<uint16_little> ();
        test_prime_field<uint32_little> ();
        test_prime_field<uint64_little> ();

        test_prime_field<int8_little> ();
        test_prime_field<int16_little> ();
        test_prime_field<int32_little> ();
        test_prime_field<int64_little> ();

        test_prime_field<uint8_big> ();
        test_prime_field<uint16_big> ();
        test_prime_field<uint32_big> ();
        test_prime_field<uint64_big> ();

        test_prime_field<int8_big> ();
        test_prime_field<int16_big> ();
        test_prime_field<int32_big> ();
        test_prime_field<int64_big> ();

        /*
        test_prime_field<N> {};
        test_prime_field<N_bytes_little> {};
        test_prime_field<N_bytes_big> {};
        test_prime_field<dec_uint> {};
        test_prime_field<base58_uint> {};

        test_prime_field<uint_big<9>> {};
        test_prime_field<uint_little<9>> {};
        test_prime_field<uint_big<10>> {};
        test_prime_field<uint_little<10>> {};
        test_prime_field<uint_big<11>> {};
        test_prime_field<uint_little<11>> {};
        test_prime_field<uint_big<12>> {};
        test_prime_field<uint_little<12>> {};

        test_prime_field<uint_big<5, short unsigned int>> {};
        test_prime_field<uint_little<5, short unsigned int>> {};
        test_prime_field<uint_big<6, short unsigned int>> {};
        test_prime_field<uint_little<6, short unsigned int>> {};
        test_prime_field<uint_big<7, short unsigned int>> {};
        test_prime_field<uint_little<7, short unsigned int>> {};
        test_prime_field<uint_big<8, short unsigned int>> {};
        test_prime_field<uint_little<8, short unsigned int>> {};
        test_prime_field<uint_big<3, unsigned int>> {};
        test_prime_field<uint_little<3, unsigned int>> {};
        test_prime_field<uint_big<4, unsigned int>> {};
        test_prime_field<uint_little<4, unsigned int>> {};
        test_prime_field<uint_big<5, unsigned int>> {};
        test_prime_field<uint_little<5, unsigned int>> {};
        test_prime_field<uint_big<6, unsigned int>> {};
        test_prime_field<uint_little<6, unsigned int>> {};

        // TODO bigger words
        test_prime_field<uint_big<2, long unsigned int>> {};
        test_prime_field<uint_little<2, long unsigned int>> {};
        test_prime_field<uint_big<3, long unsigned int>> {};
        test_prime_field<uint_little<3, long unsigned int>> {};
        test_prime_field<uint_big<4, long unsigned int>> {};
        test_prime_field<uint_little<4, long unsigned int>> {};
        test_prime_field<uint_big<5, long unsigned int>> {};
        test_prime_field<uint_little<5, long unsigned int>> {};
        test_prime_field<uint_big<1, long long unsigned int>> {};
        test_prime_field<uint_little<1, long long unsigned int>> {};
        test_prime_field<uint_big<2, long long unsigned int>> {};
        test_prime_field<uint_little<2, long long unsigned int>> {};
        test_prime_field<uint_big<3, long long unsigned int>> {};
        test_prime_field<uint_little<3, long long unsigned int>> {};
        test_prime_field<uint_big<4, long long unsigned int>> {};
        test_prime_field<uint_little<4, long long unsigned int>> {};*/
    }
    
}
