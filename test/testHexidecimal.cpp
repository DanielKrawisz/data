// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/tools.hpp>
#include <data/numbers.hpp>
#include <data/math/number/bytes.hpp>
#include <data/math/number/gmp/gmp.hpp>
#include <data/math/number/bounded.hpp>
#include <data/encoding/integer.hpp>
#include "gtest/gtest.h"

namespace data::math::number {
    
    template <complement c> using hex = encoding::hexidecimal::integer<c, encoding::hex::lower>;
    
    template <endian::order r, complement c> struct get_bytes_type {
        using value = Z_bytes<r, c>;
    };
    
    template <endian::order r> struct get_bytes_type<r, nones> {
        using value = N_bytes<r>;
    };
    
    template <endian::order r, complement c> using bytes_type = get_bytes_type<r, c>::value;
    
    template <endian::order r, complement c, size_t size> struct get_fixed_type;
    
    template <endian::order r, size_t size> struct get_fixed_type<r, nones, size> {
        using value = uint<r, size>;
    };
    
    template <endian::order r, size_t size> struct get_fixed_type<r, ones, size> {
        using value = sint<r, size>;
    };
    
    template <endian::order r, size_t size> struct get_fixed_type<r, twos, size> {
        using value = void;
    };
    
    template <endian::order r, complement c, size_t size> using fixed_type = get_fixed_type<r, c, size>::value;
    
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
            
            {
                auto x = bytes_type<endian::little, c>::read(test_x);
                auto trimmed = bytes_type<endian::little, c>::read(test_trimmed);
                
                EXPECT_EQ(x.size() * 2 + 2, test_x.size());
                EXPECT_EQ(trimmed.size() * 2 + 2, test_trimmed.size());
                
                EXPECT_EQ(test_x, encoding::hexidecimal::write(x));
                EXPECT_EQ(test_trimmed, encoding::hexidecimal::write(trimmed));
                
                EXPECT_EQ(is_zero(x), expected_sign == zero);
                EXPECT_EQ(is_positive(x), expected_sign == positive);
                EXPECT_EQ(is_negative(x), expected_sign == negative);
                EXPECT_EQ(data::sign(x), expected_sign);
                EXPECT_EQ(is_minimal(x), test_x == test_trimmed);
                EXPECT_EQ(static_cast<bytes>(trim(x)), static_cast<bytes>(trimmed));
                EXPECT_EQ(minimal_size(x), trimmed.size());
                auto extended = extend(trimmed, x.size());
                EXPECT_EQ(extended, x);
            }
            
