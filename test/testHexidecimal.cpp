// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include <data/io/wait_for_enter.hpp>
#include "gtest/gtest.h"

namespace data::math::number {
    
    template <complement c> using hex = encoding::hexidecimal::integer<c, encoding::hex::lower>;
    
    template <complement c> struct hex_test_case {
        hex_test_case(string test_x, string test_trimmed, sign expected_sign) {
            {
                auto x = hex<c>{test_x}; 
                auto trimmed = hex<c>{test_trimmed};
                
                EXPECT_EQ(is_zero(x), expected_sign == zero);
                EXPECT_EQ(is_positive(x), expected_sign == positive);
                EXPECT_EQ(is_negative(x), expected_sign == negative);
                EXPECT_EQ(data::sign(x), expected_sign);
                EXPECT_EQ(is_minimal(x), static_cast<string>(test_x) == static_cast<string>(test_trimmed));
                EXPECT_EQ(static_cast<string>(trim(x)), static_cast<string>(trimmed));
                EXPECT_EQ(minimal_size(x), trimmed.size());
                auto extended = extend(trimmed, x.size());
                EXPECT_EQ(extended, x);
            }
        }
    };
    
    TEST(HexidecimalTest, TestHexidecimalSignMinTrim) {
        
        hex_test_case<nones>("0x", "0x", zero);
        hex_test_case<ones>("0x", "0x", zero);
        hex_test_case<twos>("0x", "0x", zero);
        
        hex_test_case<nones>("0x00", "0x", zero);
        hex_test_case<ones>("0x00", "0x", zero);
        hex_test_case<twos>("0x00", "0x", zero);
        
        hex_test_case<nones>("0x0000", "0x", zero);
        hex_test_case<ones>("0x0000", "0x", zero);
        hex_test_case<twos>("0x0000", "0x", zero);
        
        hex_test_case<nones>("0x80", "0x80", positive);
        hex_test_case<ones>("0x80", "0x80", negative);
        hex_test_case<twos>("0x80", "0x", zero);
        
        hex_test_case<nones>("0x8000", "0x8000", positive);
        hex_test_case<ones>("0x8000", "0x8000", negative);
        hex_test_case<twos>("0x8000", "0x", zero);
        
        hex_test_case<nones>("0x01", "0x01", positive);
        hex_test_case<ones>("0x01", "0x01", positive);
        hex_test_case<twos>("0x01", "0x01", positive);
        
        hex_test_case<nones>("0x0001", "0x01", positive);
        hex_test_case<ones>("0x0001", "0x01", positive);
        hex_test_case<twos>("0x0001", "0x01", positive);
        
        hex_test_case<nones>("0xff", "0xff", positive);
        hex_test_case<ones>("0xff", "0xff", negative);
        hex_test_case<twos>("0xff", "0xff", negative);
        
        hex_test_case<nones>("0xffff", "0xffff", positive);
        hex_test_case<ones>("0xffff", "0xff", negative);
        hex_test_case<twos>("0xffff", "0xffff", negative);
        
        hex_test_case<nones>("0x0080", "0x80", positive);
        hex_test_case<ones>("0x0080", "0x0080", positive);
        hex_test_case<twos>("0x0080", "0x0080", positive);
        
        hex_test_case<nones>("0x81", "0x81", positive);
        hex_test_case<ones>("0x81", "0x81", negative);
        hex_test_case<twos>("0x81", "0x81", negative);
        
        hex_test_case<nones>("0x8001", "0x8001", positive);
        hex_test_case<ones>("0x8001", "0x8001", negative);
        hex_test_case<twos>("0x8001", "0x81", negative);
        
        hex_test_case<nones>("0x00ff", "0xff", positive);
        hex_test_case<ones>("0x00ff", "0x00ff", positive);
        hex_test_case<twos>("0x00ff", "0x00ff", positive);
        
        hex_test_case<nones>("0x0200", "0x0200", positive);
        hex_test_case<ones>("0x0200", "0x0200", positive);
        hex_test_case<twos>("0x0200", "0x0200", positive);
        
        hex_test_case<nones>("0x2000", "0x2000", positive);
        hex_test_case<ones>("0x2000", "0x2000", positive);
        hex_test_case<twos>("0x2000", "0x2000", positive);
    }
    
