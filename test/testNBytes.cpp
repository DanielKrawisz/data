// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include <data/list.hpp>
#include "gtest/gtest.h"
#include <iostream>

namespace data::math::number {

    template<endian::order r, std::unsigned_integral word>
    math::N N_Bytes_to_N_stupid (const math::number::N_bytes<r, word> &n) {
        math::N x {0};

        for (const word &b : n.words ().reverse ()) {
            x <<= (sizeof (word) * 8);
            x += b;
        }

        return x;
    }

    template<endian::order r, std::unsigned_integral word>
    math::number::N_bytes<r, word> inline N_to_N_Bytes_stupid (const math::N &n) {
        auto hex_string = encoding::hexidecimal::write<hex_case::lower> (n);
        size_t bytes_encoded = (hex_string.size () - 2) / 2;

        size_t bytes_extended = bytes_encoded % sizeof (word) == 0 ? bytes_encoded :
            ((bytes_encoded / sizeof (word)) + 1) * sizeof (word);

        auto hex_resized = encoding::hexidecimal::extend<negativity::nones, hex_case::lower> (hex_string, bytes_extended * 2 + 2);

        return math::number::N_bytes<r, word>::read (hex_resized);
    }

    template <std::unsigned_integral word> using Nl = math::number::N_bytes<endian::little, word>;
    template <std::unsigned_integral word> using Zl1 = math::number::Z_bytes<endian::little, math::negativity::twos, word>;
    template <std::unsigned_integral word> using Zl2 = math::number::Z_bytes<endian::little, math::negativity::BC, word>;
    template <std::unsigned_integral word> using Nb = math::number::N_bytes<endian::big, word>;
    template <std::unsigned_integral word> using Zb1 = math::number::Z_bytes<endian::big, math::negativity::twos, word>;
    template <std::unsigned_integral word> using Zb2 = math::number::Z_bytes<endian::big, math::negativity::BC, word>;

    template <typename in, std::unsigned_integral word> void N_Bytes_to_N_by_word (in x) {

        math::N n {x};

        Nb<word> big {x};
        Nl<word> little {x};

        Nb<word> stupid_big = N_to_N_Bytes_stupid<endian::big, word> (n);
        Nl<word> stupid_little = N_to_N_Bytes_stupid<endian::little, word> (n);

        EXPECT_EQ (stupid_big, big) << "expected " << std::hex << stupid_big << " to equal " << big << "; input = " << x;
        EXPECT_EQ (stupid_little, little) << "expected " << std::hex << stupid_little << " to equal " << little << "; input = " << x;

        math::N N_big = math::N (big);
        math::N N_little = math::N (little);

        math::N N_big_stupid = N_Bytes_to_N_stupid<endian::big, word> (big);
        math::N N_little_stupid = N_Bytes_to_N_stupid<endian::little, word> (little);

        EXPECT_EQ (N_big_stupid, N_big) << "expected " << std::hex << N_big_stupid << " to equal " << N_big << "; input = " << x;
        EXPECT_EQ (N_little_stupid, N_little) << "expected " << std::hex << N_little_stupid << " to equal " << N_little << "; input = " << x;

        EXPECT_EQ (N_big, n);
        EXPECT_EQ (N_little, n);

    }

    template <typename in> void N_Bytes_to_N (in x) {

        N_Bytes_to_N_by_word<in, byte> (x);
        N_Bytes_to_N_by_word<in, unsigned short> (x);
        N_Bytes_to_N_by_word<in, unsigned> (x);
        N_Bytes_to_N_by_word<in, unsigned long> (x);
        N_Bytes_to_N_by_word<in, unsigned long long> (x);

    }

    TEST (NBytesTest, TestNBytesToN) {

        N_Bytes_to_N<uint64> (0);
        N_Bytes_to_N<uint64> (1);
        N_Bytes_to_N<uint64> (3);
        N_Bytes_to_N<uint64> (767);
        N_Bytes_to_N<uint64> (7439);
        N_Bytes_to_N<uint64> (10920960978709);

        N_Bytes_to_N<string> ("0x0fabcdef123456789012323454567600000a00aabbccddeeffffffffffffffff"
                                "0000111122223333444455556666777788889999aaaabbbbccccddddeeeeffff"
                                "0369cf258be147ad05af49e38d27c16b07e5c3a18f6d4b29092b4d6f81a3c5e7"
                                "abcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcd");

        N_Bytes_to_N<string> ("0xf0abcdef123456789012323454567600000a00aabbccddeeffffffffffffffff"
                                "0000111122223333444455556666777788889999aaaabbbbccccddddeeeeffff"
                                "0369cf258be147ad05af49e38d27c16b07e5c3a18f6d4b29092b4d6f81a3c5e7"
                                "abcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcd");

    }

