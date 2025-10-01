// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include <data/list.hpp>
#include <data/io/wait_for_enter.hpp>
#include "gtest/gtest.h"

namespace data::math::number {
    
    template <negativity c> using hex = encoding::hexidecimal::integer<c, hex_case::lower>;
    
    template <endian::order r, negativity c> struct get_bytes_type {
        using value = Z_bytes<r, c, byte>;
    };
    
    template <endian::order r> struct get_bytes_type<r, negativity::nones> {
        using value = N_bytes<r, byte>;
    };
    
    template <endian::order r, negativity c> using bytes_type = get_bytes_type<r, c>::value;
    
    template <endian::order r, negativity c, size_t size> struct get_fixed_type;
    
    template <endian::order r, size_t size> struct get_fixed_type<r, negativity::nones, size> {
        using value = uint<r, size, byte>;
    };
    
    template <endian::order r, size_t size> struct get_fixed_type<r, negativity::twos, size> {
        using value = sint<r, size, byte>;
    };
    
    template <endian::order r, size_t size> struct get_fixed_type<r, negativity::BC, size> {
        using value = void;
    };
    
    template <endian::order r, negativity c, size_t size> using fixed_type = get_fixed_type<r, c, size>::value;
    
    template <negativity c> struct hex_test_case {
        hex_test_case (string test_x, string test_trimmed, signature expected_sign) {
            {
                auto x = hex<c> {test_x};
                auto trimmed = hex<c> {test_trimmed};

                EXPECT_TRUE (x.valid ());
                EXPECT_EQ (data::is_zero (x), expected_sign == zero);
                EXPECT_EQ (data::is_positive (x), expected_sign == positive);
                EXPECT_EQ (data::is_negative (x), expected_sign == negative);
                EXPECT_EQ (data::sign (x), expected_sign);
                EXPECT_EQ (is_minimal (x), static_cast<string> (test_x) == static_cast<string> (test_trimmed));
                EXPECT_EQ (static_cast<string> (trim (x)), static_cast<string> (trimmed));
                EXPECT_EQ (minimal_size (x), trimmed.size ());
                auto extended = extend (trimmed, x.size ());
                EXPECT_EQ (extended, x);
            }

            {
                auto x = bytes_type<endian::little, c>::read (test_x);
                auto trimmed = bytes_type<endian::little, c>::read (test_trimmed);
                
                EXPECT_TRUE (x.valid ());
                EXPECT_EQ (x.size () * 2 + 2, test_x.size ());
                EXPECT_EQ (trimmed.size () * 2 + 2, test_trimmed.size ());
                
                EXPECT_EQ (test_x, encoding::hexidecimal::write<hex_case::lower> (x));
                EXPECT_EQ (test_trimmed, encoding::hexidecimal::write<hex_case::lower> (trimmed));
                
                EXPECT_EQ (data::is_zero (x), expected_sign == zero);
                EXPECT_EQ (data::is_positive (x), expected_sign == positive);
                EXPECT_EQ (data::is_negative (x), expected_sign == negative);
                EXPECT_EQ (data::sign (x), expected_sign);
                EXPECT_EQ (is_minimal (x), test_x == test_trimmed);
                EXPECT_EQ (static_cast<bytes> (trim (x)), static_cast<bytes> (trimmed));
                EXPECT_EQ (minimal_size (x), trimmed.size ());
                auto extended = extend (trimmed, x.size ());
                EXPECT_EQ (extended, x);
            }
            
            {
                auto x = bytes_type<endian::big, c>::read (test_x);
                auto trimmed = bytes_type<endian::big, c>::read (test_trimmed);
                
                EXPECT_TRUE (x.valid ());
                EXPECT_EQ (x.size () * 2 + 2, test_x.size ());
                EXPECT_EQ (trimmed.size () * 2 + 2, test_trimmed.size ());
                
                EXPECT_EQ (test_x, encoding::hexidecimal::write<hex_case::lower> (x));
                EXPECT_EQ (test_trimmed, encoding::hexidecimal::write<hex_case::lower> (trimmed));
                
                EXPECT_EQ (data::is_zero (x), expected_sign == zero);
                EXPECT_EQ (data::is_positive (x), expected_sign == positive);
                EXPECT_EQ (data::is_negative (x), expected_sign == negative);
                EXPECT_EQ (data::sign (x), expected_sign);
                EXPECT_EQ (is_minimal (x), test_x == test_trimmed);
                EXPECT_EQ (static_cast<bytes> (trim (x)), static_cast<bytes> (trimmed));
                EXPECT_EQ (minimal_size (x), trimmed.size ());
                auto extended = extend (trimmed, x.size ());
                EXPECT_EQ (extended, x);
            }
        }
    };
    
    TEST (HexidecimalTest, TestHexidecimalSignMinTrim) {

        hex_test_case<negativity::nones> ("0x", "0x", zero);
        hex_test_case<negativity::twos> ("0x", "0x", zero);
        hex_test_case<negativity::BC> ("0x", "0x", zero);
        
        hex_test_case<negativity::nones> ("0x00", "0x", zero);
        hex_test_case<negativity::twos> ("0x00", "0x", zero);
        hex_test_case<negativity::BC> ("0x00", "0x", zero);

        hex_test_case<negativity::nones> ("0x0000", "0x", zero);
        hex_test_case<negativity::twos> ("0x0000", "0x", zero);
        hex_test_case<negativity::BC> ("0x0000", "0x", zero);

        hex_test_case<negativity::nones> ("0x80", "0x80", positive);
        hex_test_case<negativity::twos> ("0x80", "0x80", negative);
        hex_test_case<negativity::BC> ("0x80", "0x", zero);

        hex_test_case<negativity::nones> ("0x8000", "0x8000", positive);
        hex_test_case<negativity::twos> ("0x8000", "0x8000", negative);
        hex_test_case<negativity::BC> ("0x8000", "0x", zero);
        
        hex_test_case<negativity::nones> ("0x01", "0x01", positive);
        hex_test_case<negativity::twos> ("0x01", "0x01", positive);
        hex_test_case<negativity::BC> ("0x01", "0x01", positive);
        
        hex_test_case<negativity::nones> ("0x0001", "0x01", positive);
        hex_test_case<negativity::twos> ("0x0001", "0x01", positive);
        hex_test_case<negativity::BC> ("0x0001", "0x01", positive);

        hex_test_case<negativity::nones> ("0xff", "0xff", positive);
        hex_test_case<negativity::twos> ("0xff", "0xff", negative);
        hex_test_case<negativity::BC> ("0xff", "0xff", negative);

        hex_test_case<negativity::nones> ("0xffff", "0xffff", positive);
        hex_test_case<negativity::twos> ("0xffff", "0xff", negative);
        hex_test_case<negativity::BC> ("0xffff", "0xffff", negative);

        hex_test_case<negativity::nones> ("0x0080", "0x80", positive);
        hex_test_case<negativity::twos> ("0x0080", "0x0080", positive);
        hex_test_case<negativity::BC> ("0x0080", "0x0080", positive);
        
        hex_test_case<negativity::nones> ("0x81", "0x81", positive);
        hex_test_case<negativity::twos> ("0x81", "0x81", negative);
        hex_test_case<negativity::BC> ("0x81", "0x81", negative);
        
        hex_test_case<negativity::nones> ("0x8001", "0x8001", positive);
        hex_test_case<negativity::twos> ("0x8001", "0x8001", negative);
        hex_test_case<negativity::BC> ("0x8001", "0x81", negative);
        
        hex_test_case<negativity::nones> ("0x00ff", "0xff", positive);
        hex_test_case<negativity::twos> ("0x00ff", "0x00ff", positive);
        hex_test_case<negativity::BC> ("0x00ff", "0x00ff", positive);
        
        hex_test_case<negativity::nones> ("0x0200", "0x0200", positive);
        hex_test_case<negativity::twos> ("0x0200", "0x0200", positive);
        hex_test_case<negativity::BC> ("0x0200", "0x0200", positive);
        
        hex_test_case<negativity::nones> ("0x2000", "0x2000", positive);
        hex_test_case<negativity::twos> ("0x2000", "0x2000", positive);
        hex_test_case<negativity::BC> ("0x2000", "0x2000", positive);

        hex_test_case<negativity::nones> ("0x010000", "0x010000", positive);
        hex_test_case<negativity::twos> ("0x010000", "0x010000", positive);
        hex_test_case<negativity::BC> ("0x010000", "0x010000", positive);
    }
    
    template <negativity... > struct test_bit_and;
    template <negativity... > struct test_bit_or;
    
    template <negativity n, negativity ... p> struct test_bit_and<n, p...> : test_bit_and<n>, test_bit_and<p...> {
        test_bit_and (string left, string right, string expected) :
        test_bit_and<n> (left, right, expected), test_bit_and<p...> (left, right, expected) {}
    };
    
    template <negativity n, negativity ... p> struct test_bit_or<n, p...> : test_bit_or<n>, test_bit_or<p...> {
        test_bit_or (string left, string right, string expected) :
        test_bit_or<n> (left, right, expected), test_bit_or<p...> (left, right, expected) {}
    };
    
    template <negativity n, size_t size> struct test_bit_and_fixed;
    template <negativity n, size_t size> struct test_bit_or_fixed;
    
