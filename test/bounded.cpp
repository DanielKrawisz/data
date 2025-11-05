// Copyright (c) 2019-2015 Katrina Swales and Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/types.hpp>
#include <data/numbers.hpp>
#include <data/math/number/bounded/bounded.hpp>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "gmock/gmock-matchers.h"
#include <stdexcept>

namespace data {
    
    template<bool is_signed, endian::order o, size_t size> 
    using bounded = math::number::bounded<is_signed, o, size, byte>;
    
    using u8l = bounded<false, data::endian::little, 8>;
    using u8b = bounded<false, data::endian::big, 8>;
    using s8l = bounded<true, data::endian::little, 8>;
    using s8b  = bounded<true, data::endian::big, 8>;
    
    using u9l = bounded<false, data::endian::little, 9>;
    using u9b = bounded<false, data::endian::big, 9>;
    using s9l = bounded<true, data::endian::little, 9>;
    using s9b  = bounded<true, data::endian::big, 9>;
    
    using u10l = bounded<false, data::endian::little, 10>;
    using u10b = bounded<false, data::endian::big, 10>;
    using s10l = bounded<true, data::endian::little, 10>;
    using s10b  = bounded<true, data::endian::big, 10>;
    
    using u11l = bounded<false, data::endian::little, 11>;
    using u11b = bounded<false, data::endian::big, 11>;
    using s11l = bounded<true, data::endian::little, 11>;
    using s11b  = bounded<true, data::endian::big, 11>;
    
    using nl = math::number::N_bytes<data::endian::little, byte>;
    using nb = math::number::N_bytes<data::endian::big, byte>;
    using zl = math::number::Z_bytes<data::endian::little, neg::twos, byte>;
    using zb = math::number::Z_bytes<data::endian::big, neg::twos, byte>;
    
    TEST (Bounded, ReadString) {
        
        EXPECT_THROW (u8b::read ("-1"), exception);
        EXPECT_THROW (u8l::read ("-1"), exception);

        auto a = s8b::read ("0xffffffffffffffff");
        auto b = s8b::read ("-1");
        auto c = s8l::read ("0xffffffffffffffff");
        auto d = s8l::read ("-1");
        
        EXPECT_EQ (a, b);
        EXPECT_EQ (c, d);
        
        EXPECT_THROW (u8b::read ("0x01"), exception);
        EXPECT_THROW (u8l::read ("0x01"), exception);

        EXPECT_THROW (u8b::read ("-0x0000000000000001"), exception);
        EXPECT_THROW (u8l::read ("-0x0000000000000001"), exception);

        EXPECT_EQ (s8b::read ("0xffffffffffffffff"), s8b::read ("-1"));
        EXPECT_EQ (s8l::read ("0xffffffffffffffff"), s8l::read ("-1"));
        
        EXPECT_THROW (s8b::read ("-0xffffffffffffffff"), exception);
        EXPECT_THROW (s8l::read ("-0xffffffffffffffff"), exception);
        
        EXPECT_THROW (u8b::read ("0x000000000000000001"), exception);
        EXPECT_THROW (u8l::read ("0x000000000000000001"), exception);
        
        EXPECT_THROW (s8b::read ("0x000000000000000001"), exception);
        EXPECT_THROW (s8l::read ("0x000000000000000001"), exception);
        
        EXPECT_EQ (u11l::read ("0xaabbccddeeff0011223344"), u11l ("4433221100ffeeddccbbaa"));
        EXPECT_EQ (u11b::read ("0xaabbccddeeff0011223344"), u11b ("aabbccddeeff0011223344"));
        
    }
    