    TEST (NBytesTest, TestStringToNBytes) {

        EXPECT_THROW ((N_bytes<endian::big, byte>::read ("")), exception);
        EXPECT_THROW ((N_bytes<endian::big, byte>::read ("a")), exception);
        EXPECT_THROW ((N_bytes<endian::big, byte>::read ("-")), exception);
        EXPECT_THROW ((N_bytes<endian::big, byte>::read ("-1")), exception);
        EXPECT_THROW ((N_bytes<endian::big, byte>::read ("01")), exception);
        EXPECT_THROW ((N_bytes<endian::big, byte>::read ("0x1")), exception);

        EXPECT_THROW ((N_bytes<endian::little, byte>::read ("")), exception);
        EXPECT_THROW ((N_bytes<endian::little, byte>::read ("a")), exception);
        EXPECT_THROW ((N_bytes<endian::little, byte>::read ("-")), exception);
        EXPECT_THROW ((N_bytes<endian::little, byte>::read ("-1")), exception);
        EXPECT_THROW ((N_bytes<endian::little, byte>::read ("01")), exception);
        EXPECT_THROW ((N_bytes<endian::little, byte>::read ("0x1")), exception);

        EXPECT_THROW ((N_bytes<endian::big, uint16>::read ("")), exception);
        EXPECT_THROW ((N_bytes<endian::big, uint16>::read ("a")), exception);
        EXPECT_THROW ((N_bytes<endian::big, uint16>::read ("-")), exception);
        EXPECT_THROW ((N_bytes<endian::big, uint16>::read ("-1")), exception);
        EXPECT_THROW ((N_bytes<endian::big, uint16>::read ("01")), exception);
        EXPECT_THROW ((N_bytes<endian::big, uint16>::read ("0x1")), exception);
        EXPECT_THROW ((N_bytes<endian::big, uint16>::read ("0x01")), exception);

        EXPECT_THROW ((N_bytes<endian::little, uint16>::read ("")), exception);
        EXPECT_THROW ((N_bytes<endian::little, uint16>::read ("a")), exception);
        EXPECT_THROW ((N_bytes<endian::little, uint16>::read ("-")), exception);
        EXPECT_THROW ((N_bytes<endian::little, uint16>::read ("-1")), exception);
        EXPECT_THROW ((N_bytes<endian::little, uint16>::read ("01")), exception);
        EXPECT_THROW ((N_bytes<endian::little, uint16>::read ("0x1")), exception);
        EXPECT_THROW ((N_bytes<endian::little, uint16>::read ("0x01")), exception);

        EXPECT_THROW ((N_bytes<endian::big, uint32>::read ("")), exception);
        EXPECT_THROW ((N_bytes<endian::big, uint32>::read ("a")), exception);
        EXPECT_THROW ((N_bytes<endian::big, uint32>::read ("-")), exception);
        EXPECT_THROW ((N_bytes<endian::big, uint32>::read ("-1")), exception);
        EXPECT_THROW ((N_bytes<endian::big, uint32>::read ("01")), exception);
        EXPECT_THROW ((N_bytes<endian::big, uint32>::read ("0x1")), exception);
        EXPECT_THROW ((N_bytes<endian::big, uint32>::read ("0x01")), exception);
        EXPECT_THROW ((N_bytes<endian::big, uint32>::read ("0x0001")), exception);

        EXPECT_THROW ((N_bytes<endian::little, uint32>::read ("")), exception);
        EXPECT_THROW ((N_bytes<endian::little, uint32>::read ("a")), exception);
        EXPECT_THROW ((N_bytes<endian::little, uint32>::read ("-")), exception);
        EXPECT_THROW ((N_bytes<endian::little, uint32>::read ("-1")), exception);
        EXPECT_THROW ((N_bytes<endian::little, uint32>::read ("01")), exception);
        EXPECT_THROW ((N_bytes<endian::little, uint32>::read ("0x1")), exception);
        EXPECT_THROW ((N_bytes<endian::little, uint32>::read ("0x01")), exception);
        EXPECT_THROW ((N_bytes<endian::little, uint32>::read ("0x0001")), exception);

        EXPECT_THROW ((N_bytes<endian::big, uint64>::read ("")), exception);
        EXPECT_THROW ((N_bytes<endian::big, uint64>::read ("a")), exception);
        EXPECT_THROW ((N_bytes<endian::big, uint64>::read ("-")), exception);
        EXPECT_THROW ((N_bytes<endian::big, uint64>::read ("-1")), exception);
        EXPECT_THROW ((N_bytes<endian::big, uint64>::read ("01")), exception);
        EXPECT_THROW ((N_bytes<endian::big, uint64>::read ("0x1")), exception);
        EXPECT_THROW ((N_bytes<endian::big, uint64>::read ("0x01")), exception);
        EXPECT_THROW ((N_bytes<endian::big, uint64>::read ("0x0001")), exception);
        EXPECT_THROW ((N_bytes<endian::big, uint64>::read ("0x00000001")), exception);

        EXPECT_THROW ((N_bytes<endian::little, uint64>::read ("")), exception);
        EXPECT_THROW ((N_bytes<endian::little, uint64>::read ("a")), exception);
        EXPECT_THROW ((N_bytes<endian::little, uint64>::read ("-")), exception);
        EXPECT_THROW ((N_bytes<endian::little, uint64>::read ("-1")), exception);
        EXPECT_THROW ((N_bytes<endian::little, uint64>::read ("01")), exception);
        EXPECT_THROW ((N_bytes<endian::little, uint64>::read ("0x1")), exception);
        EXPECT_THROW ((N_bytes<endian::little, uint64>::read ("0x01")), exception);
        EXPECT_THROW ((N_bytes<endian::little, uint64>::read ("0x0001")), exception);
        EXPECT_THROW ((N_bytes<endian::little, uint64>::read ("0x00000001")), exception);

        EXPECT_EQ ((N_bytes<endian::big, byte> {0}), (N_bytes<endian::big, byte> {}));
        EXPECT_EQ ((N_bytes<endian::big, byte> {0}), (N_bytes<endian::big, byte>::read ("0")));
        EXPECT_EQ ((N_bytes<endian::big, byte> {0}), (N_bytes<endian::big, byte>::read ("0x")));
        
        EXPECT_FALSE ((N_bytes<endian::big, byte> {1}) == (N_bytes<endian::big, byte>::read ("0")));
        EXPECT_FALSE ((N_bytes<endian::big, byte> {0}) == (N_bytes<endian::big, byte>::read ("1")));
        
        EXPECT_TRUE ((N_bytes<endian::big, byte> {1}) == (N_bytes<endian::big, byte>::read ("1")));
        EXPECT_TRUE ((N_bytes<endian::big, byte> {1}) == (N_bytes<endian::big, byte>::read ("0x01")));
        EXPECT_TRUE ((N_bytes<endian::big, byte> {1}) == (N_bytes<endian::big, byte>::read ("0x0001")));

        EXPECT_EQ ((N_bytes<endian::little, byte> {0}), (N_bytes<endian::little, byte> {}));
        EXPECT_EQ ((N_bytes<endian::little, byte> {0}), (N_bytes<endian::little, byte>::read ("0")));
        EXPECT_EQ ((N_bytes<endian::little, byte> {0}), (N_bytes<endian::little, byte>::read ("0x")));
        
        EXPECT_FALSE ((N_bytes<endian::little, byte> {1}) == (N_bytes<endian::little, byte>::read ("0")));
        EXPECT_FALSE ((N_bytes<endian::little, byte> {0}) == (N_bytes<endian::little, byte>::read ("1")));
        
        EXPECT_TRUE ((N_bytes<endian::little, byte> {1}) == (N_bytes<endian::little, byte>::read ("1")));
        EXPECT_TRUE ((N_bytes<endian::little, byte> {1}) == (N_bytes<endian::little, byte>::read ("0x01")));
        EXPECT_TRUE ((N_bytes<endian::little, byte> {1}) == (N_bytes<endian::little, byte>::read ("0x0001")));
        
        EXPECT_EQ ((N_bytes<endian::big, byte>::read ("23")), (N_bytes<endian::big, byte> {23}));
        EXPECT_EQ ((N_bytes<endian::little, byte>::read ("23")), (N_bytes<endian::little, byte> {23}));
        
    }