    template <complement... > struct test_bit_and;
    template <complement... > struct test_bit_or;
    
    template <complement n, complement ... p> struct test_bit_and<n, p...> : test_bit_and<n>, test_bit_and<p...> {
        test_bit_and(string left, string right, string expected) : 
        test_bit_and<n>(left, right, expected), test_bit_and<p...>(left, right, expected) {}
    };
    
    template <complement n, complement ... p> struct test_bit_or<n, p...> : test_bit_or<n>, test_bit_or<p...> {
        test_bit_or(string left, string right, string expected) : 
        test_bit_or<n>(left, right, expected), test_bit_or<p...>(left, right, expected) {}
    };
    
    template <complement n, size_t size> struct test_bit_and_fixed;
    template <complement n, size_t size> struct test_bit_or_fixed;
    
    template <complement n> struct test_bit_and<n> {
        test_bit_and(string left, string right, string expected) {
            
            auto l = hex<n>{left};
            auto r = hex<n>{right};
            auto e = hex<n>{expected};
            EXPECT_EQ(l & r, e);
        }
    };
    
    template <complement n> struct test_bit_or<n> {
        test_bit_or(string left, string right, string expected) {
            
            auto l = hex<n>{left};
            auto r = hex<n>{right};
            auto e = hex<n>{expected};
            EXPECT_EQ(l | r, e);
            
        }
    };
    
    TEST(HexidecimalTest, TestHexBitAnd) {
        test_bit_and<nones, ones, twos>{"0x", "0x", "0x"};
        test_bit_and<nones, ones, twos>{"0x01", "0x01", "0x01"};
        test_bit_and<nones, ones, twos>{"0x", "0x01", "0x"};
        test_bit_and<nones, ones, twos>{"0xff", "0xff", "0xff"};
        test_bit_and<nones, ones, twos>{"0x", "0xff", "0x"};
    }
    
    TEST(HexidecimalTest, TestHexBitOr) {
        test_bit_or<nones, ones, twos>{"0x", "0x", "0x"};
        test_bit_or<nones, ones, twos>{"0x01", "0x01", "0x01"};
        test_bit_or<nones, ones, twos>{"0x01", "0x", "0x01"};
        test_bit_or<nones, ones, twos>{"0xff", "0xff", "0xff"};
        test_bit_or<nones, ones, twos>{"0x", "0xff", "0xff"};
    }
    
    template <complement... n> struct test_bit_negate;
    
    template <complement n, complement ... p> struct test_bit_negate<n, p...> : test_bit_negate<n>, test_bit_negate<p...> {
        test_bit_negate(string given, string expected) : 
        test_bit_negate<n>(given, expected), test_bit_negate<p...>(given, expected) {}
    };
    
    template <> struct test_bit_negate<ones> {
        test_bit_negate(string given, string expected) {
            
            auto g = hex<ones>{given};
            auto e = hex<ones>{expected};
            EXPECT_EQ(~g, e);
        }
    };
    
    template <> struct test_bit_negate<nones> {
        test_bit_negate(string given, string expected) {}
    };
    
    template <complement... n> struct test_bit_xor;
    
    template <complement n, complement ... p> struct test_bit_xor<n, p...> : test_bit_xor<n>, test_bit_xor<p...> {
        test_bit_xor(string left, string right, string expected) : 
        test_bit_xor<n>(left, right, expected), test_bit_xor<p...>(left, right, expected) {}
    };
    
    template <> struct test_bit_xor<ones> {
        test_bit_xor(string left, string right, string expected) {
            auto l = hex<ones>{left};
            auto r = hex<ones>{right};
            auto e = hex<ones>{expected};
            EXPECT_EQ(l ^ r, e);
        }
    };
    
