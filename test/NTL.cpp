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

    template <typename Z>
    Z from_dec(std::string_view input) {
        if (input.empty ())
            throw std::invalid_argument("empty decimal string");

        bool negative = false;
        std::size_t pos = 0;

        if (input[0] == '-') {
            negative = true;
            pos = 1;

            if (pos == input.size())
                throw std::invalid_argument("invalid decimal string");
        }

        Z result = 0;

        for (; pos < input.size(); ++pos) {
            char c = input[pos];

            if (c < '0' || c > '9')
                throw std::invalid_argument("invalid decimal string");

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

            math::number::NTL::export_bin (
                slice<W> {encoded},
                z.Value,
                word_order,
                byte_order,
                neg
            );

            const NTL::ZZ recovered = math::number::NTL::import_bin (
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

    struct test_value {
        string_view Decimal;
        bytes Bytes;
    };

    TEST (ImportExportBin, RoundTrip) {
        const dec_int values [] {
            dec_int {"0"},
            dec_int {"1"},
            dec_int {"-1"},
            dec_int {"-2"},
            dec_int {"127"},
            dec_int {"128"},
            dec_int {"-127"},
            dec_int {"-128"},
            dec_int {"255"},
            dec_int {"256"},
            dec_int {"257"},
            dec_int {"32767"},
            dec_int {"32768"},
            dec_int {"65535"},
            dec_int {"65536"},
            dec_int {"-32767"},
            dec_int {"-32768"},
            dec_int {"-65535"},
            dec_int {"-65536"},
            dec_int {"12345678901234567890"},
            dec_int {"-12345678901234567890"},
            dec_int {"340282366920938463463374607431768211455"},
            dec_int {"-340282366920938463463374607431768211455"},
            dec_int {"19088743"},
            dec_int {"81985529216486895"},
            dec_int {"889627103061277028662417"},
            dec_int {"18446744073709551616"},
        };

        for (const auto &value : values) {
            round_trips<endian::big> (value);
            round_trips<endian::little> (value);
        }
    }

} // namespace
