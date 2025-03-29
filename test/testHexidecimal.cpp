// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include <data/io/wait_for_enter.hpp>
#include "gtest/gtest.h"

namespace data::math::number {
    
    template <complement c> using hex = encoding::hexidecimal::integer<c, hex_case::lower>;
    
    template <endian::order r, complement c> struct get_bytes_type {
        using value = Z_bytes<r, c, byte>;
    };
    
    template <endian::order r> struct get_bytes_type<r, complement::nones> {
        using value = N_bytes<r, byte>;
    };
    
    template <endian::order r, complement c> using bytes_type = get_bytes_type<r, c>::value;
    
    template <endian::order r, complement c, size_t size> struct get_fixed_type;
    
    template <endian::order r, size_t size> struct get_fixed_type<r, complement::nones, size> {
        using value = uint<r, size, byte>;
    };
    
    template <endian::order r, size_t size> struct get_fixed_type<r, complement::ones, size> {
        using value = sint<r, size, byte>;
    };
    
    template <endian::order r, size_t size> struct get_fixed_type<r, complement::BC, size> {
        using value = void;
    };
    
    template <endian::order r, complement c, size_t size> using fixed_type = get_fixed_type<r, c, size>::value;
    
    template <complement c> struct hex_test_case {
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

        hex_test_case<complement::nones> ("0x", "0x", zero);
        hex_test_case<complement::ones> ("0x", "0x", zero);
        hex_test_case<complement::BC> ("0x", "0x", zero);
        
        hex_test_case<complement::nones> ("0x00", "0x", zero);
        hex_test_case<complement::ones> ("0x00", "0x", zero);
        hex_test_case<complement::BC> ("0x00", "0x", zero);
        
        hex_test_case<complement::nones> ("0x0000", "0x", zero);
        hex_test_case<complement::ones> ("0x0000", "0x", zero);
        hex_test_case<complement::BC> ("0x0000", "0x", zero);
        
        hex_test_case<complement::nones> ("0x80", "0x80", positive);
        hex_test_case<complement::ones> ("0x80", "0x80", negative);
        hex_test_case<complement::BC> ("0x80", "0x", zero);

        hex_test_case<complement::nones> ("0x8000", "0x8000", positive);
        hex_test_case<complement::ones> ("0x8000", "0x8000", negative);
        hex_test_case<complement::BC> ("0x8000", "0x", zero);
        
        hex_test_case<complement::nones> ("0x01", "0x01", positive);
        hex_test_case<complement::ones> ("0x01", "0x01", positive);
        hex_test_case<complement::BC> ("0x01", "0x01", positive);
        
        hex_test_case<complement::nones> ("0x0001", "0x01", positive);
        hex_test_case<complement::ones> ("0x0001", "0x01", positive);
        hex_test_case<complement::BC> ("0x0001", "0x01", positive);

        hex_test_case<complement::nones> ("0xff", "0xff", positive);
        hex_test_case<complement::ones> ("0xff", "0xff", negative);
        hex_test_case<complement::BC> ("0xff", "0xff", negative);

        hex_test_case<complement::nones> ("0xffff", "0xffff", positive);
        hex_test_case<complement::ones> ("0xffff", "0xff", negative);
        hex_test_case<complement::BC> ("0xffff", "0xffff", negative);

        hex_test_case<complement::nones> ("0x0080", "0x80", positive);
        hex_test_case<complement::ones> ("0x0080", "0x0080", positive);
        hex_test_case<complement::BC> ("0x0080", "0x0080", positive);
        
        hex_test_case<complement::nones> ("0x81", "0x81", positive);
        hex_test_case<complement::ones> ("0x81", "0x81", negative);
        hex_test_case<complement::BC> ("0x81", "0x81", negative);
        
        hex_test_case<complement::nones> ("0x8001", "0x8001", positive);
        hex_test_case<complement::ones> ("0x8001", "0x8001", negative);
        hex_test_case<complement::BC> ("0x8001", "0x81", negative);
        
        hex_test_case<complement::nones> ("0x00ff", "0xff", positive);
        hex_test_case<complement::ones> ("0x00ff", "0x00ff", positive);
        hex_test_case<complement::BC> ("0x00ff", "0x00ff", positive);
        
        hex_test_case<complement::nones> ("0x0200", "0x0200", positive);
        hex_test_case<complement::ones> ("0x0200", "0x0200", positive);
        hex_test_case<complement::BC> ("0x0200", "0x0200", positive);
        
        hex_test_case<complement::nones> ("0x2000", "0x2000", positive);
        hex_test_case<complement::ones> ("0x2000", "0x2000", positive);
        hex_test_case<complement::BC> ("0x2000", "0x2000", positive);

        hex_test_case<complement::nones> ("0x010000", "0x010000", positive);
        hex_test_case<complement::ones> ("0x010000", "0x010000", positive);
        hex_test_case<complement::BC> ("0x010000", "0x010000", positive);
    }
    
    template <complement... > struct test_bit_and;
    template <complement... > struct test_bit_or;
    