    TEST (Bounded, ZeroOne) {
        
        EXPECT_EQ (N (u8b (0)), N (0));
        EXPECT_EQ (N (u8b (1)), N (1));
        EXPECT_NE (N (u8b (1)), N (0));
        EXPECT_NE (N (u8b (0)), N (1));
        EXPECT_GT (N (u8b (1)), N (0));
        EXPECT_LT (N (u8b (0)), N (1));
        
        EXPECT_EQ (N (u8l (0)), N (0));
        EXPECT_EQ (N (u8l (1)), N (1));
        EXPECT_NE (N (u8l (1)), N (0));
        EXPECT_NE (N (u8l (0)), N (1));
        EXPECT_GT (N (u8l (1)), N (0));
        EXPECT_LT (N (u8l (0)), N (1));
        
        EXPECT_EQ (Z (s8b (0)), Z (0));
        EXPECT_EQ (Z (s8b (1)), Z (1));
        EXPECT_NE (Z (s8b (1)), Z (0));
        EXPECT_NE (Z (s8b (0)), Z (1));
        EXPECT_GT (Z (s8b (1)), Z (0));
        EXPECT_LT (Z (s8b (0)), Z (1));
        
        EXPECT_EQ (Z (s8l (0)), Z (0));
        EXPECT_EQ (Z (s8l (1)), Z (1));
        EXPECT_NE (Z (s8l (1)), Z (0));
        EXPECT_NE (Z (s8l (0)), Z (1));
        EXPECT_GT (Z (s8l (1)), Z (0));
        EXPECT_LT (Z (s8l (0)), Z (1));
        
        EXPECT_EQ (N (u8l (0)), N (0));
        EXPECT_EQ (N (u8l (1)), N (1));
        EXPECT_NE (N (u8l (1)), N (0));
        EXPECT_NE (N (u8l (0)), N (1));
        EXPECT_GT (N (u8l (1)), N (0));
        EXPECT_LT (N (u8l (0)), N (1));
        
        EXPECT_EQ (Z (s8b (0)), Z (0));
        EXPECT_EQ (Z (s8b (1)), Z (1));
        EXPECT_NE (Z (s8b (1)), Z (0));
        EXPECT_NE (Z (s8b (0)), Z (1));
        EXPECT_GT (Z (s8b (1)), Z (0));
        EXPECT_LT (Z (s8b (0)), Z (1));
        
        EXPECT_EQ (N (nl (0)), N (0));
        EXPECT_EQ (N (nl (1)), N (1));
        EXPECT_NE (N (nl (1)), N (0));
        EXPECT_NE (N (nl (0)), N (1));
        EXPECT_GT (N (nl (1)), N (0));
        EXPECT_LT (N (nl (0)), N (1));
        
        EXPECT_EQ (Z(zl(0)), Z (0));
        EXPECT_EQ (Z(zl(1)), Z (1));
        EXPECT_NE (Z(zl(1)), Z (0));
        EXPECT_NE (Z(zl(0)), Z (1));
        EXPECT_GT (Z(zl(1)), Z (0));
        EXPECT_LT (Z(zl(0)), Z (1));
        
    }
    
    TEST (Bounded, MinMax) {
        
        EXPECT_TRUE (u8b::max () > u8b::min ());
        EXPECT_TRUE (u8l::max () > u8l::min ());
        
        EXPECT_FALSE (u8b::max () < u8b::min ());
        EXPECT_FALSE (u8l::max () < u8l::min ());
        
        EXPECT_TRUE (s8b::max () > s8b::min ());
        EXPECT_TRUE (s8l::max () > s8l::min ());
        
        EXPECT_FALSE (s8b::max () < s8b::min ());
        EXPECT_FALSE (s8l::max () < s8l::min ());
        
        EXPECT_EQ (u8b::min (), u8b {0});
        EXPECT_EQ (u8l::min (), u8l {0});
        
        EXPECT_EQ (u8b::max (), u8b {"0xffffffffffffffff"});
        EXPECT_EQ (u8l::max (), u8l {"0xffffffffffffffff"});
        
        EXPECT_EQ (s8b::min (), s8b {"0x8000000000000000"});
        EXPECT_EQ (s8l::min (), s8l {"0x8000000000000000"});

        EXPECT_EQ (s8b::max (), s8b {"0x7FFFFFFFFFFFFFFF"});
        EXPECT_EQ (s8l::max (), s8l {"0x7FFFFFFFFFFFFFFF"});
        
    }
    
