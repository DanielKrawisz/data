// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include <data/math/number/bytes.hpp>
#include <data/math/number/gmp/gmp.hpp>
#include <data/math/number/bounded.hpp>
#include <data/encoding/base58.hpp>
#include "gtest/gtest.h"

namespace data::math {
    
    using Nb = number::N_bytes<endian::big>;
    using Nl = number::N_bytes<endian::little>;
    
    using Zb1 = number::Z_bytes<endian::big, number::ones>;
    using Zl1 = number::Z_bytes<endian::little, number::ones>;
    using Zb2 = number::Z_bytes<endian::big, number::twos>;
    using Zl2 = number::Z_bytes<endian::little, number::twos>;
    
    typedef int_big<9> sb09;
    typedef int_big<10> sb10;
    typedef int_big<11> sb11;
    typedef int_big<20> sb20;
    typedef int_little<9> sl09;
    typedef int_little<10> sl10;
    typedef int_little<11> sl11;
    typedef int_little<20> sl20;
    
    typedef uint_big<9> ub09;
    typedef uint_big<10> ub10;
    typedef uint_big<11> ub11;
    typedef uint_big<20> ub20;
    typedef uint_little<9> ul09;
    typedef uint_little<10> ul10;
    typedef uint_little<11> ul11;
    typedef uint_little<20> ul20;
    
    // this tests numbers that can take positive values. 
    template <typename N> requires whole_number<N> && requires(const N &a, const N &b) {
        {a == b};
        {a != b};
        {a > b};
        {a < b};
        {a <= b};
        {a >= b};
        {a + b};
        {a - b};
        {a * b};
        {a / b} -> std::same_as<N>;
    } && requires(N &a, const N &b) {
        {a += b};
        {a -= b};
        {a *= b};
    } && requires(const N &a, uint64 b) {
        {a == b};
        {a != b};
        {a > b};
        {a < b};
        {a <= b};
        {a >= b};
        {a + b};
        {a - b};
        {a * b};
    } && requires(N &a, uint64 b) {
        {a += b};
        {a *= b};
        {a -= b};
    } && requires(const N &a, int64 b) {
        {a == b};
        {a != b};
        {a > b};
        {a < b};
        {a <= b};
        {a >= b};
        {a + b};
        {a - b};
        {a * b};
    } struct test_positive_number {
        test_positive_number() {
            
            EXPECT_EQ(N{0}, N{0});
            EXPECT_EQ(N{1}, N{1});
            EXPECT_EQ(N{0}, 0);
            EXPECT_EQ(N{1}, 1);
            EXPECT_GE(N{1}, N{0});
            EXPECT_LE(N{0}, N{1});
            EXPECT_GE(N{1}, 0);
            EXPECT_LE(N{0}, 1);
            EXPECT_GT(N{1}, N{0});
            EXPECT_LT(N{0}, N{1});
            EXPECT_GT(N{1}, 0);
            EXPECT_LT(N{0}, 1);
            
            EXPECT_TRUE(number::is_zero(N{0}));
            EXPECT_FALSE(number::is_zero(N{1}));
            EXPECT_FALSE(number::is_positive(N{0}));
            EXPECT_TRUE(number::is_positive(N{1}));
            EXPECT_FALSE(number::is_negative(N{1}));
            EXPECT_FALSE(number::is_negative(N{0}));
            
            EXPECT_EQ(N{0} << 1, N{0});
            EXPECT_EQ(N{0} >> 1, N{0});
            
            EXPECT_EQ(N{1} << 1, N{2});
            EXPECT_EQ(N{1} >> 1, N{0});
            
            EXPECT_EQ(N{0} + N{0}, N{0});
            EXPECT_EQ(N{0} * N{0}, N{0});
            EXPECT_EQ(N{0} - N{0}, N{0});
            
            EXPECT_EQ(N{0} + N{1}, N{1});
            EXPECT_EQ(N{1} + N{0}, N{1});
            EXPECT_EQ(N{0} * N{1}, N{0});
            EXPECT_EQ(N{1} * N{0}, N{0});
            EXPECT_EQ(N{1} + N{1}, N{2});
            EXPECT_EQ(N{1} * N{1}, N{1});
            
            EXPECT_EQ(N{0} + 1, N{1});
            EXPECT_EQ(N{1} + 0, N{1});
            EXPECT_EQ(N{0} * 1, N{0});
            EXPECT_EQ(N{1} * 0, N{0});
            EXPECT_EQ(N{1} + 1, N{2});
            EXPECT_EQ(N{1} * 1, N{1});
            
            EXPECT_GT(double(N{1}), 0.0);
            
            EXPECT_EQ(sign(N{0}), number::zero);
            EXPECT_EQ(sign(N{1}), number::positive);
            
            EXPECT_EQ(N{1}/N{1}, N{1});
            
            N i{0};
            EXPECT_EQ(++i, 1);
            EXPECT_EQ(i++, 1);
            EXPECT_EQ(i, 2);
            EXPECT_EQ(i--, 2);
            EXPECT_EQ(--i, 0);
            
            i += 1;
            i -= 1;
            i *= 1;
            
            i += N{1};
            i -= N{1};
            i *= N{1};
            
        }
    };
    
