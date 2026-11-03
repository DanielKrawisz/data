// Copyright (c) 2026 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include <data/list.hpp>

#include <gtest/gtest.h>

namespace data::math {

    template <
        negativity ToNeg,
        negativity FromNeg,
        std::unsigned_integral InWord,
        std::unsigned_integral OutWord
    >
    void expect_port_bits (
        const bytestring<InWord> & input,
        const bytestring<OutWord> & expected
    ) {
        bytestring<OutWord> output (expected.size ());

        def::port_bits<ToNeg, FromNeg> (
            output.begin (),
            output.end (),
            input.begin (),
            input.end ()
        );

        EXPECT_EQ (output, expected)
            << "test port_bits; input size = " << input
            << "; output size = " << output.size ();
    }

    TEST (Conversions, twos_negative_one_widening) {

        bytestring<uint8_t> input {
            0xff
        };

        // 8-bit two's complement -1 -> 16-bit two's complement -1.
        expect_port_bits<
            negativity::twos,
            negativity::twos
        > (
            input,
            bytestring<uint16_t> {0xffff}
        );

        // 8-bit two's complement -1 -> 32-bit two's complement -1.
        expect_port_bits<
            negativity::twos,
            negativity::twos
        > (
            input,
            bytestring<uint32_t> {0xffffffff}
        );

        // 8-bit two's complement -1 -> 16-bit BC -1.
        expect_port_bits<
            negativity::BC,
            negativity::twos
        > (
            input,
            bytestring<uint16_t> {0x8001}
        );

        // 8-bit two's complement -1 -> 32-bit BC -1.
        expect_port_bits<
            negativity::BC,
            negativity::twos
        > (
            input,
            bytestring<uint32_t> {0x80000001}
        );
    }

    TEST (Conversions, twos_negative_one_longer_output) {

        // 8-bit words.
        {
            bytestring<uint8_t> input {0xff};

            expect_port_bits<
                negativity::twos,
                negativity::twos
            > (input, bytestring<uint8_t> {0xff, 0xff});

            expect_port_bits<
                negativity::twos,
                negativity::twos
            > (input, bytestring<uint8_t> {
                0xff, 0xff, 0xff, 0xff
            });

            expect_port_bits<
                negativity::twos,
                negativity::twos
            > (input, bytestring<uint8_t> {
                0xff, 0xff, 0xff, 0xff,
                0xff, 0xff, 0xff, 0xff
            });
        }

        // 16-bit words.
        {
            bytestring<uint16_t> input {0xffff};

            expect_port_bits<
                negativity::twos,
                negativity::twos
            > (input, bytestring<uint16_t> {0xffff, 0xffff});

            expect_port_bits<
                negativity::twos,
                negativity::twos
            > (input, bytestring<uint16_t> {
                0xffff, 0xffff, 0xffff, 0xffff
            });

            expect_port_bits<
                negativity::twos,
                negativity::twos
            > (input, bytestring<uint16_t> {
                0xffff, 0xffff, 0xffff, 0xffff,
                0xffff, 0xffff, 0xffff, 0xffff
            });
        }

        // 32-bit words.
        {
            bytestring<uint32_t> input {0xffffffff};

            expect_port_bits<
                negativity::twos,
                negativity::twos
            > (input, bytestring<uint32_t> {
                0xffffffff, 0xffffffff
            });

            expect_port_bits<
                negativity::twos,
                negativity::twos
            > (input, bytestring<uint32_t> {
                0xffffffff, 0xffffffff,
                0xffffffff, 0xffffffff
            });

            expect_port_bits<
                negativity::twos,
                negativity::twos
            > (input, bytestring<uint32_t> {
                0xffffffff, 0xffffffff,
                0xffffffff, 0xffffffff,
                0xffffffff, 0xffffffff,
                0xffffffff, 0xffffffff
            });
        }
    }

    template <
        Word ToWord,
        negativity ToNeg,
        negativity FromNeg,
        Word FromWord>
    void test_result_min_size (
        const bytestring<FromWord> & input,
        const size_t expected)
    {
        auto result = def::result_min_size<ToWord, ToNeg, FromNeg>
            (input.begin (), input.end ());

        EXPECT_EQ (result, expected)
            << "input = " << input
            << "; expected minimum size = " << expected
            << "; actual minimum size = " << result;
    }