            {
                auto x = bytes_type<endian::big, c>::read(test_x);
                auto trimmed = bytes_type<endian::big, c>::read(test_trimmed);
                
                EXPECT_EQ(x.size() * 2 + 2, test_x.size());
                EXPECT_EQ(trimmed.size() * 2 + 2, test_trimmed.size());
                
                EXPECT_EQ(test_x, encoding::hexidecimal::write(x));
                EXPECT_EQ(test_trimmed, encoding::hexidecimal::write(trimmed));
                
                EXPECT_EQ(is_zero(x), expected_sign == zero);
                EXPECT_EQ(is_positive(x), expected_sign == positive);
                EXPECT_EQ(is_negative(x), expected_sign == negative);
                EXPECT_EQ(data::sign(x), expected_sign);
                EXPECT_EQ(is_minimal(x), test_x == test_trimmed);
                EXPECT_EQ(static_cast<bytes>(trim(x)), static_cast<bytes>(trimmed));
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
    
    template <size_t size> struct test_bit_and_fixed<nones, size> {
        test_bit_and_fixed(hex<nones> left, hex<nones> right, hex<nones> expected) {
            auto ll = uint<endian::little, size>::read(extend(left, size * 2 + 2));
            auto rl = uint<endian::little, size>::read(extend(right, size * 2 + 2));
            auto el = uint<endian::little, size>::read(extend(expected, size * 2 + 2));
            EXPECT_EQ(ll & rl, el);
            
            auto lb = uint<endian::big, size>::read(extend(left, size * 2 + 2));
            auto rb = uint<endian::big, size>::read(extend(right, size * 2 + 2));
            auto eb = uint<endian::big, size>::read(extend(expected, size * 2 + 2));
            EXPECT_EQ(lb & rb, eb);
        }
    };
    
    template <size_t size> struct test_bit_or_fixed<nones, size> {
        test_bit_or_fixed(hex<nones> left, hex<nones> right, hex<nones> expected) {
            auto ll = uint<endian::little, size>::read(extend(left, size * 2 + 2));
            auto rl = uint<endian::little, size>::read(extend(right, size * 2 + 2));
            auto el = uint<endian::little, size>::read(extend(expected, size * 2 + 2));
            EXPECT_EQ(ll | rl, el);
            
            auto lb = uint<endian::big, size>::read(extend(left, size * 2 + 2));
            auto rb = uint<endian::big, size>::read(extend(right, size * 2 + 2));
            auto eb = uint<endian::big, size>::read(extend(expected, size * 2 + 2));
            EXPECT_EQ(lb | rb, eb);
        }
    };
    
    template <size_t size> struct test_bit_and_fixed<ones, size> {
        test_bit_and_fixed(hex<ones> left, hex<ones> right, hex<ones> expected) {
            auto ll = sint<endian::little, size>::read(extend(left, size * 2 + 2));
            auto rl = sint<endian::little, size>::read(extend(right, size * 2 + 2));
            auto el = sint<endian::little, size>::read(extend(expected, size * 2 + 2));
            EXPECT_EQ(ll & rl, el);
            
            auto lb = sint<endian::big, size>::read(extend(left, size * 2 + 2));
            auto rb = sint<endian::big, size>::read(extend(right, size * 2 + 2));
            auto eb = sint<endian::big, size>::read(extend(expected, size * 2 + 2));
            EXPECT_EQ(lb & rb, eb);
        }
    };
    
    template <size_t size> struct test_bit_or_fixed<ones, size> {
        test_bit_or_fixed(hex<ones> left, hex<ones> right, hex<ones> expected) {
            
            auto ll = sint<endian::little, size>::read(extend(left, size * 2 + 2));
            auto rl = sint<endian::little, size>::read(extend(right, size * 2 + 2));
            auto el = sint<endian::little, size>::read(extend(expected, size * 2 + 2));
            EXPECT_EQ(ll | rl, el);
            
            auto lb = sint<endian::big, size>::read(extend(left, size * 2 + 2));
            auto rb = sint<endian::big, size>::read(extend(right, size * 2 + 2));
            auto eb = sint<endian::big, size>::read(extend(expected, size * 2 + 2));
            EXPECT_EQ(lb | rb, eb);
        }
    };
    
    template <size_t size> struct test_bit_and_fixed<twos, size> {
        test_bit_and_fixed(hex<twos> left, hex<twos> right, hex<twos> expected) {}
    };
    
    template <size_t size> struct test_bit_or_fixed<twos, size> {
        test_bit_or_fixed(hex<twos> left, hex<twos> right, hex<twos> expected) {}
    };
    
    template <complement n> struct test_bit_and<n> {
        test_bit_and(string left, string right, string expected) {
            
            auto l = hex<n>{left};
            auto r = hex<n>{right};
            auto e = hex<n>{expected};
            EXPECT_EQ(l & r, e);
            
            auto lbl = bytes_type<endian::little, n>::read(left);
            auto rbl = bytes_type<endian::little, n>::read(right);
            auto ebl = bytes_type<endian::little, n>::read(expected);
            EXPECT_EQ(lbl & rbl, ebl);
            
            auto lbb = bytes_type<endian::big, n>::read(left);
            auto rbb = bytes_type<endian::big, n>::read(right);
            auto ebb = bytes_type<endian::big, n>::read(expected);
            EXPECT_EQ(lbb & rbb, ebb);
            
            test_bit_and_fixed<n, 10>(l, r, e);
            test_bit_and_fixed<n, 11>(l, r, e);
            test_bit_and_fixed<n, 12>(l, r, e);
            test_bit_and_fixed<n, 13>(l, r, e);
        }
    };
    
    template <complement n> struct test_bit_or<n> {
        test_bit_or(string left, string right, string expected) {
            
            auto l = hex<n>{left};
            auto r = hex<n>{right};
            auto e = hex<n>{expected};
            EXPECT_EQ(l | r, e);
            
            auto lbl = bytes_type<endian::little, n>::read(left);
            auto rbl = bytes_type<endian::little, n>::read(right);
            auto ebl = bytes_type<endian::little, n>::read(expected);
            EXPECT_EQ(lbl | rbl, ebl);
            
            auto lbb = bytes_type<endian::big, n>::read(left);
            auto rbb = bytes_type<endian::big, n>::read(right);
            auto ebb = bytes_type<endian::big, n>::read(expected);
            EXPECT_EQ(lbb | rbb, ebb);
            
            test_bit_or_fixed<n, 10>(l, r, e);
            test_bit_or_fixed<n, 11>(l, r, e);
            test_bit_or_fixed<n, 12>(l, r, e);
            test_bit_or_fixed<n, 13>(l, r, e);
            
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
    
    template <complement n, size_t size> struct test_bit_negate_fixed;
    
    template <size_t size> struct test_bit_negate_fixed<ones, size> {
        test_bit_negate_fixed(string given, string expected) {
            auto g = extend(hex<ones>{given}, size * 2 + 2);
            auto e = extend(hex<ones>{expected}, size * 2 + 2);
            
            auto gl = sint<endian::little, size>::read(g);
            auto el = sint<endian::little, size>::read(e);
            EXPECT_EQ(~gl, el);
            
            auto gb = sint<endian::big, size>::read(g);
            auto eb = sint<endian::big, size>::read(e);
            EXPECT_EQ(~gb, eb);
        }
    };
    
    template <size_t size> struct test_bit_negate_fixed<nones, size> {
        test_bit_negate_fixed(string given, string expected) {
            auto g = extend(hex<ones>{given}, size * 2 + 2);
            auto e = extend(hex<ones>{expected}, size * 2 + 2);
            
            auto gl = uint<endian::little, size>::read(g);
            auto el = uint<endian::little, size>::read(e);
            EXPECT_EQ(~gl, el);
            
            auto gb = uint<endian::big, size>::read(g);
            auto eb = uint<endian::big, size>::read(e);
            EXPECT_EQ(~gb, eb);
        }
    };
    
    template <> struct test_bit_negate<ones> {
        test_bit_negate(string given, string expected) {
            
            auto g = hex<ones>{given};
            auto e = hex<ones>{expected};
            EXPECT_EQ(~g, e);
            
            auto gbl = bytes_type<endian::little, ones>::read(given);
            auto ebl = bytes_type<endian::little, ones>::read(expected);
            EXPECT_EQ(~gbl, ebl);
            
            auto gbb = bytes_type<endian::big, ones>::read(given);
            auto ebb = bytes_type<endian::big, ones>::read(expected);
            EXPECT_EQ(~gbb, ebb);
            
            test_bit_negate_fixed<ones, 10>(g, e);
            test_bit_negate_fixed<ones, 11>(g, e);
            test_bit_negate_fixed<ones, 12>(g, e);
            test_bit_negate_fixed<ones, 13>(g, e);
        }
    };
    
    template <> struct test_bit_negate<nones> {
        test_bit_negate(string given, string expected) {
            
            auto g = hex<nones>{given};
            auto e = hex<nones>{expected};
            
            test_bit_negate_fixed<nones, 10>(g, e);
            test_bit_negate_fixed<nones, 11>(g, e);
            test_bit_negate_fixed<nones, 12>(g, e);
            test_bit_negate_fixed<nones, 13>(g, e);
        }
    };
    
    template <complement... n> struct test_bit_xor;
    
    template <complement n, complement ... p> struct test_bit_xor<n, p...> : test_bit_xor<n>, test_bit_xor<p...> {
        test_bit_xor(string left, string right, string expected) : 
        test_bit_xor<n>(left, right, expected), test_bit_xor<p...>(left, right, expected) {}
    };
    
    template <complement n, size_t size> struct test_bit_xor_fixed;
    
    template <size_t size> struct test_bit_xor_fixed<nones, size> {
        test_bit_xor_fixed(string left, string right, string expected) {
            auto l = extend(hex<ones>{left}, size * 2 + 2);
            auto r = extend(hex<ones>{right}, size * 2 + 2);
            auto e = extend(hex<ones>{expected}, size * 2 + 2);
            
            auto ll = uint<endian::little, size>::read(l);
            auto rl = uint<endian::little, size>::read(r);
            auto el = uint<endian::little, size>::read(e);
            
            auto xl = ll ^ rl;
            EXPECT_EQ(xl, el) << "expected " << xl << " to equal " << el;
            
            auto lb = uint<endian::big, size>::read(l);
            auto rb = uint<endian::big, size>::read(r);
            auto eb = uint<endian::big, size>::read(e);
            
            auto xb = lb ^ rb;
            EXPECT_EQ(xb, eb) << "expected " << xb << " to equal " << eb;
        }
    };
    
    template <size_t size> struct test_bit_xor_fixed<ones, size> {
        test_bit_xor_fixed(string left, string right, string expected) {
            auto l = extend(hex<ones>{left}, size * 2 + 2);
            auto r = extend(hex<ones>{right}, size * 2 + 2);
            auto e = extend(hex<ones>{expected}, size * 2 + 2);
            
            auto ll = sint<endian::little, size>::read(l);
            auto rl = sint<endian::little, size>::read(r);
            auto el = sint<endian::little, size>::read(e);
            
            auto xl = ll ^ rl;
            EXPECT_EQ(xl, el) << "expected " << xl << " to equal " << el;
            
            auto lb = sint<endian::big, size>::read(l);
            auto rb = sint<endian::big, size>::read(r);
            auto eb = sint<endian::big, size>::read(e);
            
            auto xb = lb ^ rb;
            EXPECT_EQ(xb, eb) << "expected " << xb << " to equal " << eb;
        }
    };
    
    template <> struct test_bit_xor<ones> {
        test_bit_xor(string left, string right, string expected) {
            auto l = hex<ones>{left};
            auto r = hex<ones>{right};
            auto e = hex<ones>{expected};
            EXPECT_EQ(l ^ r, e);
            
            auto lbl = bytes_type<endian::little, ones>::read(left);
            auto rbl = bytes_type<endian::little, ones>::read(right);
            auto ebl = bytes_type<endian::little, ones>::read(expected);
            EXPECT_EQ(lbl ^ rbl, ebl);
            
            auto lbb = bytes_type<endian::big, ones>::read(left);
            auto rbb = bytes_type<endian::big, ones>::read(right);
            auto ebb = bytes_type<endian::big, ones>::read(expected);
            EXPECT_EQ(lbb ^ rbb, ebb);
            
            test_bit_xor_fixed<ones, 10>(l, r, e);
            test_bit_xor_fixed<ones, 11>(l, r, e);
            test_bit_xor_fixed<ones, 12>(l, r, e);
            test_bit_xor_fixed<ones, 13>(l, r, e);
        }
    };
    
    template <> struct test_bit_xor<nones> {
        test_bit_xor(string left, string right, string expected) {
            auto l = hex<nones>{left};
            auto r = hex<nones>{right};
            auto e = hex<nones>{expected};
            
            test_bit_xor_fixed<nones, 10>(l, r, e);
            test_bit_xor_fixed<nones, 11>(l, r, e);
            test_bit_xor_fixed<nones, 12>(l, r, e);
            test_bit_xor_fixed<nones, 13>(l, r, e);
        }
    };
    
    TEST(HexidecimalTest, TestHexBitXor) {
        test_bit_negate<nones, ones>{"0x", "0xff"};
        test_bit_xor<nones, ones>{"0x", "0x", "0x"};
        test_bit_xor<nones, ones>{"0xff", "0xff", "0x"};
        test_bit_xor<nones, ones>{"0x", "0xff", "0xff"};
    }
    
    template <complement n, size_t size> struct test_increment_fixed;
    
    template <complement... > struct test_increment;
    
    template <complement n> struct test_increment<n> {
        test_increment(string given, string expected) {
            
            auto gh = hex<n>{given};
            auto ih = increment(gh);
            auto dh = decrement(ih);
            
            EXPECT_EQ(static_cast<string>(ih), expected) << "expected " << given << " to increment to " << expected ;
            EXPECT_EQ(dh, gh);
            EXPECT_TRUE(is_minimal(dh));
            
            auto gbl = bytes_type<endian::little, n>::read(given);
            auto ebl = bytes_type<endian::little, n>::read(expected);
            auto ibl = increment(gbl);
            EXPECT_EQ(static_cast<bytes>(ibl), static_cast<bytes>(ebl)) << "expected " << ibl << " === " << ebl;
            
            auto gbb = bytes_type<endian::big, n>::read(given);
            auto ebb = bytes_type<endian::big, n>::read(expected);
            auto ibb = increment(gbb);
            EXPECT_EQ(static_cast<bytes>(ibb), static_cast<bytes>(ebb)) << "expected " << ibb << " === " << ebb;
            
            auto dbl = decrement(ibl);
            EXPECT_EQ(dbl, gbl) << "expected " << dbl << " == " << gbl;
            EXPECT_TRUE(is_minimal(dbl));
            
            auto dbb = decrement(ibb);  
            EXPECT_EQ(dbb, gbb) << "expected " << dbb << " == " << gbb;
            EXPECT_TRUE(is_minimal(dbb));
            
            test_increment_fixed<n, 10>(gh, ih);
            test_increment_fixed<n, 11>(gh, ih);
            test_increment_fixed<n, 12>(gh, ih);
            test_increment_fixed<n, 13>(gh, ih);
        }
    };
    
    template <complement n, complement ... p> struct test_increment<n, p...> : test_increment<n>, test_increment<p...> {
        test_increment(string given, string expected) : test_increment<n>(given, expected), test_increment<p...>(given, expected) {}
    };
    
    template <size_t size> struct test_increment_fixed<nones, size> {
        test_increment_fixed(hex<nones> given, hex<nones> expected) {
            auto gbl = uint<endian::little, size>::read(extend(given, size * 2 + 2));
            auto ebl = uint<endian::little, size>::read(extend(expected, size * 2 + 2));
            
            auto ibl = increment(gbl);
            EXPECT_EQ(ibl, ebl) << "expected " << ibl << " to equal " << ebl << std::endl;
            auto dbl = decrement(ibl);
            EXPECT_EQ(dbl, gbl) << "expected " << dbl << " to equal " << gbl << std::endl;
            
            auto gbb = uint<endian::big, size>::read(extend(given, size * 2 + 2));
            auto ebb = uint<endian::big, size>::read(extend(expected, size * 2 + 2));
            
            auto ibb = increment(gbb);
            EXPECT_EQ(ibb, ebb) << "expected " << ibb << " to equal " << ebb << std::endl;
            auto dbb = decrement(ibb);
            EXPECT_EQ(dbb, gbb) << "expected " << dbb << " to equal " << gbl << std::endl;
        }
    };
    
    template <size_t size> struct test_increment_fixed<ones, size> {
        test_increment_fixed(hex<ones> given, hex<ones> expected) {
            
            auto gbl = sint<endian::little, size>::read(extend(given, size * 2 + 2));
            auto ebl = sint<endian::little, size>::read(extend(expected, size * 2 + 2));
            
            auto ibl = increment(gbl);
            EXPECT_EQ(ibl, ebl) << "expected " << ibl << " to equal " << ebl << std::endl;
            auto dbl = decrement(ibl);
            EXPECT_EQ(dbl, gbl) << "expected " << dbl << " to equal " << gbl << std::endl;
            
            auto gbb = sint<endian::big, size>::read(extend(given, size * 2 + 2));
            auto ebb = sint<endian::big, size>::read(extend(expected, size * 2 + 2));
            
            auto ibb = increment(gbb);
            EXPECT_EQ(ibb, ebb) << "expected " << ibb << " to equal " << ebb << std::endl;
            auto dbb = decrement(ibb);
            EXPECT_EQ(dbb, gbb) << "expected " << dbb << " to equal " << gbl << std::endl;
        }
    };
    
    template <size_t size> struct test_increment_fixed<twos, size> {
        test_increment_fixed(hex<twos> given, hex<twos> expected) {}
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
    
    template <typename N> struct test_negation_number {
        test_negation_number(string initial, string expected) {
            auto hi = N::read(initial);
            auto he = N::read(expected);
            auto hn = -hi;
            auto ho = -hn;
            EXPECT_EQ(hn, he) << std::hex << "expected " << hn << " to equal " << he << std::endl;
            EXPECT_EQ(ho, hi) << std::hex << "expected " << ho << " to equal " << hi << std::endl;
        }
    };
    
    template <> struct test_negation_number<void> {
        test_negation_number(string initial, string expected) {}
    };
    
    template <complement n> struct test_negation {
        test_negation(string initial, string expected) {
            
            auto hi = hex<n>{initial};
            auto he = hex<n>{expected};
            auto hn = -hi;
            auto ho = -hn;
            EXPECT_EQ(string(hn), string(he));
            EXPECT_EQ(ho, hi);
            
            test_negation_number<bytes_type<endian::little, n>>{hi, he};
            test_negation_number<bytes_type<endian::big, n>>{hi, he};
            
            test_negation_number<fixed_type<endian::little, n, 10>>{extend(hi, 22), extend(he, 22)};
            test_negation_number<fixed_type<endian::big, n, 10>>{extend(hi, 22), extend(he, 22)};
            
            test_negation_number<fixed_type<endian::little, n, 11>>{extend(hi, 24), extend(he, 24)};
            test_negation_number<fixed_type<endian::big, n, 11>>{extend(hi, 24), extend(he, 24)};
            
            test_negation_number<fixed_type<endian::little, n, 12>>{extend(hi, 26), extend(he, 26)};
            test_negation_number<fixed_type<endian::big, n, 12>>{extend(hi, 26), extend(he, 26)};
            
            test_negation_number<fixed_type<endian::little, n, 13>>{extend(hi, 28), extend(he, 28)};
            test_negation_number<fixed_type<endian::big, n, 13>>{extend(hi, 28), extend(he, 28)};
            
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
    
    template <complement c, size_t size> struct fixed_test_comparison;
    
    template <size_t size> struct fixed_test_comparison<nones, size> {
        fixed_test_comparison(string i, string j, std::strong_ordering expected) {
            auto cmpb = uint<endian::big, size>::read(extend(hex<nones>(i), size * 2 + 2)) <=> 
                uint<endian::big, size>::read(extend(hex<nones>(j), size * 2 + 2));
            EXPECT_EQ(cmpb, expected);
            auto cmpl = uint<endian::little, size>::read(extend(hex<nones>(i), size * 2 + 2)) <=> 
                uint<endian::little, size>::read(extend(hex<nones>(j), size * 2 + 2));
            EXPECT_EQ(cmpl, expected);
        }
    };
    
    template <size_t size> struct fixed_test_comparison<ones, size> {
        fixed_test_comparison(string i, string j, std::strong_ordering expected) {
            
            string exi = extend(hex<ones>(i), size * 2 + 2);
            string exj = extend(hex<ones>(j), size * 2 + 2);
            
            auto cmpb = sint<endian::big, size>::read(exi) <=> 
                sint<endian::big, size>::read(exj);
            EXPECT_EQ(cmpb, expected);
            auto cmpl = sint<endian::little, size>::read(exi) <=> 
                sint<endian::little, size>::read(exj);
            EXPECT_EQ(cmpl, expected);
        }
    };
    
    template <size_t size> struct fixed_test_comparison<twos, size> {
        fixed_test_comparison(string a, string b, std::strong_ordering expected) {}
    };
    
    template <complement c> void test_comparison(const std::vector<string> &values, const std::vector<std::vector<int>> &ordering) {
        for (const string &v : values) {
            EXPECT_THROW(hex<c>{""} <=> hex<c>{v}, std::invalid_argument);
        }
        
        for (auto eqci = ordering.begin(); eqci != ordering.end(); eqci++) {
            for (auto vi = eqci->begin(); vi != eqci->end(); vi++) {
                for (auto vj = vi + 1; vj != eqci->end(); vj++) {
                    string i = values[*vi];
                    string j = values[*vj];
                    
                    EXPECT_EQ(hex<c>(i) <=> hex<c>(j), std::weak_ordering::equivalent);
                    
                    auto cmpb = bytes_type<endian::big, c>::read(i) <=> bytes_type<endian::big, c>::read(j);
                    EXPECT_EQ(cmpb, std::weak_ordering::equivalent);
                    auto cmpl = bytes_type<endian::little, c>::read(i) <=> bytes_type<endian::little, c>::read(j);
                    EXPECT_EQ(cmpl, std::weak_ordering::equivalent);
                    
                    fixed_test_comparison<c, 10>{i, j, std::strong_ordering::equivalent};
                    fixed_test_comparison<c, 11>{i, j, std::strong_ordering::equivalent};
                    fixed_test_comparison<c, 12>{i, j, std::strong_ordering::equivalent};
                    fixed_test_comparison<c, 13>{i, j, std::strong_ordering::equivalent};
                }
                
                for (auto eqcj = eqci + 1; eqcj != ordering.end(); eqcj++) {
                    for (auto vj = eqcj->begin(); vj != eqcj->end(); vj++) {
                        string i = values[*vi];
                        string j = values[*vj];
                    
                        EXPECT_EQ(hex<c>(i) <=> hex<c>(j), std::weak_ordering::less) << "expected " << i << " < " << j;
                        EXPECT_EQ(hex<c>(j) <=> hex<c>(i), std::weak_ordering::greater) << "expected " << i << " > " << j;
                        
                        auto cmpba = bytes_type<endian::big, c>::read(i) <=> bytes_type<endian::big, c>::read(j);
                        auto cmpbb = bytes_type<endian::big, c>::read(j) <=> bytes_type<endian::big, c>::read(i);
                        EXPECT_EQ(cmpba, std::weak_ordering::less);
                        EXPECT_EQ(cmpbb, std::weak_ordering::greater);
                        auto cmpla = bytes_type<endian::little, c>::read(i) <=> bytes_type<endian::little, c>::read(j);
                        auto cmplb = bytes_type<endian::little, c>::read(j) <=> bytes_type<endian::little, c>::read(i);
                        EXPECT_EQ(cmpba, std::weak_ordering::less);
                        EXPECT_EQ(cmpbb, std::weak_ordering::greater);
                        
                        fixed_test_comparison<c, 10>{i, j, std::strong_ordering::less};
                        fixed_test_comparison<c, 11>{i, j, std::strong_ordering::less};
                        fixed_test_comparison<c, 12>{i, j, std::strong_ordering::less};
                        fixed_test_comparison<c, 13>{i, j, std::strong_ordering::less};
                        
                        fixed_test_comparison<c, 10>{j, i, std::strong_ordering::greater};
                        fixed_test_comparison<c, 11>{j, i, std::strong_ordering::greater};
                        fixed_test_comparison<c, 12>{j, i, std::strong_ordering::greater};
                        fixed_test_comparison<c, 13>{j, i, std::strong_ordering::greater};
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
        test_bit_shift(string given, int bits, string expected_left, string expected_right) : 
        test_bit_shift<n>(given, bits, expected_left, expected_right), test_bit_shift<p...>(given, bits, expected_left, expected_right) {}
    };
    
    template <complement n> struct test_bit_shift<n> {
        test_bit_shift(string given, byte bits, string expected_left, string expected_right) {
            auto l = hex<n>{expected_left};
            auto r = hex<n>{expected_right};
            auto e = hex<n>{given};
            auto x = e << bits;
            EXPECT_EQ(x, l);
            EXPECT_EQ(x >> bits, e);
            EXPECT_EQ(e >> bits, r);
            
            auto lbl = bytes_type<endian::little, n>::read(l);
            auto rbl = bytes_type<endian::little, n>::read(r);
            auto ebl = bytes_type<endian::little, n>::read(e);
            auto xbl = ebl << bits;
            EXPECT_EQ(xbl, lbl);
            EXPECT_EQ(xbl >> bits, ebl);
            EXPECT_EQ(ebl >> bits, rbl);
            
            auto lbb = bytes_type<endian::big, n>::read(l);
            auto rbb = bytes_type<endian::big, n>::read(r);
            auto ebb = bytes_type<endian::big, n>::read(e);
            auto xbb = ebb << bits;
            EXPECT_EQ(xbb, lbb);
            EXPECT_EQ(xbb >> bits, ebb);
            EXPECT_EQ(ebb >> bits, rbb);
            
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
        test_bit_shift<nones, ones, twos>("0x01", 17, "0x0200", "0x00");
        test_bit_shift<nones, ones, twos>("0x0300", 1, "0x0600", "0x0180");
        test_bit_shift<nones, ones, twos>("0x0300", 2, "0x0b00", "0x00b0");
        test_bit_shift<nones, ones, twos>("0x0300", 16, "0x030000", "0x03");
        test_bit_shift<nones, ones, twos>("0x0300", 17, "0x060000", "0x01");
        test_bit_shift<nones, ones, twos>("0x0300", 18, "0x0b0000", "0x");
        test_bit_shift<nones>("0xff", 1, "0x01fe", "0x7f");
        test_bit_shift<ones>("0xff", 1, "0xff", "0xff");
        test_bit_shift<twos>("0xff", 1, "0x80fe", "0xaf");
    }
    
    template <complement... > struct test_plus;
    
    template <complement n, complement ... p> struct test_plus<n, p...> : test_plus<n>, test_plus<p...> {
        test_plus(string left, string right, string expected) : 
        test_plus<n>(left, right, expected), test_plus<p...>(left, right, expected) {}
    };
    
    template <complement n> struct test_plus<n> {
        test_plus(string left, string right, string expected) {}
    };
    
    TEST(HexidecimalTest, TestHexidecimalPlus) {
        test_plus<nones, ones, twos>("0x", "0x", "0x");
    }
    
    template <complement... > struct test_minus;
    
    template <complement n, complement ... p> struct test_minus<n, p...> : test_minus<n>, test_minus<p...> {
        test_minus(string left, string right, string expected) : 
        test_minus<n>(left, right, expected), test_minus<p...>(left, right, expected) {}
    };
    
    template <complement n> struct test_minus<n> {
        test_minus(string left, string right, string expected) {}
    };
    
    TEST(HexidecimalTest, TestHexidecimalMinus) {
        test_plus<nones, ones, twos>("0x", "0x", "0x");
    }
    
    template <complement... > struct test_times;
    
    template <complement n, complement ... p> struct test_times<n, p...> : test_times<n>, test_times<p...> {
        test_times(string left, string right, string expected) : 
        test_times<n>(left, right, expected), test_times<p...>(left, right, expected) {}
    };
    
    template <complement n> struct test_times<n> {
        test_times(string left, string right, string expected) {}
    };
    
    TEST(HexidecimalTest, TestHexidecimalTimes) {
        test_plus<nones, ones, twos>("0x", "0x", "0x");
    }
    
} 

// this test in the first step in getting my numbers working. 
// we only work with hexidecimal format. 
namespace data {
    
    enum sizes {
        any = 0
    };
    
    template <typename N>
    struct get_number_size {
        static const sizes value = any;
    };
    
    template <bool x, endian::order r, size_t s>
    struct get_number_size<math::number::bounded<x, r, s>> {
        static const sizes value = sizes(s);
    };
    
    template <typename N>
    constexpr sizes number_size = get_number_size<N>::value;
    
    template <typename N> struct get_number_complement {
        static const math::number::complement value = math::number::ones;
    };
    
    template <endian::order r>
    struct get_number_complement<math::number::Z_bytes<r, math::number::twos>> {
        static const math::number::complement value = math::number::twos;
    };
    
    template <endian::order r>
    struct get_number_complement<math::number::N_bytes<r>> {
        static const math::number::complement value = math::number::nones;
    };
    
    template <>
    struct get_number_complement<N> {
        static const math::number::complement value = math::number::nones;
    };
    
    template <endian::order r, size_t s>
    struct get_number_complement<math::number::uint<r, s>> {
        static const math::number::complement value = math::number::nones;
    };
    
    template <typename N>
    constexpr math::number::complement number_complement = get_number_complement<N>::value;
    
    template <typename N> struct get_number_signed {
        static const bool value = true;
    };
    
    template <endian::order r>
    struct get_number_signed<math::number::N_bytes<r>> {
        static const bool value = false;
    };
    
    template <bool x, endian::order r, size_t s>
    struct get_number_signed<math::number::bounded<x, r, s>> {
        static const bool value = false;
    };
    
    template <typename N>
    constexpr bool is_signed = get_number_signed<N>::value;
    
    // the very first thing is to read in hexidecimal formats. 
    // we begin with a test that simply reads in different 
    // representations of zero in hexidecimal. 
    
    string hex_zero(size_t size) {
        std::stringstream ss;
        ss << "0x";
        for (int i = 0; i < size; i++) ss << "00";
        return ss.str();
    }
    
    string hex_negative_zero(size_t size) {
        if (size == 0) size = 1;
        std::stringstream ss;
        ss << "0x80";
        for (int i = 0; i < size - 1; i++) ss << "00";
        return ss.str();
    }
    
    template <typename N, math::number::complement c = number_complement<N>, sizes size = number_size<N>> struct test_hex_read_zero {
        test_hex_read_zero(list<size_t> xx) {
            for (size_t i : xx) {
                if (i == size) {
                    N zero = N::read(hex_zero(i));
                    EXPECT_EQ(N{}, zero);
                } else {
                    EXPECT_THROW(N::read(hex_zero(i)), std::invalid_argument);
                }
            }
        }
    };
    
    template <typename N, math::number::complement c> struct test_hex_read_zero<N, c, any> {
        test_hex_read_zero(list<size_t> xx) {
            list<N> zeros;
            for (size_t i : xx) {
                
                N zero = N::read(hex_zero(i));
                EXPECT_EQ(N{}, zero);
                EXPECT_GE(N{}, zero);
                EXPECT_LE(N{}, zero);
                for (const N &n : zeros) {
                    EXPECT_EQ(n, zero);
                    EXPECT_LE(n, zero);
                    EXPECT_EQ(n, zero);
                }
                zeros = zeros << zero;
            }
        }
    };
    
    template <typename N> struct test_hex_read_zero<N, math::number::complement::twos, any> {
        test_hex_read_zero(list<size_t> xx) {
            list<N> zeros;
            for (size_t i : xx) {
                N zero = N::read(hex_zero(i));
                N nzero = N::read(hex_zero(i));
                EXPECT_EQ(N{}, zero);
                EXPECT_GE(N{}, zero);
                EXPECT_LE(N{}, zero);
                EXPECT_EQ(N{}, nzero);
                EXPECT_GE(N{}, nzero);
                EXPECT_LE(N{}, nzero);
                for (const N &n : zeros) {
                    EXPECT_EQ(n, zero);
                    EXPECT_LE(n, zero);
                    EXPECT_EQ(n, zero);
                    EXPECT_EQ(n, nzero);
                    EXPECT_LE(n, nzero);
                    EXPECT_EQ(n, nzero);
                }
                zeros = zeros << zero << nzero;
            }
        }
    };
    
    TEST(HexidecimalTest, TestHexReadZero) {
        list<size_t> sizes{0, 1, 2, 5, 9, 10, 11, 20};
        test_hex_read_zero<N_bytes_little>{sizes};
        test_hex_read_zero<N_bytes_big>{sizes};
        test_hex_read_zero<Z_bytes_little>{sizes};
        test_hex_read_zero<Z_bytes_big>{sizes};
        test_hex_read_zero<math::number::Z_bytes<endian::little, math::number::twos>>{sizes};
        test_hex_read_zero<math::number::Z_bytes<endian::big, math::number::twos>>{sizes};
        test_hex_read_zero<N>{sizes};
        test_hex_read_zero<Z>{sizes};
        test_hex_read_zero<uint_little<9>>{sizes};
        test_hex_read_zero<int_little<9>>{sizes};
        test_hex_read_zero<uint_big<9>>{sizes};
        test_hex_read_zero<int_big<9>>{sizes};
        test_hex_read_zero<uint_little<10>>{sizes};
        test_hex_read_zero<int_little<10>>{sizes};
        test_hex_read_zero<uint_big<10>>{sizes};
        test_hex_read_zero<int_big<10>>{sizes};
        test_hex_read_zero<uint_little<11>>{sizes};
        test_hex_read_zero<int_little<11>>{sizes};
        test_hex_read_zero<uint_big<11>>{sizes};
        test_hex_read_zero<int_big<11>>{sizes};
        test_hex_read_zero<uint_little<20>>{sizes};
        test_hex_read_zero<int_little<20>>{sizes};
        test_hex_read_zero<uint_big<20>>{sizes};
        test_hex_read_zero<int_big<20>>{sizes};
    }
    
    // after this point, we don't have to test importing numbers of the wrong sizes. 
    
    template <typename N, math::number::complement c = number_complement<N>, bool u = is_signed<N>> struct test_signed_stuff {
        test_signed_stuff(const N &, const encoding::hexidecimal::integer<c, encoding::hex::lower> &) {}
    };
    
    template <typename N, math::number::complement c> struct test_signed_stuff<N, c, true> {
        test_signed_stuff(const N &n, const encoding::hexidecimal::integer<c, encoding::hex::lower> &h) {
            auto hmm = math::decrement(h);
            auto nmm = math::decrement(n);
            
            EXPECT_TRUE(math::number::is_minimal(hmm));
            EXPECT_TRUE(math::number::is_minimal(nmm));
            
            EXPECT_EQ(N::read(hmm), nmm);
            auto hhmm = encoding::hexidecimal::integer<c, encoding::hex::lower>{encoding::hexidecimal::write(nmm)};
            EXPECT_EQ(hmm, hhmm);
            
            auto hpp = math::increment(hmm);
            auto npp = math::increment(nmm);
            
            EXPECT_EQ(n, npp);
            EXPECT_EQ(h, hpp);
            
            auto nh = -h;
            auto nn = -n;
            
            EXPECT_TRUE(math::number::is_minimal(nh));
            EXPECT_TRUE(math::number::is_minimal(nn));
            
            EXPECT_EQ(N::read(nh), nn);
            auto nnh = encoding::hexidecimal::integer<c, encoding::hex::lower>{encoding::hexidecimal::write(nn)};
            EXPECT_EQ(nh, nnh);
        }
    };
    
    // we should find that the hex strings are the same when read into a number 
    // and written back. 
    template <typename N> void test_hex_read_and_write_bytes(list<string> cases) {
        list<std::pair<N, encoding::hexidecimal::integer<number_complement<N>, encoding::hex::lower>>> numbers;
        for (const string &x : cases) {
            encoding::hexidecimal::integer<number_complement<N>, encoding::hex::lower> h{x};
            N n = N::read(x);
            EXPECT_EQ(data::sign(h), data::sign(n));
            
            // some types of numbers don't work this way, so we should
            // figure out something to do about it. 
            EXPECT_EQ(math::number::is_minimal(h), math::number::is_minimal(n));
            EXPECT_EQ(x, encoding::hexidecimal::write(n));
            
            auto hpp = math::increment(h);
            auto npp = math::increment(n);
            
            EXPECT_TRUE(math::number::is_minimal(hpp));
            EXPECT_TRUE(math::number::is_minimal(npp));
            
            EXPECT_EQ(N::read(hpp), npp);
            auto hhpp = encoding::hexidecimal::integer<number_complement<N>, encoding::hex::lower>{encoding::hexidecimal::write(npp)};
            EXPECT_EQ(hpp, hhpp);
            
            test_signed_stuff<N>{n, h};
            
            for (auto &p : numbers) {
                EXPECT_EQ(n <=> p.first, h <=> p.second);
                
                auto no = n | p.first;
                auto ho = h | p.second;
            
                EXPECT_TRUE(math::number::is_minimal(ho));
                EXPECT_TRUE(math::number::is_minimal(no));
            
                EXPECT_EQ(N::read(ho), no);
                auto hho = encoding::hexidecimal::integer<number_complement<N>, encoding::hex::lower>{encoding::hexidecimal::write(no)};
                EXPECT_EQ(ho, hho);
                
                auto na = n & p.first;
                auto ha = h & p.second;
            
                EXPECT_TRUE(math::number::is_minimal(ha));
                EXPECT_TRUE(math::number::is_minimal(na));
            
                EXPECT_EQ(N::read(ha), na);
                auto hha = encoding::hexidecimal::integer<number_complement<N>, encoding::hex::lower>{encoding::hexidecimal::write(na)};
                EXPECT_EQ(ha, hha);
                
                auto np = n + p.first;
                auto hp = h + p.second;
            
                EXPECT_TRUE(math::number::is_minimal(hp));
                EXPECT_TRUE(math::number::is_minimal(np));
            
                EXPECT_EQ(N::read(hp), np);
                auto hhp = encoding::hexidecimal::integer<number_complement<N>, encoding::hex::lower>{encoding::hexidecimal::write(np)};
                EXPECT_EQ(hp, hhp);
                
                auto nm = n - p.first;
                auto hm = h - p.second;
            
                EXPECT_TRUE(math::number::is_minimal(hm));
                EXPECT_TRUE(math::number::is_minimal(nm));
            
                EXPECT_EQ(N::read(hm), nm);
                auto hhm = encoding::hexidecimal::integer<number_complement<N>, encoding::hex::lower>{encoding::hexidecimal::write(nm)};
                EXPECT_EQ(hm, hhm);
                
                auto nt = n * p.first;
                auto ht = h * p.second;
            
                EXPECT_TRUE(math::number::is_minimal(ht));
                EXPECT_TRUE(math::number::is_minimal(nt));
            
                EXPECT_EQ(N::read(ht), nt);
                auto hht = encoding::hexidecimal::integer<number_complement<N>, encoding::hex::lower>{encoding::hexidecimal::write(nt)};
                EXPECT_EQ(ht, hht);
                
            }
            
            numbers <<= std::pair{n, h};
        }
    }
    
    TEST(HexidecimalTest, TestHexReadAndWriteBytes) {
        list<string> cases{
            "0x", "0x00", "0x0000", "0xff", "0x00ff", "0xffff", "0x80", "0x8000", "0x0080", "0x01", "0x0001", "0x81", "0x8001"};
        test_hex_read_and_write_bytes<N_bytes_little>(cases);
        test_hex_read_and_write_bytes<N_bytes_big>(cases);
        test_hex_read_and_write_bytes<Z_bytes_little>(cases);
        test_hex_read_and_write_bytes<Z_bytes_big>(cases);
        test_hex_read_and_write_bytes<math::number::Z_bytes<endian::little, math::number::twos>>(cases);
        test_hex_read_and_write_bytes<math::number::Z_bytes<endian::big, math::number::twos>>(cases);
    }
    
}