    // this is for numbers that can also take negative values. 
    template <typename N>
    struct test_negative_number : test_positive_number<N> {
        test_negative_number() {
            
            EXPECT_EQ(Z{-1}, Z{-1});
            EXPECT_EQ(-Z{0}, Z{0});
            EXPECT_EQ(-Z{1}, Z{-1});
            EXPECT_EQ(-Z{-1}, Z{1});
            
            EXPECT_FALSE(number::is_zero(Z{-1}));
            EXPECT_FALSE(number::is_positive(Z{-1}));
            EXPECT_TRUE(number::is_negative(Z{-1}));
            
            EXPECT_EQ(Z{-1} << 1, Z{-2});
            EXPECT_EQ(Z{-1} >> 1, Z{0});
            
            EXPECT_EQ(sign(Z{-1}), number::negative);
            
        }
    };
    
    // we test systems of numbers that are related by the abs function. 
    // types N and Z may be the same. 
    template <typename N, typename Z> 
    requires countable<N> && integer<Z, N> && requires(const N &n) {
        {data::abs(n)} -> std::same_as<N>;
    } && requires (const Z &z) {
        {data::abs(z)} -> std::same_as<N>;
    } && requires(const N &a, const N &b) {
        {divide(a, b)};
        {divides(a, b)};
        {power(a, b)};
    } && requires(const Z &a, const Z &b) {
        {divide(a, b)};
        {divides(a, b)};
    } && requires(const N &a, const Z &b) {
        {power(b, a)};
        {a == b};
        {a != b};
        {a > b};
        {a < b};
        {a <= b};
        {a >= b};
        {a + b};
        {a - b};
        {a * b};
        {b == a};
        {b != a};
        {b > a};
        {b < a};
        {b <= a};
        {b >= a};
        {b + a};
        {b - a};
        {b * a};
        {b % a} -> std::convertible_to<N>;
    } struct number_system {
        number_system() {
            
            EXPECT_EQ(data::abs(N{0}), N{0});
            EXPECT_EQ(data::abs(N{1}), N{1});
            EXPECT_EQ(data::abs(N{2}), N{2});
            EXPECT_EQ(data::abs(Z{0}), N{0});
            EXPECT_EQ(data::abs(Z{1}), N{1});
            EXPECT_EQ(data::abs(Z{2}), N{2});
            EXPECT_EQ(data::abs(Z{-1}), N{1});
            EXPECT_EQ(data::abs(Z{-2}), N{2});
            
            EXPECT_EQ(square(N{0}), N{0});
            EXPECT_EQ(square(N{1}), N{1});
            EXPECT_EQ(square(N{2}), N{4});
            EXPECT_EQ(square(Z{0}), N{0});
            EXPECT_EQ(square(Z{1}), N{1});
            EXPECT_EQ(square(Z{2}), N{4});
            EXPECT_EQ(square(Z{-1}), N{1});
            EXPECT_EQ(square(Z{-2}), N{4});
            
            auto z = identity<plus<N>, N>{}();
            auto o = identity<times<N>, N>{}();
            
            EXPECT_EQ(z, N{0});
            EXPECT_EQ(o, N{1});
            
            EXPECT_EQ(zero<N>{}(), N{0});
            
            EXPECT_EQ(increment(zero<N>{}()), N{1});
            
            EXPECT_EQ(first<N>{}(), N{0});
            
            EXPECT_EQ(increment(zero<Z>{}()), Z{1});
            
            EXPECT_EQ(decrement(increment(zero<Z>{}())), zero<Z>{}());
            EXPECT_EQ(decrement(zero<Z>{}()), Z{-1});
            
            EXPECT_EQ(-N{0}, Z{0});
            EXPECT_EQ(-N{1}, Z{-1});
            EXPECT_EQ(N{0} - N{1}, N{0});
            EXPECT_EQ(Z{0} - Z{1}, Z{-1});
            
            EXPECT_EQ(N{1}%N{1}, N{1});
            EXPECT_EQ(Z{1}%N{1}, N{1});
        }
    };
    
