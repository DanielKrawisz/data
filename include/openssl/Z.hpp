#pragma once

#include <openssl/bn.h>
#include <openssl/crypto.h>
#include <algorithm>
#include <bit>
#include <limits>
#include <new>
#include <stdexcept>

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

#include <data/math/number/NTL/Z.hpp>

namespace OpenSSL {

    using data::Word;
    using data::negativity;
    using data::endian;
    using data::slice;
    using std::size_t;
    using data::byte_slice;
    using data::Same;

    using data::dec_uint;
    using data::dec_int;
    namespace hex = data::hex;
    using data::base58_uint;

    using data::math::number::bounded;
    using data::math::number::Z_bytes;

    using boost::endian::align;
    using boost::endian::endian_arithmetic;

    // Open SSL's arbitrary-precision integer type.
    using ZZ = BIGNUM *;

    struct Z {
        Z ();
        ~Z ();

        Z (const Z &z);

        Z (Z &&z) noexcept;

        Z &operator = (const Z &z);

        Z &operator = (Z &&z) noexcept;

        template <std::integral I> Z (I u);

        template <data::endian Order, class T, std::size_t n_bits, align Align>
        Z (endian_arithmetic<Order, T, n_bits, Align> &u);

        Z (const dec_int &u);
        Z (const dec_uint &u);

        template <negativity n, data::hex_case zz> Z (const hex::integer<n, zz> &u);
        Z (const base58_uint &u);

        template <endian r, negativity c, Word word>
        explicit Z (const Z_bytes<r, c, word> &u);

        template <bool is_signed, endian r, size_t size, Word word>
        explicit Z (const bounded<is_signed, r, size, word> &u);

        template <std::integral I>
        explicit operator I () const;

        explicit operator double () const;

        explicit operator dec_int () const;

        template <data::endian Order, class T, std::size_t n_bits, align Align>
        explicit Z (const endian_arithmetic<Order, T, n_bits, Align> &x);

        template <data::endian Order, class T, std::size_t n_bits, align Align>
        explicit operator endian_arithmetic<Order, T, n_bits, Align> () const;

        template <endian r, negativity c, Word word>
        explicit operator Z_bytes<r, c, word> () const;

        template <bool is_signed, endian r, size_t size, Word word>
        explicit operator bounded<is_signed, r, size, word> () const;

        template <negativity neg, data::hex_case zz> explicit operator hex::integer<neg, zz> () const;

        explicit Z (ZZ z);

        ZZ Value;
    };

    static_assert (data::NumberSystem<Z, Z>);

    ZZ import_dec (const dec_int &);
    dec_int export_dec (const ZZ);

    // Import an integer from a GMP-style word sequence.
    //
    // The input consists of `count` words of `U` bytes each. `order`
    // specifies the ordering of the words, `endian` specifies the
    // byte ordering within each word, and `neg` specifies the signed
    // representation of the complete bit sequence.
    //
    // The supported signed representations are:
    //
    //   nones  Unsigned magnitude.
    //   twos   Two's-complement.
    //   BC     Sign-and-magnitude.
    //
    // An empty input sequence represents zero.
    template <Word U>
    ZZ import_bin (
        slice<const U> input,
        endian word_order = endian::little,
        endian byte_order = endian::native,
        negativity neg = negativity::nones);

    // Export an integer to a GMP-style word sequence.
    //
    // The result contains `count` words of `U` bytes each, using the
    // requested word ordering, byte ordering, and signed representation.
    //
    // The representation may contain redundant sign extension when
    // exporting a signed value.
    template <Word U>
    void export_bin (
        slice<U> output,
        const ZZ x,
        endian word_order = endian::little,
        endian byte_order = endian::native,
        negativity neg = negativity::nones);

}

namespace data::math::def {
    template <> struct convert<OpenSSL::Z, base58_uint> {
        OpenSSL::Z operator () (const base58_uint &) const;
    };
}

namespace OpenSSL {
    using data::math::convert;

    template <std::integral I> inline Z::Z (I u): Z {convert<Z> (u)} {}

