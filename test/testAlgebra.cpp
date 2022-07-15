// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/math/linear/inner.hpp>
#include <data/math/algebra/symmetric_group.hpp>
#include <data/math/algebra/finite_field.hpp>
#include <data/math/algebra/alternating_group.hpp>
#include <data/math/algebra/cyclic_group.hpp>
#include <data/math/algebra/dihedral_group.hpp>
#include <data/numbers.hpp>
#include "gtest/gtest.h"

namespace data::math::linear {
    // representations of numbers. 
    constexpr auto d1 = decimal{"1"};
    constexpr auto d2 = decimal{"2"};
    constexpr auto d3 = decimal{"3"};
    constexpr auto d4 = decimal{"4"};
    constexpr auto d5 = decimal{"5"};
    constexpr auto d6 = decimal{"6"};
    constexpr auto d7 = decimal{"7"};
    constexpr auto d8 = decimal{"8"};
    constexpr auto d9 = decimal{"9"};
    constexpr auto d10 = decimal{"10"};
    constexpr auto d11 = decimal{"11"};
    constexpr auto d12 = decimal{"12"};
    constexpr auto d13 = decimal{"13"};
    constexpr auto d14 = decimal{"14"};
    constexpr auto d15 = decimal{"15"};
    constexpr auto d16 = decimal{"16"};
    constexpr auto d17 = decimal{"17"};
    constexpr auto d18 = decimal{"18"};
    constexpr auto d19 = decimal{"19"};
    
    template <typename F, typename op = plus<F>> requires group<F, op> struct test_group {
        test_group() {}
    }; 
    
    TEST(AlgebraTest, TestGroup) {
        test_group<cyclic_group<uint64, d1>>{};
        test_group<cyclic_group<uint64, d2>>{};
        test_group<cyclic_group<uint64, d3>>{};
        test_group<cyclic_group<uint64, d4>>{};
        test_group<cyclic_group<uint64, d5>>{};
        test_group<cyclic_group<uint64, d6>>{};
        test_group<cyclic_group<uint64, d7>>{};
        test_group<cyclic_group<uint64, d8>>{};
        
        test_group<dihedral_group<uint64, d1>, times<dihedral_group<uint64, d1>>>{};
        test_group<dihedral_group<uint64, d2>, times<dihedral_group<uint64, d2>>>{};
        test_group<dihedral_group<uint64, d3>, times<dihedral_group<uint64, d3>>>{};
        test_group<dihedral_group<uint64, d4>, times<dihedral_group<uint64, d4>>>{};
        test_group<dihedral_group<uint64, d5>, times<dihedral_group<uint64, d5>>>{};
        test_group<dihedral_group<uint64, d6>, times<dihedral_group<uint64, d6>>>{};
        test_group<dihedral_group<uint64, d7>, times<dihedral_group<uint64, d7>>>{};
        test_group<dihedral_group<uint64, d8>, times<dihedral_group<uint64, d8>>>{};
        
        test_group<symmetric_group<uint64, d1>, times<symmetric_group<uint64, d1>>>{};
        test_group<symmetric_group<uint64, d2>, times<symmetric_group<uint64, d2>>>{};
        test_group<symmetric_group<uint64, d3>, times<symmetric_group<uint64, d3>>>{};
        test_group<symmetric_group<uint64, d4>, times<symmetric_group<uint64, d4>>>{};
        test_group<symmetric_group<uint64, d5>, times<symmetric_group<uint64, d5>>>{};
        test_group<symmetric_group<uint64, d6>, times<symmetric_group<uint64, d6>>>{};
        test_group<symmetric_group<uint64, d7>, times<symmetric_group<uint64, d7>>>{};
        test_group<symmetric_group<uint64, d8>, times<symmetric_group<uint64, d8>>>{};
        
        test_group<alternating_group<uint64, d1>, times<alternating_group<uint64, d1>>>{};
        test_group<alternating_group<uint64, d2>, times<alternating_group<uint64, d2>>>{};
        test_group<alternating_group<uint64, d3>, times<alternating_group<uint64, d3>>>{};
        test_group<alternating_group<uint64, d4>, times<alternating_group<uint64, d4>>>{};
        test_group<alternating_group<uint64, d5>, times<alternating_group<uint64, d5>>>{};
        test_group<alternating_group<uint64, d6>, times<alternating_group<uint64, d6>>>{};
        test_group<alternating_group<uint64, d7>, times<alternating_group<uint64, d7>>>{};
        test_group<alternating_group<uint64, d8>, times<alternating_group<uint64, d8>>>{};
    }
    