    template <size_t size> struct test_bit_and_fixed<negativity::nones, size> {
        test_bit_and_fixed (hex<negativity::nones> left, hex<negativity::nones> right, hex<negativity::nones> expected) {
            auto ll = uint<endian::little, size, byte>::read (extend (left, size * 2 + 2));
            auto rl = uint<endian::little, size, byte>::read (extend (right, size * 2 + 2));
            auto el = uint<endian::little, size, byte>::read (extend (expected, size * 2 + 2));
            EXPECT_EQ (ll & rl, el);
            
            auto lb = uint<endian::big, size, byte>::read (extend (left, size * 2 + 2));
            auto rb = uint<endian::big, size, byte>::read (extend (right, size * 2 + 2));
            auto eb = uint<endian::big, size, byte>::read (extend (expected, size * 2 + 2));
            EXPECT_EQ (lb & rb, eb);
        }
    };
    
    template <size_t size> struct test_bit_or_fixed<negativity::nones, size> {
        test_bit_or_fixed (hex<negativity::nones> left, hex<negativity::nones> right, hex<negativity::nones> expected) {
            auto ll = uint<endian::little, size, byte>::read (extend (left, size * 2 + 2));
            auto rl = uint<endian::little, size, byte>::read (extend (right, size * 2 + 2));
            auto el = uint<endian::little, size, byte>::read (extend (expected, size * 2 + 2));
            EXPECT_EQ (ll | rl, el);
            
            auto lb = uint<endian::big, size, byte>::read (extend (left, size * 2 + 2));
            auto rb = uint<endian::big, size, byte>::read (extend (right, size * 2 + 2));
            auto eb = uint<endian::big, size, byte>::read (extend (expected, size * 2 + 2));
            EXPECT_EQ (lb | rb, eb);
        }
    };
    
    template <size_t size> struct test_bit_and_fixed<negativity::twos, size> {
        test_bit_and_fixed (hex<negativity::twos> left, hex<negativity::twos> right, hex<negativity::twos> expected) {
            auto ll = sint<endian::little, size, byte>::read (extend (left, size * 2 + 2));
            auto rl = sint<endian::little, size, byte>::read (extend (right, size * 2 + 2));
            auto el = sint<endian::little, size, byte>::read (extend (expected, size * 2 + 2));
            EXPECT_EQ (ll & rl, el);
            
            auto lb = sint<endian::big, size, byte>::read (extend (left, size * 2 + 2));
            auto rb = sint<endian::big, size, byte>::read (extend (right, size * 2 + 2));
            auto eb = sint<endian::big, size, byte>::read (extend (expected, size * 2 + 2));
            EXPECT_EQ (lb & rb, eb);
        }
    };
    
    template <size_t size> struct test_bit_or_fixed<negativity::twos, size> {
        test_bit_or_fixed (hex<negativity::twos> left, hex<negativity::twos> right, hex<negativity::twos> expected) {
            
            auto ll = sint<endian::little, size, byte>::read (extend (left, size * 2 + 2));
            auto rl = sint<endian::little, size, byte>::read (extend (right, size * 2 + 2));
            auto el = sint<endian::little, size, byte>::read (extend (expected, size * 2 + 2));
            EXPECT_EQ (ll | rl, el);
            
            auto lb = sint<endian::big, size, byte>::read (extend (left, size * 2 + 2));
            auto rb = sint<endian::big, size, byte>::read (extend (right, size * 2 + 2));
            auto eb = sint<endian::big, size, byte>::read (extend (expected, size * 2 + 2));
            EXPECT_EQ (lb | rb, eb);
        }
    };
    
    template <size_t size> struct test_bit_and_fixed<negativity::BC, size> {
        test_bit_and_fixed (hex<negativity::BC> left, hex<negativity::BC> right, hex<negativity::BC> expected) {}
    };
    
    template <size_t size> struct test_bit_or_fixed<negativity::BC, size> {
        test_bit_or_fixed (hex<negativity::BC> left, hex<negativity::BC> right, hex<negativity::BC> expected) {}
    };
    
    template <negativity n> struct test_bit_and<n> {
        test_bit_and (string left, string right, string expected) {
            
            auto l = hex<n> {left};
            auto r = hex<n> {right};
            auto e = hex<n> {expected};
            EXPECT_EQ (l & r, e);
            
            auto lbl = bytes_type<endian::little, n>::read (left);
            auto rbl = bytes_type<endian::little, n>::read (right);
            auto ebl = bytes_type<endian::little, n>::read (expected);
            EXPECT_EQ (lbl & rbl, ebl);
            
            auto lbb = bytes_type<endian::big, n>::read (left);
            auto rbb = bytes_type<endian::big, n>::read (right);
            auto ebb = bytes_type<endian::big, n>::read (expected);
            EXPECT_EQ (lbb & rbb, ebb);
            
            test_bit_and_fixed<n, 10> (l, r, e);
            test_bit_and_fixed<n, 11> (l, r, e);
            test_bit_and_fixed<n, 12> (l, r, e);
            test_bit_and_fixed<n, 13> (l, r, e);
        }
    };
    
    template <negativity n> struct test_bit_or<n> {
        test_bit_or (string left, string right, string expected) {
            
            auto l = hex<n> {left};
            auto r = hex<n> {right};
            auto e = hex<n> {expected};
            EXPECT_EQ (l | r, e);
            
            auto lbl = bytes_type<endian::little, n>::read (left);
            auto rbl = bytes_type<endian::little, n>::read (right);
            auto ebl = bytes_type<endian::little, n>::read (expected);
            EXPECT_EQ (lbl | rbl, ebl);
            
            auto lbb = bytes_type<endian::big, n>::read (left);
            auto rbb = bytes_type<endian::big, n>::read (right);
            auto ebb = bytes_type<endian::big, n>::read (expected);
            EXPECT_EQ (lbb | rbb, ebb);
            
            test_bit_or_fixed<n, 10> (l, r, e);
            test_bit_or_fixed<n, 11> (l, r, e);
            test_bit_or_fixed<n, 12> (l, r, e);
            test_bit_or_fixed<n, 13> (l, r, e);
            
        }
    };

    TEST (HexidecimalTest, TestHexBitAnd) {

        test_bit_and<negativity::nones, negativity::twos, negativity::BC> {"0x", "0x", "0x"};
        test_bit_and<negativity::nones, negativity::twos, negativity::BC> {"0x01", "0x01", "0x01"};
        test_bit_and<negativity::nones, negativity::twos, negativity::BC> {"0x", "0x01", "0x"};
        test_bit_and<negativity::nones, negativity::twos, negativity::BC> {"0xff", "0xff", "0xff"};
        test_bit_and<negativity::nones, negativity::twos, negativity::BC> {"0x", "0xff", "0x"};

    }
    
    TEST (HexidecimalTest, TestHexBitOr) {

        test_bit_or<negativity::nones, negativity::twos, negativity::BC> {"0x", "0x", "0x"};
        test_bit_or<negativity::nones, negativity::twos, negativity::BC> {"0x01", "0x01", "0x01"};
        test_bit_or<negativity::nones, negativity::twos, negativity::BC> {"0x01", "0x", "0x01"};
        test_bit_or<negativity::nones, negativity::twos, negativity::BC> {"0xff", "0xff", "0xff"};
        test_bit_or<negativity::nones, negativity::twos, negativity::BC> {"0x", "0xff", "0xff"};

    }
    
    template <negativity... n> struct test_bit_negate;
    
    template <negativity n, negativity ... p> struct test_bit_negate<n, p...> : test_bit_negate<n>, test_bit_negate<p...> {
        test_bit_negate (string given, string expected) :
        test_bit_negate<n> (given, expected), test_bit_negate<p...> (given, expected) {}
    };
    
    template <negativity n, size_t size> struct test_bit_negate_fixed;
    
    template <size_t size> struct test_bit_negate_fixed<negativity::twos, size> {
        test_bit_negate_fixed (string given, string expected) {
            auto g = extend (hex<negativity::twos> {given}, size * 2 + 2);
            auto e = extend (hex<negativity::twos> {expected}, size * 2 + 2);
            
            auto gl = sint<endian::little, size, byte>::read (g);
            auto el = sint<endian::little, size, byte>::read (e);
            EXPECT_EQ (~gl, el);
            
            auto gb = sint<endian::big, size, byte>::read (g);
            auto eb = sint<endian::big, size, byte>::read (e);
            EXPECT_EQ (~gb, eb);
        }
    };
    
    template <size_t size> struct test_bit_negate_fixed<negativity::nones, size> {
        test_bit_negate_fixed (string given, string expected) {
            auto g = extend (hex<negativity::twos> {given}, size * 2 + 2);
            auto e = extend (hex<negativity::twos> {expected}, size * 2 + 2);
            
            auto gl = uint<endian::little, size, byte>::read (g);
            auto el = uint<endian::little, size, byte>::read (e);
            EXPECT_EQ (~gl, el);
            
            auto gb = uint<endian::big, size, byte>::read (g);
            auto eb = uint<endian::big, size, byte>::read (e);
            EXPECT_EQ (~gb, eb);
        }
    };
    
