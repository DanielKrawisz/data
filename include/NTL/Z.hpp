// Copyright (c) 2026 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/slice.hpp>

#include <data/encoding/integer.hpp>
#include <data/encoding/endian.hpp>
#include <data/encoding/base58.hpp>

#include <data/math/number/bounded/bounded.hpp>
#include <data/math/number/bytes/Z.hpp>

#include <NTL/ZZ.h>

namespace NTL {

    template <std::signed_integral I> void conv (ZZ &, const I &);

    template <std::unsigned_integral I> void conv (ZZ &, const I &);

    template <bool is_signed, data::endian r, std::size_t size>
    void conv (ZZ &, const data::endian_integral<is_signed, r, size> &);

    void conv (ZZ &x, const data::encoding::decimal::string &);

    void conv (ZZ &x, const data::encoding::signed_decimal::string &);

    template <data::negativity neg, data::hex_case cc>
    void conv (ZZ &x, const data::encoding::hexidecimal::integer<neg, cc> &);

    void conv (ZZ &x, const data::encoding::base58::string &);

    template <data::endian r, data::negativity neg, std::unsigned_integral word>
    void conv (ZZ &x, const data::math::number::Z_bytes<r, neg, word> &);

    template <data::endian r, std::unsigned_integral word>
    void conv (ZZ &x, const data::math::number::N_bytes<r, word> &);

    template <bool is_signed, data::endian r, std::size_t size, std::unsigned_integral word>
    void conv (ZZ &x, const data::math::number::bounded<is_signed, r, size, word> &);

    template <std::signed_integral I> void conv (I &, const ZZ &);

    template <std::unsigned_integral I> void conv (I &, const ZZ &);

    void conv (data::encoding::decimal::string &, const ZZ &);

    void conv (data::encoding::signed_decimal::string &, const ZZ &);

    void conv (data::encoding::base58::string &, const ZZ &);

    template <bool is_signed, data::endian r, std::size_t size, std::unsigned_integral word>
    void conv (data::math::number::bounded<is_signed, r, size, word> &, const ZZ &);

    template <bool is_signed, data::endian r, std::size_t size>
    void conv (data::endian_integral<is_signed, r, size> &, const ZZ &);

    template <std::unsigned_integral U>
    ZZ import_bin (
        data::slice<const U> data,
        // the ordering of the overall array.
        data::endian order = data::endian::little,
        // the ordering of each value in the array.
        data::endian e = data::endian::native,
        data::negativity neg = data::negativity::nones
    );

    template <std::unsigned_integral U>
    void export_bin (
        data::slice<U> output,
        const ZZ &,
        // the ordering of the overall array.
        data::endian word_order = data::endian::little,
        // the ordering of each value in the array.
        data::endian byte_order = data::endian::native,
        data::negativity neg = data::negativity::twos
    );

    size_t inline bit_width (const ZZ &x) {
        if (sign (x) >= 0) return NumBits (x) + 1;
        return NumBits (abs (x) - 1) + 1;
    }

    template <typename T>
    concept Compatible =
        std::integral<T> && (sizeof (T) > sizeof (char)) &&
        (sizeof (T) < sizeof (long) ||
        data::Same<T, long>);

    template <typename T>
    concept Incompatible =
        std::integral<T> &&
        !Compatible<T>;

    template <data::negativity neg, data::hex_case cc>
    void inline conv (ZZ &x, const data::encoding::hexidecimal::integer<neg, cc> &u) {
        if (!u.valid ()) throw data::exception {} << "Invalid hexidecimal string";
        data::bytes data = *data::encoding::hex::read (data::string_view (u).substr (2));
        x = import_bin<data::byte> (data::byte_slice (data),
            data::endian::big, data::endian::native, neg);
    }

    template <data::endian r, data::negativity neg, std::unsigned_integral word>
    void inline conv (ZZ &x, const data::math::number::Z_bytes<r, neg, word> &u) {
        x = import_bin<word> (data::slice<const word> (u), r, data::endian::native, neg);
    }

    template <data::endian r, std::unsigned_integral word>
    void inline conv (ZZ &x, const data::math::number::N_bytes<r, word> &u) {
        x = import_bin<word> (
            data::slice<const word> (u), r,
            data::endian::native,
            data::negativity::nones);
    }