    TEST (Conversions, negative_128) {

        expect_port_bits<
            negativity::BC,
            negativity::twos> (
            bytestring<byte> {0x80},
            bytestring<byte> {0x80, 0x80});

        expect_port_bits<
            negativity::BC,
            negativity::twos> (
            bytestring<byte> {0x80, 0xff},
            bytestring<byte> {0x80, 0x80});

        expect_port_bits<
            negativity::BC,
            negativity::twos> (
            bytestring<byte> {0x80, 0x00},
            bytestring<byte> {0x80, 0x00});

        // -128 in different input widths
        test_result_min_size<uint8_t, negativity::BC, negativity::twos>
        (bytestring<byte> {0x80}, 2);

        test_result_min_size<uint8_t, negativity::BC, negativity::twos>
        (bytestring<byte> {0x80, 0xff}, 2);

        test_result_min_size<uint8_t, negativity::BC, negativity::twos>
        (bytestring<byte> {0x80, 0xff, 0xff, 0xff}, 2);

        test_result_min_size<uint8_t, negativity::BC, negativity::twos>
        (bytestring<byte> {0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, 2);

        expect_port_bits<
            negativity::BC,
            negativity::twos> (
            bytestring<unsigned short> {0xff80},
            bytestring<byte> {0x80, 0x80});

        expect_port_bits<
            negativity::BC,
            negativity::twos> (
            bytestring<unsigned short> {0xff80, 0xffff},
            bytestring<byte> {0x80, 0x80});

        expect_port_bits<
            negativity::BC,
            negativity::twos> (
            bytestring<unsigned short> {0x0080, 0x0000},
            bytestring<byte> {0x80, 0x00});

    }

    TEST (Conversions, Input256) {
        // uint8_t input
        expect_port_bits<negativity::nones, negativity::nones> (
            bytestring<uint8_t> {0x00, 0x01},
            bytestring<uint8_t> {0x00, 0x01});

        expect_port_bits<negativity::nones, negativity::nones> (
            bytestring<uint8_t> {0x00, 0x01},
            bytestring<uint16_t> {0x0100});

        // uint16_t input
        expect_port_bits<negativity::nones, negativity::nones> (
            bytestring<uint16_t> {0x0100},
            bytestring<uint8_t> {0x00, 0x01});

        expect_port_bits<negativity::nones, negativity::nones> (
            bytestring<uint16_t> {0x0100},
            bytestring<uint16_t> {0x0100});

        // uint32_t input
        expect_port_bits<negativity::nones, negativity::nones> (
            bytestring<uint32_t> {0x00000100},
            bytestring<uint8_t> {0x00, 0x01});

        expect_port_bits<negativity::nones, negativity::nones> (
            bytestring<uint32_t> {0x00000100},
            bytestring<uint16_t> {0x0100});

        // uint64_t input
        expect_port_bits<negativity::nones, negativity::nones> (
            bytestring<uint64_t> {0x0000000000000100},
            bytestring<uint8_t> {0x00, 0x01});

        expect_port_bits<negativity::nones, negativity::nones> (
            bytestring<uint64_t> {0x0000000000000100},
            bytestring<uint16_t> {0x0100});

        // uint8_t input: minimum output size is 2 bytes.
        test_result_min_size<uint8_t, negativity::nones, negativity::nones>
            (bytestring<uint8_t> {0x00, 0x01}, 2);

        test_result_min_size<uint16_t, negativity::nones, negativity::nones>
            (bytestring<uint8_t> {0x00, 0x01}, 1);

        test_result_min_size<uint32_t, negativity::nones, negativity::nones>
            (bytestring<uint8_t> {0x00, 0x01}, 1);

        test_result_min_size<uint64_t, negativity::nones, negativity::nones>
            (bytestring<uint8_t> {0x00, 0x01}, 1);


        // uint16_t input: minimum output size is 1 word.
        test_result_min_size<uint8_t, negativity::nones, negativity::nones>
            (bytestring<uint16_t> {0x0100}, 2);

        test_result_min_size<uint16_t, negativity::nones, negativity::nones>
            (bytestring<uint16_t> {0x0100}, 1);

        test_result_min_size<uint32_t, negativity::nones, negativity::nones>
            (bytestring<uint16_t> {0x0100}, 1);

        test_result_min_size<uint64_t, negativity::nones, negativity::nones>
            (bytestring<uint16_t> {0x0100}, 1);


        // uint32_t input: minimum output size is 1 word.
        test_result_min_size<uint8_t, negativity::nones, negativity::nones>
            (bytestring<uint32_t> {0x00000100}, 2);

        test_result_min_size<uint16_t, negativity::nones, negativity::nones>
            (bytestring<uint32_t> {0x00000100}, 1);

        test_result_min_size<uint32_t, negativity::nones, negativity::nones>
            (bytestring<uint32_t> {0x00000100}, 1);

        test_result_min_size<uint64_t, negativity::nones, negativity::nones>
            (bytestring<uint32_t> {0x00000100}, 1);


        // uint64_t input: minimum output size is 1 word.
        test_result_min_size<uint8_t, negativity::nones, negativity::nones>
            (bytestring<uint64_t> {0x0000000000000100}, 2);

        test_result_min_size<uint16_t, negativity::nones, negativity::nones>
            (bytestring<uint64_t> {0x0000000000000100}, 1);

        test_result_min_size<uint32_t, negativity::nones, negativity::nones>
            (bytestring<uint64_t> {0x0000000000000100}, 1);

        test_result_min_size<uint64_t, negativity::nones, negativity::nones>
            (bytestring<uint64_t> {0x0000000000000100}, 1);
    }

    TEST (Conversions, InputNeg65536) {
        // uint8_t input
        expect_port_bits<negativity::BC, negativity::twos> (
            bytestring<uint8_t> {0x00, 0x80},
            bytestring<byte> {0x00, 0x80, 0x80});

        // uint8_t input
        expect_port_bits<negativity::BC, negativity::twos> (
            bytestring<uint8_t> {0x00, 0x00, 0xff},
            bytestring<byte> {0x00, 0x00, 0x01, 0x80});

        // uint8_t input
        expect_port_bits<negativity::BC, negativity::twos> (
            bytestring<uint8_t> {0x00, 0x00, 0xff},
            bytestring<byte> {0x00, 0x00, 0x81});

        // uint16_t input
        expect_port_bits<negativity::BC, negativity::twos> (
            bytestring<uint16_t> {0x0000, 0xffff},
            bytestring<byte> {0x00, 0x00, 0x01, 0x80});

        // uint32_t input
        expect_port_bits<negativity::BC, negativity::twos> (
            bytestring<uint32_t> {0xffff0000},
            bytestring<byte> {0x00, 0x00, 0x01, 0x80});

        // uint64_t input
        expect_port_bits<negativity::BC, negativity::twos> (
            bytestring<uint64_t> {0xffffffffffff0000},
            bytestring<byte> {0x00, 0x00, 0x01, 0x80});

        // uint8_t input
        test_result_min_size<uint8_t, negativity::BC, negativity::twos>
        (bytestring<uint8_t> {0x00, 0x00, 0xff}, 3);

        test_result_min_size<uint16_t, negativity::BC, negativity::twos>
        (bytestring<uint8_t> {0x00, 0x00, 0xff}, 2);

        test_result_min_size<uint32_t, negativity::BC, negativity::twos>
        (bytestring<uint8_t> {0x00, 0x00, 0xff}, 1);

        test_result_min_size<uint64_t, negativity::BC, negativity::twos>
        (bytestring<uint8_t> {0x00, 0x00, 0xff}, 1);


        // uint16_t input
        test_result_min_size<uint8_t, negativity::BC, negativity::twos>
        (bytestring<uint16_t> {0x0000, 0xffff}, 3);

        test_result_min_size<uint16_t, negativity::BC, negativity::twos>
        (bytestring<uint16_t> {0x0000, 0xffff}, 2);

        test_result_min_size<uint32_t, negativity::BC, negativity::twos>
        (bytestring<uint16_t> {0x0000, 0xffff}, 1);

        test_result_min_size<uint64_t, negativity::BC, negativity::twos>
        (bytestring<uint16_t> {0x0000, 0xffff}, 1);


        // uint32_t input
        test_result_min_size<uint8_t, negativity::BC, negativity::twos>
        (bytestring<uint32_t> {0xffff0000}, 3);

        test_result_min_size<uint16_t, negativity::BC, negativity::twos>
        (bytestring<uint32_t> {0xffff0000}, 2);

        test_result_min_size<uint32_t, negativity::BC, negativity::twos>
        (bytestring<uint32_t> {0xffff0000}, 1);

        test_result_min_size<uint64_t, negativity::BC, negativity::twos>
        (bytestring<uint32_t> {0xffff0000}, 1);


        // uint64_t input
        test_result_min_size<uint8_t, negativity::BC, negativity::twos>
        (bytestring<uint64_t> {0xffffffffffff0000}, 3);

        test_result_min_size<uint16_t, negativity::BC, negativity::twos>
        (bytestring<uint64_t> {0xffffffffffff0000}, 2);

        test_result_min_size<uint32_t, negativity::BC, negativity::twos>
        (bytestring<uint64_t> {0xffffffffffff0000}, 1);

        test_result_min_size<uint64_t, negativity::BC, negativity::twos>
        (bytestring<uint64_t> {0xffffffffffff0000}, 1);
    }


    template <typename... T> struct type_list {};

    // Push front
    template <typename List, typename T>
    struct push_front;

    template <typename... Ts, typename T>
    struct push_front<type_list<Ts...>, T>
    {
        using type = type_list<T, Ts...>;
    };

    template <typename List, typename T>
    using push_front_t = typename push_front<List, T>::type;

    template <typename T, typename List>
    struct contains;

    template <typename T, typename... Ts>
    struct contains<T, type_list<Ts...>>
        : std::bool_constant<(std::same_as<T, Ts> || ...)>
    {};

    template <typename T, typename List>
    inline constexpr bool contains_v = contains<T, List>::value;

    // Union

    template <typename A, typename B>
    struct union_types;

    template <typename... As, typename... Bs>
    struct union_types<type_list<As...>, type_list<Bs...>>
    {
    private:
        template <typename Result, typename... Ts>
        struct append_unique;

        template <typename Result>
        struct append_unique<Result>
        {
            using type = Result;
        };

        template <typename... Rs, typename T, typename... Ts>
        struct append_unique<type_list<Rs...>, T, Ts...>
            : append_unique<
                std::conditional_t<
                    contains_v<T, type_list<Rs...>>,
                    type_list<Rs...>,
                    type_list<Rs..., T>
                >,
                Ts...
            >
        {};

    public:
        using type = typename append_unique<
            type_list<As...>, Bs...
        >::type;
    };

    template <typename A, typename B>
    using union_t = typename union_types<A, B>::type;


    // Difference: A minus B

    template <typename A, typename B>
    struct difference;

    template <typename B>
    struct difference<type_list<>, B>
    {
        using type = type_list<>;
    };

    template <typename T, typename... Ts, typename B>
    struct difference<type_list<T, Ts...>, B>
    {
    private:
        using tail = typename difference<type_list<Ts...>, B>::type;

    public:
        using type = std::conditional_t<
            contains_v<T, B>,
            tail,
            std::conditional_t<
                contains_v<T, tail>,
                tail,
                push_front_t<tail, T>
            >
        >;
    };

    template <typename A, typename B>
    using difference_t = typename difference<A, B>::type;

    template <typename InputTypes, typename InvalidOutputTypes>
    struct test_case;

    template <typename... InputTypes, typename... InvalidOutputTypes>
    struct test_case<type_list<InputTypes...>, type_list<InvalidOutputTypes...>> {
        test_case (const dec_int &value);
    };

    TEST (Conversions, Basic) {

        using no_types = type_list<>;

        using all_types = type_list<

            byte,
            uint16,
            uint32,
            uint64,

            int8,
            int16,
            int32,
            int64,

            N_bytes<endian::little, byte>,
            Z_bytes<endian::little, byte>,
            Z_bytes_BC<endian::little, byte>,

            N_bytes<endian::little, uint16>,
            Z_bytes<endian::little, uint16>,
            Z_bytes_BC<endian::little, uint16>,

            N_bytes<endian::little, uint32>,
            Z_bytes<endian::little, uint32>,
            Z_bytes_BC<endian::little, uint32>,

            N_bytes<endian::little, uint64>,
            Z_bytes<endian::little, uint64>,
            Z_bytes_BC<endian::little, uint64>,

            ubound<endian::little, 1, byte>,
            ubound<endian::little, 2, byte>,
            ubound<endian::little, 3, byte>,
            ubound<endian::little, 4, byte>,
            ubound<endian::little, 8, byte>,

            bound<endian::little, 1, byte>,
            bound<endian::little, 2, byte>,
            bound<endian::little, 3, byte>,
            bound<endian::little, 4, byte>,
            bound<endian::little, 8, byte>,

            ubound<endian::little, 1, uint16>,
            ubound<endian::little, 2, uint16>,
            ubound<endian::little, 3, uint16>,
            ubound<endian::little, 4, uint16>,

            bound<endian::little, 1, uint16>,
            bound<endian::little, 2, uint16>,
            bound<endian::little, 3, uint16>,
            bound<endian::little, 4, uint16>,

            ubound<endian::little, 1, uint32>,
            ubound<endian::little, 2, uint32>,
            ubound<endian::little, 3, uint32>,

            bound<endian::little, 1, uint32>,
            bound<endian::little, 2, uint32>,
            bound<endian::little, 3, uint32>,

            ubound<endian::little, 1, uint64>,
            ubound<endian::little, 2, uint64>,

            bound<endian::little, 1, uint64>,
            bound<endian::little, 2, uint64>,

            N, Z>;

        // 0 to 127
        test_case<
            all_types,
            no_types> {dec_int {"0"}};

        test_case<
            all_types,
            no_types> {dec_int {"1"}};

        test_case<
            all_types,
            no_types> {dec_int {"2"}};

        test_case<
            all_types,
            no_types> {dec_int {"3"}};

        test_case<
            all_types,
            no_types> {dec_int {"23"}};

        test_case<
            all_types,
            no_types> {dec_int {"28"}};

        test_case<
            all_types,
            no_types> {dec_int {"113"}};

        test_case<
            all_types,
            no_types> {dec_int {"127"}};

        using signed_types = type_list<

            int8,
            int16,
            int32,
            int64,

            Z_bytes<endian::little, byte>,
            Z_bytes_BC<endian::little, byte>,

            Z_bytes<endian::little, uint16>,
            Z_bytes_BC<endian::little, uint16>,

            Z_bytes<endian::little, uint32>,
            Z_bytes_BC<endian::little, uint32>,

            Z_bytes<endian::little, uint64>,
            Z_bytes_BC<endian::little, uint64>,

            bound<endian::little, 1, byte>,
            bound<endian::little, 2, byte>,
            bound<endian::little, 3, byte>,
            bound<endian::little, 4, byte>,
            bound<endian::little, 8, byte>,

            bound<endian::little, 1, uint16>,
            bound<endian::little, 2, uint16>,
            bound<endian::little, 3, uint16>,
            bound<endian::little, 4, uint16>,

            bound<endian::little, 1, uint32>,
            bound<endian::little, 2, uint32>,
            bound<endian::little, 3, uint32>,

            bound<endian::little, 1, uint64>,
            bound<endian::little, 2, uint64>,

            Z>;

        using unsigned_types = difference_t<all_types, signed_types>;

        // -1 to -128
        test_case<
            signed_types,
            unsigned_types> {dec_int {"-1"}};

        test_case<
            signed_types,
            unsigned_types> {dec_int {"-2"}};

        test_case<
            signed_types,
            unsigned_types> {dec_int {"-3"}};

        test_case<
            signed_types,
            unsigned_types> {dec_int {"-45"}};

        test_case<
            signed_types,
            unsigned_types> {dec_int {"-76"}};

        test_case<
            signed_types,
            unsigned_types> {dec_int {"-127"}};

        test_case<
            signed_types,
            unsigned_types> {dec_int {"-128"}};

        // types that must reject anything higher than max int8.
        using reject_int_8_greater = type_list<int8, bound<endian::little, 1, byte>>;

        // types that accept something higher than max int 8.
        using accept_int_8_greater = difference_t<all_types, reject_int_8_greater>;

        // 128 to 255
        test_case<
            accept_int_8_greater,
            reject_int_8_greater> {dec_int {"128"}};

        test_case<
            accept_int_8_greater,
            reject_int_8_greater> {dec_int {"229"}};

        test_case<
            accept_int_8_greater,
            reject_int_8_greater> {dec_int {"255"}};

        // types that must reject anything lower than min int8.
        using reject_int_8_lesser = union_t<reject_int_8_greater, unsigned_types>;

        using accept_int_8_lesser = difference_t<all_types, reject_int_8_lesser>;

        // -129 to -32768
        test_case<
            accept_int_8_lesser,
            reject_int_8_greater> {dec_int {"-129"}};

        test_case<
            accept_int_8_lesser,
            reject_int_8_greater> {dec_int {"-235"}};

        test_case<
            accept_int_8_lesser,
            reject_int_8_greater> {dec_int {"-1145"}};

        test_case<
            accept_int_8_lesser,
            reject_int_8_greater> {dec_int {"-32767"}};

        test_case<
            accept_int_8_lesser,
            reject_int_8_greater> {dec_int {"-32768"}};

        // types that must reject anything higher than max uint8.
        using reject_uint_8_greater = union_t<reject_int_8_greater, type_list<byte, ubound<endian::little, 1, byte>>>;

        using accept_uint_8_greater = difference_t<all_types, reject_uint_8_greater>;

        // 256 to 32767
        test_case<
            accept_uint_8_greater,
            reject_uint_8_greater> {dec_int {"256"}};

        test_case<
            accept_uint_8_greater,
            reject_uint_8_greater> {dec_int {"767"}};

        test_case<
            accept_uint_8_greater,
            reject_uint_8_greater> {dec_int {"916"}};

        test_case<
            accept_uint_8_greater,
            reject_uint_8_greater> {dec_int {"1000"}};

        test_case<
            accept_uint_8_greater,
            reject_uint_8_greater> {dec_int {"1145"}};

        test_case<
            accept_uint_8_greater,
            reject_uint_8_greater> {dec_int {"7493"}};

        test_case<
            accept_uint_8_greater,
            reject_uint_8_greater> {dec_int {"32767"}};

        // types that cannot accept anything above the max value of an int16.
        using reject_int_16_greater = union_t<reject_uint_8_greater,
            type_list<int16, bound<endian::little, 2, byte>, bound<endian::little, 1, uint16>>>;

        using accept_int_16_greater = difference_t<all_types, reject_int_16_greater>;

        // 32768 to 65535
        test_case<
            accept_int_16_greater,
            reject_int_16_greater> {dec_int {"32768"}};

        test_case<
            accept_int_16_greater,
            reject_int_16_greater> {dec_int {"50000"}};

        test_case<
            accept_int_16_greater,
            reject_int_16_greater> {dec_int {"65535"}};

        // types that cannot accept anything below the min value of an int16.
        using reject_int_16_lesser = union_t<reject_int_16_greater, unsigned_types>;

        using accept_int_16_lesser = difference_t<all_types, reject_int_16_lesser>;

        // -32769 to -8388608
        test_case<
            accept_int_16_lesser,
            reject_int_16_greater> {dec_int {"-32769"}};

        test_case<
            accept_int_16_lesser,
            reject_int_16_greater> {dec_int {"-65535"}};

        test_case<
            accept_int_16_lesser,
            reject_int_16_greater> {dec_int {"-65536"}};

        test_case<
            accept_int_16_lesser,
            reject_int_16_greater> {dec_int {"-8388608"}};

        // types that must reject anything higher than max uint16.
        using reject_uint_16_greater = union_t<reject_int_16_greater,
            type_list<uint16, ubound<endian::little, 2, byte>, ubound<endian::little, 1, uint16>>>;

        using accept_uint_16_greater = difference_t<all_types, reject_uint_16_greater>;

        // 65536 to 8388607
        test_case<
            accept_uint_16_greater,
            reject_uint_16_greater> {dec_int {"65536"}};

        test_case<
            accept_uint_16_greater,
            reject_uint_16_greater> {dec_int {"100000"}};

        test_case<
            accept_uint_16_greater,
            reject_uint_16_greater> {dec_int {"749384"}};

        test_case<
            accept_uint_16_greater,
            reject_uint_16_greater> {dec_int {"8388607"}};

        // types that must reject anything higher than max int24. (2147483647)
        using reject_int_24_greater = union_t<reject_uint_16_greater,
            type_list<bound<endian::little, 3, byte>>>;

        using accept_int_24_greater = difference_t<all_types, reject_int_24_greater>;

        // 8388608 to 16777215
        test_case<
            accept_int_24_greater,
            reject_int_24_greater> {dec_int {"8388608"}};

        test_case<
            accept_int_24_greater,
            reject_int_24_greater> {dec_int {"12345678"}};

        test_case<
            accept_int_24_greater,
            reject_int_24_greater> {dec_int {"16777215"}};

        // types that must reject anything below min int24. (-2147483648)
        using reject_int_24_lesser = union_t<reject_int_24_greater, unsigned_types>;

        using accept_int_24_lesser = difference_t<all_types, reject_int_24_lesser>;

        // -8388609 to -2147483648
        test_case<
            accept_int_24_lesser,
            reject_int_24_lesser> {dec_int {"-2147483648"}};

        // types that must reject anything higher than max uint24.
        using reject_uint_24_greater = union_t<reject_int_24_greater, type_list<ubound<endian::little, 3, byte>>>;

        using accept_uint_24_greater = difference_t<all_types, reject_uint_24_greater>;

        // 16777216 to 2147483647
        test_case<
            accept_uint_24_greater,
            reject_uint_24_greater> {dec_int {"16777216"}};

        test_case<
            accept_uint_24_greater,
            reject_uint_24_greater> {dec_int {"19088743"}};

        test_case<
            accept_uint_24_greater,
            reject_uint_24_greater> {dec_int {"483749384"}};

        test_case<
            accept_uint_24_greater,
            reject_uint_24_greater> {dec_int {"2147483647"}};

        // types that must reject a value above max int 32
        using reject_int_32_greater = union_t<reject_uint_24_greater,
            type_list<int32, bound<endian::little, 4, byte>, bound<endian::little, 2, uint16>, bound<endian::little, 1, uint32>>>;

        using accept_int_32_greater = difference_t<all_types, reject_int_32_greater>;

        // 2147483648 to 4294967295
        test_case<
            accept_int_32_greater,
            reject_int_32_greater> {dec_int {"2147483648"}};

        test_case<
            accept_int_32_greater,
            reject_int_32_greater> {dec_int {"3000000000"}};

        test_case<
            accept_int_32_greater,
            reject_int_32_greater> {dec_int {"4294967295"}};

        // types that must reject a value below min int 32 (-2147483648)
        using reject_int_32_lesser = union_t<reject_int_32_greater, unsigned_types>;

        // types that accept a value below min int 32 (-2147483648)
        using accept_int_32_lesser = difference_t<all_types, reject_int_32_lesser>;

        // -2147483649 to -140737488355328
        test_case<
            accept_int_32_lesser,
            reject_int_32_lesser> {dec_int {"-2147483649"}};

        test_case<
            accept_int_32_lesser,
            reject_int_32_lesser> {dec_int {"-140737488355328"}};

        // types that must reject a value above max uint 32
        using reject_uint_32_greater = union_t<reject_int_32_greater,
            type_list<uint32,
                ubound<endian::little, 4, byte>,
                ubound<endian::little, 2, uint16>,
                ubound<endian::little, 1, uint32>>>;

        // types that accept a value above max uint 32
        using accept_uint_32_greater = difference_t<all_types, reject_uint_32_greater>;

        // 4294967296 to 140737488355327
        test_case<
            accept_uint_32_greater,
            reject_uint_32_greater> {dec_int {"4294967296"}};

        test_case<
            accept_uint_32_greater,
            reject_uint_32_greater> {dec_int {"10000000000"}};

        test_case<
            accept_uint_32_greater,
            reject_uint_32_greater> {dec_int {"7206483749384"}};

        test_case<
            accept_uint_32_greater,
            reject_uint_32_greater> {dec_int {"140737488355327"}};

        using reject_int_48_greater = union_t<reject_uint_32_greater,
            type_list<bound<endian::little, 3, uint16>>>;

        using accept_int_48_greater = difference_t<all_types, reject_int_48_greater>;

        // 140737488355328 to 281474976710655
        test_case<
            accept_int_48_greater,
            reject_int_48_greater> {dec_int {"140737488355328"}};

        test_case<
            accept_int_48_greater,
            reject_int_48_greater> {dec_int {"281474976710655"}};

        using reject_int_48_lesser = union_t<reject_int_48_greater, unsigned_types>;

        using accept_int_48_lesser = difference_t<all_types, reject_int_48_lesser>;

        // -140737488355329 to -9223372036854775808
        test_case<
            accept_int_48_lesser,
            reject_int_48_lesser> {dec_int {"-140737488355329"}};

        test_case<
            accept_int_48_lesser,
            reject_int_48_lesser> {dec_int {"-9223372036854775808"}};

        using reject_uint_48_greater = union_t<reject_int_48_greater, type_list<ubound<endian::little, 3, uint16>>>;

        using accept_uint_48_greater = difference_t<all_types, reject_uint_48_greater>;

        // 281474976710656 to 9223372036854775807
        test_case<
            accept_uint_48_greater,
            reject_uint_48_greater> {dec_int {"281474976710656"}};

        test_case<
            accept_uint_48_greater,
            reject_uint_48_greater> {dec_int {"81985529216486895"}};

        test_case<
            accept_uint_48_greater,
            reject_uint_48_greater> {dec_int {"9223372036854775807"}};

        using reject_int_64_greater = union_t<reject_uint_48_greater,
            type_list<int64,
                bound<endian::little, 8, byte>,
                bound<endian::little, 4, uint16>,
                bound<endian::little, 2, uint32>,
                bound<endian::little, 1, uint64>>>;

        using accept_int_64_greater = difference_t<all_types, reject_int_64_greater>;

        // 9223372036854775808 to 18446744073709551615
        test_case<
            accept_int_64_greater,
            reject_int_64_greater> {dec_int {"9223372036854775808"}};

        test_case<
            accept_int_64_greater,
            reject_int_64_greater>  {dec_int {"10000000000000000000"}};

        test_case<
            accept_int_64_greater,
            reject_int_64_greater>  {dec_int {"12345678901234567890"}};

        test_case<
            accept_int_64_greater,
            reject_int_64_greater>  {dec_int {"18446744073709551615"}};

        using reject_int_64_lesser = union_t<reject_int_64_greater, unsigned_types>;

        using accept_int_64_lesser = difference_t<all_types, reject_int_64_lesser>;

        // -9223372036854775809 to -39614081257132168796771975168
        test_case<
            accept_int_64_lesser,
            reject_int_64_lesser> {dec_int {"-9223372036854775809"}};

        test_case<
            accept_int_64_lesser,
            reject_int_64_lesser> {dec_int {"-12345678901234567890"}};

        test_case<
            accept_int_64_lesser,
            reject_int_64_lesser> {dec_int {"-39614081257132168796771975168"}};

        using reject_uint_64_greater = union_t<reject_int_64_greater,
            type_list<uint64,
                ubound<endian::little, 8, byte>,
                ubound<endian::little, 4, uint16>,
                ubound<endian::little, 2, uint32>,
                ubound<endian::little, 1, uint64>>>;

        using accept_uint_64_greater = difference_t<all_types, reject_uint_64_greater>;

        // 18446744073709551616 to 39614081257132168796771975167
        test_case<
            accept_uint_64_greater,
            reject_uint_64_greater> {dec_int {"18446744073709551616"}};

        test_case<
            accept_uint_64_greater,
            reject_uint_64_greater> {dec_int {"100000000000000000000"}};

        test_case<
            accept_uint_64_greater,
            reject_uint_64_greater> {dec_int {"24397842987206483749384"}};

        test_case<
            accept_uint_64_greater,
            reject_uint_64_greater> {dec_int {"889627103061277028662417"}};

        using reject_int_96_greater = union_t<reject_uint_64_greater,
            type_list<bound<endian::little, 3, uint32>>>;

        using accept_int_96_greater = difference_t<all_types, reject_int_96_greater>;

        // 39614081257132168796771975168 to 79228162514264337593543950335
        test_case<
            accept_int_96_greater,
            reject_int_96_greater> {dec_int {"39614081257132168796771975168"}};

        test_case<
            accept_int_96_greater,
            reject_int_96_greater> {dec_int {"79228162514264337593543950335"}};

        using reject_int_96_lesser = union_t<reject_int_96_greater, unsigned_types>;

        using accept_int_96_lesser = difference_t<all_types, reject_int_96_lesser>;

        // -39614081257132168796771975169 to -170141183460469231731687303715884105728
        test_case<
            accept_int_96_lesser,
            reject_int_96_lesser> {dec_int {"-39614081257132168796771975169"}};

        test_case<
            accept_int_96_lesser,
            reject_int_96_lesser> {dec_int {"-170141183460469231731687303715884105728"}};

        using reject_uint_96_greater = union_t<reject_int_96_greater, type_list<ubound<endian::little, 3, uint32>>>;

        using accept_uint_96_greater = difference_t<all_types, reject_uint_96_greater>;

        // 79228162514264337593543950336 to 170141183460469231731687303715884105727

        test_case<
            accept_uint_96_greater,
            reject_uint_96_greater> {dec_int {"79228162514264337593543950336"}};

        test_case<
            accept_uint_96_greater,
            reject_uint_96_greater> {dec_int {"98980987676898761029390303474536547398"}};

        test_case<
            accept_uint_96_greater,
            reject_uint_96_greater> {dec_int {"98980987676898761029390303474536547399"}};

        test_case<
            accept_uint_96_greater,
            reject_uint_96_greater> {dec_int {"98980987676898761029390303474536547400"}};

            test_case<
            accept_uint_96_greater,
            reject_uint_96_greater> {dec_int {"170141183460469231731687303715884105727"}};

        using reject_int_128_greater = union_t<reject_uint_96_greater, type_list<bound<endian::little, 2, uint64>>>;

        using accept_int_128_greater = difference_t<all_types, reject_int_128_greater>;

        // 170141183460469231731687303715884105728 to 340282366920938463463374607431768211455

        test_case<
            accept_int_128_greater,
            reject_int_128_greater> {dec_int {"170141183460469231731687303715884105728"}};

        test_case<
            accept_int_128_greater,
            reject_int_128_greater> {dec_int {"340282366920938463463374607431768211455"}};

        using reject_int_128_lesser = union_t<reject_int_128_greater, unsigned_types>;

        using accept_int_128_lesser = difference_t<all_types, reject_int_128_lesser>;

        // -170141183460469231731687303715884105729 to -infinity

        test_case<
            accept_int_128_lesser,
            reject_int_128_lesser> {dec_int {"-170141183460469231731687303715884105729"}};

        test_case<
            accept_int_128_lesser,
            reject_int_128_lesser> {dec_int {"-340282366920938463463374607431768211455"}};

        using reject_uint_128_greater = union_t<reject_int_128_greater, type_list<ubound<endian::little, 2, uint64>>>;

        using accept_uint_128_greater = difference_t<all_types, reject_uint_128_greater>;

        // 340282366920938463463374607431768211456 to infinity

        test_case<
            accept_uint_128_greater,
            reject_uint_128_greater> {dec_int {"340282366920938463463374607431768211456"}};

        test_case<
            accept_uint_128_greater,
            reject_uint_128_greater> {dec_int {"115792089237316195423570985008687907852837564279074904382605163141518161494337"}};

        test_case<
            accept_uint_128_greater,
            reject_uint_128_greater> {dec_int {"115792089237316195423570985008687907853269984665640564039457584007908834671663"}};

    }

    template <typename To, typename From>
    void expect_conversion (
        const From & input,
        const To & expected_result,
        std::string_view description
    ) {
        To result;
        ASSERT_NO_THROW (result = convert<To> (input))
            << description << "; input = " << input;

        EXPECT_EQ (result, expected_result)
            << description
            << "; input = " << input
            << "; result = " << result
            << "; expected = " << expected_result;
    }

    template <typename To, typename From>
    void expect_conversion_throws (
        const From & input,
        std::string_view description
    ) {
        EXPECT_THROW (
            (void) convert<To> (input),
            exception
        ) << description << "; input = " << input;
    }

    template <typename T>
    std::string type_name () {
        int status = 0;

        std::unique_ptr<char, decltype (&std::free)> name {
            abi::__cxa_demangle (typeid (T).name (), nullptr, nullptr, &status),
            &std::free
        };

        return status == 0 ? name.get () : typeid (T).name ();
    }

    // construct from its decimal representation.
    template <typename T>
    T construct_value (const dec_int &value) {
        if constexpr (std::integral<T>) {
            T result {};
            auto [end, error] = std::from_chars (
                value.data (), value.data() + value.size(), result
            );

            if (error != std::errc {} || end != value.data () + value.size ())
                throw exception {} << "invalid integral value " << value << " for type " << type_name<T> ();

            return result;
        } else {
            T result;
            EXPECT_NO_THROW (result = T {value}) << "Try to construct " << type_name<T> () << " from " << value;
            return result;
        }
    }

    template <typename To, typename From>
    void test_positive_pair (
        const From & input,
        const To & expected
    ) {
        if constexpr (
            !std::same_as<To, From> &&
            !(std::integral<To> && std::integral<From>)
        ) {
            auto description =
            "conversion from " + type_name<From> () +
            " to " + type_name<To> ();

            expect_conversion<To> (
                input,
                expected,
                description
            );
        }
    }

    template <typename From, typename... InvalidOutputTypes>
    void test_negative_for_input (
        const From & input,
        type_list<InvalidOutputTypes...>
    ) {
        (expect_conversion_throws<InvalidOutputTypes> (
            input,
            "conversion from " + type_name<From> () +
            " to " + type_name<InvalidOutputTypes> ()
        ), ...);
    }

    template <
        typename Tuple,
        std::size_t I,
        std::size_t... Js
    >
    void test_positive_row (
        const Tuple & values,
        std::index_sequence<Js...>
    ) {
        (test_positive_pair (
            std::get<Js> (values),
            std::get<I> (values)
        ), ...);
    }

    template <typename Tuple, std::size_t... Is>
    void test_positive_cases (
        const Tuple & values,
        std::index_sequence<Is...>
    ) {
        (test_positive_row< Tuple, Is > (
            values,
            std::make_index_sequence<std::tuple_size_v<Tuple>> {}
        ), ...);
    }

    template <
        typename Tuple,
        typename... InvalidOutputTypes,
        std::size_t... Is
    >
    void test_negative_cases (
        const Tuple & values,
        type_list<InvalidOutputTypes...> invalid_output_types,
        std::index_sequence<Is...>
    ) {
        (test_negative_for_input (
            std::get<Is> (values),
            invalid_output_types
        ), ...);
    }

    template <
        typename... InputTypes,
        typename... InvalidOutputTypes
    >
    test_case<
        type_list<InputTypes...>,
        type_list<InvalidOutputTypes...>
    >::test_case (const dec_int & value) {

        std::tuple<InputTypes...> inputs {
            construct_value<InputTypes> (value)...
        };

        EXPECT_NO_THROW (test_positive_cases (
            inputs,
            std::index_sequence_for<InputTypes...> {}
        ));

        EXPECT_NO_THROW (test_negative_cases (
            inputs,
            type_list<InvalidOutputTypes...> {},
            std::index_sequence_for<InputTypes...> {}
        ));
    }
}