    template <> struct test_bit_xor<nones> {
        test_bit_xor(string left, string right, string expected) {
            auto l = hex<nones>{left};
            auto r = hex<nones>{right};
            auto e = hex<nones>{expected};
        }
    };
    
    TEST(HexidecimalTest, TestHexBitXor) {
        test_bit_negate<nones, ones>{"0x", "0xff"};
        test_bit_xor<nones, ones>{"0x", "0x", "0x"};
        test_bit_xor<nones, ones>{"0xff", "0xff", "0x"};
        test_bit_xor<nones, ones>{"0x", "0xff", "0xff"};
    }
    
    template <complement... > struct test_increment;
    
    template <complement n> struct test_increment<n> {
        test_increment(string given, string expected) {
            
            auto gh = hex<n>{given};
            auto ih = increment(gh);
            auto dh = decrement(ih);
            
            EXPECT_EQ(static_cast<string>(ih), expected) << "expected " << given << " to increment to " << expected ;
            EXPECT_EQ(dh, gh);
            EXPECT_TRUE(is_minimal(dh));
            
        }
    };
    
    template <complement n, complement ... p> struct test_increment<n, p...> : test_increment<n>, test_increment<p...> {
        test_increment(string given, string expected) : test_increment<n>(given, expected), test_increment<p...>(given, expected) {}
    };
    
    TEST(HexidecimalTest, TestHexidecimalIncrement) {
        test_increment<nones, ones, twos>{"0x", "0x01"};
        test_increment<nones, ones, twos>{"0x00", "0x01"};
        test_increment<nones, ones, twos>{"0x0000", "0x01"};
        test_increment<nones, ones, twos>{"0x01", "0x02"};
        test_increment<nones>{"0xff", "0x0100"};
        test_increment<nones>{"0x7f", "0x80"};
        test_increment<nones>{"0xffff", "0x010000"};
        test_increment<nones, ones, twos>{"0x00ff", "0x0100"};
        test_increment<ones>{"0xff", "0x"};
        test_increment<ones>{"0xffff", "0x"};
        test_increment<ones>{"0x7f", "0x0080"};
        test_increment<ones>{"0x80", "0x81"};
        test_increment<twos>{"0x80", "0x01"};
        test_increment<twos>{"0x81", "0x"};
        test_increment<twos>{"0x82", "0x81"};
    }
    
    template <complement n> struct test_negation {
        test_negation(string initial, string expected) {
            
            auto hi = hex<n>{initial};
            auto he = hex<n>{expected};
            auto hn = -hi;
            auto ho = -hn;
            EXPECT_EQ(string(hn), string(he));
            EXPECT_EQ(ho, hi);
            
        }
    };
    
    template <> struct test_negation<nones> {
        test_negation(string initial, string expected) {}
    };
    
    TEST(HexidecimalTest, TestNegation) {
        
        test_negation<twos>{"0x", "0x"};
        test_negation<twos>{"0x00", "0x"};
        test_negation<twos>{"0x80", "0x"};
        test_negation<twos>{"0x81", "0x01"};
        test_negation<twos>{"0x01", "0x81"};
        test_negation<twos>{"0x8001", "0x01"};
        test_negation<twos>{"0x0001", "0x81"};
        
        test_negation<ones>{"0x", "0x"};
        test_negation<ones>{"0x00", "0x"};
        test_negation<ones>{"0x80", "0x0080"};
        test_negation<ones>{"0xff", "0x01"};
        test_negation<ones>{"0xffff", "0x01"};
        test_negation<ones>{"0x01", "0xff"};
        test_negation<ones>{"0x02", "0xfe"};
        test_negation<ones>{"0xfe", "0x02"};
        test_negation<ones>{"0x03ff", "0xfc01"};
    }
    