    template <> struct test_bit_negate<negativity::twos> {
        test_bit_negate (string given, string expected) {
            
            auto g = hex<negativity::twos> {given};
            auto e = hex<negativity::twos> {expected};
            EXPECT_EQ (~g, e);
            
            auto gbl = bytes_type<endian::little, negativity::twos>::read (given);
            auto ebl = bytes_type<endian::little, negativity::twos>::read (expected);
            EXPECT_EQ (~gbl, ebl);
            
            auto gbb = bytes_type<endian::big, negativity::twos>::read (given);
            auto ebb = bytes_type<endian::big, negativity::twos>::read (expected);
            EXPECT_EQ (~gbb, ebb);
            
            test_bit_negate_fixed<negativity::twos, 10> (g, e);
            test_bit_negate_fixed<negativity::twos, 11> (g, e);
            test_bit_negate_fixed<negativity::twos, 12> (g, e);
            test_bit_negate_fixed<negativity::twos, 13> (g, e);
        }
    };
    
    template <> struct test_bit_negate<negativity::nones> {
        test_bit_negate (string given, string expected) {
            
            auto g = hex<negativity::nones> {given};
            auto e = hex<negativity::nones> {expected};
            
            test_bit_negate_fixed<negativity::nones, 10> (g, e);
            test_bit_negate_fixed<negativity::nones, 11> (g, e);
            test_bit_negate_fixed<negativity::nones, 12> (g, e);
            test_bit_negate_fixed<negativity::nones, 13> (g, e);
        }
    };
    
    template <negativity... n> struct test_bit_xor;
    
    template <negativity n, negativity ... p> struct test_bit_xor<n, p...> : test_bit_xor<n>, test_bit_xor<p...> {
        test_bit_xor (string left, string right, string expected) :
        test_bit_xor<n> (left, right, expected), test_bit_xor<p...> (left, right, expected) {}
    };
    
    template <negativity n, size_t size> struct test_bit_xor_fixed;
    
    template <size_t size> struct test_bit_xor_fixed<negativity::nones, size> {
        test_bit_xor_fixed (string left, string right, string expected) {
            auto l = extend (hex<negativity::twos> {left}, size * 2 + 2);
            auto r = extend (hex<negativity::twos> {right}, size * 2 + 2);
            auto e = extend (hex<negativity::twos> {expected}, size * 2 + 2);
            
            auto ll = uint<endian::little, size, byte>::read (l);
            auto rl = uint<endian::little, size, byte>::read (r);
            auto el = uint<endian::little, size, byte>::read (e);
            
            auto xl = ll ^ rl;
            EXPECT_EQ (xl, el) << "expected " << xl << " to equal " << el;
            
            auto lb = uint<endian::big, size, byte>::read (l);
            auto rb = uint<endian::big, size, byte>::read (r);
            auto eb = uint<endian::big, size, byte>::read (e);
            
            auto xb = lb ^ rb;
            EXPECT_EQ (xb, eb) << "expected " << xb << " to equal " << eb;
        }
    };
    
    template <size_t size> struct test_bit_xor_fixed<negativity::twos, size> {
        test_bit_xor_fixed (string left, string right, string expected) {

            auto l = extend (hex<negativity::twos> {left}, size * 2 + 2);
            auto r = extend (hex<negativity::twos> {right}, size * 2 + 2);
            auto e = extend (hex<negativity::twos> {expected}, size * 2 + 2);
            
            auto ll = sint<endian::little, size, byte>::read (l);
            auto rl = sint<endian::little, size, byte>::read (r);
            auto el = sint<endian::little, size, byte>::read (e);
            
            auto xl = ll ^ rl;
            EXPECT_EQ (xl, el) << "expected " << xl << " to equal " << el;
            
            auto lb = sint<endian::big, size, byte>::read (l);
            auto rb = sint<endian::big, size, byte>::read (r);
            auto eb = sint<endian::big, size, byte>::read (e);
            
            auto xb = lb ^ rb;
            EXPECT_EQ (xb, eb) << "expected " << xb << " to equal " << eb;

        }
    };

    template <> struct test_bit_xor<negativity::twos> {
        test_bit_xor (string left, string right, string expected) {

            auto l = hex<negativity::twos> {left};
            auto r = hex<negativity::twos> {right};
            auto e = hex<negativity::twos> {expected};
            EXPECT_EQ (l ^ r, e);
            
            auto lbl = bytes_type<endian::little, negativity::twos>::read (left);
            auto rbl = bytes_type<endian::little, negativity::twos>::read (right);
            auto ebl = bytes_type<endian::little, negativity::twos>::read (expected);
            EXPECT_EQ (lbl ^ rbl, ebl);
            
            auto lbb = bytes_type<endian::big, negativity::twos>::read (left);
            auto rbb = bytes_type<endian::big, negativity::twos>::read (right);
            auto ebb = bytes_type<endian::big, negativity::twos>::read (expected);
            EXPECT_EQ (lbb ^ rbb, ebb);
            
            test_bit_xor_fixed<negativity::twos, 10> (l, r, e);
            test_bit_xor_fixed<negativity::twos, 11> (l, r, e);
            test_bit_xor_fixed<negativity::twos, 12> (l, r, e);
            test_bit_xor_fixed<negativity::twos, 13> (l, r, e);

        }
    };
    
    template <> struct test_bit_xor<negativity::nones> {
        test_bit_xor (string left, string right, string expected) {

            auto l = hex<negativity::nones> {left};
            auto r = hex<negativity::nones> {right};
            auto e = hex<negativity::nones> {expected};
            
            test_bit_xor_fixed<negativity::nones, 10> (l, r, e);
            test_bit_xor_fixed<negativity::nones, 11> (l, r, e);
            test_bit_xor_fixed<negativity::nones, 12> (l, r, e);
            test_bit_xor_fixed<negativity::nones, 13> (l, r, e);

        }
    };

    TEST (HexidecimalTest, TestHexBitXor) {

        test_bit_negate<negativity::nones, negativity::twos> {"0x", "0xff"};
        test_bit_xor<negativity::nones, negativity::twos> {"0x", "0x", "0x"};
        test_bit_xor<negativity::nones, negativity::twos> {"0xff", "0xff", "0x"};
        test_bit_xor<negativity::nones, negativity::twos> {"0x", "0xff", "0xff"};

    }
    
    template <negativity n, size_t size> struct test_increment_fixed;
    
    template <negativity... > struct test_increment;

    template <negativity n> struct test_increment_bytes;
    
    template <negativity n> struct test_increment<n> {
        test_increment (string given, string expected) {

            auto gh = hex<n> {given};
            auto ih = data::increment (gh);
            auto dh = data::decrement (ih);
            
            EXPECT_EQ (static_cast<string> (ih), expected) << "expected " << given << " to increment to " << expected ;
            EXPECT_EQ (dh, gh) << "expected " << dh << " == " << gh << std::endl;
            EXPECT_TRUE (is_minimal (hex<n> (ih)));
            EXPECT_TRUE (is_minimal (dh));

            test_increment_bytes<n> {given, expected};
            
            test_increment_fixed<n, 10> {gh, ih};
            test_increment_fixed<n, 11> {gh, ih};
            test_increment_fixed<n, 12> {gh, ih};
            test_increment_fixed<n, 13> {gh, ih};

        }
    };
    
    template <negativity n, negativity ... p> struct test_increment<n, p...> : test_increment<n>, test_increment<p...> {
        test_increment (string given, string expected) :
        test_increment<n> (given, expected), test_increment<p...> (given, expected) {}
    };

    template <negativity n> struct test_increment_bytes {
        test_increment_bytes (string given, string expected) {

            auto gbl = bytes_type<endian::little, n>::read (given);
            auto ebl = bytes_type<endian::little, n>::read (expected);
            auto ibl = data::increment (gbl);
            EXPECT_EQ (static_cast<bytes> (ibl), static_cast<bytes> (ebl)) << "expected " << ibl << " === " << ebl;
            EXPECT_TRUE (is_minimal (bytes_type<endian::little, n> (ibl)));

            auto gbb = bytes_type<endian::big, n>::read (given);
            auto ebb = bytes_type<endian::big, n>::read (expected);
            auto ibb = data::increment (gbb);
            EXPECT_EQ (static_cast<bytes> (ibb), static_cast<bytes> (ebb)) << "expected " << ibb << " === " << ebb;
            EXPECT_TRUE (is_minimal (bytes_type<endian::big, n> (ibb)));

            auto dbl = data::decrement (ibl);
            EXPECT_EQ (dbl, gbl) << "expected " << dbl << " == " << gbl;
            EXPECT_TRUE (is_minimal (dbl));

            auto dbb = data::decrement (ibb);
            EXPECT_EQ (dbb, gbb) << "expected " << dbb << " == " << gbb;
            EXPECT_TRUE (is_minimal (dbb));

        }
    };
    
    template <size_t size> struct test_increment_fixed<negativity::nones, size> {
        test_increment_fixed (hex<negativity::nones> given, hex<negativity::nones> expected) {

            auto gbl = uint<endian::little, size, byte>::read (extend (given, size * 2 + 2));
            auto ebl = uint<endian::little, size, byte>::read (extend (expected, size * 2 + 2));
            
            auto ibl = data::increment (gbl);
            EXPECT_EQ (ibl, ebl) << "expected " << ibl << " to equal " << ebl << std::endl;
            auto dbl = data::decrement (ibl);
            EXPECT_EQ (dbl, gbl) << "expected " << dbl << " to equal " << gbl << std::endl;
            
            auto gbb = uint<endian::big, size, byte>::read (extend (given, size * 2 + 2));
            auto ebb = uint<endian::big, size, byte>::read (extend (expected, size * 2 + 2));
            
            auto ibb = data::increment (gbb);
            EXPECT_EQ (ibb, ebb) << "expected " << ibb << " to equal " << ebb << std::endl;
            auto dbb = data::decrement (ibb);
            EXPECT_EQ (dbb, gbb) << "expected " << dbb << " to equal " << gbl << std::endl;
        }
    };
    