    template <typename N> struct not_natural {
        static const bool value = true;
    };
    
    template <natural N> struct not_natural<N> {
        static const bool value = false;
    };
    
    template <typename N> struct not_integer {
        static const bool value = true;
    };
    
    template <integer N> struct not_integer<N> {
        static const bool value = false;
    };
    
    template <typename N> constexpr bool is_not_natural = not_natural<N>::value;
    template <typename N> constexpr bool is_not_integer = not_integer<N>::value;
    template <typename N> constexpr bool is_natural = !not_natural<N>::value;
    template <typename N> constexpr bool is_integer = !not_integer<N>::value;
    
    // there are two kinds of number systems that we test here. One
    // with two types for natural numbers N and integers Z, such that
    // abs(Z) => N. The other kind has one type of integers such that
    // abs(Z) => Z. 
    
    // this is for the version with two types. 
    template <typename N, typename Z> requires natural<N> && is_not_integer<N> && is_not_natural<Z>
    struct test_natural_integer_system : number_system<N, Z>, test_positive_number<N>, test_negative_number<Z> {};
    
    // this is for one type. 
    template <typename Z> requires is_not_natural<Z>
    struct test_integer_system : number_system<Z, Z>, test_negative_number<Z> {};
    
    TEST(NumbersTest, TestNumberSystem) {
        test_natural_integer_system<uint64, int64>{};
        test_natural_integer_system<N, Z>{};
        test_natural_integer_system<Nl, Zl1>{};
        test_natural_integer_system<Nb, Zb1>{};
        test_integer_system<Zl2>{};
        test_integer_system<Zb2>{};
        // TODO make these work. 
        
        test_natural_integer_system<ub09, sb09>{};
        test_natural_integer_system<ul09, sl09>{};
        test_natural_integer_system<ub10, sb10>{};
        test_natural_integer_system<ul10, sl10>{};
        test_natural_integer_system<ub11, sb11>{};
        test_natural_integer_system<ul11, sl11>{};
        test_natural_integer_system<ub20, sb20>{};
        test_natural_integer_system<ul20, sl20>{};
        
    }
    
    TEST(NumbersTest, TestStringNumbers) {
        test_positive_number<dec_uint>{};/*
        test_positive_number<base58_uint>{};
        test_positive_number<hex_uint>{};
        test_negative_number<hex_int_ones>{};
        test_negative_number<hex_int_twos>{};*/
    }
    
    // some number systems have a positive and negative zero. 
    template <typename Z> struct test_positive_and_negative_zeros {
        test_positive_and_negative_zeros(const Z &positive_zero, const Z &negative_zero) {
            EXPECT_EQ(zero<Z>{}(), positive_zero);
            EXPECT_EQ(zero<Z>{}(), negative_zero);
            EXPECT_FALSE(number::is_negative_zero(positive_zero));
            EXPECT_TRUE(number::is_negative_zero(negative_zero));
            EXPECT_TRUE(number::is_positive_zero(positive_zero));
            EXPECT_FALSE(number::is_positive_zero(negative_zero));
        }
    };
    
    TEST(NumbersTest, TestNumbersPositiveAndNegativeZero) {
        test_positive_and_negative_zeros<Zb2>(Zb2::zero(0), Zb2::zero(1, true));
        test_positive_and_negative_zeros<Zl2>(Zl2::zero(0), Zl2::zero(1, true));
    }
    
    template <typename N> 
    requires requires(const N &n) {
        {number::is_minimal(n)} -> std::same_as<bool>;
        {number::trim(n)} -> std::same_as<N>;
        {number::minimal_size(n)} -> std::same_as<size_t>;
    } && requires(const N &n, size_t x) {
        {number::extend(n, x)} -> std::same_as<N>;
    } struct test_minimal_and_trim {
        test_minimal_and_trim(const N &minimal, const N &non_minimal) {
            EXPECT_EQ(minimal, non_minimal);
            EXPECT_TRUE(number::is_minimal(minimal));
            EXPECT_FALSE(number::is_minimal(non_minimal));
            auto trimmed = number::trim(non_minimal);
            EXPECT_EQ(minimal.size(), trimmed.size());
            EXPECT_EQ(minimal, trimmed);
            EXPECT_TRUE(number::is_minimal(trimmed));
            EXPECT_EQ(number::minimal_size(non_minimal), minimal.size());
            auto extended = number::extend(minimal, non_minimal.size());
            EXPECT_EQ(extended, trimmed);
            EXPECT_EQ(extended.size(), non_minimal.size());
        }
    };
    
