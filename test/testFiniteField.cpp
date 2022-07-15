// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/data.hpp"
#include "data/math/algebra/finite_field.hpp"
#include "gtest/gtest.h"

namespace data {
    // representations of numbers. 
    constexpr auto d2 = decimal {"2"};
    constexpr auto d3 = decimal {"3"};
    constexpr auto d4 = decimal {"4"};
    constexpr auto d5 = decimal {"5"};
    constexpr auto d6 = decimal {"6"};
    constexpr auto d7 = decimal {"7"};
    constexpr auto d8 = decimal {"8"};
    constexpr auto d9 = decimal {"9"};
    constexpr auto d10 = decimal {"10"};
    constexpr auto d11 = decimal {"11"};
    constexpr auto d12 = decimal {"12"};
    constexpr auto d13 = decimal {"13"};
    constexpr auto d14 = decimal {"14"};
    constexpr auto d15 = decimal {"15"};
    constexpr auto d16 = decimal {"16"};
    constexpr auto d17 = decimal {"17"};
    constexpr auto d18 = decimal {"18"};
    constexpr auto d19 = decimal {"19"};
    
    template <auto & mod>
    using prime_field_element = math::prime_field_element<uint64, int64, mod>;
    
    template <auto & mod>
    using prime_field = math::prime_field<uint64, int64, mod>;
    
    TEST (PrimeFieldTest, TestPrimeField) {
        // We have to prove that a number is prime in order to 
        // construct a prime field, so we generate the first 8 primes. 
        stack<math::number::prime<uint64>> primes = reverse (math::number::eratosthenes<uint64> {8}.Primes);
        
        prime_field<d2> f2 {primes[0]};
        prime_field<d3> f3 {primes[1]};
        prime_field<d4> f4 {math::number::prime<uint64> {}};
        prime_field<d5> f5 {primes[2]};
        prime_field<d6> f6 {math::number::prime<uint64> {}};
        prime_field<d7> f7 {primes[3]};
        prime_field<d8> f8 {math::number::prime<uint64> {}};
        prime_field<d9> f9 {math::number::prime<uint64> {}};
        prime_field<d10> f10 {math::number::prime<uint64> {}};
        prime_field<d11> f11 {primes[4]};
        prime_field<d12> f12 {math::number::prime<uint64> {}};
        prime_field<d13> f13 {primes[5]};
        prime_field<d14> f14 {math::number::prime<uint64> {}};
        prime_field<d15> f15 {math::number::prime<uint64> {}};
        prime_field<d16> f16 {math::number::prime<uint64> {}};
        prime_field<d17> f17 {primes[6]};
        prime_field<d16> f18 {math::number::prime<uint64> {}};
        prime_field<d19> f19 {primes[7]};
        
        // fields are only valid when we have given 
        // a proof for the correct prime number. 
        EXPECT_TRUE (f2.valid ());
        EXPECT_TRUE (f3.valid ());
        EXPECT_FALSE (f4.valid ());
        EXPECT_TRUE (f5.valid ());
        EXPECT_FALSE (f6.valid ());
        EXPECT_TRUE (f7.valid ());
        EXPECT_FALSE (f8.valid ());
        EXPECT_FALSE (f9.valid ());
        EXPECT_FALSE (f10.valid ());
        EXPECT_TRUE (f11.valid ());
        EXPECT_FALSE (f12.valid ());
        EXPECT_TRUE (f13.valid ());
        EXPECT_FALSE (f14.valid ());
        EXPECT_FALSE (f15.valid ());
        EXPECT_FALSE (f16.valid ());
        EXPECT_TRUE (f17.valid ());
        EXPECT_FALSE (f18.valid ());
        EXPECT_TRUE (f19.valid ());
        
        // make the zero element for each prime field. 
        prime_field_element<d2> f2_0 = f2.make (0);
        prime_field_element<d3> f3_0 = f3.make (0);
        prime_field_element<d5> f5_0 = f5.make (0);
        prime_field_element<d7> f7_0 = f7.make (0);
        prime_field_element<d11> f11_0 = f11.make (0);
        prime_field_element<d13> f13_0 = f13.make (0);
        prime_field_element<d17> f17_0 = f17.make (0);
        prime_field_element<d19> f19_0 = f19.make (0);
        
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
        prime_field_element<d2> f2_1 = f2.make (1);
        prime_field_element<d3> f3_1 = f3.make (1);
        prime_field_element<d5> f5_1 = f5.make (1);
        prime_field_element<d7> f7_1 = f7.make (1);
        prime_field_element<d11> f11_1 = f11.make (1);
        prime_field_element<d13> f13_1 = f13.make (1);
        prime_field_element<d17> f17_1 = f17.make (1);
        prime_field_element<d19> f19_1 = f19.make (1);
        
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
        
        prime_field_element<d3> f3_2 = f3.make (2);
        prime_field_element<d5> f5_2 = f5.make (2);
        prime_field_element<d7> f7_2 = f7.make (2);
        prime_field_element<d11> f11_2 = f11.make (2);
        prime_field_element<d13> f13_2 = f13.make (2);
        prime_field_element<d17> f17_2 = f17.make (2);
        prime_field_element<d19> f19_2 = f19.make (2);
        
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
        EXPECT_EQ (f5_0 - f5_1, f5.make (4));
        EXPECT_EQ (f7_0 - f7_1, f7.make (6));
        EXPECT_EQ (f11_0 - f11_1, f11.make (10));
        EXPECT_EQ (f13_0 - f13_1, f13.make (12));
        EXPECT_EQ (f17_0 - f17_1, f17.make (16));
        EXPECT_EQ (f19_0 - f19_1, f19.make (18));
        
        EXPECT_EQ (f3_2.inverse (), f3_2);
        EXPECT_EQ (f5_2.inverse (), f5.make (3));
        EXPECT_EQ (f7_2.inverse (), f7.make (4));
        EXPECT_EQ (f11_2.inverse (), f11.make (6));
        EXPECT_EQ (f13_2.inverse (), f13.make (7));
        EXPECT_EQ (f17_2.inverse (), f17.make (9));
        EXPECT_EQ (f19_2.inverse (), f19.make (10));
        
        EXPECT_EQ (f5.make (3).inverse (), f5_2);
        EXPECT_EQ (f7.make (3).inverse (), f7.make (5));
        EXPECT_EQ (f11.make (3).inverse (), f11.make (4));
        EXPECT_EQ (f13.make (3).inverse (), f13.make (9));
        EXPECT_EQ (f17.make (3).inverse (), f17.make (6));
        EXPECT_EQ (f19.make (3).inverse (), f19.make (13));
        
        EXPECT_EQ (f3_2 * f3_2, f3_1);
        EXPECT_EQ (f5_2 * f5.make (3), f5_1);
        EXPECT_EQ (f7_2 * f7.make (4), f7_1);
        EXPECT_EQ (f11_2 * f11.make (6), f11_1);
        EXPECT_EQ (f13_2 * f13.make (7), f13_1);
        EXPECT_EQ (f17_2 * f17.make (9), f17_1);
        EXPECT_EQ (f19_2 * f19.make (10), f19_1);
        
        EXPECT_EQ (f5.make (3) * f5_2, f5_1);
        EXPECT_EQ (f7.make (3) * f7.make (5), f7_1);
        EXPECT_EQ (f11.make (3) * f11.make (4), f11_1);
        EXPECT_EQ (f13.make (3) * f13.make (9), f13_1);
        EXPECT_EQ (f17.make (3) * f17.make (6), f17_1);
        EXPECT_EQ (f19.make (3) * f19.make (13), f19_1);
        
    }
    
}