    template <size_t size> struct test_increment_fixed<negativity::twos, size> {
        test_increment_fixed (hex<negativity::twos> given, hex<negativity::twos> expected) {
            
            auto gbl = sint<endian::little, size, byte>::read (extend (given, size * 2 + 2));
            auto ebl = sint<endian::little, size, byte>::read (extend (expected, size * 2 + 2));
            
            auto ibl = data::increment (gbl);
            EXPECT_EQ (ibl, ebl) << "expected " << ibl << " to equal " << ebl << std::endl;
            auto dbl = data::decrement (ibl);
            EXPECT_EQ (dbl, gbl) << "expected " << dbl << " to equal " << gbl << std::endl;
            
            auto gbb = sint<endian::big, size, byte>::read (extend (given, size * 2 + 2));
            auto ebb = sint<endian::big, size, byte>::read (extend (expected, size * 2 + 2));
            
            auto ibb = data::increment (gbb);
            EXPECT_EQ (ibb, ebb) << "expected " << ibb << " to equal " << ebb << std::endl;
            auto dbb = data::decrement (ibb);
            EXPECT_EQ (dbb, gbb) << "expected " << dbb << " to equal " << gbl << std::endl;
        }
    };
    
    template <size_t size> struct test_increment_fixed<negativity::BC, size> {
        test_increment_fixed (hex<negativity::BC> given, hex<negativity::BC> expected) {}
    };

    TEST (HexidecimalTest, TestHexidecimalIncrement) {

        test_increment<negativity::nones, negativity::twos, negativity::BC> {"0x", "0x01"};
        test_increment<negativity::nones, negativity::twos, negativity::BC> {"0x00", "0x01"};
        test_increment<negativity::nones, negativity::twos, negativity::BC> {"0x0000", "0x01"};
        test_increment<negativity::nones, negativity::twos, negativity::BC> {"0x01", "0x02"};
        test_increment<negativity::nones> {"0xff", "0x0100"};
        test_increment<negativity::nones> {"0x7f", "0x80"};
        test_increment<negativity::nones> {"0xffff", "0x010000"};
        test_increment<negativity::nones, negativity::twos, negativity::BC> {"0x00ff", "0x0100"};
        test_increment<negativity::twos> {"0xff", "0x"};
        test_increment<negativity::twos> {"0xffff", "0x"};
        test_increment<negativity::twos> {"0x7f", "0x0080"};
        test_increment<negativity::twos> {"0x80", "0x81"};
        test_increment<negativity::twos> {"0xfe", "0xff"};
        test_increment<negativity::BC> {"0x80", "0x01"};
        test_increment<negativity::BC> {"0x81", "0x"};
        test_increment<negativity::BC> {"0x82", "0x81"};
        test_increment<negativity::BC> {"0x7f", "0x0080"};
        test_increment<negativity::BC> {"0x8080", "0xff"};

        // TODO need to test that decrement 0 is 0 for unsigned numbers.

    }

    template <typename N> struct test_negation_number {
        test_negation_number (string initial, string expected) {

            auto hi = N::read (initial);
            auto he = N::read (expected);
            auto hn = -hi;
            auto ho = -hn;

            EXPECT_EQ (hn, he) << std::hex << "expected " << hn << " to equal " << he << std::endl;
            EXPECT_EQ (ho, hi) << std::hex << "expected " << ho << " to equal " << hi << std::endl;
        }
    };

    template <endian::order r, negativity n>
    struct test_negation_number<Z_bytes<r, n, byte>> {
        test_negation_number (string initial, string expected) {

            auto hi = Z_bytes<r, n, byte>::read (initial);
            auto he = Z_bytes<r, n, byte>::read (expected);
            auto hn = -hi;
            auto ho = -hn;

            EXPECT_EQ (hn, he) << std::hex << "expected " << hn << " to equal " << he << std::endl;
            EXPECT_EQ (ho, hi) << std::hex << "expected " << ho << " to equal " << hi << std::endl;
            EXPECT_TRUE (is_minimal (hn));
            EXPECT_TRUE (is_minimal (ho));
        }
    };

    template <> struct test_negation_number<void> {
        test_negation_number (string initial, string expected) {}
    };

    template <negativity n> struct test_negation {
        test_negation (string initial, string expected) {
            
            auto hi = hex<n> {initial};
            auto he = hex<n> {expected};
            auto hn = -hi;
            auto ho = -hn;
            EXPECT_EQ (string (hn), string (he)) << "expected " << n << " " << hn << " === " << he << std::endl;
            EXPECT_EQ (ho, hi);
            EXPECT_TRUE (is_minimal (hn));
            EXPECT_TRUE (is_minimal (ho));
            
            test_negation_number<bytes_type<endian::little, n>> {hi, he};
            test_negation_number<bytes_type<endian::big, n>> {hi, he};
            
            test_negation_number<fixed_type<endian::little, n, 10>> {extend (hi, 22), extend (he, 22)};
            test_negation_number<fixed_type<endian::big, n, 10>> {extend (hi, 22), extend (he, 22)};
            
            test_negation_number<fixed_type<endian::little, n, 11>> {extend (hi, 24), extend (he, 24)};
            test_negation_number<fixed_type<endian::big, n, 11>> {extend (hi, 24), extend (he, 24)};
            
            test_negation_number<fixed_type<endian::little, n, 12>> {extend (hi, 26), extend (he, 26)};
            test_negation_number<fixed_type<endian::big, n, 12>> {extend (hi, 26), extend (he, 26)};
            
            test_negation_number<fixed_type<endian::little, n, 13>> {extend (hi, 28), extend (he, 28)};
            test_negation_number<fixed_type<endian::big, n, 13>> {extend (hi, 28), extend (he, 28)};
            
        }
    };
    
    template <> struct test_negation<negativity::nones> {
        test_negation (string initial, string expected) {}
    };

    TEST (HexidecimalTest, TestNegation) {
        
        test_negation<negativity::BC> {"0x", "0x"};
        test_negation<negativity::BC> {"0x00", "0x"};
        test_negation<negativity::BC> {"0x80", "0x"};
        test_negation<negativity::BC> {"0x81", "0x01"};
        test_negation<negativity::BC> {"0x01", "0x81"};
        test_negation<negativity::BC> {"0x8001", "0x01"};
        test_negation<negativity::BC> {"0x0001", "0x81"};
        
        test_negation<negativity::twos> {"0x", "0x"};
        test_negation<negativity::twos> {"0x00", "0x"};
        test_negation<negativity::twos> {"0x80", "0x0080"};
        test_negation<negativity::twos> {"0xff", "0x01"};
        test_negation<negativity::twos> {"0xffff", "0x01"};
        test_negation<negativity::twos> {"0x01", "0xff"};
        test_negation<negativity::twos> {"0x02", "0xfe"};
        test_negation<negativity::twos> {"0xfe", "0x02"};
        test_negation<negativity::twos> {"0x0100", "0xff00"};

    }
    
    template <negativity c, size_t size> struct fixed_test_comparison;
    
    template <size_t size> struct fixed_test_comparison<negativity::nones, size> {
        fixed_test_comparison (string i, string j, std::strong_ordering expected) {
            auto cmpb = uint<endian::big, size, byte>::read (extend (hex<negativity::nones> (i), size * 2 + 2)) <=>
                uint<endian::big, size, byte>::read (extend (hex<negativity::nones> (j), size * 2 + 2));
            EXPECT_EQ (cmpb, expected);
            auto cmpl = uint<endian::little, size, byte>::read (extend (hex<negativity::nones> (i), size * 2 + 2)) <=>
                uint<endian::little, size, byte>::read (extend (hex<negativity::nones> (j), size * 2 + 2));
            EXPECT_EQ (cmpl, expected);
        }
    };
    
    template <size_t size> struct fixed_test_comparison<negativity::twos, size> {
        fixed_test_comparison (string i, string j, std::strong_ordering expected) {
            
            string exi = extend (hex<negativity::twos> (i), size * 2 + 2);
            string exj = extend (hex<negativity::twos> (j), size * 2 + 2);

            auto sbi = sint<endian::big, size, byte>::read (exi);
            auto sbj = sint<endian::big, size, byte>::read (exj);

            auto sli = sint<endian::little, size, byte>::read (exi);
            auto slj = sint<endian::little, size, byte>::read (exj);

            auto cmpb = sbi <=> sbj;
            auto cmpl = sli <=> slj;

            EXPECT_EQ (cmpb, expected);
            EXPECT_EQ (cmpl, expected);
        }
    };
    
    // we do nothing here because we don't have fixed size 2s negativity numbers.
    template <size_t size> struct fixed_test_comparison<negativity::BC, size> {
        fixed_test_comparison (string a, string b, std::strong_ordering expected) {}
    };
    