    template <bool is_signed, data::endian r, std::size_t size, std::unsigned_integral word>
    void inline conv (ZZ &x, const data::math::number::bounded<is_signed, r, size, word> &u) {
        x = import_bin<word> (
            data::slice<const word> (u), r, data::endian::native,
            is_signed ? data::negativity::twos : data::negativity::nones);
    }

    template <bool is_signed, data::endian r, std::size_t size>
    void inline conv (ZZ &x, const data::endian_integral<is_signed, r, size> &u) {
        x = import_bin<data::byte> (
            data::slice<const data::byte> (u), r, data::endian::native,
            is_signed ? data::negativity::twos : data::negativity::nones);
    }

    void inline conv (ZZ &x, const data::encoding::decimal::string &u) {
        conv (x, u.c_str ());
    }

    template <std::unsigned_integral I> void inline conv (ZZ &u, const I &x) {
        u = import_bin (
            data::slice<const I> {&x, 1},
            data::endian::little, // size is one so does nothing.
            data::endian::native,
            data::negativity::nones);
    }

    template <std::unsigned_integral I> void inline conv (I &x, const ZZ &z) {
        export_bin (
            data::slice<I> {&x, 1}, z,
            data::endian::little, // size is one so does nothing.
            data::endian::native,
            data::negativity::nones);
    }

    template <std::signed_integral I> void inline conv (ZZ &u, const I &x) {
        u = import_bin (
            data::slice<const std::make_unsigned_t<I>> {(const std::make_unsigned_t<I> *) (&x), 1},
            data::endian::little, // size is one so does nothing.
            data::endian::native,
            data::negativity::twos);
    }

    template <std::signed_integral I> void inline conv (I &x, const ZZ &z) {
        export_bin (
            data::slice<std::make_unsigned_t<I>> {(std::make_unsigned_t<I> *) (&x), 1}, z,
            data::endian::little, // size is one so does nothing.
            data::endian::native,
            data::negativity::twos);
    }

    template <bool is_signed, data::endian r, std::size_t size, std::unsigned_integral word>
    void inline conv (data::math::number::bounded<is_signed, r, size, word> &x, const ZZ &u) {
        export_bin<word> (data::slice<word> (x), u, r, data::endian::native,
            is_signed ? data::negativity::twos : data::negativity::nones);
    }

    template <bool is_signed, data::endian r, std::size_t size>
    void inline conv (data::endian_integral<is_signed, r, size> &x, const ZZ &u) {
        export_bin<data::byte> (data::slice<data::byte> (x), u, r, data::endian::native,
            is_signed ? data::negativity::twos : data::negativity::nones);
    }

    template <std::unsigned_integral U>
    bool is_negative (
        data::slice<const U> input,
        data::endian word_order,
        data::endian byte_order,
        data::negativity neg)
    {
        if (neg == data::negativity::nones || input.empty ())
            return false;

        const U word = word_order == data::endian::little
            ? input.back ()
            : input.front ();

        constexpr std::size_t bytes = sizeof (U);

        const std::size_t byte_index = byte_order == data::endian::little
            ? bytes - 1
            : 0;

        const U sign_bit = U (1) << (byte_index * 8 + 7);

        return (word & sign_bit) != 0;
    }

