// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/math/abs.hpp"
#include "data/data.hpp"
#include <data/math/number/bounded/bounded.hpp>
#include "gtest/gtest.h"

namespace data {

    template <size_t size> 
    using int_little = data::math::number::bounded<true, endian::little, size>;
    
    template <size_t size> 
    using int_big = data::math::number::bounded<true, endian::big, size>;
    
    template <size_t size> 
    using uint_little = data::math::number::bounded<false, endian::little, size>;
    
    template <size_t size> 
    using uint_big = data::math::number::bounded<false, endian::big, size>;
    
    using Z_bytes_little = data::math::number::Z_bytes<endian::little>;
    using Z_bytes_big = data::math::number::Z_bytes<endian::big>;
    
    using N_bytes_little = data::math::number::N_bytes<endian::little>;
    using N_bytes_big = data::math::number::N_bytes<endian::big>;
    
    using N = data::math::number::GMP::N;
    using Z = data::math::number::GMP::Z;

    TEST(AbsTest, TestDecrement) {
        
        int_big<9> ib9{0};
        int_big<10> ib10{0};
        int_big<11> ib11{0};
        int_big<20> ib20{0};
        int_little<9> il9{0};
        int_little<10> il10{0};
        int_little<11> il11{0};
        int_little<20> il20{0};
        
        EXPECT_EQ( --ib9,  int_big<9>{"0xffffffffffffffffff"});
        EXPECT_EQ(--ib10, int_big<10>{"0xffffffffffffffffffff"});
        EXPECT_EQ(--ib11, int_big<11>{"0xffffffffffffffffffffff"});
        EXPECT_EQ(--ib20, int_big<20>{"0xffffffffffffffffffffffffffffffffffffffff"});
        EXPECT_EQ( --il9,  int_little<9>{"0xffffffffffffffffff"});
        EXPECT_EQ(--il10, int_little<10>{"0xffffffffffffffffffff"});
        EXPECT_EQ(--il11, int_little<11>{"0xffffffffffffffffffffff"});
        EXPECT_EQ(--il20, int_little<20>{"0xffffffffffffffffffffffffffffffffffffffff"});
        
        EXPECT_LT(int_big<9>{"0x800000000000000000"}, 
                  int_big<9>{"0x7fffffffffffffffff"});
        EXPECT_LT(int_big<10>{"0x80000000000000000000"}, 
                  int_big<10>{"0x7fffffffffffffffffff"});
        EXPECT_LT(int_big<11>{"0x8000000000000000000000"}, 
                  int_big<11>{"0x7fffffffffffffffffffff"});
        EXPECT_LT(int_big<20>{"0x8000000000000000000000000000000000000000"},
                  int_big<20>{"0x7fffffffffffffffffffffffffffffffffffffff"});
        EXPECT_LT(int_little<9>{"0x800000000000000000"}, 
                  int_little<9>{"0x7fffffffffffffffff"});
        EXPECT_LT(int_little<10>{"0x80000000000000000000"}, 
                  int_little<10>{"0x7fffffffffffffffffff"});
        EXPECT_LT(int_little<11>{"0x8000000000000000000000"}, 
                  int_little<11>{"0x7fffffffffffffffffffff"});
        EXPECT_LT(int_little<20>{"0x8000000000000000000000000000000000000000"}, 
                  int_little<20>{"0x7fffffffffffffffffffffffffffffffffffffff"});
        
    }
    
    template <typename...> struct test_case_negative;
    
    template <typename X> struct test_case_negative<X> {
        test_case_negative(int64 g, int64 e) {
            std::cout << "test negative " << g << " to " << e << std::endl;
            auto given = X{g};
            auto minus = -given;
            auto expected = X{e};
            EXPECT_EQ(minus, expected) << "expected -" << given << " -> " << minus << " to equal " << expected;
        }
    };
    
    template <typename X, typename... P> struct test_case_negative<X, P...> : test_case_negative<X>, test_case_negative<P...> {
        test_case_negative(int64 g, int64 e): test_case_negative<X>{g, e}, test_case_negative<P...>{g, e} {}
    };
    
    TEST(AbsTest, TestNegative) {
        
        using test_case = test_case_negative<int64, Z, Z_bytes_little, Z_bytes_big, 
            int_big<9>, int_big<10>, int_big<11>, int_big<20>, 
            int_little<9>, int_little<10>, int_little<11>, int_little<20>>;
        
        test_case{0, 0};
        test_case{-1, 1};
        test_case{1, -1};
        test_case{-2, 2};
        test_case{2, -2};
        test_case{-707685211092354, 707685211092354};
        test_case{707685211092354, -707685211092354};
        
    }
    
    template <typename...> struct test_case_abs;
    
    template <typename X> struct test_case_abs<X> {
        test_case_abs(int64 g, int64 e) {
            auto given = X{g};
            auto abs = math::abs<X>{}(given);
            auto expected = X{e};
            EXPECT_EQ(abs, expected) << "expected abs(" << given << ") -> " << abs << " to equal " << expected;
        }
    };
    
    template <typename X, typename... P> struct test_case_abs<X, P...> : test_case_abs<X>, test_case_abs<P...> {
        test_case_abs(int64 g, int64 e): test_case_abs<X>{g, e}, test_case_abs<P...>{g, e} {}
    };
    
    TEST(AbsTest, TestAbs) {
        
        using test_case = test_case_abs<int64, int_big<20>, int_big<9>, 
            int_big<10>, int_big<11>, int_little<20>, int_little<9>, int_little<10>, int_little<11>, 
            Z, Z_bytes_little, Z_bytes_big>;
        
        test_case{0, 0};
        test_case{1, 1};
        test_case{-1, 1};
        test_case(70768521109235, 70768521109235);
        test_case(-70768521109235, 70768521109235);
        
    }
}