    template <negativity c> void test_comparison (const std::vector<string> &values, const std::vector<std::vector<int>> &ordering) {
        for (const string &v : values) {
            EXPECT_THROW (hex<c> {""} <=> hex<c> {v}, exception);
        }
        
        for (auto eqci = ordering.begin (); eqci != ordering.end (); eqci++) {
            for (auto vi = eqci->begin (); vi != eqci->end (); vi++) {
                for (auto vj = vi + 1; vj != eqci->end (); vj++) {
                    string i = values[*vi];
                    string j = values[*vj];
                    
                    EXPECT_EQ (hex<c> (i) <=> hex<c> (j), std::weak_ordering::equivalent);

                    auto cmpb = bytes_type<endian::big, c>::read (i) <=> bytes_type<endian::big, c>::read (j);
                    EXPECT_EQ (cmpb, std::weak_ordering::equivalent);
                    auto cmpl = bytes_type<endian::little, c>::read (i) <=> bytes_type<endian::little, c>::read (j);
                    EXPECT_EQ (cmpl, std::weak_ordering::equivalent);

                    fixed_test_comparison<c, 10> {i, j, std::strong_ordering::equivalent};
                    fixed_test_comparison<c, 11> {i, j, std::strong_ordering::equivalent};
                    fixed_test_comparison<c, 12> {i, j, std::strong_ordering::equivalent};
                    fixed_test_comparison<c, 13> {i, j, std::strong_ordering::equivalent};
                }
                
                for (auto eqcj = eqci + 1; eqcj != ordering.end (); eqcj++) {
                    for (auto vj = eqcj->begin (); vj != eqcj->end (); vj++) {
                        string i = values[*vi];
                        string j = values[*vj];
                    
                        EXPECT_EQ (hex<c> (i) <=> hex<c> (j), std::weak_ordering::less) << "expected " << i << " < " << j;
                        EXPECT_EQ (hex<c> (j) <=> hex<c> (i), std::weak_ordering::greater) << "expected " << i << " > " << j;

                        auto cmpba = bytes_type<endian::big, c>::read (i) <=> bytes_type<endian::big, c>::read (j);
                        auto cmpbb = bytes_type<endian::big, c>::read (j) <=> bytes_type<endian::big, c>::read (i);
                        EXPECT_EQ (cmpba, std::weak_ordering::less);
                        EXPECT_EQ (cmpbb, std::weak_ordering::greater);
                        auto cmpla = bytes_type<endian::little, c>::read (i) <=> bytes_type<endian::little, c>::read (j);
                        auto cmplb = bytes_type<endian::little, c>::read (j) <=> bytes_type<endian::little, c>::read (i);
                        EXPECT_EQ (cmpba, std::weak_ordering::less);
                        EXPECT_EQ (cmpbb, std::weak_ordering::greater);

                        fixed_test_comparison<c, 10> {i, j, std::strong_ordering::less};
                        fixed_test_comparison<c, 11> {i, j, std::strong_ordering::less};
                        fixed_test_comparison<c, 12> {i, j, std::strong_ordering::less};
                        fixed_test_comparison<c, 13> {i, j, std::strong_ordering::less};
                        
                        fixed_test_comparison<c, 10> {j, i, std::strong_ordering::greater};
                        fixed_test_comparison<c, 11> {j, i, std::strong_ordering::greater};
                        fixed_test_comparison<c, 12> {j, i, std::strong_ordering::greater};
                        fixed_test_comparison<c, 13> {j, i, std::strong_ordering::greater};
                    }
                }
            }
        }
    }

    TEST (HexidecimalTest, TestHexidecimalComparisons) {
        std::vector<string> values {
            "0x", "0x00", "0x0000", "0x80", "0x8000", "0x01", "0x0001", 
            "0xff", "0xffff", "0x0080", "0x81", "0x8001", "0x00ff", 
            "0x0200", "0x2000"};
        
        std::vector<std::vector<int>> o0 {{0, 1, 2}, {5, 6}, {3, 9}, {10}, {7, 12}, {13}, {14}, {4}, {11}, {8}};
        std::vector<std::vector<int>> o1 {{4}, {11}, {3}, {10}, {7, 8}, {0, 1, 2}, {5, 6}, {9}, {12}, {13}, {14}};
        std::vector<std::vector<int>> o2 {{8}, {7}, {10, 11}, {0, 1, 2, 3, 4}, {5, 6}, {9}, {12}, {13}, {14}};
        
        test_comparison<negativity::nones> (values, o0);
        test_comparison<negativity::twos> (values, o1);
        test_comparison<negativity::BC> (values, o2);
        
    }

    // the number with negativity two should be converted to the
    // number Bitcoin format.
    void test_negativity_twos_to_BC (string twos, string bc) {

        auto t = hex<negativity::twos> {twos};
        auto b = hex<negativity::BC> {bc};

        // BC converted to twos should be equal (but not identical);
        EXPECT_EQ (b, t) << "expected " << b << " to equal " << t << " ; converted to twos: " << hex<negativity::twos> (t);

        auto tb = static_cast <std::string> (hex<negativity::BC> (t));
        auto bb = static_cast <std::string> (b);
        EXPECT_EQ (tb, bb) << "expected " << tb << " to equal " << bb;

        auto tbb = bytes_type<endian::big, negativity::twos>::read (t);
        auto bbb = bytes_type<endian::big, negativity::BC>::read (b);

        EXPECT_EQ (tbb, bbb) << "expected " << std::hex << tbb << " to equal " << bbb;
        EXPECT_EQ (static_cast<bytes> (bytes_type<endian::big, negativity::BC> (bbb)), static_cast<bytes> (bbb));

        auto tbl = bytes_type<endian::little, negativity::twos>::read (t);
        auto bbl = bytes_type<endian::little, negativity::BC>::read (b);

        EXPECT_EQ (tbl, bbl) << "expected " << std::hex << tbl << " to equal " << bbl;
        EXPECT_EQ (static_cast<bytes> (bytes_type<endian::little, negativity::BC> (tbl)), static_cast<bytes> (bbl));

    }

    // the number with negativity BC should be converted to the
    // number in negativity twos.
    void test_negativity_BC_to_twos (string bc, string twos) {

        auto b = hex<negativity::BC> {bc};
        auto t = hex<negativity::twos> {twos};

        EXPECT_EQ (b, t) << "expected " << b << " to equal " << t;

        auto bt = static_cast <std::string> (hex<negativity::twos> (b));
        auto tt = static_cast <std::string> (t);
        EXPECT_EQ (bt, tt) << "expected " << bt << " === " << tt;

        auto bbb = bytes_type<endian::big, negativity::BC>::read (b);
        auto tbb = bytes_type<endian::big, negativity::twos>::read (t);

        EXPECT_EQ (bbb, tbb) << "expected " << std::hex << bbb << " to equal " << tbb;

        auto otbb = static_cast<bytes> (bytes_type<endian::big, negativity::twos> (bbb));
        auto ttbb = static_cast<bytes> (tbb);
        EXPECT_EQ (otbb, ttbb) << "expected " << otbb << " === " << ttbb;

        auto tbl = bytes_type<endian::little, negativity::BC>::read (b);
        auto obl = bytes_type<endian::little, negativity::twos>::read (t);

        EXPECT_EQ (tbl, obl) << "expected " << std::hex << tbl << " to equal " << obl;
        auto otbl = static_cast<bytes> (bytes_type<endian::little, negativity::twos> (tbl));
        auto ttbl = static_cast<bytes> (obl);
        EXPECT_EQ (otbl, ttbl) << "expected " << otbl << " === " << ttbl;
    }

    // test conversions between negativity ones and twos.
    // we try to convert the second number into the first.
    TEST (HexidecimalTest, TestComplementTwosAndBC) {

        test_negativity_twos_to_BC ("0x", "0x");
        test_negativity_twos_to_BC ("0x00", "0x");
        test_negativity_BC_to_twos ("0x", "0x");
        test_negativity_BC_to_twos ("0x00", "0x");
        test_negativity_BC_to_twos ("0x80", "0x");
        test_negativity_BC_to_twos ("0x8000", "0x");

        test_negativity_twos_to_BC ("0x01", "0x01");
        test_negativity_twos_to_BC ("0x0001", "0x01");
        test_negativity_BC_to_twos ("0x01", "0x01");
        test_negativity_BC_to_twos ("0x0001", "0x01");

        test_negativity_twos_to_BC ("0x7f", "0x7f");
        test_negativity_twos_to_BC ("0x007f", "0x7f");
        test_negativity_BC_to_twos ("0x7f", "0x7f");
        test_negativity_BC_to_twos ("0x007f", "0x7f");

        test_negativity_twos_to_BC ("0xff", "0x81");
        test_negativity_twos_to_BC ("0xffff", "0x81");
        test_negativity_BC_to_twos ("0x81", "0xff");
        test_negativity_BC_to_twos ("0x8001", "0xff");

        test_negativity_twos_to_BC ("0x80", "0x8080");
        test_negativity_BC_to_twos ("0x8080", "0x80");

        test_negativity_twos_to_BC ("0x81", "0xff");
        test_negativity_BC_to_twos ("0xff", "0x81");

        test_negativity_twos_to_BC ("0xc0", "0xc0");
        test_negativity_BC_to_twos ("0xc0", "0xc0");

        test_negativity_twos_to_BC ("0x00ff", "0x00ff");
        test_negativity_twos_to_BC ("0x0080", "0x0080");

        test_negativity_twos_to_BC ("0xff80", "0x8080");
        test_negativity_BC_to_twos ("0x807f", "0x81");

        test_negativity_twos_to_BC ("0xff7f", "0x8081");
        test_negativity_BC_to_twos ("0x8081", "0xff7f");

        test_negativity_twos_to_BC ("0xfe7f", "0x8181");
        test_negativity_BC_to_twos ("0x8181", "0xfe7f");

    }
    
    template <negativity... > struct test_bit_shift;
    
