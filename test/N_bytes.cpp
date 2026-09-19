// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include <data/list.hpp>

#include <gtest/gtest.h>

namespace data::math::number {

    TEST (NBytes, StringToNBytes) {

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

    TEST (NBytes, NBytesToHexString) {

        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N_bytes<endian::big, byte> {"0"}), std::string {"0x"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N_bytes<endian::big, byte> {"127"}), std::string {"0x7f"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N_bytes<endian::big, byte> {"128"}), std::string {"0x80"});

        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N_bytes<endian::little, byte> {"0"}), std::string {"0x"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N_bytes<endian::little, byte> {"127"}), std::string {"0x7f"});
        EXPECT_EQ (encoding::hexidecimal::write<hex_case::lower> (N_bytes<endian::little, byte> {"128"}), std::string {"0x80"});

    }

    TEST (NBytes, ZeroSize) {

        EXPECT_EQ ((N_bytes<endian::big, byte> {"0x"}.size ()), 0);
        EXPECT_EQ ((N_bytes<endian::big, byte> {"0x00"}.size ()), 1);
        EXPECT_EQ ((N_bytes<endian::big, byte> {"0x000000"}.size ()), 3);

        EXPECT_EQ ((N_bytes<endian::little, byte> {"0x"}.size ()), 0);
        EXPECT_EQ ((N_bytes<endian::little, byte> {"0x00"}.size ()), 1);
        EXPECT_EQ ((N_bytes<endian::little, byte> {"0x000000"}.size ()), 3);

    }

    TEST (NBytes, ZeroAndNegativeNBytes) {

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

    template <endian o, std::unsigned_integral word>
    void test_N_Bytes_to_string_decimal () {
        EXPECT_EQ (encoding::decimal::write (N_bytes<o, word> {1}), std::string {"1"});
        EXPECT_EQ (encoding::decimal::write (N_bytes<o, word> {23}), std::string {"23"});
        EXPECT_EQ (encoding::decimal::write (N_bytes<o, word> {767}), std::string {"767"});
        EXPECT_EQ (encoding::decimal::write (N_bytes<o, word> {"5704566599993321"}), std::string {"5704566599993321"});
    }

    TEST (NBytes, NBytesToString) {

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

    TEST (NBytes, ConstructFromBuiltIn) {
        EXPECT_EQ ((bytes (N_bytes_big {byte (0)})), (bytes {}));
        EXPECT_EQ ((bytes (N_bytes_big {int8 (0)})), (bytes {}));
        EXPECT_EQ ((bytes (N_bytes_big {int64 (0)})), (bytes {}));
        EXPECT_EQ ((bytes (N_bytes_big {uint64 (0)})), (bytes {}));

        EXPECT_EQ ((bytes (N_bytes_big {byte (1)})), (bytes {0x01}));
        EXPECT_EQ ((bytes (N_bytes_big {int8 (1)})), (bytes {0x01}));
        EXPECT_EQ ((bytes (N_bytes_big {int64 (1)})), (bytes {0x01}));
        EXPECT_EQ ((bytes (N_bytes_big {uint64 (1)})), (bytes {0x01}));

        EXPECT_EQ ((bytes (N_bytes_big {byte (0x80)})), (bytes {0x80}));
        EXPECT_EQ ((bytes (N_bytes_big {uint64 (0x80)})), (bytes {0x80}));

        EXPECT_EQ ((bytes (N_bytes_big {byte (0xff)})), (bytes {0xff}));
        EXPECT_EQ ((bytes (N_bytes_big {uint64 (0xff)})), (bytes {0xff}));

        EXPECT_EQ ((bytes (N_bytes_big {uint16 (0x0100)})), (bytes {0x01, 0x00}));
        EXPECT_EQ ((bytes (N_bytes_big {uint64 (0x0100)})), (bytes {0x01, 0x00}));

        EXPECT_EQ ((bytes (N_bytes_big {uint16 (0x0102)})), (bytes {0x01, 0x02}));
        EXPECT_EQ ((bytes (N_bytes_big {uint64 (0x0102)})), (bytes {0x01, 0x02}));

        EXPECT_EQ ((bytes (N_bytes_big {uint16 (0x8003)})), (bytes {0x80, 0x03}));
        EXPECT_EQ ((bytes (N_bytes_big {uint64 (0x8003)})), (bytes {0x80, 0x03}));
    }

    template <endian r>
    struct test_bit_shift {
        test_bit_shift (string num, int shift) {
            EXPECT_EQ ((N_bytes<r, byte> (num) >> shift), (N_bytes<r, byte> (num) << -shift));
            EXPECT_EQ ((N_bytes<r, byte> (num) << shift), (N_bytes<r, byte> (num) >> -shift));
            EXPECT_EQ ((N_bytes<r, byte> (num) << shift >> shift), (N_bytes<r, byte> (num)));
            EXPECT_EQ ((N::read (num) >> shift), (N (N_bytes<r, byte> (num) >> shift)));
            EXPECT_EQ ((N::read (num) << shift), (N (N_bytes<r, byte> (num) << shift)));
        }
    };

    TEST (NBytes, NBitShift) {
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

    TEST (NBytes, Increment) {

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