    template <data::endian Order, class T, std::size_t n_bits, align Align>
    inline Z::Z (endian_arithmetic<Order, T, n_bits, Align> &u): Z {convert<Z> (u)} {}

    inline Z::Z (const dec_int &u): Z {convert<Z> (u)} {}

    inline Z::Z (const dec_uint &u): Z {convert<Z> (u)} {}

    template <negativity n, data::hex_case zz> inline Z::Z (const hex::integer<n, zz> &u):
        Z {convert<Z> (u)} {}

    inline Z::Z (const base58_uint &u): Z {convert<Z> (u)} {}

    template <endian r, negativity c, Word word>
    inline Z::Z (const Z_bytes<r, c, word> &u): Z {convert<Z> (u)} {}

    template <bool is_signed, endian r, size_t size, Word word>
    inline Z::Z (const bounded<is_signed, r, size, word> &u): Z {convert<Z> (u)} {}

    template <std::integral I>
    inline Z::operator I () const {
        return convert<I> (*this);
    }

    inline Z::operator dec_int () const {
        return convert<dec_int> (*this);
    }

    template <data::endian Order, class T, std::size_t n_bits, align Align>
    inline Z::Z (const endian_arithmetic<Order, T, n_bits, Align> &x): Z {convert<Z> (x)} {}

    template <data::endian Order, class T, std::size_t n_bits, align Align>
    inline Z::operator endian_arithmetic<Order, T, n_bits, Align> () const {
        return convert<endian_arithmetic<Order, T, n_bits, Align>> (*this);
    }

    template <endian r, negativity c, Word word>
    inline Z::operator Z_bytes<r, c, word> () const {
        return convert<Z_bytes<r, c, word>> (*this);
    }

    template <bool is_signed, endian r, size_t size, Word word>
    inline Z::operator bounded<is_signed, r, size, word> () const {
        return convert<bounded<is_signed, r, size, word>> (*this);
    }

    template <negativity neg, data::hex_case zz> inline Z::operator hex::integer<neg, zz> () const {
        return convert<hex::integer<neg, zz>> (*this);
    }
}

namespace data::math::def {

    OpenSSL::Z inline convert<OpenSSL::Z, Z>::operator () (const Z &u) const {
        return OpenSSL::Z (Z_bytes<endian::little, uint64> (u));
    }

    Z inline convert<Z, OpenSSL::Z>::operator () (const OpenSSL::Z &u) const {
        return Z (Z_bytes<endian::little, uint64> (u));
    }

    OpenSSL::Z inline convert<OpenSSL::Z, N>::operator () (const N &u) const {
        return OpenSSL::Z (Z_bytes<endian::little, uint64> (u));
    }

    N inline convert<N, OpenSSL::Z>::operator () (const OpenSSL::Z &u) const {
        return N (N_bytes<endian::little, uint64> (u));
    }

    dec_int inline convert<dec_int, OpenSSL::Z>::operator () (const OpenSSL::Z &z) const {
        return OpenSSL::export_dec (z.Value);
    }

    template <std::integral I>
    OpenSSL::Z inline convert<OpenSSL::Z, I>::operator () (I u) const {
        return OpenSSL::Z {OpenSSL::import_bin (
            slice<const std::make_unsigned_t<I>> {reinterpret_cast<const std::make_unsigned_t<I> *> (&u), 1},
            endian::little, endian::native,
            std::unsigned_integral<I> ? negativity::nones : negativity::twos)};
    }

    template <std::integral I>
    I inline convert<I, OpenSSL::Z>::operator () (const OpenSSL::Z & u) const {
        I x;
        OpenSSL::export_bin (
            slice<std::make_unsigned_t<I>> {reinterpret_cast<std::make_unsigned_t<I> *> (&x), 1},
            u.Value, endian::little, endian::native,
            std::unsigned_integral<I> ? negativity::nones : negativity::twos);
        return x;
    }