    TEST(NumbersTest, TestNumbersMinimalAndTrim) {
        test_minimal_and_trim<Nb>(Nb::zero(0), Nb::zero(1));
        test_minimal_and_trim<Nl>(Nl::zero(0), Nl::zero(1));
        test_minimal_and_trim<Zb1>(Zb1::zero(0), Zb1::zero(1));
        test_minimal_and_trim<Zl1>(Zl1::zero(0), Zl1::zero(1));
        test_minimal_and_trim<Zb2>(Zb2::zero(0), Zb2::zero(1));
        test_minimal_and_trim<Zl2>(Zl2::zero(0), Zl2::zero(1));
        test_minimal_and_trim<hex_uint>(hex_uint{"0x"}, hex_uint{"0x0000"});
        test_minimal_and_trim<hex_int_ones>(hex_int_ones{"0x"}, hex_int_ones{"0x0000"});
        test_minimal_and_trim<hex_int_twos>(hex_int_twos{"0x"}, hex_int_twos{"0x0000"});
    }
    
    template <typename N> struct test_zero_size {
        test_zero_size(){
            EXPECT_EQ(N::read("0x").size(), 0);
            EXPECT_EQ(N::read("0x00").size(), 1);
            EXPECT_EQ(N::read("0x0000").size(), 2);
            EXPECT_EQ(N::zero().size(), 0);
            EXPECT_EQ(N::zero(1).size(), 1);
            EXPECT_EQ(N::zero(2).size(), 2);
        }
    };
    
    TEST(NumbersTest, TestZeroSize) {
        test_zero_size<Nb>{};
        test_zero_size<Nl>{};
        test_zero_size<Zb1>{};
        test_zero_size<Zl1>{};
        test_zero_size<Zb2>{};
        test_zero_size<Zl2>{};
    }
    
    template <typename N, bool> struct test_first_and_next {
        test_first_and_next() {
            EXPECT_EQ(first<N>{}(), zero<N>{}());
            EXPECT_EQ(next(first<N>{}()), increment(zero<N>{}()));
            EXPECT_EQ(next(next(first<N>{}())), increment(increment(zero<N>{}())));
        }
    };
    
    template <typename N> struct test_first_and_next<N, true> {
        test_first_and_next() {
            EXPECT_EQ(first<N>{}(), zero<N>{}());
            EXPECT_EQ(next(first<N>{}()), decrement(zero<N>{}()));
            EXPECT_EQ(next(next(first<N>{}())), increment(zero<N>{}()));
        }
    };
    
    TEST(NumbersTest, TestNumbersFirstAndNext) {
        test_first_and_next<N, false>{};
        test_first_and_next<Nb, false>{};
        test_first_and_next<Nl, false>{};
        test_first_and_next<Zb2, true>{};
        test_first_and_next<Zl2, true>{};
        test_first_and_next<ub09, false>{};
        test_first_and_next<ul09, false>{};
        test_first_and_next<ub10, false>{};
        test_first_and_next<ul10, false>{};
        test_first_and_next<ub11, false>{};
        test_first_and_next<ul11, false>{};
        test_first_and_next<ub20, false>{};
        test_first_and_next<ul20, false>{};
    }
    
    template <natural N>  
    struct test_natural {
        test_natural() {
            EXPECT_EQ(lowest<N>{}(), zero<N>{}());
            EXPECT_EQ(next(next(lowest<N>{}())), N{2});
        }
    };
    
    TEST(NumbersTest, TestNumbersLowest) {
        test_natural<N>{};
        test_natural<Nb>{};
        test_natural<Nl>{};
        test_natural<ub09>{};
        test_natural<ul09>{};
        test_natural<ub10>{};
        test_natural<ul10>{};
        test_natural<ub11>{};
        test_natural<ul11>{};
        test_natural<ub20>{};
        test_natural<ul20>{};
    }
    
    template <typename N, typename Z> 
    requires requires(const N &a, const N &b) {
        {a & b} -> std::same_as<N>;
        {a | b} -> std::same_as<N>;
    } && requires(const Z &a, const Z &b) {
        {a & b} -> std::same_as<Z>;
        {a | b} -> std::same_as<Z>;
    } struct test_bit_operations_basic {
        test_bit_operations_basic(){
            
            // TODO
        }
    };
    
    template <typename N, typename Z> 
    requires requires(const N &a, const Z &b) {
        {a & b} -> std::same_as<Z>;
        {a | b} -> std::same_as<Z>;
        {b & a} -> std::same_as<Z>;
        {b | a} -> std::same_as<Z>;
    } struct test_bit_operations: test_bit_operations_basic<N, Z> {
        test_bit_operations(){
            
            // TODO
        }
    };
    