    template <negativity n, negativity ... p> struct test_bit_shift<n, p...> : test_bit_shift<n>, test_bit_shift<p...> {
        test_bit_shift (
            string given, int bits, 
            string expected_left, 
            string expected_right) : 
        test_bit_shift<n> (given, bits, expected_left, expected_right),
        test_bit_shift<p...> (given, bits, expected_left, expected_right) {}
    };
    
    template <negativity n> struct test_bit_shift<n> {
        test_bit_shift (string given, byte bits, string expected_left, string expected_right) {

            auto l = hex<n> {expected_left};
            auto r = hex<n> {expected_right};
            auto g = hex<n> {given};
            auto x = g << bits;

            EXPECT_EQ (x, l);
            EXPECT_EQ (x >> bits, g) << "expected " << x << " >> " << uint64 (bits) << " => " << g;
            EXPECT_EQ (g >> bits, r) << "expected " << g << " >> " << uint64 (bits) << " => " << r;

            auto lbl = bytes_type<endian::little, n>::read (l);
            auto rbl = bytes_type<endian::little, n>::read (r);
            auto gbl = bytes_type<endian::little, n>::read (g);
            auto xbl = gbl << bits;
            EXPECT_EQ (xbl, lbl);
            EXPECT_EQ (xbl >> bits, gbl);
            EXPECT_EQ (gbl >> bits, rbl) << "expected " << std::hex << gbl << " >> " << uint64 (bits) << " => " << rbl;
            
            auto lbb = bytes_type<endian::big, n>::read (l);
            auto rbb = bytes_type<endian::big, n>::read (r);
            auto gbb = bytes_type<endian::big, n>::read (g);
            auto xbb = gbb << bits;
            EXPECT_EQ (xbb, lbb);
            EXPECT_EQ (xbb >> bits, gbb);
            EXPECT_EQ (gbb >> bits, rbb) << "expected " << std::hex << gbb << " >> " << uint64 (bits) << " => " << rbb;
            
        }
    };

    TEST (HexidecimalTest, TestHexidecimalBitShift) {

        test_bit_shift<negativity::nones, negativity::twos, negativity::BC> ("0x", 0, "0x", "0x");
        test_bit_shift<negativity::nones, negativity::twos, negativity::BC> ("0x", 1, "0x", "0x");
        test_bit_shift<negativity::nones, negativity::twos, negativity::BC> ("0x", 21, "0x", "0x");
        test_bit_shift<negativity::nones, negativity::twos, negativity::BC> ("0x01", 0, "0x01", "0x01");
        test_bit_shift<negativity::nones, negativity::twos, negativity::BC> ("0x01", 1, "0x02", "0x");
        test_bit_shift<negativity::nones, negativity::twos, negativity::BC> ("0x01", 2, "0x04", "0x");
        test_bit_shift<negativity::nones, negativity::twos, negativity::BC> ("0x01", 5, "0x0020", "0x");
        test_bit_shift<negativity::nones, negativity::twos, negativity::BC> ("0x01", 17, "0x020000", "0x00");
        test_bit_shift<negativity::nones, negativity::twos, negativity::BC> ("0x0300", 1, "0x0600", "0x0180");
        test_bit_shift<negativity::nones, negativity::twos, negativity::BC> ("0x0300", 2, "0x0c00", "0x00c0");
        test_bit_shift<negativity::nones, negativity::twos, negativity::BC> ("0x0300", 8, "0x030000", "0x03");
        test_bit_shift<negativity::nones, negativity::twos, negativity::BC> ("0x0300", 9, "0x060000", "0x01");
        test_bit_shift<negativity::nones, negativity::twos, negativity::BC> ("0x0300", 16, "0x03000000", "0x");
        test_bit_shift<negativity::nones, negativity::twos, negativity::BC> ("0x0300", 17, "0x06000000", "0x");
        test_bit_shift<negativity::nones, negativity::twos, negativity::BC> ("0x0300", 18, "0x0c000000", "0x");
        test_bit_shift<negativity::nones> ("0xff", 1, "0x01fe", "0x7f");
        test_bit_shift<negativity::twos> ("0xff", 1, "0xfe", "0xff");
        test_bit_shift<negativity::BC> ("0xff", 1, "0x80fe", "0xc0");

    }
    
    template <negativity... > struct test_plus;

    template <negativity n> struct test_plus_bytes;
    
    template <negativity n, negativity ... p> struct test_plus<n, p...> : test_plus<n>, test_plus<p...> {
        test_plus (string left, string right, string expected) :
        test_plus<n> (left, right, expected), test_plus<p...> (left, right, expected) {}
    };
    
    template <negativity n> struct test_plus<n> {
        test_plus (string left, string right, string expected) {
            auto l = hex<n>::read (left);
            auto r = hex<n>::read (right);
            auto e = hex<n>::read (expected);
            auto z = l + r;
            EXPECT_EQ (z, e) << "expected " << n << " " << std::hex << l << " + " << r << " = " << e
                << " but got " << z << std::endl;

            test_plus_bytes<n> (left, right, expected);
        }
    };

    template <negativity n> struct test_plus_bytes {
        test_plus_bytes (string left, string right, string expected) {

            auto lbl = bytes_type<endian::little, n>::read (left);
            auto rbl = bytes_type<endian::little, n>::read (right);
            auto ebl = bytes_type<endian::little, n>::read (expected);
            auto zbl = lbl + rbl;
            EXPECT_EQ (zbl, ebl) << "expected " << n << " " << std::hex << lbl << " + " << rbl << " = " << ebl
                << " but got " << zbl << std::endl;

            auto lbb = bytes_type<endian::big, n>::read (left);
            auto rbb = bytes_type<endian::big, n>::read (right);
            auto ebb = bytes_type<endian::big, n>::read (expected);
            auto zbb = lbb + rbb;
            EXPECT_EQ (zbb, ebb) << "expected " << n << " " << std::hex << lbb << " + " << rbb << " = " << ebb
                << " but got " << zbb << std::endl;

        }
    };

    TEST (HexidecimalTest, TestHexidecimalPlus) {

        test_plus<negativity::nones, negativity::twos, negativity::BC> ("0x", "0x", "0x");
        
        test_plus<negativity::nones, negativity::twos, negativity::BC> ("0", "0", "0");
        test_plus<negativity::nones, negativity::twos, negativity::BC> ("0", "1", "1");
        test_plus<negativity::nones, negativity::twos, negativity::BC> ("1", "1", "2");
        test_plus<negativity::nones, negativity::twos, negativity::BC>
            ("231938875480", "397027301409876", "397259240285356");
        test_plus<negativity::nones, negativity::twos, negativity::BC>
            ("23173210900987658780938875480", "39702733535456767789001409876", "62875944436444426569940285356");
        
        test_plus<negativity::twos, negativity::BC> ("-1", "0", "-1");
        test_plus<negativity::twos, negativity::BC> ("-1", "-1", "-2");
        test_plus<negativity::twos, negativity::BC> ("-1", "1", "0");
        test_plus<negativity::twos, negativity::BC> ("1", "-1", "0");
        test_plus<negativity::twos, negativity::BC> ("-2", "1", "-1");
        test_plus<negativity::twos, negativity::BC> ("2", "-1", "1");
        test_plus<negativity::twos, negativity::BC>
            ("-23173210900987658780938875480", "23173210900987658780938875480", "0");
        test_plus<negativity::twos, negativity::BC>
            ("23173210900987658780938875480", "-23173210900987658780938875480", "0");

        test_plus<negativity::BC> {"0x", "0x01", "0x01"};
        test_plus<negativity::BC> {"0x8000", "0x01", "0x01"};
        test_plus<negativity::BC> {"0x", "0x81", "0x81"};

    }
    
    template <negativity... > struct test_minus;
    
    template <negativity n, negativity ... p> struct test_minus<n, p...> : test_minus<n>, test_minus<p...> {
        test_minus (string left, string right, string expected) :
        test_minus<n> (left, right, expected), test_minus<p...> (left, right, expected) {}
    };

    template <negativity n> struct test_minus_bytes;
    
    template <negativity n> struct test_minus<n> {
        test_minus (string left, string right, string expected) {
            auto l = hex<n>::read (left);
            auto r = hex<n>::read (right);
            auto e = hex<n>::read (expected);
            auto x = l - r;
            EXPECT_EQ (x, e) << "expected " << n << " " << left << " - " << right << " -> " << expected << " but got " << x << std::endl;

            test_minus_bytes<n> (left, right, expected);
        }
    };

    template <negativity n> struct test_minus_bytes {
        test_minus_bytes (string left, string right, string expected) {

            auto lbl = bytes_type<endian::little, n>::read (left);
            auto rbl = bytes_type<endian::little, n>::read (right);
            auto ebl = bytes_type<endian::little, n>::read (expected);
            auto xbl = lbl - rbl;
            EXPECT_EQ (xbl, ebl)
                << "expected " << n << " " << std::hex << lbl << " - " << rbl << " -> " << ebl << " but got " << xbl << std::endl;

            auto lbb = bytes_type<endian::big, n>::read (left);
            auto rbb = bytes_type<endian::big, n>::read (right);
            auto ebb = bytes_type<endian::big, n>::read (expected);
            auto xbb = lbb - rbb;
            EXPECT_EQ (xbb, ebb)
                << "expected " << n << " " << std::hex << lbb << " - " << rbb << " -> " << ebb << " but got " << xbb << std::endl;}
    };