    TEST (Bounded, Sign) {
        
        EXPECT_FALSE (s8b {0} < 0);
        EXPECT_FALSE (s8b {1} < 0);
        EXPECT_TRUE (s8b {-1} < 0);
        
        EXPECT_FALSE (s8l {0} < 0);
        EXPECT_FALSE (s8l {1} < 0);
        EXPECT_TRUE (s8l {-1} < 0);
        
        EXPECT_FALSE (s9b {0} < 0);
        EXPECT_FALSE (s9b {1} < 0);
        EXPECT_TRUE (s9b {-1} < 0);
        
        EXPECT_FALSE (s9l {0} < 0);
        EXPECT_FALSE (s9l {1} < 0);
        EXPECT_TRUE (s9l {-1} < 0);
        
        EXPECT_FALSE (s10b {0} < 0);
        EXPECT_FALSE (s10b {1} < 0);
        EXPECT_TRUE (s10b {-1} < 0);
        
        EXPECT_FALSE (s10l {0} < 0);
        EXPECT_FALSE (s10l {1} < 0);
        EXPECT_TRUE (s10l {-1} < 0);
        
        EXPECT_FALSE (s11b {0} < 0);
        EXPECT_FALSE (s11b {1} < 0);
        EXPECT_TRUE (s11b {-1} < 0);
        
        EXPECT_FALSE (s11l {0} < 0);
        EXPECT_FALSE (s11l {1} < 0);
        EXPECT_TRUE (s11l {-1} < 0);
        
        EXPECT_FALSE (zb {0} < 0);
        EXPECT_FALSE (zb {1} < 0);
        EXPECT_TRUE (zb {-1} < 0);
        
        EXPECT_FALSE (zl {0} < 0);
        EXPECT_FALSE (zl {1} < 0);
        EXPECT_TRUE (zl {-1} < 0);
        
    }
    
    TEST (Bounded, Negative) {
        
        EXPECT_EQ (-s8b::read ("1"), s8b::read ("-1"));
        EXPECT_EQ (-s8b::read ("-1"), s8b::read ("1"));

        EXPECT_EQ (-s9b::read ("1"), s9b::read ("-1"));
        EXPECT_EQ (-s9b::read ("-1"), s9b::read ("1"));
        
        EXPECT_EQ (-s10b::read ("1"), s10b::read ("-1"));
        EXPECT_EQ (-s10b::read ("-1"), s10b::read ("1"));
        
        EXPECT_EQ (-s11b::read ("1"), s11b::read ("-1"));
        EXPECT_EQ (-s11b::read ("-1"), s11b::read ("1"));
        
        EXPECT_EQ (-s8l::read ("1"), s8l::read ("-1"));
        EXPECT_EQ (-s8l::read ("-1"), s8l::read ("1"));
        
        EXPECT_EQ (-s9l::read ("1"), s9l::read ("-1"));
        EXPECT_EQ (-s9l::read ("-1"), s9l::read ("1"));
        
        EXPECT_EQ (-s10l::read ("1"), s10l::read ("-1"));
        EXPECT_EQ (-s10l::read ("-1"), s10l::read ("1"));
        
        EXPECT_EQ (-s11l::read ("1"), s11l::read ("-1"));
        EXPECT_EQ (-s11l::read ("-1"), s11l::read ("1"));
        
        EXPECT_EQ (-zb::read ("1"), zb::read ("-1"));
        EXPECT_EQ (-zb::read ("-1"), zb::read ("1"));
        
        EXPECT_EQ (-zl::read ("1"), zl::read ("-1"));
        EXPECT_EQ (-zl::read ("-1"), zl::read ("1"));
        
    }
    
    template<bool is_signed, endian::order o, size_t size> void test_bounded_read_write_string (string x) {
        bounded<is_signed, o, size> n;
        
        EXPECT_NO_THROW ( (n = bounded<is_signed, o, size>{x}) );
        EXPECT_EQ (encoding::hexidecimal::write (n), x);
        
        if (o == endian::little) {
            EXPECT_EQ (n[0], 0xff);
            EXPECT_EQ (n[size - 1], 0x00);
        } else {
            EXPECT_EQ (n[size - 1], 0xff);
            EXPECT_EQ (n[0], 0x00);
        }
        
    }
    
