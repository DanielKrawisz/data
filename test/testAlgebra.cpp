// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/math/number/natural.hpp>
#include <data/math/linear/inner.hpp>
#include <data/math/algebra/symmetric_group.hpp>
#include <data/math/algebra/finite_field.hpp>
#include <data/math/algebra/alternating_group.hpp>
#include <data/math/algebra/cyclic_group.hpp>
#include <data/math/algebra/dihedral_group.hpp>
#include <data/math.hpp>
#include "gtest/gtest.h"

namespace data::math::linear {
    
    template <typename F, typename op = plus<F>> requires group<F, op> struct test_group {
        test_group () {}
    }; 
    
    TEST (AlgebraTest, TestGroup) {

        test_group<cyclic_group<uint64 {1}>> {};
        test_group<cyclic_group<uint64 {2}>> {};
        test_group<cyclic_group<uint64 {3}>> {};
        test_group<cyclic_group<uint64 {4}>> {};
        test_group<cyclic_group<uint64 {5}>> {};
        test_group<cyclic_group<uint64 {6}>> {};
        test_group<cyclic_group<uint64 {7}>> {};
        test_group<cyclic_group<uint64 {8}>> {};
        
        test_group<dihedral_group<uint64 {1}>, times<dihedral_group<uint64 {1}>>> {};
        test_group<dihedral_group<uint64 {2}>, times<dihedral_group<uint64 {2}>>> {};
        test_group<dihedral_group<uint64 {3}>, times<dihedral_group<uint64 {3}>>> {};
        test_group<dihedral_group<uint64 {4}>, times<dihedral_group<uint64 {4}>>> {};
        test_group<dihedral_group<uint64 {5}>, times<dihedral_group<uint64 {5}>>> {};
        test_group<dihedral_group<uint64 {6}>, times<dihedral_group<uint64 {6}>>> {};
        test_group<dihedral_group<uint64 {7}>, times<dihedral_group<uint64 {7}>>> {};
        test_group<dihedral_group<uint64 {8}>, times<dihedral_group<uint64 {8}>>> {};
        
        test_group<symmetric_group<uint64 {1}>, times<symmetric_group<uint64 {1}>>> {};
        test_group<symmetric_group<uint64 {2}>, times<symmetric_group<uint64 {2}>>> {};
        test_group<symmetric_group<uint64 {3}>, times<symmetric_group<uint64 {3}>>> {};
        test_group<symmetric_group<uint64 {4}>, times<symmetric_group<uint64 {4}>>> {};
        test_group<symmetric_group<uint64 {5}>, times<symmetric_group<uint64 {5}>>> {};
        test_group<symmetric_group<uint64 {6}>, times<symmetric_group<uint64 {6}>>> {};
        test_group<symmetric_group<uint64 {7}>, times<symmetric_group<uint64 {7}>>> {};
        test_group<symmetric_group<uint64 {8}>, times<symmetric_group<uint64 {8}>>> {};
        
        test_group<alternating_group<uint64 {1}>, times<alternating_group<uint64 {1}>>> {};
        test_group<alternating_group<uint64 {2}>, times<alternating_group<uint64 {2}>>> {};
        test_group<alternating_group<uint64 {3}>, times<alternating_group<uint64 {3}>>> {};
        test_group<alternating_group<uint64 {4}>, times<alternating_group<uint64 {4}>>> {};
        test_group<alternating_group<uint64 {5}>, times<alternating_group<uint64 {5}>>> {};
        test_group<alternating_group<uint64 {6}>, times<alternating_group<uint64 {6}>>> {};
        test_group<alternating_group<uint64 {7}>, times<alternating_group<uint64 {7}>>> {};
        test_group<alternating_group<uint64 {8}>, times<alternating_group<uint64 {8}>>> {};
    }
    
    template <typename F> requires field<F> struct test_field {
        test_field () {

            EXPECT_EQ (F {0} * F {0}, F {0});
            EXPECT_EQ (F {0} + F {0}, F {0});
            EXPECT_EQ (F {1} * F {0}, F {0});
            EXPECT_EQ (F {1} + F {0}, F {1});
            EXPECT_EQ (F {1} * F {1}, F {1});

        }
    }; 
    