    TEST (NBytesTest, TestNBytesToHexString) {
        
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N_bytes<endian::big, byte> {"0"}), std::string {"0x"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N_bytes<endian::big, byte> {"127"}), std::string {"0x7f"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N_bytes<endian::big, byte> {"128"}), std::string {"0x80"});
        
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N_bytes<endian::little, byte> {"0"}), std::string {"0x"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N_bytes<endian::little, byte> {"127"}), std::string {"0x7f"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N_bytes<endian::little, byte> {"128"}), std::string {"0x80"});
        
    }
    
    TEST (NBytesTest, TestZeroSize) {
        
        EXPECT_EQ ((N_bytes<endian::big, byte> {"0x"}.size ()), 0);
        EXPECT_EQ ((N_bytes<endian::big, byte> {"0x00"}.size ()), 1);
        EXPECT_EQ ((N_bytes<endian::big, byte> {"0x000000"}.size ()), 3);
        
        EXPECT_EQ ((N_bytes<endian::little, byte> {"0x"}.size ()), 0);
        EXPECT_EQ ((N_bytes<endian::little, byte> {"0x00"}.size ()), 1);
        EXPECT_EQ ((N_bytes<endian::little, byte> {"0x000000"}.size ()), 3);
        
    }
    
    TEST (NBytesTest, TestZeroAndNegativeNBytes) {
        
        EXPECT_EQ ((N_bytes<endian::big, byte>::read ("0x")), (N_bytes<endian::big, byte> {0}));
        EXPECT_EQ ((N_bytes<endian::big, byte>::read ("0x00")), (N_bytes<endian::big, byte> {0}));
        EXPECT_EQ ((N_bytes<endian::big, byte>::read ("0x000000")), (N_bytes<endian::big, byte> {0}));
        EXPECT_EQ ((N_bytes<endian::little, byte>::read ("0x")), (N_bytes<endian::little, byte> {0}));
        EXPECT_EQ ((N_bytes<endian::little, byte>::read ("0x00")), (N_bytes<endian::little, byte> {0}));
        EXPECT_EQ ((N_bytes<endian::little, byte>::read ("0x000000")), (N_bytes<endian::little, byte> {0}));
        
        EXPECT_EQ ((N_bytes<endian::big, byte>::read ("0x01")), (N_bytes<endian::big, byte> {1}));
        EXPECT_EQ ((N_bytes<endian::big, byte>::read ("0x0001")), (N_bytes<endian::big, byte> {1}));
        EXPECT_EQ ((N_bytes<endian::big, byte>::read ("0x00000001")), (N_bytes<endian::big, byte> {1}));
        EXPECT_EQ ((N_bytes<endian::little, byte>::read ("0x01")), (N_bytes<endian::little, byte> {1}));
        EXPECT_EQ ((N_bytes<endian::little, byte>::read ("0x0001")), (N_bytes<endian::little, byte> {1}));
        EXPECT_EQ ((N_bytes<endian::little, byte>::read ("0x00000001")), (N_bytes<endian::little, byte> {1}));
        
    }

    void test_N_to_N_bytes (string x) {
        EXPECT_EQ ((N_bytes<endian::big, byte> {math::N (x)}), (N_bytes<endian::big, byte>::read (x)));
        EXPECT_EQ ((N_bytes<endian::little, byte> {math::N (x)}), (N_bytes<endian::little, byte>::read (x)));
    }
    
    TEST (NBytesTest, TestNToNBytes) {

        test_N_to_N_bytes ("0");
        test_N_to_N_bytes ("1");
        test_N_to_N_bytes ("23");
        test_N_to_N_bytes ("5704566599993321");
        test_N_to_N_bytes ("98980987676898761029390303474536547398");
        test_N_to_N_bytes ("98980987676898761029390303474536547399");
        test_N_to_N_bytes ("98980987676898761029390303474536547400");
        
    }

    template <endian::order r> 
    struct test_bit_shift {
        test_bit_shift (string num, int shift) {
            EXPECT_EQ ((N_bytes<r, byte> (num) >> shift), (N_bytes<r, byte> (num) << -shift));
            EXPECT_EQ ((N_bytes<r, byte> (num) << shift), (N_bytes<r, byte> (num) >> -shift));
            EXPECT_EQ ((N_bytes<r, byte> (num) << shift >> shift), (N_bytes<r, byte> (num)));
            EXPECT_EQ ((math::N (num) >> shift), (math::N (N_bytes<r, byte> (num) >> shift)));
            EXPECT_EQ ((math::N (num) << shift), (math::N (N_bytes<r, byte> (num) << shift)));
        }
    };
    
    TEST (NBytesTest, TestNBitShift) {
        EXPECT_EQ ((N_bytes<endian::big, byte> {"1"} << 1), (N_bytes<endian::big, byte> {"2"}));
        EXPECT_EQ ((N_bytes<endian::little, byte> {"1"} << 1), (N_bytes<endian::little, byte> {"2"}));
        EXPECT_EQ ((N_bytes<endian::big, byte> {"2"} >> 1), (N_bytes<endian::big, byte> {"1"}));
        EXPECT_EQ ((N_bytes<endian::little, byte> {"2"} >> 1), (N_bytes<endian::little, byte> {"1"}));

        for (const string &num : list<string> {"0", "1", "23", "5704566599993321"})
            for (const int &shift : list<int> {0, 1, 5, 8, 15, 16}) {
                test_bit_shift<endian::big> {num, shift};
                test_bit_shift<endian::little> {num, shift};
            };
        
    }

    template <endian::order o, std::unsigned_integral word>
    void test_N_Bytes_to_string_decimal () {
        EXPECT_EQ (encoding::decimal::write (N_bytes<o, word> {1}), std::string {"1"});
        EXPECT_EQ (encoding::decimal::write (N_bytes<o, word> {23}), std::string {"23"});
        EXPECT_EQ (encoding::decimal::write (N_bytes<o, word> {767}), std::string {"767"});
        EXPECT_EQ (encoding::decimal::write (N_bytes<o, word> {"5704566599993321"}), std::string {"5704566599993321"});
    }
    
    TEST (NBytesTest, TestNBytesToString) {
        
        test_N_Bytes_to_string_decimal<endian::big, byte> ();
        test_N_Bytes_to_string_decimal<endian::little, byte> ();
        test_N_Bytes_to_string_decimal<endian::big, unsigned short> ();
        test_N_Bytes_to_string_decimal<endian::little, unsigned short> ();
        test_N_Bytes_to_string_decimal<endian::big, unsigned> ();
        test_N_Bytes_to_string_decimal<endian::little, unsigned> ();
        test_N_Bytes_to_string_decimal<endian::big, unsigned long> ();
        test_N_Bytes_to_string_decimal<endian::little, unsigned long> ();
        test_N_Bytes_to_string_decimal<endian::big, unsigned long long> ();
        test_N_Bytes_to_string_decimal<endian::little, unsigned long long> ();
        
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (
            N_bytes<endian::big, byte> {1}.trim ()), std::string {"0x01"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (
            N_bytes<endian::little, byte> {1}.trim ()), std::string {"0x01"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (
            N_bytes<endian::big, byte> {23}.trim ()), std::string {"0x17"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (
            N_bytes<endian::little, byte> {23}.trim ()), std::string {"0x17"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (
            N_bytes<endian::big, byte> {"5704566599993321"}), std::string {"0x144445e9ca47e9"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (
            N_bytes<endian::little, byte> {"5704566599993321"}), std::string {"0x144445e9ca47e9"});
        
    }
    
    TEST (NBytesTest, TestIncrement) {
        
        auto nl0 = N_bytes<endian::big, byte>::read ("0x");
        auto nl1 = N_bytes<endian::big, byte>::read ("0x00");
        auto nl3 = N_bytes<endian::big, byte>::read ("0x000000");
        auto nb0 = N_bytes<endian::little, byte>::read ("0x");
        auto nb1 = N_bytes<endian::little, byte>::read ("0x00");
        auto nb3 = N_bytes<endian::little, byte>::read ("0x000000");
        
        EXPECT_EQ (++nl0, (N_bytes<endian::big, byte> {1}));
        EXPECT_EQ (++nl1, (N_bytes<endian::big, byte> {1}));
        EXPECT_EQ (++nl3, (N_bytes<endian::big, byte> {1}));
        EXPECT_EQ (++nb0, (N_bytes<endian::little, byte> {1}));
        EXPECT_EQ (++nb1, (N_bytes<endian::little, byte> {1}));
        EXPECT_EQ (++nb3, (N_bytes<endian::little, byte> {1}));
        
    }
    
}