    TEST (Bounded, ReadWriteString) {

        test_bounded_read_write_string<true, endian::big, 9> ("0x00000000ffffffffff");
        test_bounded_read_write_string<true, endian::little, 9> ("0x00000000ffffffffff");
        test_bounded_read_write_string<false, endian::big, 9> ("0x00000000ffffffffff");
        test_bounded_read_write_string<false, endian::little, 9> ("0x00000000ffffffffff");
        
        test_bounded_read_write_string<true, endian::big, 10> ("0x0000000000ffffffffff");
        test_bounded_read_write_string<true, endian::little, 10> ("0x0000000000ffffffffff");
        test_bounded_read_write_string<false, endian::big, 10> ("0x0000000000ffffffffff");
        test_bounded_read_write_string<false, endian::little, 10> ("0x0000000000ffffffffff");
        
        test_bounded_read_write_string<true, endian::big, 11> ("0x0000000000ffffffffffff");
        test_bounded_read_write_string<true, endian::little, 11> ("0x0000000000ffffffffffff");
        test_bounded_read_write_string<false, endian::big, 11> ("0x0000000000ffffffffffff");
        test_bounded_read_write_string<false, endian::little, 11> ("0x0000000000ffffffffffff");
        
        test_bounded_read_write_string<true, endian::big, 12> ("0x000000000000ffffffffffff");
        test_bounded_read_write_string<true, endian::little, 12> ("0x000000000000ffffffffffff");
        test_bounded_read_write_string<false, endian::big, 12> ("0x000000000000ffffffffffff");
        test_bounded_read_write_string<false, endian::little, 12> ("0x000000000000ffffffffffff");

    }

    template <typename N>
    concept ConstexprArithmetic = requires {
        { []() constexpr { (void) N {0}; }() };
        { []() constexpr { (void) N {1}; }() };

        { []() constexpr { (void) (N {1} == N {2}); }() };
        { []() constexpr { (void) (N {1} != N {2}); }() };
        { []() constexpr { (void) (N {1} < N {2}); }() };
        { []() constexpr { (void) (N {1} > N {2}); }() };
        { []() constexpr { (void) (N {1} <= N {2}); }() };
        { []() constexpr { (void) (N {1} >= N {2}); }() };

        { []() constexpr { (void) (mul_2_pow (N {7}, 3)); }() };
        { []() constexpr { (void) (div_2 (N {25})); }() };
        { []() constexpr { (void) (mod_2 (N {77})); }() };
        // TODO uncomment this and get the function working.
        //{ []() constexpr { (void) (size_in_base_2 (N {2021021})); }() };

        { []() constexpr { (void) (bit_not (N {11})); }() };
        { []() constexpr { (void) (~N {11}); }() };
        { []() constexpr { (void) (bit_and (N {11}, N {1001})); }() };
        { []() constexpr { (void) (N {11} & N {1001}); }() };
        { []() constexpr { (void) (bit_or (N {11}, N {1001})); }() };
        { []() constexpr { (void) (bit_xor (N {11}, N {1001})); }() };
        //{ []() constexpr { (void) (bit_nor (N {11}, N {1001})); }() };
        //{ []() constexpr { (void) (bit_nand (N {11}, N {1001})); }() };
        { []() constexpr { (void) (bit_shift_left (N {131}, 1)); }() };
        { []() constexpr { (void) (bit_shift_right (N {131}, 1)); }() };

        { []() constexpr { (void) (square (N {37})); }() };
        { []() constexpr { (void) (quadrance (N {3})); }() };
        { []() constexpr { (void) (math::inner (N {3}, N {5})); }() };

        { []() constexpr { (void) (abs (N {2})); }() };
        { []() constexpr { (void) (negate (N {2})); }() };
        { []() constexpr { (void) (-N {2}); }() };

        { []() constexpr { (void) (N {1} + N {2}); }() };
        { []() constexpr { (void) (N {2} - N {1}); }() };
        { []() constexpr { (void) (N {5} * N {8}); }() };

        { []() constexpr { (void) (plus (N {1}, N {2})); }() };
        { []() constexpr { (void) (minus (N {2}, N {1})); }() };
        { []() constexpr { (void) (times (N {5}, N {8})); }() };
        { []() constexpr { (void) (pow (N {3}, N {7})); }() };

        { []() constexpr { (void) (N {13} / N {5}); }() };
        { []() constexpr { (void) (N {13} % N {5}); }() };

        { []() constexpr { (void) (divide (N {13}, math::nonzero {N {5}})); }() };
        { []() constexpr { (void) (mod (N {13}, math::nonzero {N {5}})); }() };
        { []() constexpr { (void) (divmod (N {13}, math::nonzero {N {5}})); }() };

        { []() constexpr { (void) (mul_2_mod (N {7}, math::nonzero {N {5}})); }() };
        { []() constexpr { (void) (square_mod (N {37}, math::nonzero {N {5}})); }() };
        { []() constexpr { (void) (negate_mod (N {233}, math::nonzero {N {65537}})); }() };
        { []() constexpr { (void) (plus_mod (N {1}, N {2}, math::nonzero {N {65537}})); }() };
        { []() constexpr { (void) (minus_mod (N {2}, N {1}, math::nonzero {N {65537}})); }() };
        { []() constexpr { (void) (times_mod (N {5}, N {8}, math::nonzero {N {65537}})); }() };
        { []() constexpr { (void) (pow_mod (N {3}, N {7}, math::nonzero {N {65537}})); }() };
        { []() constexpr { (void) (invert_mod (N {577}, math::nonzero {N {65537}})); }() };
        { []() constexpr { (void) (GCD (N {4056}, N {98432})); }() };

    };

