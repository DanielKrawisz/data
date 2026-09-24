// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <gtest/gtest.h>

#include <array>
#include <cstdint>
#include <string_view>
#include <vector>

#include <data/concepts.hpp>
#include <data/numbers.hpp>

namespace data {

    using negativity = arithmetic::negativity;

    static const endian orders [] {
        endian::big,
        endian::little
    };

    // -----------------------------------------------------------------------------
    // Negative export with nones must fail.
    // -----------------------------------------------------------------------------

    TEST (NTL, NegativeNonesThrows)
    {
        const Z negative {-1};

        for (const auto word_order : orders) {
            for (const auto byte_order : orders) {
                std::vector<byte> output (16);

                EXPECT_THROW (
                    NTL::export_bin (
                        slice<byte> {output},
                        negative.Value,
                        word_order,
                        byte_order,
                        negativity::nones
                    ),
                    exception
                );
            }
        }
    }

    // -----------------------------------------------------------------------------
    // Output buffer too small
    // -----------------------------------------------------------------------------

    TEST (NTL, BufferTooSmall) {
        const Z value {dec_uint {"12345678901234567890"}};

        for (const auto word_order : orders) {
            for (const auto byte_order : orders) {
                std::vector<byte> output (7);

                EXPECT_THROW (
                    NTL::export_bin(
                        slice<byte> {output},
                        value.Value,
                        word_order,
                        byte_order,
                        negativity::nones
                    ),
                    exception
                );
            }
        }
    }


    // -----------------------------------------------------------------------------
    // Fixed-width sign extension / zero extension
    // -----------------------------------------------------------------------------

    TEST (NTL, NonesZeroExtends) {
        const Z one {1};

        for (size_t size : {1u, 2u, 4u, 8u, 16u}) {
            std::vector<byte> expected (size, 0);
            expected.front () = 1;

            std::vector<byte> output (size);

            // Use uint8_t here so the word representation itself cannot
            // obscure what is being tested.
            NTL::export_bin (
                slice<byte> {output},
                one.Value,
                endian::little,
                endian::little,
                negativity::nones
            );

            EXPECT_EQ (output, expected);
        }
    }


    TEST (NTL, TwosComplementSignExtends) {
        const Z minus_one {-1};

        for (const auto word_order : orders) {
            for (const auto byte_order : orders) {
                for (size_t size : {1u, 2u, 4u, 8u, 16u}) {
                    std::vector<byte> output (size, 0);

                    NTL::export_bin (
                        slice<byte> {output},
                        minus_one.Value,
                        word_order,
                        byte_order,
                        negativity::twos
                    );

                    EXPECT_EQ (
                        output,
                        std::vector<byte> (
                            size,
                            0xFF
                        )
                    );
                }
            }
        }
    }

    template <typename Z>
    Z from_dec (std::string_view input) {
        if (input.empty ())
            throw std::invalid_argument ("empty decimal string");

        bool negative = false;
        std::size_t pos = 0;

        if (input[0] == '-') {
            negative = true;
            pos = 1;

            if (pos == input.size())
                throw std::invalid_argument ("invalid decimal string");
        }

        Z result = 0;

        for (; pos < input.size(); ++pos) {
            char c = input[pos];

            if (c < '0' || c > '9')
                throw std::invalid_argument ("invalid decimal string");

            result *= 10;
            result += c - '0';
        }

        return negative ? Z (-result) : result;
    }

    // -----------------------------------------------------------------------------
    // Exact-width export/import round trips
    //
    // These test the inverse relationship independently of the expected byte
    // representation tests above.
    // -----------------------------------------------------------------------------

    template<typename W, endian word_order, negativity neg>
    void round_trip (
        const dec_int &value,
        endian byte_order
    ) {

        Z z (value);

        using compare_type = std::conditional_t<
            neg == negativity::nones,
            math::number::N_bytes<word_order, W>,
            math::number::Z_bytes<word_order, neg, W>>;

        compare_type compare = from_dec<compare_type> (value);

        size_t min_size = compare.size ();

        for (size_t words : std::vector<size_t> {min_size, min_size + 1, min_size + 5}) {

            compare_type encoded = compare_type::zero (words);

            NTL::export_bin (
                slice<W> {encoded},
                z.Value,
                word_order,
                byte_order,
                neg
            );

            const NTL::ZZ recovered = NTL::import_bin (
                slice<const W> {encoded},
                word_order,
                byte_order,
                neg
            );

            EXPECT_EQ (Z (recovered), z) << "expected " << Z (recovered) << " to match " << z;

            // reverse byte endian if we need to.
            for (W &w : encoded)
                if (byte_order == endian::little)
                    w = encoding::endian::native<W, endian::little>::to (w);
                else
                    w = encoding::endian::native<W, endian::big>::to (w);

            EXPECT_EQ ((bytestring<W> (extend (compare, words))), bytestring<W> (encoded)) << "for value " << value << "; read as " << compare << " encoded to " << encoded;
        }
    }