    template <typename F> requires field<F> struct test_field {
        test_field() {}
    }; 
    
    template <auto & mod>
    using prime_field = math::prime_field_element<uint64, int64, mod>;
    
    TEST(AlgebraTest, TestField) {
        test_field<double>{};
        test_field<complex128>{};/*
        test_field<Q>{};
        test_field<QC>{};
        test_field<prime_field<d2>>{};
        test_field<prime_field<d3>>{};
        test_field<prime_field<d5>>{};
        test_field<prime_field<d7>>{};
        test_field<prime_field<d11>>{};
        test_field<prime_field<d13>>{};
        test_field<prime_field<d17>>{};
        test_field<prime_field<d19>>{};*/
    }
    
    template <typename F, typename V> requires space<F, V> struct test_vector_space {
        test_vector_space() {}
    }; 
    
    template <typename F, typename V> requires inner_space<F, V> struct test_inner_space {
        test_inner_space() {}
    }; 
    
    template <typename F, typename V> requires division_algebra<F, V> struct test_division_algebra : test_vector_space<F, V> {
        test_division_algebra() {}
    }; 
    
    TEST(AlgebraTest, TestVectorSpaces) {
        
        test_inner_space<double, array<double, 0>>{};
        test_inner_space<double, array<double, 1>>{};
        test_inner_space<double, array<double, 2>>{};
        
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<double, array<double, 0>>::value == 0);
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<double, array<double, 1>>::value == 1);
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<double, array<double, 2>>::value == 2);
        
        test_inner_space<double, array<double, 0, 1>>{};
        test_inner_space<double, array<double, 1, 2>>{};
        test_inner_space<double, array<double, 2, 3>>{};
        
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<double, array<double, 0, 1>>::value == 0);
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<double, array<double, 1, 2>>::value == 2);
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<double, array<double, 2, 3>>::value == 6);
        /*
        test_inner_space<complex128, array<complex128, 0>>{};
        test_inner_space<complex128, array<complex128, 1>>{};
        test_inner_space<complex128, array<complex128, 2>>{};
        
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<complex128, array<complex128, 0>>::value == 0);
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<complex128, array<complex128, 1>>::value == 1);
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<complex128, array<complex128, 2>>::value == 2);*/
        /*
        test_inner_space<Q, array<Q, 0>>{};
        test_inner_space<Q, array<Q, 1>>{};
        test_inner_space<Q, array<Q, 2>>{};
        
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<Q, array<Q, 0>>::value == 0);
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<Q, array<Q, 1>>::value == 1);
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<Q, array<Q, 2>>::value == 2);
        
        test_inner_space<CQ, array<CQ, 0>>{};
        test_inner_space<CQ, array<CQ, 1>>{};
        test_inner_space<CQ, array<CQ, 2>>{};
        
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<CQ, array<CQ, 0>>::value == 0);
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<CQ, array<CQ, 1>>::value == 1);
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<CQ, array<CQ, 2>>::value == 2);
        
        test_division_algebra<double, double>{};
        test_division_algebra<double, complex128>{};
        test_division_algebra<double, quat256>{};
        test_division_algebra<double, oct512>{};
        
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<double, double>::value == 1);
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<double, complex128>::value == 2);
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<double, quat256>::value == 4);
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<double, oct512>::value == 8);
        
        test_vector_space<complex128, complex128>{};
        test_vector_space<complex128, quat256>{};
        test_vector_space<complex128, oct512>{};
        
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<complex128, complex128>::value == 1);
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<complex128, quat256>::value == 2);
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<complex128, oct512>::value == 4);
        
        test_division_algebra<Q, Q>{};
        test_division_algebra<Q, CQ>{};
        test_division_algebra<Q, HQ>{};
        test_division_algebra<Q, OQ>{};
        
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<Q, Q>::value == 1);
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<Q, CQ>::value == 2);
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<Q, HQ>::value == 4);
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<Q, OQ>::value == 8);
        
        test_vector_space<CQ, CQ>{};
        test_vector_space<CQ, HQ>{};
        test_vector_space<CQ, OQ>{};
        
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<CQ, CQ>::value == 1);
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<CQ, HQ>::value == 2);
        [](bool x) -> void { EXPECT_TRUE(x); } (dimensions<CQ, OQ>::value == 4);*/
        
    }
}