    template <std::unsigned_integral U>
    static ZZ import_bin (
        data::slice<const U> input,
        // the ordering of the overall array.
        data::endian word_order,
        // the ordering of each value in the array.
        data::endian byte_order,
        data::negativity neg
    ) {

        if (input.size () == 0) return ZZ ();

        if (word_order != data::endian::big && word_order != data::endian::little)
            throw std::invalid_argument ("invalid word order");

        if (byte_order != data::endian::big && byte_order != data::endian::little)
            throw std::invalid_argument ("invalid byte order");

        bool input_is_negative = is_negative (input, word_order, byte_order, neg);

        // under the right circumstances, we can send the input directly to NTL.
        if (word_order == data::endian::little &&
            !input_is_negative &&
            sizeof (U) == sizeof (unsigned char) &&
            (byte_order == data::endian::native || sizeof (unsigned char) == 1))
            return ZZFromBytes (reinterpret_cast<const unsigned char *> (input.data ()), input.size ());

        data::bytestring<U> bytes (input.size ());

        if (word_order == data::endian::little)
            for (size_t i = 0; i < input.size (); ++i)
                bytes[i] = input[i];
        else for (size_t i = 0; i < input.size (); ++i)
                bytes[i] = input[input.size () - i - 1];

        if (!input_is_negative) {
            // switch to little endian.
            if (byte_order != data::endian::little && sizeof (U) > 1)
                for (U &x : bytes) x = boost::endian::endian_reverse<U> (x);

            // from now on we are using little endian word and byte order.
            return ZZFromBytes (
                reinterpret_cast<const unsigned char *> (bytes.data ()),
                bytes.size () * sizeof (U));
        }

        // first convert to native endian.
        if (byte_order != data::endian::native && sizeof (U) > 1)
            for (U &x : bytes) x = boost::endian::endian_reverse<U> (x);

        // negate
        if (neg == data::negativity::twos) data::arithmetic::twos::negate<data::endian::little, U> (bytes);
        else data::arithmetic::BC::negate<data::endian::little, U> (bytes);

        // switch to little endian
        if (data::endian::native != data::endian::little && sizeof (U) > 1)
            for (U &x : bytes) x = boost::endian::endian_reverse<U> (x);

        auto zzz = ZZFromBytes (
            reinterpret_cast<const unsigned char *> (bytes.data ()),
            bytes.size () * sizeof (U));

        return -zzz;

    }

    template <std::unsigned_integral U>
    void negate_twos (data::slice<U> output) {
        // The words are stored little-endian, so the two's-complement
        // carry propagates from the first word toward the last.
        for (auto &x : output)
            x = ~x;

        // Add one to the multiword value.  add_with_carry propagates the
        // carry to the next word when the current addition overflows.
        for (auto &x : output)
            if (!data::arithmetic::add_with_carry (x, x, U (1))) return;
    }

    template <std::unsigned_integral U>
    void negate_bc (data::slice<U> output) {
        output.back () ^= U (1) << (sizeof (U) * 8 - 1);
    }

    template <std::unsigned_integral U>
    void export_bin (
        data::slice<U> output,
        const ZZ &x,
        // the ordering of the overall array.
        data::endian word_order,
        // the ordering of each value in the array.
        data::endian byte_order,
        data::negativity neg
    ) {

        auto sign = NTL::sign (x);

        if (neg == data::negativity::nones && sign < 0)
            throw data::exception {} << "negative value";

        constexpr size_t bits = sizeof (U) * 8;

        const size_t capacity = output.size () * bits;

        // Determine that the minimal size is enough given the buffer.
        size_t required = sign == 0 ? 0:
            neg == data::negativity::nones
                ? NumBits (x)
                : bit_width (x);

        auto abs = NTL::abs (x);

        if (neg == data::negativity::BC &&
            sign < 0 &&
            bit_width (x) == NumBits (abs))
            ++required;

        if (required > capacity)
            throw data::exception {} << "integer does not fit";

        {
            auto *output_bytes = reinterpret_cast<unsigned char *> (output.data ());
            const size_t nbytes = output.size () * sizeof (U);

            std::fill (output_bytes, output_bytes + nbytes, 0);

            BytesFromZZ (output_bytes, abs, nbytes);
        }

        // at this point, word order and byte order are both little endian.

        // if x is negative, apply the appropriate negativity function.
        if (sign < 0) {
            // first convert to native endian.
            if (data::endian::little != data::endian::native && sizeof (U) > 1)
                for (U &x : output) x = boost::endian::endian_reverse<U> (x);

            if (neg == data::negativity::twos)
                negate_twos (output);
            else if (neg == data::negativity::BC)
                negate_bc (output);

            if (data::endian::native != byte_order && sizeof (U) > 1)
                for (U &x : output) x = boost::endian::endian_reverse<U> (x);
        } else if (byte_order != data::endian::little && sizeof (U) > 1)
            for (U &x : output) x = boost::endian::endian_reverse<U> (x);

        // reverse to big endian if we have to.
        if (word_order == data::endian::big)
            for (int i = 0; i < output.size () / 2; i++)
                std::swap (output[i], output[output.size () - 1 - i]);
    }

}
