// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/numbers.hpp"
#include "gtest/gtest.h"
#include <data/version.hpp>
namespace data {
    
    TEST (AbsTest, TestDecrement) {

        int_big<9> ib9 {0};
        int_big<10> ib10 {0};
        int_big<11> ib11 {0};
        int_big<20> ib20 {0};
        int_little<9> il9 {0};
        int_little<10> il10 {0};
        int_little<11> il11 {0};
        int_little<20> il20 {0};
        
        EXPECT_EQ ( --ib9,  int_big<9> {"0xffffffffffffffffff"});
        EXPECT_EQ (--ib10, int_big<10> {"0xffffffffffffffffffff"});
        EXPECT_EQ (--ib11, int_big<11> {"0xffffffffffffffffffffff"});
        EXPECT_EQ (--ib20, int_big<20> {"0xffffffffffffffffffffffffffffffffffffffff"});
        EXPECT_EQ ( --il9,  int_little<9> {"0xffffffffffffffffff"});
        EXPECT_EQ (--il10, int_little<10> {"0xffffffffffffffffffff"});
        EXPECT_EQ (--il11, int_little<11> {"0xffffffffffffffffffffff"});
        EXPECT_EQ (--il20, int_little<20> {"0xffffffffffffffffffffffffffffffffffffffff"});
        
        EXPECT_LT (int_big<9> {"0x800000000000000000"},
                   int_big<9> {"0x7fffffffffffffffff"});
        EXPECT_LT (int_big<10> {"0x80000000000000000000"},
                   int_big<10> {"0x7fffffffffffffffffff"});
        EXPECT_LT (int_big<11> {"0x8000000000000000000000"},
                   int_big<11> {"0x7fffffffffffffffffffff"});
        EXPECT_LT (int_big<20> {"0x8000000000000000000000000000000000000000"},
                   int_big<20> {"0x7fffffffffffffffffffffffffffffffffffffff"});
        EXPECT_LT (int_little<9> {"0x800000000000000000"},
                   int_little<9> {"0x7fffffffffffffffff"});
        EXPECT_LT (int_little<10> {"0x80000000000000000000"},
                   int_little<10> {"0x7fffffffffffffffffff"});
        EXPECT_LT (int_little<11> {"0x8000000000000000000000"},
                   int_little<11> {"0x7fffffffffffffffffffff"});
        EXPECT_LT (int_little<20> {"0x8000000000000000000000000000000000000000"},
                   int_little<20> {"0x7fffffffffffffffffffffffffffffffffffffff"});
        
    }
    
    template <typename...> struct test_case_negative;
    
    template <typename X> struct test_case_negative<X> {
        test_case_negative (int64 g, int64 e) {
            auto given = X {g};
            auto minus = -given;
            auto expected = X {e};
        }
    };
    
    template <typename X, typename... P> struct test_case_negative<X, P...> : test_case_negative<X>, test_case_negative<P...> {
        test_case_negative (int64 g, int64 e): test_case_negative<X> {g, e}, test_case_negative<P...> {g, e} {}
    };
    
    TEST (AbsTest, TestNegative) {
        
        using test_case = test_case_negative<int64, Z, Z_bytes_little, Z_bytes_big, 
            Z_bytes_twos_little, Z_bytes_twos_big, 
            int_big<9>, int_big<10>, int_big<11>, int_big<20>, 
            int_little<9>, int_little<10>, int_little<11>, int_little<20>, 
            dec_int, hex_int, hex_int_twos>;
        
        test_case {0, 0};
        test_case {-1, 1};
        test_case {1, -1};
        test_case {-2, 2};
        test_case {2, -2};
        test_case {-707685211092354, 707685211092354};
        test_case {707685211092354, -707685211092354};
        
    }
    
    template <typename...> struct test_case_abs;
    
    template <typename Z, typename N> struct test_case_abs<Z, N> {
        test_case_abs (int64 g, uint64 e) {
            auto given = Z {g};
            auto expected = N {e};
            auto abs = math::abs<Z> {} (given);
            EXPECT_EQ (abs, expected) << "expected abs (" << given << ") -> " << abs << " to equal " << expected;
        }
    };
    
    template <typename Z, typename N, typename... P> struct test_case_abs<Z, N, P...> : test_case_abs<Z, N>, test_case_abs<P...> {
        test_case_abs (int64 g, uint64 e): test_case_abs<Z, N> {g, e}, test_case_abs<P...> {g, e} {}
    };
    
    TEST (AbsTest, TestAbs) {
        
        using test_case = test_case_abs<
            int64, uint64, Z, N,
            int_big<20>, uint_big<20>,
            int_big<9>, uint_big<9>,
            int_big<10>, uint_big<10>, 
            int_big<11>, uint_big<11>,
            int_little<20>, uint_little<20>, 
            int_little<9>, uint_little<9>, 
            int_little<10>, uint_little<10>, 
            int_little<11>, uint_little<11>,
            Z_bytes_little, N_bytes_little,
            Z_bytes_big, N_bytes_big,
            Z_bytes_twos_big, Z_bytes_twos_big,
            Z_bytes_twos_little, Z_bytes_twos_little,
            dec_int, dec_uint,
            hex_int, hex_uint,
            hex_int_twos, hex_int_twos>;
        
        test_case {0, 0u};
        test_case {1, 1u};
        test_case {-1, 1u};
        test_case (70768521109235, 70768521109235u);
        test_case (-70768521109235, 70768521109235u);
        
    }
}