    template <endian word_order>
    void round_trips (const dec_int &value) {

        for (const auto byte_order : orders) {
            if (!is_negative (value))
                round_trip<byte, word_order, negativity::nones> (value, byte_order);

            round_trip<byte, word_order, negativity::twos> (value, byte_order);

            round_trip<byte, word_order, negativity::BC> (value, byte_order);

            if (!is_negative (value))
                round_trip<uint16, word_order, negativity::nones> (value, byte_order);

            round_trip<uint16, word_order, negativity::twos> (value, byte_order);

            round_trip<uint16, word_order, negativity::BC> (value, byte_order);

            if (!is_negative (value))
                round_trip<uint32, word_order, negativity::nones> (value, byte_order);

            round_trip<uint32, word_order, negativity::twos> (value, byte_order);

            round_trip<uint32, word_order, negativity::BC> (value, byte_order);

            if (!is_negative (value))
                round_trip<uint64, word_order, negativity::nones> (value, byte_order);

            round_trip<uint64, word_order, negativity::twos> (value, byte_order);

            round_trip<uint64, word_order, negativity::BC> (value, byte_order);
        }
    }

    void test_dec_to_hex (const dec_int &dec) {

        Z n_from_dec = Z::read (dec);

        // test that reading from dec works correctly with independent function.
        EXPECT_EQ (n_from_dec, from_dec<Z> (dec));

        // test that when we write to dec, we get the same as the input.
        EXPECT_EQ (dec, encoding::signed_decimal::write (n_from_dec));

        // now we read in Z_bytes types from dec.
        Z_bytes_little nlx = *encoding::signed_decimal::read<endian::little, negativity::twos, byte> (dec);
        Z_bytes_big nbx = *encoding::signed_decimal::read<endian::big, negativity::twos, byte> (dec);

        // we also convert to Z_bytes from the Z value that we read.
        Z_bytes_little nxl (n_from_dec);
        Z_bytes_big nxb (n_from_dec);

        // They ought to be equal to one another.
        EXPECT_EQ (nlx, nxl) << std::dec << "expected " << nlx << " to equal " << nxl;
        EXPECT_EQ (nbx, nxb) << std::dec << "expected " << nbx << " to equal " << nxb;

        EXPECT_EQ (n_from_dec, Z (nlx));
        EXPECT_EQ (n_from_dec, Z (nbx));

        string nlxx = encoding::signed_decimal::write (nlx);
        string nbxx = encoding::signed_decimal::write (nbx);
        EXPECT_EQ (dec, nlxx) << std::hex << "expected " << dec << " to equal " << nbxx;
        EXPECT_EQ (dec, nbxx) << std::hex << "expected " << dec << " to equal " << nbxx;

        auto nbxl = math::convert<Z_bytes_little> (nbx);
        EXPECT_EQ (nlx, nbxl) << "expected " << nlx << " == " << nbxl;

        auto nlxb = math::convert<Z_bytes_big> (nlx);
        EXPECT_EQ (nbx, nlxb) << "expected " << nbx << " == " << nlxb;

        auto nxh = encoding::hexidecimal::write<hex_case::lower> (n_from_dec);

        EXPECT_EQ (nxh, encoding::hexidecimal::write<hex_case::lower> (nlx));
        EXPECT_EQ (nxh, encoding::hexidecimal::write<hex_case::lower> (nbx));

        EXPECT_EQ (n_from_dec, Z (nxh));
        EXPECT_EQ (nlx, Z_bytes_little (nxh));
        EXPECT_EQ (nbx, Z_bytes_big (nxh));

    }

