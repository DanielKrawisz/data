// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include "gtest/gtest.h"
#include <iostream>

namespace data::math::number {

    template<endian::order r>
    math::N N_Bytes_to_N_stupid (const math::number::N_bytes<r, byte> &n) {
        math::N x {0};
        for (const byte &b : n.words ().reverse ()) {
            x <<= 8;
            x += b;
        }
        return x;
    }

    template<endian::order r>
    math::number::N_bytes<r, byte> inline N_to_N_Bytes_stupid (const math::N &n) {
        return math::number::N_bytes<r, byte>::read (encoding::hexidecimal::write<hex_case::lower> (n));
    }

    template <typename in> void N_Bytes_to_N (in x) {

        math::N n {x};

        N_bytes_big big {x};
        N_bytes_little little {x};

        N_bytes_big stupid_big = N_to_N_Bytes_stupid<endian::big> (n);
        N_bytes_little stupid_little = N_to_N_Bytes_stupid<endian::little> (n);

        EXPECT_EQ (stupid_big, big) << "expected " << std::hex << stupid_big << " to equal " << big << "; input = " << x;
        EXPECT_EQ (stupid_little, little) << "expected " << std::hex << stupid_little << " to equal " << little << "; input = " << x;

        math::N N_big = math::N (big);
        math::N N_little = math::N (little);

        math::N N_big_stupid = N_Bytes_to_N_stupid (big);
        math::N N_little_stupid = N_Bytes_to_N_stupid (little);

        EXPECT_EQ (N_big_stupid, N_big);
        EXPECT_EQ (N_little_stupid, N_little);

        EXPECT_EQ (N_big, n);
        EXPECT_EQ (N_little, n);

    }

    TEST (NBytesTest, TestNBytesToN) {

        N_Bytes_to_N<uint64> (0);
        N_Bytes_to_N<uint64> (1);
        N_Bytes_to_N<uint64> (3);
        N_Bytes_to_N<uint64> (767);
        N_Bytes_to_N<uint64> (7439);
        N_Bytes_to_N<string> ("0x0f00000a00aabbccddeeffffffffffffffff");
        N_Bytes_to_N<string> ("0xf000000a00aabbccddeeffffffffffffffff");

    }

    TEST (NBytesTest, TestStringToNBytes) {

        EXPECT_THROW ((N_bytes<endian::big, byte>::read ("")), std::invalid_argument);
        EXPECT_THROW ((N_bytes<endian::big, byte>::read ("a")), std::invalid_argument);
        EXPECT_THROW ((N_bytes<endian::big, byte>::read ("-")), std::invalid_argument);
        EXPECT_THROW ((N_bytes<endian::big, byte>::read ("-1")), std::invalid_argument);
        EXPECT_THROW ((N_bytes<endian::big, byte>::read ("01")), std::invalid_argument);
        EXPECT_THROW ((N_bytes<endian::big, byte>::read ("0x1")), std::invalid_argument);
        
        EXPECT_THROW ((N_bytes<endian::little, byte>::read ("")), std::invalid_argument);
        EXPECT_THROW ((N_bytes<endian::little, byte>::read ("a")), std::invalid_argument);
        EXPECT_THROW ((N_bytes<endian::little, byte>::read ("-")), std::invalid_argument);
        EXPECT_THROW ((N_bytes<endian::little, byte>::read ("-1")), std::invalid_argument);
        EXPECT_THROW ((N_bytes<endian::little, byte>::read ("01")), std::invalid_argument);
        EXPECT_THROW ((N_bytes<endian::little, byte>::read ("0x1")), std::invalid_argument);

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
    
    TEST (NBytesTest, TestNBytesToString) {
        
        EXPECT_EQ (encoding::decimal::write (N_bytes<endian::big, byte> {1}), std::string {"1"});
        EXPECT_EQ (encoding::decimal::write (N_bytes<endian::little, byte> {1}), std::string {"1"});
        EXPECT_EQ (encoding::decimal::write (N_bytes<endian::big, byte> {23}), std::string {"23"});
        EXPECT_EQ (encoding::decimal::write (N_bytes<endian::little, byte> {23}), std::string {"23"});
        EXPECT_EQ (encoding::decimal::write (N_bytes<endian::big, byte> {"5704566599993321"}), std::string {"5704566599993321"});
        EXPECT_EQ (encoding::decimal::write (N_bytes<endian::little, byte> {"5704566599993321"}), std::string {"5704566599993321"});
        
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