    template <data::endian Order, class T, std::size_t n_bits, alignment Align>
    OpenSSL::Z inline convert<OpenSSL::Z, endian_arithmetic<Order, T, n_bits, Align>>::operator () (const endian_arithmetic<Order, T, n_bits, Align> &u) const {
        return OpenSSL::Z {OpenSSL::import_bin (slice<const byte> (u.data (), sizeof (T)), Order, endian::native,
            std::unsigned_integral<T> ? negativity::nones : negativity::twos)};
    }

    template <data::endian Order, class T, std::size_t n_bits, alignment Align>
    endian_arithmetic<Order, T, n_bits, Align> inline convert<endian_arithmetic<Order, T, n_bits, Align>, OpenSSL::Z>::operator () (const OpenSSL::Z &u) const {
        endian_arithmetic<Order, T, n_bits, Align> x;
        OpenSSL::export_bin (slice<byte> {x.data (), sizeof (T)}, u.Value, Order, endian::native,
            std::unsigned_integral<T> ? negativity::nones : negativity::twos);
        return x;
    }

    template <endian r, negativity neg, Word word>
    number::Z_bytes<r, neg, word> convert<number::Z_bytes<r, neg, word>, OpenSSL::Z>::operator () (const OpenSSL::Z &u) const {
        if constexpr (neg == negativity::nones) {
            if (data::is_negative (u)) throw exception {} << "try to convert negative integer " << u << " to unsigned type";
        }

        if (data::is_zero (u)) return number::Z_bytes<r, neg, word> {};

        number::Z_bytes<r, neg, word> x;
        size_t new_size;
        size_t bit_width = data::bit_width (u);
        constexpr const size_t word_bits = sizeof (word) * CHAR_BIT;

        if constexpr (neg == negativity::nones) {
            new_size = (bit_width + word_bits - 2) / word_bits;
        } else {
            if constexpr (neg == negativity::BC) {
                if (bit_width == data::bit_width (data::abs (u)) - 1) ++bit_width;
            }
            new_size = (bit_width + word_bits - 1) / word_bits;
        }

        x.resize (new_size);

        OpenSSL::export_bin (slice<word> {x.data (), x.size ()}, u.Value, r, endian::native, neg);
        return x;
    }

    template <endian r, negativity neg, Word word>
    OpenSSL::Z inline convert<OpenSSL::Z, number::Z_bytes<r, neg, word>>::operator () (const number::Z_bytes<r, neg, word> &u) const {
        return OpenSSL::Z {OpenSSL::import_bin (slice<const word> {u.data (), u.size ()}, r, endian::native, neg)};
    }

    template <bool is_signed, endian r, size_t size, Word word>
    OpenSSL::Z inline convert<OpenSSL::Z, number::bounded<is_signed, r, size, word>>::operator () (const number::bounded<is_signed, r, size, word> &u) const {
        return OpenSSL::Z {OpenSSL::import_bin (slice<const word> {u.data (), u.size ()}, r, endian::native, is_signed ? negativity::twos : negativity::nones)};
    }

    template <bool is_signed, endian r, size_t size, Word word>
    number::bounded<is_signed, r, size, word> convert<number::bounded<is_signed, r, size, word>, OpenSSL::Z>::operator () (const OpenSSL::Z &u) const {
        if constexpr (!is_signed) {
            if (data::is_negative (u)) throw exception {} << "try to convert negative integer " << u << " to unsigned type";
        } else {
            if (u < numeric_limits<number::bounded<is_signed, r, size, word>>::min ())
                throw exception {} << "openssl integer " << u << " is too low to fit in a signed number of size " << (size * sizeof (word));
        }

        if (u > numeric_limits<number::bounded<is_signed, r, size, word>>::max ())
        throw exception {} << "openssl integer " << u << " is too high to fit in a number of size " << size;

        number::bounded<is_signed, r, size, word> x;
        OpenSSL::export_bin (slice<word> {x.data (), x.size ()}, u.Value, r, endian::native,
            is_signed ? negativity::twos : negativity::nones);
        return x;
    }