    template <complement c> void test_comparison(const std::vector<string> &values, const std::vector<std::vector<int>> &ordering) {
        for (const string &v : values) {
            EXPECT_THROW(hex<c>{""} <=> hex<c>{v}, exception);
        }
        
        for (auto eqci = ordering.begin(); eqci != ordering.end(); eqci++) {
            for (auto vi = eqci->begin(); vi != eqci->end(); vi++) {
                for (auto vj = vi + 1; vj != eqci->end(); vj++) {
                    string i = values[*vi];
                    string j = values[*vj];
                    
                    EXPECT_EQ(hex<c>(i) <=> hex<c>(j), std::weak_ordering::equivalent);
                }
                
                for (auto eqcj = eqci + 1; eqcj != ordering.end(); eqcj++) {
                    for (auto vj = eqcj->begin(); vj != eqcj->end(); vj++) {
                        string i = values[*vi];
                        string j = values[*vj];
                    
                        EXPECT_EQ(hex<c>(i) <=> hex<c>(j), std::weak_ordering::less) << "expected " << i << " < " << j;
                        EXPECT_EQ(hex<c>(j) <=> hex<c>(i), std::weak_ordering::greater) << "expected " << i << " > " << j;
                    }
                }
            }
        }
    }
    
    TEST(HexidecimalTest, TestHexidecimalComparisons) {
        std::vector<string> values{
            "0x", "0x00", "0x0000", "0x80", "0x8000", "0x01", "0x0001", 
            "0xff", "0xffff", "0x0080", "0x81", "0x8001", "0x00ff", 
            "0x0200", "0x2000"};
        
        std::vector<std::vector<int>> o0{{0, 1, 2}, {5, 6}, {3, 9}, {10}, {7, 12}, {13}, {14}, {4}, {11}, {8}};
        std::vector<std::vector<int>> o1{{4}, {11}, {3}, {10}, {7, 8}, {0, 1, 2}, {5, 6}, {9}, {12}, {13}, {14}};
        std::vector<std::vector<int>> o2{{8}, {7}, {10, 11}, {0, 1, 2, 3, 4}, {5, 6}, {9}, {12}, {13}, {14}};
        
        test_comparison<nones>(values, o0);
        test_comparison<ones>(values, o1);
        test_comparison<twos>(values, o2);
        
    }
    
    template <complement... > struct test_bit_shift;
    
    template <complement n, complement ... p> struct test_bit_shift<n, p...> : test_bit_shift<n>, test_bit_shift<p...> {
        test_bit_shift(
            string given, int bits, 
            string expected_left, 
            string expected_right) : 
        test_bit_shift<n>(given, bits, expected_left, expected_right), 
        test_bit_shift<p...>(given, bits, expected_left, expected_right) {}
    };
    
    template <complement n> struct test_bit_shift<n> {
        test_bit_shift(string given, byte bits, string expected_left, string expected_right) {
            auto l = hex<n>{expected_left};
            auto r = hex<n>{expected_right};
            auto g = hex<n>{given};
            auto x = g << bits;
            EXPECT_EQ(x, l);
            EXPECT_EQ(x >> bits, g);
            EXPECT_EQ(g >> bits, r);
            
        }
    };
    
