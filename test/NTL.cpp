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
    using order = endian::order;

    static const order orders [] {
        order::big,
        order::little
    };

    // -----------------------------------------------------------------------------
    // Negative export with nones must fail.
    // -----------------------------------------------------------------------------

    TEST (ExportBin, NegativeNonesThrows)
    {
        const Z negative {-1};

        for (const auto word_order : orders) {
            for (const auto byte_order : orders) {
                std::vector<byte> output (16);

                EXPECT_THROW (
                    math::number::NTL::export_bin (
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

    TEST (ExportBin, BufferTooSmall) {
        const Z value {dec_uint {"12345678901234567890"}};

        for (const auto word_order : orders) {
            for (const auto byte_order : orders) {
                std::vector<byte> output (7);

                EXPECT_THROW (
                    math::number::NTL::export_bin(
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
    // Exact-width export/import round trips
    //
    // These test the inverse relationship independently of the expected byte
    // representation tests above.
    // -----------------------------------------------------------------------------

    template<typename W>
    void round_trip (
        const Z &value,
        size_t words,
        order word_order,
        order byte_order,
        negativity neg
    ) {
        std::vector<W> output (words);

        math::number::NTL::export_bin (
            slice<W> {output},
            value.Value,
            word_order,
            byte_order,
            neg
        );

        const NTL::ZZ recovered = math::number::NTL::import_bin (
            slice<const W> {output},
            word_order,
            byte_order,
            neg
        );

        EXPECT_EQ (Z (recovered), value) << "expected " << Z (recovered) << " to match " << value;
    }

    void round_trips (const Z &value) {

        for (const auto word_order : orders) {
            for (const auto byte_order : orders) {
                if (value >= Z(0)) {
                    round_trip<byte> (
                        value, 32,
                        word_order, byte_order,
                        negativity::nones
                    );
                }

                round_trip<byte> (
                    value, 32,
                    word_order, byte_order,
                    negativity::twos
                );

                round_trip<byte> (
                    value, 32,
                    word_order, byte_order,
                    negativity::BC
                );

                if (value >= Z(0)) {
                    round_trip<uint16>(
                        value, 16,
                        word_order, byte_order,
                        negativity::nones
                    );
                }

                round_trip<uint16> (
                    value, 16,
                    word_order, byte_order,
                    negativity::twos
                );

                round_trip<uint16> (
                    value, 16,
                    word_order, byte_order,
                    negativity::BC
                );

                if (value >= Z(0)) {
                    round_trip<uint32>(
                        value, 8,
                        word_order, byte_order,
                        negativity::nones
                    );
                }

                round_trip<uint32> (
                    value, 8,
                    word_order, byte_order,
                    negativity::twos
                );

                round_trip<uint32> (
                    value, 8,
                    word_order, byte_order,
                    negativity::BC
                );

                if (value >= Z(0)) {
                    round_trip<uint64>(
                        value, 4,
                        word_order, byte_order,
                        negativity::nones
                    );
                }

                round_trip<uint64> (
                    value, 4,
                    word_order, byte_order,
                    negativity::twos
                );

                round_trip<uint64> (
                    value, 4,
                    word_order, byte_order,
                    negativity::BC
                );
            }
        }
    }

    // -----------------------------------------------------------------------------
    // Test data
    //
    // The canonical representation of every test number is a big-endian sequence
    // of bytes.  Nothing here is produced by Z, import_bin, or export_bin.
    // -----------------------------------------------------------------------------

    struct test_value {
        string_view decimal;
        std::vector<byte> bytes;
    };

    TEST (ImportExportBin, RoundTrip) {
        const Z values [] {
            Z (0),
            Z (1),
            Z (127),
            Z (128),
            Z (255),
            Z (256),
            Z (-1),
            Z (-2),
            Z (-127),
            Z (-128),
            Z (dec_uint {"12345678901234567890"}),
            Z (dec_int {"-12345678901234567890"}),
            Z (dec_uint {"340282366920938463463374607431768211455"}),
            Z (dec_int {"-340282366920938463463374607431768211455"}),
            Z (dec_int {"19088743"}),
            Z (dec_int {"81985529216486895"}),
            Z (dec_int {"889627103061277028662417"}),
            Z (dec_int {"18446744073709551616"})
        };

        for (const auto &value : values) round_trips (value);
    }

    const test_value test_values [] {
        {"0",    {0x00}},
        {"1",    {0x01}},
        {"2",    {0x02}},
        {"127",  {0x7F}},
        {"128",  {0x80}},
        {"255",  {0xFF}},
        {"256",  {0x01, 0x00}},
        {"257",  {0x01, 0x01}},
        {"32767",  {0x7F, 0xFF}},
        {"32768",  {0x80, 0x00}},
        {"65535",  {0xFF, 0xFF}},
        {"65536",  {0x01, 0x00, 0x00}},
        {
            "19088743",
            {0x01, 0x23, 0x45, 0x67}
        },
        {
            "81985529216486895",
            {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF}
        },
        {
            "889627103061277028662417",
            {
                0xBC, 0x62, 0xC8, 0x2A, 0x13,
                0xD4, 0x8A, 0xF0, 0x3C, 0x91
            }
        },
        {
            "12345678901234567890",
            {
                0xAB, 0x54, 0xA9, 0x8C,
                0xEB, 0x1F, 0x0A, 0xD2
            }
        },/*
        {
            "18446744073709551616",
            {
                0x01, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00,
                0x00
            }
        },
        {
            "340282366920938463463374607431768211455",
            {
                0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF,
                0xFF
            }
        },*/
    };


    // -----------------------------------------------------------------------------
    // Helpers
    // -----------------------------------------------------------------------------

    template<typename W>
    std::vector<W> bytes_to_words (
        std::span<const byte> bytes,
        order word_order,
        order byte_order
    ) {
        EXPECT_EQ (bytes.size () % sizeof (W), 0u);

        const size_t word_count = bytes.size () / sizeof (W);
        std::vector<W> result (word_count);

        for (size_t i = 0; i < word_count; ++i) {
            W word = 0;

            for (size_t j = 0; j < sizeof(W); ++j) {
                size_t byte_index;

                if (byte_order == order::big)
                    byte_index = j;
                else
                    byte_index = sizeof(W) - 1 - j;

                word |= static_cast<W>(bytes[i * sizeof (W) + byte_index])
                << (8 * j);
            }

            result [
                word_order == order::big
                ? i
                : word_count - 1 - i
            ] = word;
        }

        return result;
    }


    template<typename W>
    std::vector<byte>
    words_to_bytes (
        std::span<const W> words,
        order word_order,
        order byte_order
    ) {
        std::vector<byte> result (words.size () * sizeof (W));

        for (size_t i = 0; i < words.size (); ++i) {
            const W word = words [
                word_order == order::big ? i : words.size () - 1 - i
            ];

            for (size_t j = 0; j < sizeof(W); ++j) {
                const size_t byte_index =
                byte_order == order::big ? j : sizeof (W) - 1 - j;

                result[i * sizeof (W) + byte_index] =
                static_cast<byte> (word >> (8 * j));
            }
        }

        return result;
    }


    // Convert a positive canonical byte sequence to a fixed-width two's
    // complement or sign/magnitude representation.
    //
    // The input is already the magnitude in big-endian byte order.
    std::vector<byte>
    encode_positive (
        std::span<const byte> magnitude,
        size_t size,
        negativity neg
    ) {
        std::vector<byte> result (size, 0);

        assert (magnitude.size () < size);

        std::copy (
            magnitude.begin (),
            magnitude.end (),
            result.end () - magnitude.size ()
        );

        if (neg == negativity::nones)
            return result;

        return result;
    }


    std::vector<byte>
    encode_negative_twos (
        std::span<const byte> magnitude,
        size_t size
    ) {
        std::vector<byte> result (size, 0);

        assert (magnitude.size () < size);

        std::copy (
            magnitude.begin (),
            magnitude.end (),
            result.end () - magnitude.size ()
        );

        // Two's complement: invert and add one.
        for (auto &b : result)
            b = static_cast<byte> (~b);

        unsigned carry = 1;

        for (auto i = result.rbegin (); i != result.rend (); ++i) {
            const unsigned x = *i + carry;
            *i = static_cast<byte>(x);
            carry = x >> 8;
        }

        return result;
    }


    std::vector<byte>
    encode_negative_bc (
        std::span<const byte> magnitude,
        size_t size
    ) {
        std::vector<byte> result (size, 0);

        assert (magnitude.size () < size);

        std::copy (
            magnitude.begin (),
            magnitude.end (),
            result.end () - magnitude.size()
        );

        // Sign-and-magnitude uses the most significant bit as the sign.
        EXPECT_EQ (result.front () & 0x80, 0);

        result.front () |= 0x80;

        return result;
    }

    std::vector<byte> encode (
        std::span<const byte> magnitude,
        size_t size,
        bool negative,
        negativity neg
    ) {
        if (!negative)
            return encode_positive (magnitude, size, neg);

        switch (neg) {
            case negativity::twos:
                return encode_negative_twos (magnitude, size);

            case negativity::BC:
                return encode_negative_bc (magnitude, size);

            case negativity::nones:
                return {};
            default: return {};
        }
    }

    template<typename W>
    void check_import (
        const test_value &value,
        bool negative,
        size_t byte_count,
        order word_order,
        order byte_order,
        negativity neg
    ) {
        const auto magnitude = std::span {
            value.bytes
        };

        const auto canonical =
            encode (magnitude, byte_count, negative, neg);

        ASSERT_EQ(canonical.size () % sizeof (W), 0u);

        const auto words = bytes_to_words<W> (
            canonical,
            word_order,
            byte_order
        );

        const Z expected =
            negative
                ? Z (-dec_int {value.decimal})
                : Z (dec_int {value.decimal});

        const NTL::ZZ actual = math::number::NTL::import_bin (
            slice<const W> {words},
            word_order,
            byte_order,
            neg
        );

        EXPECT_EQ (Z (actual), expected);
    }

    // -----------------------------------------------------------------------------
    // Import tests
    // -----------------------------------------------------------------------------

    // Pattern values specifically exercise byte and word ordering.
    TEST (ImportBin, Values) {

        for (const auto &value : test_values) {
            for (const auto word_order : orders) {
                for (const auto byte_order : orders) {
                    check_import<byte> (
                        value, false, 16,
                        word_order, byte_order,
                        negativity::nones
                    );

                    check_import<uint16> (
                        value, false, 16,
                        word_order, byte_order,
                        negativity::nones
                    );

                    check_import<uint32> (
                        value, false, 16,
                        word_order, byte_order,
                        negativity::nones
                    );

                    check_import<uint64> (
                        value, false, 16,
                        word_order, byte_order,
                        negativity::nones
                    );
                }
            }
        }
    }

/*
    // -----------------------------------------------------------------------------
    // Two's-complement import
    // -----------------------------------------------------------------------------

    TEST (ImportBin, TwosComplementBoundaries) {
        struct test {
            string_view decimal;
            std::vector<byte> bytes;
        };

        const test cases[] {
            {"0",    {0x00}},
            {"1",    {0x01}},
            {"127",  {0x7F}},
            {"128",  {0x00, 0x80}},
            {"255",  {0x00, 0xFF}},
            {"32767", {0x7F, 0xFF}},

            {"-1",   {0xFF}},
            {"-2",   {0xFE}},
            {"-127", {0x81}},
            {"-128", {0x80}},

            {"-1",   {0xFF, 0xFF}},
            {"-2",   {0xFF, 0xFE}},
            {"-32767", {0x80, 0x01}},
            {"-32768", {0x80, 0x00}},
        };

        for (const auto &c : cases) {
            const bool negative = c.decimal.front() == '-';

            const string_view magnitude =
                negative
                    ? c.decimal.substr (1)
                    : c.decimal;

            const test_value value {
                magnitude,
                c.bytes
            };

            for (const auto word_order : orders) {
                for (const auto byte_order : orders) {
                    check_import<byte> (
                        value, negative, c.bytes.size(),
                        word_order, byte_order,
                        negativity::twos
                    );

                    if (c.bytes.size () % sizeof (uint16) == 0) {
                        check_import<uint16>(
                            value, negative, c.bytes.size(),
                            word_order, byte_order,
                            negativity::twos
                        );
                    }

                    if (c.bytes.size () % sizeof (uint32) == 0) {
                        check_import<uint32>(
                            value, negative, c.bytes.size(),
                            word_order, byte_order,
                            negativity::twos
                        );
                    }
                }
            }
        }
    }*/

    // -----------------------------------------------------------------------------
    // Sign-and-magnitude (BC) import
    // -----------------------------------------------------------------------------
/*
    TEST (ImportBin, BCBoundaries)
    {
        struct test {
            string_view magnitude;
            std::vector<byte> positive;
            std::vector<byte> negative;
        };

        const test cases [] {
            {
                "0",
                {0x00},
                {0x80}
            },
            {
                "1",
                {0x01},
                {0x81}
            },
            {
                "2",
                {0x02},
                {0x82}
            },
            {
                "127",
                {0x7F},
                {0xFF}
            },
            {
                "1",
                {0x00, 0x01},
                {0x80, 0x01}
            },
            {
                "2",
                {0x00, 0x02},
                {0x80, 0x02}
            },
            {
                "32767",
                {0x7F, 0xFF},
                {0xFF, 0xFF}
            },
        };

        for (const auto &c : cases) {
            for (const auto word_order : orders) {
                for (const auto byte_order : orders) {
                    const test_value positive {
                        c.magnitude,
                        c.positive
                    };

                    const test_value negative {
                        c.magnitude,
                        c.negative
                    };

                    check_import<byte> (
                        positive, false, c.positive.size (),
                        word_order, byte_order,
                        negativity::BC
                    );

                    check_import<byte> (
                        negative, true, c.negative.size (),
                        word_order, byte_order,
                        negativity::BC
                    );

                    if (c.positive.size () % sizeof (uint16) == 0) {
                        check_import<uint16>(
                            positive, false, c.positive.size (),
                            word_order, byte_order,
                            negativity::BC
                        );

                        check_import<uint16> (
                            negative, true, c.negative.size (),
                            word_order, byte_order,
                            negativity::BC
                        );
                    }
                }
            }
        }
    }*/


    // -----------------------------------------------------------------------------
    // Export tests
    // -----------------------------------------------------------------------------

    template<typename W>
    void check_export (
        const test_value& value,
        bool negative,
        size_t byte_count,
        order word_order,
        order byte_order,
        negativity neg
    ) {
        const Z expected =
            negative
            ? Z (-dec_int {value.decimal})
            : Z (dec_int {value.decimal});

        const auto expected_bytes =
        encode (value.bytes, byte_count, negative, neg);

        ASSERT_EQ (expected_bytes.size () % sizeof (W), 0u);

        std::vector<W> output (
            expected_bytes.size() / sizeof(W)
        );

        math::number::NTL::export_bin (
            slice<W> {output},
            expected.Value,
            word_order,
            byte_order,
            neg
        );

        const auto actual_bytes = words_to_bytes<W> (
            output,
            word_order,
            byte_order
        );

        EXPECT_EQ (actual_bytes, expected_bytes);

        // Independent inverse check.
        EXPECT_EQ (
            math::number::NTL::import_bin (
                slice<const W> {output},
                word_order,
                byte_order,
                neg
            ),
            expected.Value
        );
    }

    TEST (ExportBin, Values) {

        for (const auto &value : test_values) {
            for (const auto word_order : orders) {
                for (const auto byte_order : orders) {
                    check_export<byte> (
                        value, false, 16,
                        word_order, byte_order,
                        negativity::nones
                    );

                    check_export<uint16> (
                        value, false, 16,
                        word_order, byte_order,
                        negativity::nones
                    );

                    check_export<uint32> (
                        value, false, 16,
                        word_order, byte_order,
                        negativity::nones
                    );

                    check_export<uint64> (
                        value, false, 32,
                        word_order, byte_order,
                        negativity::nones
                    );
                }
            }
        }
    }


    // -----------------------------------------------------------------------------
    // Fixed-width sign extension / zero extension
    // -----------------------------------------------------------------------------

    TEST(ExportBin, NonesZeroExtends) {
        const Z one {1};

        for (size_t size : {1u, 2u, 4u, 8u, 16u}) {
            std::vector<byte> expected (size, 0);
            expected.front () = 1;

            std::vector<byte> output (size);

            // Use uint8_t here so the word representation itself cannot
            // obscure what is being tested.
            math::number::NTL::export_bin (
                slice<byte> {output},
                one.Value,
                endian::little,
                endian::little,
                negativity::nones
            );

            EXPECT_EQ (output, expected);
        }
    }


    TEST (ExportBin, TwosComplementSignExtends) {
        const Z minus_one {-1};

        for (const auto word_order : orders) {
            for (const auto byte_order : orders) {
                for (size_t size : {1u, 2u, 4u, 8u, 16u}) {
                    std::vector<byte> output (size, 0);

                    math::number::NTL::export_bin (
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

} // namespace