    template <negativity neg, hex_case zz>
    OpenSSL::Z convert<OpenSSL::Z, hex::integer<neg, zz>>::operator () (const hex::integer<neg, zz> &u) const {
        return hex::integer<neg, zz> (Z_bytes_big (u));
    }

    OpenSSL::Z inline convert<OpenSSL::Z, dec_uint>::operator () (const dec_uint &u) const {
        if (!u.valid ()) throw exception {} << "try to read integer from invalid dec_int " << u;
        return OpenSSL::Z {std::move (OpenSSL::import_dec (u))};
    }

    OpenSSL::Z inline convert<OpenSSL::Z, dec_int>::operator () (const dec_int &u) const {
        if (!u.valid ()) throw exception {} << "try to read integer from invalid dec_int " << u;
        return OpenSSL::Z {std::move (OpenSSL::import_dec (u))};
    }

    OpenSSL::Z inline convert<OpenSSL::Z, base58_uint>::operator () (const base58_uint &u) const {
        auto decoded = encoding::base58::decode<OpenSSL::Z> (u);
        if (!decoded) throw exception {} << "try to read integer from invalid base58 string " << u;
        return *decoded;
    }
}

namespace OpenSSL {

    std::ostream inline &operator << (std::ostream &o, const Z &n) {
        return o << export_dec (n.Value);
    }

    Z inline &operator ++ (Z &a) {
        if (!BN_add_word(a.Value, 1))
            throw std::runtime_error {"BN_add_word failed"};

        return a;
    }

    Z inline &operator -- (Z &a) {
        if (!BN_sub_word(a.Value, 1))
            throw std::runtime_error {"BN_sub_word failed"};

        return a;
    }

    Z inline operator ++ (Z &a, int) {
        Z r {a};
        ++a;
        return r;
    }

    Z inline operator -- (Z &a, int) {
        Z r {a};
        --a;
        return r;
    }

    template <std::integral I> Z inline operator + (const Z &a, I b) {
        return a + Z {b};
    }

    template <std::integral I> Z inline operator - (const Z &a, I b) {
        return a - Z {b};
    }

    template <std::integral I> Z inline operator * (const Z &a, I b) {
        return a * Z {b};
    }

    template <std::integral I> bool inline operator == (const Z &a, I b) {
        return a == Z {b};
    }

    template <std::integral I> std::strong_ordering inline operator <=> (const Z &a, I b) {
        return a <=> Z {b};
    }

    Z inline operator / (const Z &a, const Z &b) {
        return data::divmod (a, data::math::nonzero<Z> {b}).Quotient;
    }

    Z inline operator % (const Z &a, const Z &b) {
        return data::divmod (a, data::math::nonzero<Z> {b}).Remainder;
    }

    template <bool is_signed, endian r, size_t size, Word word>
    bool inline operator == (const Z &a, const bounded<is_signed, r, size, word> &b) {
        return a == Z (b);
    }

    template <bool is_signed, endian r, size_t size, Word word>
    std::strong_ordering inline operator <=> (const Z &a, const bounded<is_signed, r, size, word> &b) {
        return a <=> Z (b);
    }

    template <endian r, negativity neg, Word word>
    bool inline operator == (const Z &a, const Z_bytes<r, neg, word> &b) {
        return a == Z (b);
    }

    template <endian r, negativity neg, Word word>
    std::strong_ordering inline operator <=> (const Z &a, const Z_bytes<r, neg, word> &b) {
        return a <=> Z (b);
    }

