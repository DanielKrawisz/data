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
                if (value >= Z (0)) {
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

                if (value >= Z (0)) {
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
        string_view Decimal;
        bytes Bytes;
    };

    TEST (ImportExportBin, RoundTrip) {
        const Z values [] {
            Z (0),
            Z (1),
            Z (127),
            Z (128),
            Z (255),
            Z (256),
            Z (257),
            Z {dec_int {"32767"}},
            Z {dec_int {"32768"}},
            Z {dec_int {"65535"}},
            Z {dec_int {"65536"}},
            Z (-1),
            Z (-2),
            Z (-127),
            Z (-128),
            Z {dec_int {"-32767"}},
            Z {dec_int {"-32768"}},
            Z {dec_int {"-65535"}},
            Z {dec_int {"-65536"}},
            Z (dec_uint {"12345678901234567890"}),
            Z (dec_int {"-12345678901234567890"}),
            Z (dec_uint {"340282366920938463463374607431768211455"}),
            Z (dec_int {"-340282366920938463463374607431768211455"}),
            Z (dec_int {"19088743"}),
            Z (dec_int {"81985529216486895"}),
            Z (dec_int {"889627103061277028662417"}),
            Z (dec_int {"18446744073709551616"}),
        };

        for (const auto &value : values) round_trips (value);
    }

} // namespace