    template <typename N, typename Z> 
    requires requires(const N &a) {
        {~a} -> std::same_as<Z>;
    } && requires(const Z &a) {
        {~a} -> std::same_as<Z>;
    } && requires(const N &a, const N &b) {
        {a ^ b} -> std::same_as<Z>;
    } && requires(const Z &a, const Z &b) {
        {a ^ b} -> std::same_as<Z>;
    } && requires(const N &a, const Z &b) {
        {a ^ b} -> std::same_as<Z>;
        {b ^ a} -> std::same_as<Z>;
    } struct test_bit_operations_with_negate_1 : test_bit_operations<N, Z> {
        test_bit_operations_with_negate_1(){
            
            EXPECT_EQ(~N{0}, Z{-1});
            EXPECT_EQ(~Z{0}, Z{-1});
            EXPECT_EQ(~Z{-1}, Z{-0});
        }
    };
    
    template <typename N, typename Z> 
    requires requires(const N &a) {
        {~a} -> std::same_as<N>;
    } && requires(const Z &a) {
        {~a} -> std::same_as<Z>;
    } && requires(const N &a, const N &b) {
        {a ^ b} -> std::same_as<N>;
    } && requires(const Z &a, const Z &b) {
        {a ^ b} -> std::same_as<Z>;
    } struct test_bit_operations_with_negate_2 : test_bit_operations_basic<N, Z> {
        test_bit_operations_with_negate_2(){
            
            
        }
    };
    
    TEST(NumbersTest, TestNumbersBitOperations) {
        test_bit_operations_with_negate_1<Nb, Zb1>{};
        test_bit_operations_with_negate_1<Nl, Zl1>{};
        test_bit_operations<Nb, Zb2>{};
        test_bit_operations<Nl, Zl2>{};
        test_bit_operations_with_negate_2<ub09, sb09>{};
        test_bit_operations_with_negate_2<ul09, sl09>{};
        test_bit_operations_with_negate_2<ub10, sb10>{};
        test_bit_operations_with_negate_2<ul10, sl10>{};
        test_bit_operations_with_negate_2<ub11, sb11>{};
        test_bit_operations_with_negate_2<ul11, sl11>{};
        test_bit_operations_with_negate_2<ub20, sb20>{};
        test_bit_operations_with_negate_2<ul20, sl20>{};
    }
    
    template <typename N, typename Z> 
    requires requires(const N &a, const N &b) {
        {a ^ b} -> std::same_as<N>;
        {a | b} -> std::same_as<bool>;
    } && requires(const Z &a, const N &b) {
        {a ^ b} -> std::same_as<Z>;
        {a | b} -> std::same_as<bool>;
    } && requires(const N &a, uint64 b) {
        {a ^ b} -> std::same_as<N>;
        {a | b} -> std::same_as<bool>;
    } && requires(const Z &a, uint64 b) {
        {a ^ b} -> std::same_as<Z>;
        {a | b} -> std::same_as<bool>;
    } struct test_number_theory_operations {
        void operator()(){
            EXPECT_EQ(N{0} ^ N{0}, N{1});
            EXPECT_EQ(N{1} ^ N{0}, N{1});
            EXPECT_EQ(N{2} ^ N{0}, N{1});
            EXPECT_EQ(N{0} ^ N{1}, N{0});
            EXPECT_EQ(N{1} ^ N{1}, N{1});
            EXPECT_EQ(N{2} ^ N{1}, N{2});
            
            EXPECT_EQ(Z{0} ^ N{0}, Z{1});
            EXPECT_EQ(Z{1} ^ N{0}, Z{1});
            EXPECT_EQ(Z{2} ^ N{0}, Z{1});
            EXPECT_EQ(Z{0} ^ N{1}, Z{0});
            EXPECT_EQ(Z{1} ^ N{1}, Z{1});
            EXPECT_EQ(Z{2} ^ N{1}, Z{2});
        }
    };
    
    TEST(NumbersTest, TestNumbersDividesAndPower) {
        test_number_theory_operations<N, Z>{};
    }
    
    template <typename N> struct test_division {
        test_division() {
            division<N> div = math::divide(N::read("0x805AA786A57B3BFC0DFDF2EC86760339F018114A7E30C2D2701CF294DC60829D9B011CD8E391"), N{58});
            
            EXPECT_EQ(div.Quotient, N::read("281747799128083566676930618873129531941258515171631968206030619132595956076515625422029804"));
            EXPECT_EQ(div.Remainder, N{25});
        };
    };
    
    TEST(NumbersTest, TestDivision) {
        
        test_division<N>{};
        test_division<N_bytes_big>{};
        test_division<N_bytes_little>{};
        
    }
    
}