    template <Word U>
    static ZZ import_bin (
        slice<const U> input,
        endian word_order,
        endian byte_order,
        negativity neg
    ) {

        if (input.size () == 0)
            return BN_new ();

        bool negative = NTL::is_negative (input, word_order, byte_order, neg);

        if (word_order == endian::big && !negative &&
            (sizeof (U) == sizeof (unsigned char) || byte_order == endian::big)
        ) return BN_bin2bn (
            reinterpret_cast<const unsigned char *> (input.data ()),
            input.size () * sizeof (U),
            nullptr);

        data::bytestring<U> bytes (input.size ());

        // make word order endian::big from here on.
        // switch byte_order to native if necessary.
        if (word_order == endian::big) {
            if (byte_order != endian::native && sizeof (U) > 1)
                for (size_t i = 0; i < input.size (); ++i)
                    bytes[i] = std::byteswap (input[i]);
            else
                for (size_t i = 0; i < input.size (); ++i)
                    bytes[i] = input[i];
        } else if (byte_order != endian::native && sizeof (U) > 1)
            for (size_t i = 0; i < input.size (); ++i)
                bytes[i] = std::byteswap (input[-i - 1]);
        else
            for (size_t i = 0; i < input.size (); ++i)
                bytes[i] = input[-i - 1];

        // make positive
        if (negative) {
            if (neg == negativity::twos) data::arithmetic::twos::negate<endian::big> (bytes);
            else data::arithmetic::BC::negate<endian::big> (bytes);
        }

        // switch bytes to big endian.
        if (endian::native != endian::big && sizeof (U) > 1)
            for (size_t i = 0; i < bytes.size (); ++i)
                bytes[i] = std::byteswap (bytes[i]);

        ZZ result = BN_bin2bn (
            reinterpret_cast<const unsigned char *> (bytes.data ()),
            bytes.size () * sizeof (U),
            nullptr);

        BN_set_negative (result, negative);

        return result;
    }

    template <Word U>
    void export_bin (
        slice<U> output,
        const ZZ x,
        endian word_order,
        endian byte_order,
        negativity neg) {

        if (BN_is_zero (x)) {
            std::fill (output.begin (), output.end (), U {0});
            return;
        }

        auto size = output.size () * sizeof (U);

        if (size > std::numeric_limits<int>::max ())
            throw data::exception {"output is too large"};

        auto bytes = reinterpret_cast<unsigned char *> (output.data ());

        if (neg == negativity::nones) {
            if (BN_is_negative (x))
                throw data::exception {"integer does not fit"};

            if (BN_bn2binpad (x, bytes, static_cast<int> (size)) < 0)
                throw data::exception {"integer does not fit"};
        } else if (neg == negativity::twos) {
            // OpenSSL may expect an extra byte for the sign bit
            // so we make a temporary buffer to export to and then
            // copy it into the output, skipping the extra byte.
            if (BN_is_negative (x)) {

                BIGNUM *a = BN_dup (x);

                if (!a) throw data::exception {} << "bad allocation";

                size_t bit_width = data::bit_width (Z (a)); // a gets deleted here.

                if (size * CHAR_BIT < bit_width) throw data::exception {} << "openssl BIGNUM * is too low to fit into size " << size;

                data::bytestring<unsigned char> temp;
                temp.resize (size + 1);
                if (BN_signed_bn2bin (x, temp.data (), size + 1) < 0)
                    throw data::exception {} << "cannot export openssl integer into size " << size;
                std::copy (temp.begin () + 1, temp.end (), bytes);
            } else if (BN_signed_bn2bin (x, bytes, static_cast<int> (size)) < 0)
                throw data::exception {} << "cannot export openssl integer into size " << size;
        } else {
            BIGNUM *a = BN_dup (x);

            if (!a) throw data::exception {} << "bad allocation";

            // set positive
            BN_set_negative (a, 0);

            Z abs_value (a);// this means that a will get deleted at the end of this block.

            if (data::bit_width (abs_value) > size * 8)
                throw data::exception {"integer does not fit"};

            if (BN_bn2binpad (a, bytes, static_cast<int> (size)) < 0)
                throw data::exception {"integer does not fit"};

            if (BN_is_negative (x))
                bytes[0] |= 0x80;
        }

        // Adjust byte ordering within each word.
        if (byte_order != endian::big && sizeof (U) > 1)
            for (auto &word : output)
                word = std::byteswap (word);

        // Adjust the ordering of the words.
        if (word_order != endian::big)
            std::reverse (output.begin (), output.end ());
    }
}