    TEST(HexidecimalTest, TestHexidecimalBitShift) {
        test_bit_shift<nones, ones, twos>("0x", 0, "0x", "0x");
        test_bit_shift<nones, ones, twos>("0x", 1, "0x", "0x");
        test_bit_shift<nones, ones, twos>("0x", 21, "0x", "0x");
        test_bit_shift<nones, ones, twos>("0x01", 0, "0x01", "0x01");
        test_bit_shift<nones, ones, twos>("0x01", 1, "0x02", "0x");
        test_bit_shift<nones, ones, twos>("0x01", 2, "0x04", "0x");
        test_bit_shift<nones, ones, twos>("0x01", 5, "0x0020", "0x");
        test_bit_shift<nones, ones, twos>("0x01", 17, "0x020000", "0x00");
        test_bit_shift<nones, ones, twos>("0x0300", 1, "0x0600", "0x0180");
        test_bit_shift<nones, ones, twos>("0x0300", 2, "0x0c00", "0x00c0");
        test_bit_shift<nones, ones, twos>("0x0300", 8, "0x030000", "0x03");
        test_bit_shift<nones, ones, twos>("0x0300", 9, "0x060000", "0x01");
        test_bit_shift<nones, ones, twos>("0x0300", 16, "0x03000000", "0x");
        test_bit_shift<nones, ones, twos>("0x0300", 17, "0x06000000", "0x");
        test_bit_shift<nones, ones, twos>("0x0300", 18, "0x0c000000", "0x");
        test_bit_shift<nones>("0xff", 1, "0x01fe", "0x7f");
        test_bit_shift<ones>("0xff", 1, "0xfe", "0xff");
        test_bit_shift<twos>("0xff", 1, "0x80fe", "0xbf");
    }
    
    template <complement... > struct test_plus;
    
    template <complement n, complement ... p> struct test_plus<n, p...> : test_plus<n>, test_plus<p...> {
        test_plus(string left, string right, string expected) : 
        test_plus<n>(left, right, expected), test_plus<p...>(left, right, expected) {}
    };
    
    template <complement n> struct test_plus<n> {
        test_plus(string left, string right, string expected) {
            auto l = hex<n>::read(left);
            auto r = hex<n>::read(right);
            auto e = hex<n>::read(expected);
            auto z = l + r;
            EXPECT_EQ(z, e) << "expected " << n << " " << std::hex << left << " + " << right << " = " << expected 
                << " but got " << z << std::endl;
        }
    };
    
    TEST(HexidecimalTest, TestHexidecimalPlus) {
        test_plus<nones, ones, twos>("0x", "0x", "0x");
        
        test_plus<nones, ones, twos>("0", "0", "0");
        test_plus<nones, ones, twos>("0", "1", "1");
        test_plus<nones, ones, twos>("1", "1", "2");
        test_plus<nones, ones, twos>("231938875480", "397027301409876", "397259240285356");
        test_plus<nones, ones, twos>("23173210900987658780938875480", "39702733535456767789001409876", "62875944436444426569940285356");
        
        test_plus<ones, twos>("-1", "0", "-1");
        test_plus<ones, twos>("-1", "-1", "-2");
        test_plus<ones, twos>("-1", "1", "0");
        test_plus<ones, twos>("1", "-1", "0");
        test_plus<ones, twos>("-2", "1", "-1");
        test_plus<ones, twos>("2", "-1", "1");
        test_plus<ones, twos>("-23173210900987658780938875480", "23173210900987658780938875480", "0");
        test_plus<ones, twos>("23173210900987658780938875480", "-23173210900987658780938875480", "0");
        
        test_plus<twos>{"0x", "0x01", "0x01"};
        test_plus<twos>{"0x8000", "0x01", "0x01"};
        test_plus<twos>{"0x", "0x81", "0x81"};
    }
    
    template <complement... > struct test_minus;
    
    template <complement n, complement ... p> struct test_minus<n, p...> : test_minus<n>, test_minus<p...> {
        test_minus(string left, string right, string expected) : 
        test_minus<n>(left, right, expected), test_minus<p...>(left, right, expected) {}
    };
    
    template <complement n> struct test_minus<n> {
        test_minus(string left, string right, string expected) {
            auto l = hex<n>::read(left);
            auto r = hex<n>::read(right);
            auto e = hex<n>::read(expected);
            auto x = l - r;
            EXPECT_EQ(x, e) << "expected " << n << " " << left << " - " << right << " -> " << expected << " but got " << x << std::endl;
        }
    };
    