    template <complement n, complement ... p> struct test_bit_and<n, p...> : test_bit_and<n>, test_bit_and<p...> {
        test_bit_and (string left, string right, string expected) :
        test_bit_and<n> (left, right, expected), test_bit_and<p...> (left, right, expected) {}
    };
    
    template <complement n, complement ... p> struct test_bit_or<n, p...> : test_bit_or<n>, test_bit_or<p...> {
        test_bit_or (string left, string right, string expected) :
        test_bit_or<n> (left, right, expected), test_bit_or<p...> (left, right, expected) {}
    };
    
    template <complement n, size_t size> struct test_bit_and_fixed;
    template <complement n, size_t size> struct test_bit_or_fixed;
    
    template <size_t size> struct test_bit_and_fixed<complement::nones, size> {
        test_bit_and_fixed (hex<complement::nones> left, hex<complement::nones> right, hex<complement::nones> expected) {
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
    
    template <size_t size> struct test_bit_or_fixed<complement::nones, size> {
        test_bit_or_fixed (hex<complement::nones> left, hex<complement::nones> right, hex<complement::nones> expected) {
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
    
    template <size_t size> struct test_bit_and_fixed<complement::ones, size> {
        test_bit_and_fixed (hex<complement::ones> left, hex<complement::ones> right, hex<complement::ones> expected) {
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
    
    template <size_t size> struct test_bit_or_fixed<complement::ones, size> {
        test_bit_or_fixed (hex<complement::ones> left, hex<complement::ones> right, hex<complement::ones> expected) {
            
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
    
    template <size_t size> struct test_bit_and_fixed<complement::BC, size> {
        test_bit_and_fixed (hex<complement::BC> left, hex<complement::BC> right, hex<complement::BC> expected) {}
    };
    
    template <size_t size> struct test_bit_or_fixed<complement::BC, size> {
        test_bit_or_fixed (hex<complement::BC> left, hex<complement::BC> right, hex<complement::BC> expected) {}
    };
    
    template <complement n> struct test_bit_and<n> {
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
    
    template <complement n> struct test_bit_or<n> {
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

        test_bit_and<complement::nones, complement::ones, complement::BC> {"0x", "0x", "0x"};
        test_bit_and<complement::nones, complement::ones, complement::BC> {"0x01", "0x01", "0x01"};
        test_bit_and<complement::nones, complement::ones, complement::BC> {"0x", "0x01", "0x"};
        test_bit_and<complement::nones, complement::ones, complement::BC> {"0xff", "0xff", "0xff"};
        test_bit_and<complement::nones, complement::ones, complement::BC> {"0x", "0xff", "0x"};

    }
    
    TEST (HexidecimalTest, TestHexBitOr) {

        test_bit_or<complement::nones, complement::ones, complement::BC> {"0x", "0x", "0x"};
        test_bit_or<complement::nones, complement::ones, complement::BC> {"0x01", "0x01", "0x01"};
        test_bit_or<complement::nones, complement::ones, complement::BC> {"0x01", "0x", "0x01"};
        test_bit_or<complement::nones, complement::ones, complement::BC> {"0xff", "0xff", "0xff"};
        test_bit_or<complement::nones, complement::ones, complement::BC> {"0x", "0xff", "0xff"};

    }
    
    template <complement... n> struct test_bit_negate;
    
    template <complement n, complement ... p> struct test_bit_negate<n, p...> : test_bit_negate<n>, test_bit_negate<p...> {
        test_bit_negate (string given, string expected) :
        test_bit_negate<n> (given, expected), test_bit_negate<p...> (given, expected) {}
    };
    
    template <complement n, size_t size> struct test_bit_negate_fixed;
    
    template <size_t size> struct test_bit_negate_fixed<complement::ones, size> {
        test_bit_negate_fixed (string given, string expected) {
            auto g = extend (hex<complement::ones> {given}, size * 2 + 2);
            auto e = extend (hex<complement::ones> {expected}, size * 2 + 2);
            
            auto gl = sint<endian::little, size, byte>::read (g);
            auto el = sint<endian::little, size, byte>::read (e);
            EXPECT_EQ (~gl, el);
            
            auto gb = sint<endian::big, size, byte>::read (g);
            auto eb = sint<endian::big, size, byte>::read (e);
            EXPECT_EQ (~gb, eb);
        }
    };
    
    template <size_t size> struct test_bit_negate_fixed<complement::nones, size> {
        test_bit_negate_fixed (string given, string expected) {
            auto g = extend (hex<complement::ones> {given}, size * 2 + 2);
            auto e = extend (hex<complement::ones> {expected}, size * 2 + 2);
            
            auto gl = uint<endian::little, size, byte>::read (g);
            auto el = uint<endian::little, size, byte>::read (e);
            EXPECT_EQ (~gl, el);
            
            auto gb = uint<endian::big, size, byte>::read (g);
            auto eb = uint<endian::big, size, byte>::read (e);
            EXPECT_EQ (~gb, eb);
        }
    };
    
    template <> struct test_bit_negate<complement::ones> {
        test_bit_negate (string given, string expected) {
            
            auto g = hex<complement::ones> {given};
            auto e = hex<complement::ones> {expected};
            EXPECT_EQ (~g, e);
            
            auto gbl = bytes_type<endian::little, complement::ones>::read (given);
            auto ebl = bytes_type<endian::little, complement::ones>::read (expected);
            EXPECT_EQ (~gbl, ebl);
            
            auto gbb = bytes_type<endian::big, complement::ones>::read (given);
            auto ebb = bytes_type<endian::big, complement::ones>::read (expected);
            EXPECT_EQ (~gbb, ebb);
            
            test_bit_negate_fixed<complement::ones, 10> (g, e);
            test_bit_negate_fixed<complement::ones, 11> (g, e);
            test_bit_negate_fixed<complement::ones, 12> (g, e);
            test_bit_negate_fixed<complement::ones, 13> (g, e);
        }
    };
    
    template <> struct test_bit_negate<complement::nones> {
        test_bit_negate (string given, string expected) {
            
            auto g = hex<complement::nones> {given};
            auto e = hex<complement::nones> {expected};
            
            test_bit_negate_fixed<complement::nones, 10> (g, e);
            test_bit_negate_fixed<complement::nones, 11> (g, e);
            test_bit_negate_fixed<complement::nones, 12> (g, e);
            test_bit_negate_fixed<complement::nones, 13> (g, e);
        }
    };
    
    template <complement... n> struct test_bit_xor;
    
    template <complement n, complement ... p> struct test_bit_xor<n, p...> : test_bit_xor<n>, test_bit_xor<p...> {
        test_bit_xor (string left, string right, string expected) :
        test_bit_xor<n> (left, right, expected), test_bit_xor<p...> (left, right, expected) {}
    };
    
    template <complement n, size_t size> struct test_bit_xor_fixed;
    
    template <size_t size> struct test_bit_xor_fixed<complement::nones, size> {
        test_bit_xor_fixed (string left, string right, string expected) {
            auto l = extend (hex<complement::ones> {left}, size * 2 + 2);
            auto r = extend (hex<complement::ones> {right}, size * 2 + 2);
            auto e = extend (hex<complement::ones> {expected}, size * 2 + 2);
            
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
    
    template <size_t size> struct test_bit_xor_fixed<complement::ones, size> {
        test_bit_xor_fixed (string left, string right, string expected) {

            auto l = extend (hex<complement::ones> {left}, size * 2 + 2);
            auto r = extend (hex<complement::ones> {right}, size * 2 + 2);
            auto e = extend (hex<complement::ones> {expected}, size * 2 + 2);
            
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

    template <> struct test_bit_xor<complement::ones> {
        test_bit_xor (string left, string right, string expected) {

            auto l = hex<complement::ones> {left};
            auto r = hex<complement::ones> {right};
            auto e = hex<complement::ones> {expected};
            EXPECT_EQ (l ^ r, e);
            
            auto lbl = bytes_type<endian::little, complement::ones>::read (left);
            auto rbl = bytes_type<endian::little, complement::ones>::read (right);
            auto ebl = bytes_type<endian::little, complement::ones>::read (expected);
            EXPECT_EQ (lbl ^ rbl, ebl);
            
            auto lbb = bytes_type<endian::big, complement::ones>::read (left);
            auto rbb = bytes_type<endian::big, complement::ones>::read (right);
            auto ebb = bytes_type<endian::big, complement::ones>::read (expected);
            EXPECT_EQ (lbb ^ rbb, ebb);
            
            test_bit_xor_fixed<complement::ones, 10> (l, r, e);
            test_bit_xor_fixed<complement::ones, 11> (l, r, e);
            test_bit_xor_fixed<complement::ones, 12> (l, r, e);
            test_bit_xor_fixed<complement::ones, 13> (l, r, e);

        }
    };
    
    template <> struct test_bit_xor<complement::nones> {
        test_bit_xor (string left, string right, string expected) {

            auto l = hex<complement::nones> {left};
            auto r = hex<complement::nones> {right};
            auto e = hex<complement::nones> {expected};
            
            test_bit_xor_fixed<complement::nones, 10> (l, r, e);
            test_bit_xor_fixed<complement::nones, 11> (l, r, e);
            test_bit_xor_fixed<complement::nones, 12> (l, r, e);
            test_bit_xor_fixed<complement::nones, 13> (l, r, e);

        }
    };

    TEST (HexidecimalTest, TestHexBitXor) {

        test_bit_negate<complement::nones, complement::ones> {"0x", "0xff"};
        test_bit_xor<complement::nones, complement::ones> {"0x", "0x", "0x"};
        test_bit_xor<complement::nones, complement::ones> {"0xff", "0xff", "0x"};
        test_bit_xor<complement::nones, complement::ones> {"0x", "0xff", "0xff"};

    }
    
    template <complement n, size_t size> struct test_increment_fixed;
    
    template <complement... > struct test_increment;

    template <complement n> struct test_increment_bytes;
    
    template <complement n> struct test_increment<n> {
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
    
    template <complement n, complement ... p> struct test_increment<n, p...> : test_increment<n>, test_increment<p...> {
        test_increment (string given, string expected) :
        test_increment<n> (given, expected), test_increment<p...> (given, expected) {}
    };

    template <complement n> struct test_increment_bytes {
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
    
    template <size_t size> struct test_increment_fixed<complement::nones, size> {
        test_increment_fixed (hex<complement::nones> given, hex<complement::nones> expected) {

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
    
    template <size_t size> struct test_increment_fixed<complement::ones, size> {
        test_increment_fixed (hex<complement::ones> given, hex<complement::ones> expected) {
            
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
    
    template <size_t size> struct test_increment_fixed<complement::BC, size> {
        test_increment_fixed (hex<complement::BC> given, hex<complement::BC> expected) {}
    };

    TEST (HexidecimalTest, TestHexidecimalIncrement) {

        test_increment<complement::nones, complement::ones, complement::BC> {"0x", "0x01"};
        test_increment<complement::nones, complement::ones, complement::BC> {"0x00", "0x01"};
        test_increment<complement::nones, complement::ones, complement::BC> {"0x0000", "0x01"};
        test_increment<complement::nones, complement::ones, complement::BC> {"0x01", "0x02"};
        test_increment<complement::nones> {"0xff", "0x0100"};
        test_increment<complement::nones> {"0x7f", "0x80"};
        test_increment<complement::nones> {"0xffff", "0x010000"};
        test_increment<complement::nones, complement::ones, complement::BC> {"0x00ff", "0x0100"};
        test_increment<complement::ones> {"0xff", "0x"};
        test_increment<complement::ones> {"0xffff", "0x"};
        test_increment<complement::ones> {"0x7f", "0x0080"};
        test_increment<complement::ones> {"0x80", "0x81"};
        test_increment<complement::ones> {"0xfe", "0xff"};
        test_increment<complement::BC> {"0x80", "0x01"};
        test_increment<complement::BC> {"0x81", "0x"};
        test_increment<complement::BC> {"0x82", "0x81"};
        test_increment<complement::BC> {"0x7f", "0x0080"};
        test_increment<complement::BC> {"0x8080", "0xff"};

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

    template <endian::order r, complement n>
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

    template <complement n> struct test_negation {
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
    
    template <> struct test_negation<complement::nones> {
        test_negation (string initial, string expected) {}
    };

    TEST (HexidecimalTest, TestNegation) {
        
        test_negation<complement::BC> {"0x", "0x"};
        test_negation<complement::BC> {"0x00", "0x"};
        test_negation<complement::BC> {"0x80", "0x"};
        test_negation<complement::BC> {"0x81", "0x01"};
        test_negation<complement::BC> {"0x01", "0x81"};
        test_negation<complement::BC> {"0x8001", "0x01"};
        test_negation<complement::BC> {"0x0001", "0x81"};
        
        test_negation<complement::ones> {"0x", "0x"};
        test_negation<complement::ones> {"0x00", "0x"};
        test_negation<complement::ones> {"0x80", "0x0080"};
        test_negation<complement::ones> {"0xff", "0x01"};
        test_negation<complement::ones> {"0xffff", "0x01"};
        test_negation<complement::ones> {"0x01", "0xff"};
        test_negation<complement::ones> {"0x02", "0xfe"};
        test_negation<complement::ones> {"0xfe", "0x02"};
        test_negation<complement::ones> {"0x0100", "0xff00"};

    }
    
    template <complement c, size_t size> struct fixed_test_comparison;
    
    template <size_t size> struct fixed_test_comparison<complement::nones, size> {
        fixed_test_comparison (string i, string j, std::strong_ordering expected) {
            auto cmpb = uint<endian::big, size, byte>::read (extend (hex<complement::nones> (i), size * 2 + 2)) <=>
                uint<endian::big, size, byte>::read (extend (hex<complement::nones> (j), size * 2 + 2));
            EXPECT_EQ (cmpb, expected);
            auto cmpl = uint<endian::little, size, byte>::read (extend (hex<complement::nones> (i), size * 2 + 2)) <=>
                uint<endian::little, size, byte>::read (extend (hex<complement::nones> (j), size * 2 + 2));
            EXPECT_EQ (cmpl, expected);
        }
    };
    
    template <size_t size> struct fixed_test_comparison<complement::ones, size> {
        fixed_test_comparison (string i, string j, std::strong_ordering expected) {
            
            string exi = extend (hex<complement::ones> (i), size * 2 + 2);
            string exj = extend (hex<complement::ones> (j), size * 2 + 2);

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
    
    // we do nothing here because we don't have fixed size 2s complement numbers.
    template <size_t size> struct fixed_test_comparison<complement::BC, size> {
        fixed_test_comparison (string a, string b, std::strong_ordering expected) {}
    };
    
    template <complement c> void test_comparison (const std::vector<string> &values, const std::vector<std::vector<int>> &ordering) {
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
        
        test_comparison<complement::nones> (values, o0);
        test_comparison<complement::ones> (values, o1);
        test_comparison<complement::BC> (values, o2);
        
    }

    // the number with complement one should be converted to the
    // number Bitcoin format.
    void test_complement_ones_to_BC (string ones, string bc) {

        auto o = hex<complement::ones> {ones};
        auto b = hex<complement::BC> {bc};

        // twos converted to ones should be equal (but not identical);
        EXPECT_EQ (o, b) << "expected " << o << " to equal " << b << " ; converted to ones: " << hex<complement::ones> (b);

        auto ob = static_cast <std::string> (hex<complement::BC> (o));
        auto bb = static_cast <std::string> (b);
        EXPECT_EQ (ob, bb) << "expected " << ob << " to equal " << bb;

        auto obb = bytes_type<endian::big, complement::ones>::read (o);
        auto tbb = bytes_type<endian::big, complement::BC>::read (b);

        EXPECT_EQ (obb, tbb) << "expected " << std::hex << obb << " to equal " << tbb;
        EXPECT_EQ (static_cast<bytes> (bytes_type<endian::big, complement::BC> (obb)), static_cast<bytes> (tbb));

        auto obl = bytes_type<endian::little, complement::ones>::read (o);
        auto tbl = bytes_type<endian::little, complement::BC>::read (b);

        EXPECT_EQ (obl, tbl) << "expected " << std::hex << obl << " to equal " << tbl;
        EXPECT_EQ (static_cast<bytes> (bytes_type<endian::little, complement::BC> (obl)), static_cast<bytes> (tbl));

    }

    // the number with complement twos should be converted to the
    // number in complement one.
    void test_complement_BC_to_ones (string bc, string ones) {

        auto b = hex<complement::BC> {bc};
        auto o = hex<complement::ones> {ones};

        EXPECT_EQ (b, o) << "expected " << b << " to equal " << o;

        auto bo = static_cast <std::string> (hex<complement::ones> (b));
        auto oo = static_cast <std::string> (o);
        EXPECT_EQ (bo, oo) << "expected " << bo << " === " << oo;

        auto bbb = bytes_type<endian::big, complement::BC>::read (b);
        auto obb = bytes_type<endian::big, complement::ones>::read (o);

        EXPECT_EQ (bbb, obb) << "expected " << std::hex << bbb << " to equal " << obb;

        auto otbb = static_cast<bytes> (bytes_type<endian::big, complement::ones> (bbb));
        auto ttbb = static_cast<bytes> (obb);
        EXPECT_EQ (otbb, ttbb) << "expected " << otbb << " === " << ttbb;

        auto tbl = bytes_type<endian::little, complement::BC>::read (b);
        auto obl = bytes_type<endian::little, complement::ones>::read (o);

        EXPECT_EQ (tbl, obl) << "expected " << std::hex << tbl << " to equal " << obl;
        auto otbl = static_cast<bytes> (bytes_type<endian::little, complement::ones> (tbl));
        auto ttbl = static_cast<bytes> (obl);
        EXPECT_EQ (otbl, ttbl) << "expected " << otbl << " === " << ttbl;
    }

    // test conversions between complement ones and twos.
    // we try to convert the second number into the first.
    TEST (HexidecimalTest, TestComplementOnesAndTwos) {

        test_complement_ones_to_BC ("0x", "0x");
        test_complement_ones_to_BC ("0x00", "0x");
        test_complement_BC_to_ones ("0x", "0x");
        test_complement_BC_to_ones ("0x00", "0x");
        test_complement_BC_to_ones ("0x80", "0x");
        test_complement_BC_to_ones ("0x8000", "0x");

        test_complement_ones_to_BC ("0x01", "0x01");
        test_complement_ones_to_BC ("0x0001", "0x01");
        test_complement_BC_to_ones ("0x01", "0x01");
        test_complement_BC_to_ones ("0x0001", "0x01");

        test_complement_ones_to_BC ("0x7f", "0x7f");
        test_complement_ones_to_BC ("0x007f", "0x7f");
        test_complement_BC_to_ones ("0x7f", "0x7f");
        test_complement_BC_to_ones ("0x007f", "0x7f");

        test_complement_ones_to_BC ("0xff", "0x81");
        test_complement_ones_to_BC ("0xffff", "0x81");
        test_complement_BC_to_ones ("0x81", "0xff");
        test_complement_BC_to_ones ("0x8001", "0xff");

        test_complement_ones_to_BC ("0x80", "0x8080");
        test_complement_BC_to_ones ("0x8080", "0x80");

        test_complement_ones_to_BC ("0x81", "0xff");
        test_complement_BC_to_ones ("0xff", "0x81");

        test_complement_ones_to_BC ("0xc0", "0xc0");
        test_complement_BC_to_ones ("0xc0", "0xc0");

        test_complement_ones_to_BC ("0x00ff", "0x00ff");
        test_complement_ones_to_BC ("0x0080", "0x0080");

        test_complement_ones_to_BC ("0xff80", "0x8080");
        test_complement_BC_to_ones ("0x807f", "0x81");

        test_complement_ones_to_BC ("0xff7f", "0x8081");
        test_complement_BC_to_ones ("0x8081", "0xff7f");

        test_complement_ones_to_BC ("0xfe7f", "0x8181");
        test_complement_BC_to_ones ("0x8181", "0xfe7f");

    }
    
    template <complement... > struct test_bit_shift;
    
    template <complement n, complement ... p> struct test_bit_shift<n, p...> : test_bit_shift<n>, test_bit_shift<p...> {
        test_bit_shift (
            string given, int bits, 
            string expected_left, 
            string expected_right) : 
        test_bit_shift<n> (given, bits, expected_left, expected_right),
        test_bit_shift<p...> (given, bits, expected_left, expected_right) {}
    };
    
    template <complement n> struct test_bit_shift<n> {
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

        test_bit_shift<complement::nones, complement::ones, complement::BC> ("0x", 0, "0x", "0x");
        test_bit_shift<complement::nones, complement::ones, complement::BC> ("0x", 1, "0x", "0x");
        test_bit_shift<complement::nones, complement::ones, complement::BC> ("0x", 21, "0x", "0x");
        test_bit_shift<complement::nones, complement::ones, complement::BC> ("0x01", 0, "0x01", "0x01");
        test_bit_shift<complement::nones, complement::ones, complement::BC> ("0x01", 1, "0x02", "0x");
        test_bit_shift<complement::nones, complement::ones, complement::BC> ("0x01", 2, "0x04", "0x");
        test_bit_shift<complement::nones, complement::ones, complement::BC> ("0x01", 5, "0x0020", "0x");
        test_bit_shift<complement::nones, complement::ones, complement::BC> ("0x01", 17, "0x020000", "0x00");
        test_bit_shift<complement::nones, complement::ones, complement::BC> ("0x0300", 1, "0x0600", "0x0180");
        test_bit_shift<complement::nones, complement::ones, complement::BC> ("0x0300", 2, "0x0c00", "0x00c0");
        test_bit_shift<complement::nones, complement::ones, complement::BC> ("0x0300", 8, "0x030000", "0x03");
        test_bit_shift<complement::nones, complement::ones, complement::BC> ("0x0300", 9, "0x060000", "0x01");
        test_bit_shift<complement::nones, complement::ones, complement::BC> ("0x0300", 16, "0x03000000", "0x");
        test_bit_shift<complement::nones, complement::ones, complement::BC> ("0x0300", 17, "0x06000000", "0x");
        test_bit_shift<complement::nones, complement::ones, complement::BC> ("0x0300", 18, "0x0c000000", "0x");
        test_bit_shift<complement::nones> ("0xff", 1, "0x01fe", "0x7f");
        test_bit_shift<complement::ones> ("0xff", 1, "0xfe", "0xff");
        test_bit_shift<complement::BC> ("0xff", 1, "0x80fe", "0xc0");

    }
    
    template <complement... > struct test_plus;

    template <complement n> struct test_plus_bytes;
    
    template <complement n, complement ... p> struct test_plus<n, p...> : test_plus<n>, test_plus<p...> {
        test_plus (string left, string right, string expected) :
        test_plus<n> (left, right, expected), test_plus<p...> (left, right, expected) {}
    };
    
    template <complement n> struct test_plus<n> {
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

    template <complement n> struct test_plus_bytes {
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

        test_plus<complement::nones, complement::ones, complement::BC> ("0x", "0x", "0x");
        
        test_plus<complement::nones, complement::ones, complement::BC> ("0", "0", "0");
        test_plus<complement::nones, complement::ones, complement::BC> ("0", "1", "1");
        test_plus<complement::nones, complement::ones, complement::BC> ("1", "1", "2");
        test_plus<complement::nones, complement::ones, complement::BC>
            ("231938875480", "397027301409876", "397259240285356");
        test_plus<complement::nones, complement::ones, complement::BC>
            ("23173210900987658780938875480", "39702733535456767789001409876", "62875944436444426569940285356");
        
        test_plus<complement::ones, complement::BC> ("-1", "0", "-1");
        test_plus<complement::ones, complement::BC> ("-1", "-1", "-2");
        test_plus<complement::ones, complement::BC> ("-1", "1", "0");
        test_plus<complement::ones, complement::BC> ("1", "-1", "0");
        test_plus<complement::ones, complement::BC> ("-2", "1", "-1");
        test_plus<complement::ones, complement::BC> ("2", "-1", "1");
        test_plus<complement::ones, complement::BC>
            ("-23173210900987658780938875480", "23173210900987658780938875480", "0");
        test_plus<complement::ones, complement::BC>
            ("23173210900987658780938875480", "-23173210900987658780938875480", "0");

        test_plus<complement::BC> {"0x", "0x01", "0x01"};
        test_plus<complement::BC> {"0x8000", "0x01", "0x01"};
        test_plus<complement::BC> {"0x", "0x81", "0x81"};

    }
    
    template <complement... > struct test_minus;
    
    template <complement n, complement ... p> struct test_minus<n, p...> : test_minus<n>, test_minus<p...> {
        test_minus (string left, string right, string expected) :
        test_minus<n> (left, right, expected), test_minus<p...> (left, right, expected) {}
    };

    template <complement n> struct test_minus_bytes;
    
    template <complement n> struct test_minus<n> {
        test_minus (string left, string right, string expected) {
            auto l = hex<n>::read (left);
            auto r = hex<n>::read (right);
            auto e = hex<n>::read (expected);
            auto x = l - r;
            EXPECT_EQ (x, e) << "expected " << n << " " << left << " - " << right << " -> " << expected << " but got " << x << std::endl;

            test_minus_bytes<n> (left, right, expected);
        }
    };

    template <complement n> struct test_minus_bytes {
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

    // TODO remember to remove this when we do two's complement. '
    template <> struct test_minus_bytes<complement::BC> {
        test_minus_bytes (string left, string right, string expected) {}
    };

    TEST (HexidecimalTest, TestHexidecimalMinus) {

        test_minus<complement::nones, complement::ones, complement::BC> ("0x", "0x", "0x");

        test_minus<complement::nones, complement::ones, complement::BC> ("0x008000", "0x00ff", "0x7f01");
        test_minus<complement::nones, complement::ones, complement::BC> ("0x008000", "0x00000000ff", "0x7f01");
        
        test_minus<complement::nones, complement::ones, complement::BC> ("0", "0", "0");

        test_minus<complement::nones, complement::ones, complement::BC> ("1", "0", "1");
        test_minus<complement::nones, complement::ones, complement::BC> ("1", "1", "0");

        test_minus<complement::nones, complement::ones, complement::BC> ("37", "12", "25");
        test_minus<complement::nones, complement::ones, complement::BC>
            ("4502938948920982780930898389", "4502938948920982780930898389", "0");

        test_minus<complement::ones, complement::BC> ("12", "37", "-25");
        test_minus<complement::nones> ("12", "37", "0");

        test_minus<complement::nones> ("4502938948920982780930898389", "4502938948920982780930898390", "0");
        test_minus<complement::ones, complement::BC>
            ("4502938948920982780930898389", "4502938948920982780930898390", "-1");
        test_minus<complement::ones, complement::BC> ("1", "-1", "2");

        test_minus<complement::ones> {"0x00ff", "0xff", "0x0100"};
        test_minus<complement::ones> {"0xff", "0x00ff", "0xff00"};

        test_minus<complement::nones, complement::ones> {"0x80", "0x", "0x80"};
        test_minus<complement::BC> {"0x80", "0x", "0x"};

        test_minus<complement::BC> {"0x", "0x01", "0x81"};
        test_minus<complement::BC> {"0x8000", "0x01", "0x81"};
    }
    
    template <complement... > struct test_times;
    
    template <complement n, complement ... p> struct test_times<n, p...> : test_times<n>, test_times<p...> {
        test_times (string left, string right, string expected) :
        test_times<n> (left, right, expected), test_times<p...> (left, right, expected) {}
    };
    
    template <complement n> struct test_times<n> {
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

    template <> struct test_times<complement::BC> {
        test_times (string left, string right, string expected) {}
    };

    TEST (HexidecimalTest, TestHexidecimalTimes) {
        
        test_times<complement::nones, complement::ones, complement::BC> ("0", "0", "0");
        test_times<complement::nones, complement::ones, complement::BC> ("0", "1", "0");
        test_times<complement::nones, complement::ones, complement::BC> ("1", "1", "1");
        test_times<complement::nones, complement::ones, complement::BC> ("1", "2", "2");
        test_times<complement::nones, complement::ones, complement::BC> ("2", "1", "2");
        test_times<complement::nones, complement::ones, complement::BC> ("2", "2", "4");
        test_times<complement::nones, complement::ones, complement::BC> ("2", "5", "10");
        test_times<complement::nones, complement::ones, complement::BC> ("5", "7", "35");
        test_times<complement::nones, complement::ones, complement::BC> ("17", "19", "323");
        test_times<complement::nones, complement::ones, complement::BC>
            ("23173210900987658780938875480", "39702733535456767789001409876",
            "920039817562855061210426612476533348173557348698006240480");
        
        test_times<complement::ones, complement::BC> ("-1", "0", "0");
        test_times<complement::ones, complement::BC> ("0", "-1", "0");
        test_times<complement::ones, complement::BC> ("-1", "1", "-1");
        test_times<complement::ones, complement::BC> ("1", "-1", "-1");
    }
    
    template <complement... > struct test_divide;
    
    template <complement n, complement ... p> struct test_divide<n, p...> : test_divide<n>, test_divide<p...> {
        test_divide (string numerator, string denominator, string quotient, string remainder) :
        test_divide<n> (numerator, denominator, quotient, remainder),
        test_divide<p...> (numerator, denominator, quotient, remainder) {}
    };
    
    template <complement n> struct test_divide<n> {
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
        test_divide<complement::nones, complement::ones, complement::BC> ("0", "1", "0", "0");
        test_divide<complement::nones, complement::ones, complement::BC> ("1", "1", "1", "0");
    }

    TEST (HexidecimalTest, TestHexidecimalOnes) {
        using hex_uint = data::hex::uint<hex_case::lower>;
        using hex_int1 = data::hex::int1<hex_case::lower>;
        
        EXPECT_EQ (string (hex_int1 {"0x"} + hex_uint {"0x"}), string {"0x"});
        EXPECT_EQ (string (hex_int1 {"0x"} * hex_uint {"0x"}), string {"0x"});
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
    
    template <typename N> struct get_number_complement {
        static const math::number::complement value = math::number::complement::ones;
    };
    
    template <endian::order r>
    struct get_number_complement<math::number::Z_bytes<r, math::number::complement::BC, byte>> {
        static const math::number::complement value = math::number::complement::BC;
    };
    
    template <endian::order r>
    struct get_number_complement<math::number::N_bytes<r, byte>> {
        static const math::number::complement value = math::number::complement::nones;
    };
    
    template <>
    struct get_number_complement<N> {
        static const math::number::complement value = math::number::complement::nones;
    };
    
    template <endian::order r, size_t s>
    struct get_number_complement<math::uint<r, s, byte>> {
        static const math::number::complement value = math::number::complement::nones;
    };
    
    template <typename N>
    constexpr math::number::complement number_complement = get_number_complement<N>::value;
    
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
    
    template <typename N, math::number::complement c = number_complement<N>, sizes size = number_size<N>>
    struct test_hex_read_zero {
        test_hex_read_zero (list<size_t> xx) {
            for (size_t i : xx) {
                if (i == size) {
                    N zero = N::read (hex_zero (i));
                    EXPECT_EQ (N {}, zero);
                } else {
                    EXPECT_THROW (N::read (hex_zero (i)), std::invalid_argument);
                }
            }
        }
    };
    
    template <typename N, math::number::complement c> struct test_hex_read_zero<N, c, any> {
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
    
    template <typename N> struct test_hex_read_zero<N, math::number::complement::BC, any> {
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
    
    template <typename N, math::number::complement c = number_complement<N>, bool u = is_signed<N>> struct test_signed_stuff {
        test_signed_stuff (const N &, const encoding::hexidecimal::integer<c, hex_case::lower> &) {}
    };
    
    template <typename N, math::number::complement zz> struct test_signed_stuff<N, zz, true> {
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
    template <typename N, math::number::complement zz> void test_hex_read_and_write_bytes (list<string> cases) {
        list<std::pair<N, encoding::hexidecimal::integer<number_complement<N>, hex_case::lower>>> numbers;
        for (const string &x : cases) {
            using H = encoding::hexidecimal::integer<number_complement<N>, hex_case::lower>;

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
            auto hhpp = encoding::hexidecimal::integer<number_complement<N>, hex_case::lower>
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
                auto hho = encoding::hexidecimal::integer<number_complement<N>, hex_case::lower>
                    {encoding::hexidecimal::write<hex_case::lower> (no)};
                EXPECT_EQ (ho, hho);
                
                auto na = n & p.first;
                auto ha = h & p.second;

                EXPECT_TRUE (math::number::is_minimal (ha));
                EXPECT_TRUE (math::number::is_minimal (na));
            
                EXPECT_EQ (N::read (ha), na);
                auto hha = encoding::hexidecimal::integer<number_complement<N>, hex_case::lower>
                    {encoding::hexidecimal::write<hex_case::lower> (na)};
                EXPECT_EQ (ha, hha);

                auto np = n + p.first;
                auto hp = h + p.second;

                EXPECT_TRUE (math::number::is_minimal (hp));
                EXPECT_TRUE (math::number::is_minimal (np));

                EXPECT_EQ (N::read (hp), np);
                auto hhp = encoding::hexidecimal::integer<number_complement<N>, hex_case::lower>
                    {encoding::hexidecimal::write<hex_case::lower> (np)};
                EXPECT_EQ (hp, hhp);

                auto nm = n - p.first;
                auto hm = h - p.second;

                EXPECT_TRUE (math::number::is_minimal (hm));
                EXPECT_TRUE (math::number::is_minimal (nm));

                EXPECT_EQ (N::read (hm), nm);
                auto hhm = encoding::hexidecimal::integer<number_complement<N>, hex_case::lower>
                    {encoding::hexidecimal::write<hex_case::lower> (nm)};
                EXPECT_EQ (hm, hhm);

                auto nt = n * p.first;
                auto ht = h * p.second;
            
                EXPECT_TRUE (math::number::is_minimal (ht));
                EXPECT_TRUE (math::number::is_minimal (nt));
            
                EXPECT_EQ (N::read (ht), nt);
                auto hht = encoding::hexidecimal::integer<number_complement<N>, hex_case::lower>
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
        test_hex_read_and_write_bytes<N_bytes_little, math::number::complement::nones> (cases);
        test_hex_read_and_write_bytes<N_bytes_big, math::number::complement::nones> (cases);
        test_hex_read_and_write_bytes<Z_bytes_little, math::number::complement::ones> (cases);
        test_hex_read_and_write_bytes<Z_bytes_big, math::number::complement::ones> (cases);
        test_hex_read_and_write_bytes<Z_bytes_BC_little, math::number::complement::BC> (cases);
        test_hex_read_and_write_bytes<Z_bytes_BC_big, math::number::complement::BC> (cases);
    }

}