    TEST (NTL, RoundTrip) {
        const dec_int values [] {
            dec_int {"0"},
            dec_int {"1"},
            dec_int {"-1"},
            dec_int {"-2"},
            dec_int ("3"),
            dec_int ("-3"),
            dec_int {"127"},
            dec_int {"128"},
            dec_int {"-127"},
            dec_int {"-128"},
            dec_int {"255"},
            dec_int {"256"},
            dec_int {"257"},
            dec_int ("229"),
            dec_int ("767"),
            dec_int ("916"),
            dec_int ("1145"),
            dec_int ("-1145"),
            dec_int {"7493"},
            dec_int {"32767"},
            dec_int {"32768"},
            dec_int {"65535"},
            dec_int {"65536"},
            dec_int {"-32767"},
            dec_int {"-32768"},
            dec_int {"-65535"},
            dec_int {"-65536"},
            dec_int ("749384"),
            dec_int {"19088743"},
            dec_int ("483749384"),
            dec_int ("7206483749384"),
            dec_int {"81985529216486895"},
            dec_int {"18446744073709551616"},
            dec_int {"12345678901234567890"},
            dec_int {"-12345678901234567890"},
            dec_int ("24397842987206483749384"),
            dec_int {"889627103061277028662417"},
            dec_int ("98980987676898761029390303474536547398"),
            dec_int ("98980987676898761029390303474536547399"),
            dec_int ("98980987676898761029390303474536547400"),
            dec_int {"340282366920938463463374607431768211455"},
            dec_int {"-340282366920938463463374607431768211455"},
            dec_int {"115792089237316195423570985008687907852837564279074904382605163141518161494337"},
            dec_int {"115792089237316195423570985008687907853269984665640564039457584007908834671663"},

            dec_int (math::convert<dec_int> (hex::int2<hex_case::lower> {
                "0x0f00000a00aabbccddeeffffffffffffffff"})),

            dec_int (math::convert<dec_int> (hex::int2<hex_case::lower>
                {"0xf000000a00aabbccddeeffffffffffffffff"})),

            dec_int (math::convert<dec_int> (hex::uint<hex_case::lower> (
                "0x0fabcdef123456789012323454567600000a00aabbccddeeffffffffffffffff"
                "0000111122223333444455556666777788889999aaaabbbbccccddddeeeeffff"
                "0369cf258be147ad05af49e38d27c16b07e5c3a18f6d4b29092b4d6f81a3c5e7"
                "abcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcd"))),

            dec_int (math::convert<dec_int> (hex::uint<hex_case::lower> ("0xf0abcdef123456789012323454567600000a00aabbccddeeffffffffffffffff"
                "0000111122223333444455556666777788889999aaaabbbbccccddddeeeeffff"
                "0369cf258be147ad05af49e38d27c16b07e5c3a18f6d4b29092b4d6f81a3c5e7"
                "abcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcd")))
        };

        for (const auto &value : values) {
            round_trips<endian::big> (value);
            round_trips<endian::little> (value);
            test_dec_to_hex (value);
        }
    }

    template <std::integral T>
    void test_builtin_conversion (dec_int value) {
        T built_in;
        const auto [ptr, ec] = std::from_chars(
            value.data (),
            value.data () + value.size (),
            built_in
        );

        const Z from_builtin (built_in);
        const Z from_string (value);

        EXPECT_EQ (from_builtin, from_string);
        EXPECT_EQ (static_cast<T> (from_builtin), built_in);
    }

    template <std::integral... T>
    void test_builtin_conversions (dec_int value)
    {
        (test_builtin_conversion<T> (value), ...);
    }

    TEST (NTL, BuiltIn) {
        test_builtin_conversions<
            char, signed char, unsigned char,
            short int, short unsigned int, int, unsigned int,
            long int, long unsigned int, long long int, long long unsigned int> (dec_int {"0"});

        test_builtin_conversions<
            char, signed char, unsigned char,
            short int, short unsigned int, int, unsigned int,
            long int, long unsigned int, long long int, long long unsigned int> (dec_int {"1"});

        test_builtin_conversions<
            signed char, short int, int, long int, long long int> (dec_int {"-1"});

        test_builtin_conversions<
            char, signed char, unsigned char,
            short int, short unsigned int, int, unsigned int,
            long int, long unsigned int, long long int, long long unsigned int> (dec_int {"23"});

        test_builtin_conversions<
            char, signed char, unsigned char,
            short int, short unsigned int, int, unsigned int,
            long int, long unsigned int, long long int, long long unsigned int> (dec_int {"127"});

        test_builtin_conversions<
            signed char, short int, int, long int, long long int> (dec_int {"-45"});

        test_builtin_conversions<
            signed char, short int, int, long int, long long int> (dec_int {"-128"});

        test_builtin_conversions<byte, int16, uint16, int32, uint32, int64, uint64> (dec_int {"255"});

        test_builtin_conversions<int16, uint16, int32, uint32, int64, uint64> (dec_int {"32767"});

        test_builtin_conversions<int16, int32, int64> (dec_int {"-32768"});

        test_builtin_conversions<uint16, int32, uint32, int64, uint64> (dec_int {"65535"});

        test_builtin_conversions<int32, uint32, int64, uint64> (dec_int {"2147483647"});

        test_builtin_conversions<int32, int64> (dec_int {"-2147483648"});
    }

} // namespace