    // TODO remember to remove this when we do two's negativity. '
    template <> struct test_minus_bytes<negativity::BC> {
        test_minus_bytes (string left, string right, string expected) {}
    };

    TEST (HexidecimalTest, TestHexidecimalMinus) {

        test_minus<negativity::nones, negativity::twos, negativity::BC> ("0x", "0x", "0x");

        test_minus<negativity::nones, negativity::twos, negativity::BC> ("0x008000", "0x00ff", "0x7f01");
        test_minus<negativity::nones, negativity::twos, negativity::BC> ("0x008000", "0x00000000ff", "0x7f01");
        
        test_minus<negativity::nones, negativity::twos, negativity::BC> ("0", "0", "0");

        test_minus<negativity::nones, negativity::twos, negativity::BC> ("1", "0", "1");
        test_minus<negativity::nones, negativity::twos, negativity::BC> ("1", "1", "0");

        test_minus<negativity::nones, negativity::twos, negativity::BC> ("37", "12", "25");
        test_minus<negativity::nones, negativity::twos, negativity::BC>
            ("4502938948920982780930898389", "4502938948920982780930898389", "0");

        test_minus<negativity::twos, negativity::BC> ("12", "37", "-25");
        test_minus<negativity::nones> ("12", "37", "0");

        test_minus<negativity::nones> ("4502938948920982780930898389", "4502938948920982780930898390", "0");
        test_minus<negativity::twos, negativity::BC>
            ("4502938948920982780930898389", "4502938948920982780930898390", "-1");
        test_minus<negativity::twos, negativity::BC> ("1", "-1", "2");

        test_minus<negativity::twos> {"0x00ff", "0xff", "0x0100"};
        test_minus<negativity::twos> {"0xff", "0x00ff", "0xff00"};

        test_minus<negativity::nones, negativity::twos> {"0x80", "0x", "0x80"};
        test_minus<negativity::BC> {"0x80", "0x", "0x"};

        test_minus<negativity::BC> {"0x", "0x01", "0x81"};
        test_minus<negativity::BC> {"0x8000", "0x01", "0x81"};
    }
    
    template <negativity... > struct test_times;
    
    template <negativity n, negativity ... p> struct test_times<n, p...> : test_times<n>, test_times<p...> {
        test_times (string left, string right, string expected) :
        test_times<n> (left, right, expected), test_times<p...> (left, right, expected) {}
    };
    
    template <negativity n> struct test_times<n> {
        test_times (string left, string right, string expected) {
            auto l = hex<n>::read (left);
            auto r = hex<n>::read (right);
            auto e = hex<n>::read (expected);
            auto z = l * r;
            EXPECT_EQ (z, e) << "expected " << std::hex << l << " * " << r << " = " << e
                << " but got " << z << std::endl;
            
            auto lbl = bytes_type<endian::little, n>::read (left);
            auto rbl = bytes_type<endian::little, n>::read (right);
            auto ebl = bytes_type<endian::little, n>::read (expected);
            EXPECT_EQ (lbl * rbl, ebl);
            
            auto lbb = bytes_type<endian::big, n>::read (left);
            auto rbb = bytes_type<endian::big, n>::read (right);
            auto ebb = bytes_type<endian::big, n>::read (expected);
            EXPECT_EQ (lbb * rbb, ebb);
        }
    };

    template <> struct test_times<negativity::BC> {
        test_times (string left, string right, string expected) {}
    };

    TEST (HexidecimalTest, TestHexidecimalTimes) {
        
        test_times<negativity::nones, negativity::twos, negativity::BC> ("0", "0", "0");
        test_times<negativity::nones, negativity::twos, negativity::BC> ("0", "1", "0");
        test_times<negativity::nones, negativity::twos, negativity::BC> ("1", "1", "1");
        test_times<negativity::nones, negativity::twos, negativity::BC> ("1", "2", "2");
        test_times<negativity::nones, negativity::twos, negativity::BC> ("2", "1", "2");
        test_times<negativity::nones, negativity::twos, negativity::BC> ("2", "2", "4");
        test_times<negativity::nones, negativity::twos, negativity::BC> ("2", "5", "10");
        test_times<negativity::nones, negativity::twos, negativity::BC> ("5", "7", "35");
        test_times<negativity::nones, negativity::twos, negativity::BC> ("17", "19", "323");
        test_times<negativity::nones, negativity::twos, negativity::BC>
            ("23173210900987658780938875480", "39702733535456767789001409876",
            "920039817562855061210426612476533348173557348698006240480");
        
        test_times<negativity::twos, negativity::BC> ("-1", "0", "0");
        test_times<negativity::twos, negativity::BC> ("0", "-1", "0");
        test_times<negativity::twos, negativity::BC> ("-1", "1", "-1");
        test_times<negativity::twos, negativity::BC> ("1", "-1", "-1");
    }
    
    template <negativity... > struct test_divide;
    
    template <negativity n, negativity ... p> struct test_divide<n, p...> : test_divide<n>, test_divide<p...> {
        test_divide (string numerator, string denominator, string quotient, string remainder) :
        test_divide<n> (numerator, denominator, quotient, remainder),
        test_divide<p...> (numerator, denominator, quotient, remainder) {}
    };
    
    template <negativity n> struct test_divide<n> {
        test_divide (string numerator, string denominator, string quotient, string remainder) {

            auto num = hex<n>::read (numerator);
            auto den = hex<n>::read (denominator);
            auto qu = hex<n>::read (quotient);
            auto re = hex<n>::read (remainder);
            auto x = data::divide (num, math::nonzero {den});

            EXPECT_EQ (x.Quotient, qu);
            EXPECT_EQ (x.Remainder, re);
            
        }
    };

    TEST (HexidecimalTest, TestHexidecimalDivide) {
        test_divide<negativity::nones, negativity::twos, negativity::BC> ("0", "1", "0", "0");
        test_divide<negativity::nones, negativity::twos, negativity::BC> ("1", "1", "1", "0");
    }