    template <auto mod>
    using prime_field = math::prime_field_element<mod, uint64>;
    
    TEST (AlgebraTest, TestField) {
        test_field<double> {};
        test_field<complex128> {};
        test_field<Q> {};
        test_field<QC> {};
        test_field<prime_field<uint64 {2}>> {};
        test_field<prime_field<uint64 {3}>> {};
        test_field<prime_field<uint64 {5}>> {};
        test_field<prime_field<uint64 {7}>> {};
        test_field<prime_field<uint64 {11}>> {};
        test_field<prime_field<uint64 {13}>> {};
        test_field<prime_field<uint64 {17}>> {};
        test_field<prime_field<uint64 {19}>> {};
    }
    
    template <typename F, typename V> requires space<F, V> struct test_vector_space {
        test_vector_space () {}
    }; 
    
    template <typename F, typename V> requires inner_space<F, V> struct test_inner_space {
        test_inner_space () {}
    }; 
    
    template <typename F, typename V> requires field<V> struct test_division_algebra : test_vector_space<F, V> {
        test_division_algebra () {}
    }; 
    
    TEST (AlgebraTest, TestVectorSpaces) {
        
        test_inner_space<double, array<double, 0>> {};
        test_inner_space<double, array<double, 1>> {};
        test_inner_space<double, array<double, 2>> {};

        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<double, array<double, 0>>::value == 0);
        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<double, array<double, 1>>::value == 1);
        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<double, array<double, 2>>::value == 2);
        
        test_inner_space<double, array<double, 0, 1>> {};
        test_inner_space<double, array<double, 1, 2>> {};
        test_inner_space<double, array<double, 2, 3>> {};
        
        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<double, array<double, 0, 1>>::value == 0);
        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<double, array<double, 1, 2>>::value == 2);
        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<double, array<double, 2, 3>>::value == 6);

        test_inner_space<complex128, array<complex128, 0>> {};
        test_inner_space<complex128, array<complex128, 1>> {};
        test_inner_space<complex128, array<complex128, 2>> {};

        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<complex128, array<complex128, 0>>::value == 0);
        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<complex128, array<complex128, 1>>::value == 1);
        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<complex128, array<complex128, 2>>::value == 2);

        test_inner_space<Q, array<Q, 0>> {};
        test_inner_space<Q, array<Q, 1>> {};
        test_inner_space<Q, array<Q, 2>> {};
        
        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<Q, array<Q, 0>>::value == 0);
        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<Q, array<Q, 1>>::value == 1);
        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<Q, array<Q, 2>>::value == 2);
        
        test_inner_space<QC, array<QC, 0>> {};
        test_inner_space<QC, array<QC, 1>> {};
        test_inner_space<QC, array<QC, 2>> {};
        
        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<QC, array<QC, 0>>::value == 0);
        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<QC, array<QC, 1>>::value == 1);
        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<QC, array<QC, 2>>::value == 2);

        test_division_algebra<double, double> {};
        test_division_algebra<double, complex128> {};
        test_division_algebra<double, quat256> {};
        test_division_algebra<double, oct512> {};

        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<double, double>::value == 1);
        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<double, complex128>::value == 2);
        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<double, quat256>::value == 4);
        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<double, oct512>::value == 8);
        
        test_vector_space<complex128, complex128> {};
        test_vector_space<complex128, quat256> {};
        test_vector_space<complex128, oct512> {};

        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<complex128, complex128>::value == 1);
        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<complex128, quat256>::value == 2);
        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<complex128, oct512>::value == 4);

        test_division_algebra<Q, Q> {};
        test_division_algebra<Q, QC> {};
        test_division_algebra<Q, QH> {};
        test_division_algebra<Q, QO> {};
        
        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<Q, Q>::value == 1);
        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<Q, QC>::value == 2);
        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<Q, QH>::value == 4);
        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<Q, QO>::value == 8);
        
        test_vector_space<QC, QC> {};
        test_vector_space<QC, QH> {};
        test_vector_space<QC, QO> {};
        
        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<QC, QC>::value == 1);
        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<QC, QH>::value == 2);
        [] (bool x) -> void { EXPECT_TRUE (x); } (dimensions<QC, QO>::value == 4);
        
    }
}