    TEST(HexidecimalTest, TestHexidecimalMinus) {
        test_minus<nones, ones, twos>("0x", "0x", "0x");
        
        test_minus<nones, ones, twos>("0", "0", "0");
        test_minus<nones, ones, twos>("1", "0", "1");
        test_minus<nones, ones, twos>("1", "1", "0");
        
        test_minus<nones, ones, twos>("37", "12", "25");
        test_minus<nones, ones, twos>("4502938948920982780930898389", "4502938948920982780930898389", "0");
        
        test_minus<nones>("4502938948920982780930898389", "4502938948920982780930898390", "0");
        test_minus<ones, twos>("4502938948920982780930898389", "4502938948920982780930898390", "-1");
        test_minus<ones, twos>("1", "-1", "2");
        
        test_minus<twos>{"0x", "0x01", "0x81"};
        test_minus<twos>{"0x8000", "0x01", "0x81"};
    }
    
    template <complement... > struct test_times;
    
    template <complement n, complement ... p> struct test_times<n, p...> : test_times<n>, test_times<p...> {
        test_times(string left, string right, string expected) : 
        test_times<n>(left, right, expected), test_times<p...>(left, right, expected) {}
    };
    
    template <complement n> struct test_times<n> {
        test_times(string left, string right, string expected) {
            auto l = hex<n>::read(left);
            auto r = hex<n>::read(right);
            auto e = hex<n>::read(expected);
            auto z = l * r;
            EXPECT_EQ(z, e) << "expected " << std::hex << l << " * " << r << " = " << e 
                << " but got " << z << std::endl;
        }
    };
    
    TEST(HexidecimalTest, TestHexidecimalTimes) {
        
        test_times<nones, ones, twos>("0", "0", "0");
        test_times<nones, ones, twos>("0", "1", "0");
        test_times<nones, ones, twos>("1", "1", "1");
        test_times<nones, ones, twos>("1", "2", "2");
        test_times<nones, ones, twos>("2", "1", "2");
        test_times<nones, ones, twos>("2", "2", "4");
        test_times<nones, ones, twos>("2", "5", "10");
        test_times<nones, ones, twos>("5", "7", "35");
        test_times<nones, ones, twos>("17", "19", "323");
        test_times<nones, ones, twos>("23173210900987658780938875480", "39702733535456767789001409876",
            "920039817562855061210426612476533348173557348698006240480");
        
        test_times<ones, twos>("-1", "0", "0");
        test_times<ones, twos>("0", "-1", "0");
        test_times<ones, twos>("-1", "1", "-1");
        test_times<ones, twos>("1", "-1", "-1");
    }
    
    template <complement... > struct test_divide;
    
    template <complement n, complement ... p> struct test_divide<n, p...> : test_divide<n>, test_divide<p...> {
        test_divide(string numerator, string denominator, string quotient, string remainder) : 
        test_divide<n>(numerator, denominator, quotient, remainder), 
        test_divide<p...>(numerator, denominator, quotient, remainder) {}
    };
    
    template <complement n> struct test_divide<n> {
        test_divide(string numerator, string denominator, string quotient, string remainder) {
            auto num = hex<n>::read(numerator);
            auto den = hex<n>::read(denominator);
            auto qu = hex<n>::read(quotient);
            auto re = hex<n>::read(remainder);
            auto x = num.divide(den);
            EXPECT_EQ(x.Quotient, qu);
            EXPECT_EQ(x.Remainder, re);
        }
    };
    
    TEST(HexidecimalTest, TestHexidecimalDivide) {
        test_divide<nones, ones, twos>("0", "1", "0", "0");
        test_divide<nones, ones, twos>("1", "1", "1", "0");
    }
    
    TEST(HexidecimalTest, TestHexidecimalOnes) {
        using hex_uint = data::hex::uint<encoding::hex::lower>;
        using hex_int1 = data::hex::int1<encoding::hex::lower>;
        
        EXPECT_EQ(string(hex_int1{"0x"} + hex_uint{"0x"}), string{"0x"});
        EXPECT_EQ(string(hex_int1{"0x"} * hex_uint{"0x"}), string{"0x"});
    }
    
} 