    TEST (HexidecimalTest, TestHexidecimalOnes) {
        using hex_uint = data::hex::uint<hex_case::lower>;
        using hex_int2 = data::hex::int2<hex_case::lower>;
        
        EXPECT_EQ (string (hex_int2 {"0x"} + hex_uint {"0x"}), string {"0x"});
        EXPECT_EQ (string (hex_int2 {"0x"} * hex_uint {"0x"}), string {"0x"});
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
    struct get_number_size<math::number::bounded<x, r, s, byte>> {
        static const sizes value = sizes (s);
    };
    
    template <typename N>
    constexpr sizes number_size = get_number_size<N>::value;
    
    template <typename N> struct get_number_negativity {
        static const math::negativity value = math::negativity::twos;
    };
    
    template <endian::order r>
    struct get_number_negativity<math::number::Z_bytes<r, math::negativity::BC, byte>> {
        static const math::negativity value = math::negativity::BC;
    };
    
    template <endian::order r>
    struct get_number_negativity<math::number::N_bytes<r, byte>> {
        static const math::negativity value = math::negativity::nones;
    };
    
    template <>
    struct get_number_negativity<N> {
        static const math::negativity value = math::negativity::nones;
    };
    
    template <endian::order r, size_t s>
    struct get_number_negativity<math::uint<r, s, byte>> {
        static const math::negativity value = math::negativity::nones;
    };
    
    template <typename N>
    constexpr math::negativity number_negativity = get_number_negativity<N>::value;
    
    template <typename N> struct get_number_signed {
        static const bool value = true;
    };
    
    template <endian::order r>
    struct get_number_signed<math::number::N_bytes<r, byte>> {
        static const bool value = false;
    };
    
    template <bool x, endian::order r, size_t s>
    struct get_number_signed<math::number::bounded<x, r, s, byte>> {
        static const bool value = false;
    };
    
    template <typename N>
    constexpr bool is_signed = get_number_signed<N>::value;
    
    // the very first thing is to read in hexidecimal formats. 
    // we begin with a test that simply reads in different 
    // representations of zero in hexidecimal. 
    
    string hex_zero (size_t size) {
        std::stringstream ss;
        ss << "0x";
        for (int i = 0; i < size; i++) ss << "00";
        return ss.str ();
    }
    
    string hex_negative_zero (size_t size) {
        if (size == 0) size = 1;
        std::stringstream ss;
        ss << "0x80";
        for (int i = 0; i < size - 1; i++) ss << "00";
        return ss.str ();
    }
    
    template <typename N, math::negativity c = number_negativity<N>, sizes size = number_size<N>>
    struct test_hex_read_zero {
        test_hex_read_zero (list<size_t> xx) {
            for (size_t i : xx) {
                if (i == size) {
                    N zero = N::read (hex_zero (i));
                    EXPECT_EQ (N {}, zero);
                } else {
                    EXPECT_THROW (N::read (hex_zero (i)), exception);
                }
            }
        }
    };
    
    template <typename N, math::negativity c> struct test_hex_read_zero<N, c, any> {
        test_hex_read_zero (list<size_t> xx) {
            list<N> zeros;
            for (size_t i : xx) {
                
                N zero = N (hex_zero (i));
                EXPECT_EQ (N {}, zero);
                EXPECT_GE (N {}, zero);
                EXPECT_LE (N {}, zero);

                for (const N &n : zeros) {
                    EXPECT_EQ (n, zero);
                    EXPECT_LE (n, zero);
                    EXPECT_EQ (n, zero);
                }

                zeros = zeros << zero;
            }
        }
    };
    
    template <typename N> struct test_hex_read_zero<N, math::negativity::BC, any> {
        test_hex_read_zero (list<size_t> xx) {
            list<N> zeros;
            for (size_t i : xx) {

                N zero = N::read (hex_zero (i));
                N nzero = N::read (hex_zero (i));

                EXPECT_EQ (N {}, zero);
                EXPECT_GE (N {}, zero);
                EXPECT_LE (N {}, zero);
                EXPECT_EQ (N {}, nzero);
                EXPECT_GE (N {}, nzero);
                EXPECT_LE (N {}, nzero);

                for (const N &n : zeros) {
                    EXPECT_EQ (n, zero);
                    EXPECT_LE (n, zero);
                    EXPECT_EQ (n, zero);
                    EXPECT_EQ (n, nzero);
                    EXPECT_LE (n, nzero);
                    EXPECT_EQ (n, nzero);
                }

                zeros = zeros << zero << nzero;
            }
        }
    };

    TEST (HexidecimalTest, TestHexReadZero) {

        list<size_t> sizes {0, 1, 2, 5, 9, 10, 11, 20};

        test_hex_read_zero<N_bytes_little> {sizes};
        test_hex_read_zero<N_bytes_big> {sizes};
        test_hex_read_zero<Z_bytes_little> {sizes};
        test_hex_read_zero<Z_bytes_big> {sizes};
        test_hex_read_zero<Z_bytes_BC_little> {sizes};
        test_hex_read_zero<Z_bytes_BC_big> {sizes};
        test_hex_read_zero<N> {sizes};
        test_hex_read_zero<Z> {sizes};
        test_hex_read_zero<uint_little<9>> {sizes};
        test_hex_read_zero<int_little<9>> {sizes};
        test_hex_read_zero<uint_big<9>> {sizes};
        test_hex_read_zero<int_big<9>> {sizes};
        test_hex_read_zero<uint_little<10>> {sizes};
        test_hex_read_zero<int_little<10>> {sizes};
        test_hex_read_zero<uint_big<10>> {sizes};
        test_hex_read_zero<int_big<10>> {sizes};
        test_hex_read_zero<uint_little<11>> {sizes};
        test_hex_read_zero<int_little<11>> {sizes};
        test_hex_read_zero<uint_big<11>> {sizes};
        test_hex_read_zero<int_big<11>> {sizes};
        test_hex_read_zero<uint_little<20>> {sizes};
        test_hex_read_zero<int_little<20>> {sizes};
        test_hex_read_zero<uint_big<20>> {sizes};
        test_hex_read_zero<int_big<20>> {sizes};
    }
    
    // after this point, we don't have to test importing numbers of the wrong sizes. 
    
    template <typename N, math::negativity c = number_negativity<N>, bool u = is_signed<N>> struct test_signed_stuff {
        test_signed_stuff (const N &, const encoding::hexidecimal::integer<c, hex_case::lower> &) {}
    };
    
    template <typename N, math::negativity zz> struct test_signed_stuff<N, zz, true> {
        test_signed_stuff (const N &n, const encoding::hexidecimal::integer<zz, hex_case::lower> &h) {
            using H = encoding::hexidecimal::integer<zz, hex_case::lower>;

            auto hmm = math::number::decrement<H> {} (h);
            auto nmm = math::number::decrement<N> {} (n);

            EXPECT_TRUE (math::number::is_minimal (hmm));
            EXPECT_TRUE (math::number::is_minimal (nmm));
            
            auto hnm = N::read (hmm);
            EXPECT_EQ (hnm, nmm) << std::hex << "expected " << hnm << " == " << nmm;
            auto hhmm = encoding::hexidecimal::integer<zz, hex_case::lower>
                {encoding::hexidecimal::write<hex_case::lower> (nmm)};
            EXPECT_EQ (hmm, hhmm) << std::hex << "expected " << hmm << " == " << hhmm;
            
            auto hpp = increment (hmm);
            auto npp = increment (nmm);

            EXPECT_EQ (n, npp) << "expected " << std::hex << n << " == " << npp;
            EXPECT_EQ (h, hpp) << "expected " << std::hex << h << " == " << hpp;
            
            auto nh = -h;
            auto nn = -n;

            EXPECT_TRUE (math::number::is_minimal (nh));
            EXPECT_TRUE (math::number::is_minimal (nn));
            
            EXPECT_EQ (N::read (nh), nn);
            auto nnh = encoding::hexidecimal::integer<zz, hex_case::lower>
                {encoding::hexidecimal::write<hex_case::lower> (nn)};
            EXPECT_EQ (nh, nnh);
        }
    };
    
    // we should find that the hex strings are the same when read into a number 
    // and written back. 
    template <typename N, math::negativity zz> void test_hex_read_and_write_bytes (list<string> cases) {
        list<std::pair<N, encoding::hexidecimal::integer<number_negativity<N>, hex_case::lower>>> numbers;
        for (const string &x : cases) {
            using H = encoding::hexidecimal::integer<number_negativity<N>, hex_case::lower>;

            H h {x};
            N n = N::read (x);
            EXPECT_EQ (data::sign (h), data::sign (n));
            
            // some types of numbers don't work this way, so we should
            // figure out something to do about it. 
            EXPECT_EQ (math::number::is_minimal (h), math::number::is_minimal (n));
            EXPECT_EQ (x, encoding::hexidecimal::write<hex_case::lower> (n));
            
            auto hpp = increment (h);
            auto npp = increment (n);

            EXPECT_TRUE (math::number::is_minimal (H (hpp)));
            EXPECT_TRUE (math::number::is_minimal (N (npp)));
            
            EXPECT_EQ (N::read (H (hpp)), N (npp)) << "expected " << hpp << " == " << std::hex << npp;
            auto hhpp = encoding::hexidecimal::integer<number_negativity<N>, hex_case::lower>
                {encoding::hexidecimal::write<hex_case::lower> (N (npp))};
            EXPECT_EQ (hpp, hhpp);
            
            test_signed_stuff<N, zz> {n, h};

            for (auto &p : numbers) {
                EXPECT_EQ (n <=> p.first, h <=> p.second);

                auto no = n | p.first;
                auto ho = h | p.second;

                EXPECT_TRUE (math::number::is_minimal (ho));
                EXPECT_TRUE (math::number::is_minimal (no));

                EXPECT_EQ (N::read (ho), no);
                auto hho = encoding::hexidecimal::integer<number_negativity<N>, hex_case::lower>
                    {encoding::hexidecimal::write<hex_case::lower> (no)};
                EXPECT_EQ (ho, hho);
                
                auto na = n & p.first;
                auto ha = h & p.second;

                EXPECT_TRUE (math::number::is_minimal (ha));
                EXPECT_TRUE (math::number::is_minimal (na));
            
                EXPECT_EQ (N::read (ha), na);
                auto hha = encoding::hexidecimal::integer<number_negativity<N>, hex_case::lower>
                    {encoding::hexidecimal::write<hex_case::lower> (na)};
                EXPECT_EQ (ha, hha);

                auto np = n + p.first;
                auto hp = h + p.second;

                EXPECT_TRUE (math::number::is_minimal (hp));
                EXPECT_TRUE (math::number::is_minimal (np));

                EXPECT_EQ (N::read (hp), np);
                auto hhp = encoding::hexidecimal::integer<number_negativity<N>, hex_case::lower>
                    {encoding::hexidecimal::write<hex_case::lower> (np)};
                EXPECT_EQ (hp, hhp);

                auto nm = n - p.first;
                auto hm = h - p.second;

                EXPECT_TRUE (math::number::is_minimal (hm));
                EXPECT_TRUE (math::number::is_minimal (nm));

                EXPECT_EQ (N::read (hm), nm);
                auto hhm = encoding::hexidecimal::integer<number_negativity<N>, hex_case::lower>
                    {encoding::hexidecimal::write<hex_case::lower> (nm)};
                EXPECT_EQ (hm, hhm);

                auto nt = n * p.first;
                auto ht = h * p.second;
            
                EXPECT_TRUE (math::number::is_minimal (ht));
                EXPECT_TRUE (math::number::is_minimal (nt));
            
                EXPECT_EQ (N::read (ht), nt);
                auto hht = encoding::hexidecimal::integer<number_negativity<N>, hex_case::lower>
                    {encoding::hexidecimal::write<hex_case::lower> (nt)};
                EXPECT_EQ (ht, hht);
                
            }
            
            numbers <<= std::pair {n, h};
        }
    }

    TEST (HexidecimalTest, TestHexReadAndWriteBytes) {
        list<string> cases {
            "0x", "0x00", "0x0000", "0xc0", "0xff", "0x00ff", "0xffff",
            "0x80", "0x8000", "0x0080", "0x01", "0x0001", "0x81", "0x8001"};
        test_hex_read_and_write_bytes<N_bytes_little, math::negativity::nones> (cases);
        test_hex_read_and_write_bytes<N_bytes_big, math::negativity::nones> (cases);
        test_hex_read_and_write_bytes<Z_bytes_little, math::negativity::twos> (cases);
        test_hex_read_and_write_bytes<Z_bytes_big, math::negativity::twos> (cases);
        test_hex_read_and_write_bytes<Z_bytes_BC_little, math::negativity::BC> (cases);
        test_hex_read_and_write_bytes<Z_bytes_BC_big, math::negativity::BC> (cases);
    }

}