    static_assert (ConstexprArithmetic<uint32>);
    static_assert (ConstexprArithmetic<uint32_big>);
    static_assert (ConstexprArithmetic<uint32_little>);

    static_assert (ConstexprArithmetic<int32>);
    static_assert (ConstexprArithmetic<int32_big>);
    static_assert (ConstexprArithmetic<int32_little>);

    static_assert (ConstexprArithmetic<uint64>);
    static_assert (ConstexprArithmetic<uint64_big>);
    static_assert (ConstexprArithmetic<uint64_little>);

    static_assert (ConstexprArithmetic<int64>);
    static_assert (ConstexprArithmetic<int64_big>);
    static_assert (ConstexprArithmetic<int64_little>);

    static_assert (ConstexprArithmetic<uint80>);
    static_assert (ConstexprArithmetic<uint80_big>);
    static_assert (ConstexprArithmetic<uint80_little>);

    static_assert (ConstexprArithmetic<int80>);
    static_assert (ConstexprArithmetic<int80_big>);
    static_assert (ConstexprArithmetic<int80_little>);

    static_assert (ConstexprArithmetic<uint128>);
    static_assert (ConstexprArithmetic<uint128_big>);
    static_assert (ConstexprArithmetic<uint128_little>);

    static_assert (ConstexprArithmetic<int128>);
    static_assert (ConstexprArithmetic<int128_big>);
    static_assert (ConstexprArithmetic<int128_little>);

    static_assert (ConstexprArithmetic<uint160>);
    static_assert (ConstexprArithmetic<uint160_big>);
    static_assert (ConstexprArithmetic<uint160_little>);

    static_assert (ConstexprArithmetic<int160>);
    static_assert (ConstexprArithmetic<int160_big>);
    static_assert (ConstexprArithmetic<int160_little>);

    static_assert (ConstexprArithmetic<uint256>);
    static_assert (ConstexprArithmetic<uint256_big>);
    static_assert (ConstexprArithmetic<uint256_little>);

    static_assert (ConstexprArithmetic<int256>);
    static_assert (ConstexprArithmetic<int256_big>);
    static_assert (ConstexprArithmetic<int256_little>);

}
