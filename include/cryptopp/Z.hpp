#pragma once

#include <openssl/bn.h>

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

#include <cryptopp/integer.h>
#include <cryptopp/misc.h>

#include <data/math/number/NTL/Z.hpp>

namespace CryptoPP {

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

    struct Z : Integer {
        Z (): Integer {} {}

        explicit Z (Integer &&u): Integer {u} {}
        explicit Z (const Integer &u): Integer {u} {}

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

    };

    static_assert (data::NumberSystem<Z, Z>);

    Integer import_dec (const dec_int &);
    dec_int export_dec (const Integer &);

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
    template <Word U> Integer import_bin (
        slice<const U> data,
        // the ordering of the overall array.
        endian word_order = endian::little,
        // the ordering of each value in the array.
        endian byte_order = endian::native,
        negativity neg = negativity::nones
    );

    // Export an integer to a GMP-style word sequence.
    //
    // The result contains `count` words of `U` bytes each, using the
    // requested word ordering, byte ordering, and signed representation.
    //
    // The representation may contain redundant sign extension when
    // exporting a signed value.
    template <Word U> void export_bin (
        slice<U> output,
        const Integer &,
        // the ordering of the overall array.
        endian word_order = endian::little,
        // the ordering of each value in the array.
        endian byte_order = endian::native,
        negativity neg = negativity::twos);
}

namespace data::math::def {
    template <> struct convert<CryptoPP::Z, base58_uint> {
        CryptoPP::Z operator () (const base58_uint &) const;
    };
}

namespace CryptoPP {
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

    CryptoPP::Z inline convert<CryptoPP::Z, Z>::operator () (const Z &u) const {
        return CryptoPP::Z (Z_bytes<endian::little, uint64> (u));
    }

    Z inline convert<Z, CryptoPP::Z>::operator () (const CryptoPP::Z &u) const {
        return Z (Z_bytes<endian::little, uint64> (u));
    }

    CryptoPP::Z inline convert<CryptoPP::Z, N>::operator () (const N &u) const {
        return CryptoPP::Z (Z_bytes<endian::little, uint64> (u));
    }

    N inline convert<N, CryptoPP::Z>::operator () (const CryptoPP::Z &u) const {
        return N (N_bytes<endian::little, uint64> (u));
    }

    dec_int inline convert<dec_int, CryptoPP::Z>::operator () (const CryptoPP::Z &z) const {
        return CryptoPP::export_dec (z);
    }

    template <std::integral I>
    CryptoPP::Z inline convert<CryptoPP::Z, I>::operator () (I u) const {
        return CryptoPP::Z {CryptoPP::import_bin (
            slice<const std::make_unsigned_t<I>> {reinterpret_cast<const std::make_unsigned_t<I> *> (&u), 1},
            endian::little, endian::native,
            std::unsigned_integral<I> ? negativity::nones : negativity::twos)};
    }

    template <std::integral I>
    I inline convert<I, CryptoPP::Z>::operator () (const CryptoPP::Z & u) const {
        I x;
        CryptoPP::export_bin (
            slice<std::make_unsigned_t<I>> {reinterpret_cast<std::make_unsigned_t<I> *> (&x), 1},
            reinterpret_cast<const CryptoPP::Integer &> (u),
            endian::little, endian::native,
            std::unsigned_integral<I> ? negativity::nones : negativity::twos);
        return x;
    }

    template <data::endian Order, class T, std::size_t n_bits, alignment Align>
    CryptoPP::Z inline convert<CryptoPP::Z, endian_arithmetic<Order, T, n_bits, Align>>::operator () (const endian_arithmetic<Order, T, n_bits, Align> &u) const {
        return CryptoPP::Z {CryptoPP::import_bin (slice<const byte> (u.data (), sizeof (T)), Order, endian::native,
            std::unsigned_integral<T> ? negativity::nones : negativity::twos)};
    }

    template <data::endian Order, class T, std::size_t n_bits, alignment Align>
    endian_arithmetic<Order, T, n_bits, Align> inline convert<endian_arithmetic<Order, T, n_bits, Align>, CryptoPP::Z>::operator () (const CryptoPP::Z &u) const {
        endian_arithmetic<Order, T, n_bits, Align> x;
        CryptoPP::export_bin (slice<byte> {x.data (), sizeof (T)}, u, Order, endian::native,
            std::unsigned_integral<T> ? negativity::nones : negativity::twos);
        return x;
    }

    template <endian r, negativity neg, Word word>
    number::Z_bytes<r, neg, word> convert<number::Z_bytes<r, neg, word>, CryptoPP::Z>::operator () (const CryptoPP::Z &u) const {
        if constexpr (neg == negativity::nones) {
            if (data::is_negative (u)) throw exception {} << "try to convert negative integer " << u << " to unsigned type";
        }

        if (data::is_zero (u))
            return number::Z_bytes<r, neg, word> {};

        number::Z_bytes<r, neg, word> x;
        size_t new_size;
        size_t bit_width = data::bit_width (u);
        constexpr const size_t word_bits = sizeof (word) * CHAR_BIT;

        if constexpr (neg == negativity::nones) {
            new_size = (bit_width + word_bits - 2) / word_bits;
        } else {
            if constexpr (neg == negativity::BC) {
                if (bit_width == data::bit_width (data::abs (u)) - 1)
                    ++bit_width;
            }
            new_size = (bit_width + word_bits - 1) / word_bits;
        }

        x.resize (new_size);
        CryptoPP::export_bin (slice<word> {x.data (), x.size ()}, u, r, endian::native, neg);
        return x;
    }

    template <endian r, negativity neg, Word word>
    CryptoPP::Z inline convert<CryptoPP::Z, number::Z_bytes<r, neg, word>>::operator () (const number::Z_bytes<r, neg, word> &u) const {
        return CryptoPP::Z {CryptoPP::import_bin (slice<const word> {u.data (), u.size ()}, r, endian::native, neg)};
    }

    template <bool is_signed, endian r, size_t size, Word word>
    CryptoPP::Z inline convert<CryptoPP::Z, number::bounded<is_signed, r, size, word>>::operator () (const number::bounded<is_signed, r, size, word> &u) const {
        return CryptoPP::Z {CryptoPP::import_bin (slice<const word> {u.data (), size}, r, endian::native, is_signed ? negativity::twos : negativity::nones)};
    }

    template <bool is_signed, endian r, size_t size, Word word>
    number::bounded<is_signed, r, size, word> convert<number::bounded<is_signed, r, size, word>, CryptoPP::Z>::operator () (const CryptoPP::Z &u) const {
        if constexpr (!is_signed)
            if (data::is_negative (u)) throw exception {} << "try to convert negative integer " << u << " to unsigned type";
        else if (u < numeric_limits<number::bounded<is_signed, r, size, word>>::min ())
            throw exception {} << "cryptopp integer " << u << " is too low to fit in a signed number of size " << (size * sizeof (word));

        if (u > numeric_limits<number::bounded<is_signed, r, size, word>>::max ())
            throw exception {} << "cryptopp integer " << u << " is too high to fit in a number of size " << size;

        number::bounded<is_signed, r, size, word> x;
        CryptoPP::export_bin (slice<word> {x.data (), x.size ()}, u, r, endian::native,
            is_signed ? negativity::twos : negativity::nones);
        return x;
    }

    template <negativity neg, hex_case zz>
    CryptoPP::Z convert<CryptoPP::Z, hex::integer<neg, zz>>::operator () (const hex::integer<neg, zz> &u) const {
        return CryptoPP::Z (Z_bytes_big (u));
    }

    CryptoPP::Z inline convert<CryptoPP::Z, dec_uint>::operator () (const dec_uint &u) const {
        if (!u.valid ()) throw exception {} << "try to read integer from invalid dec_int " << u;
        return CryptoPP::Z {std::move (CryptoPP::import_dec (u))};
    }

    CryptoPP::Z inline convert<CryptoPP::Z, dec_int>::operator () (const dec_int &u) const {
        if (!u.valid ()) throw exception {} << "try to read integer from invalid dec_int " << u;
        return CryptoPP::Z {std::move (CryptoPP::import_dec (u))};
    }

    CryptoPP::Z inline convert<CryptoPP::Z, base58_uint>::operator () (const base58_uint &u) const {
        auto decoded = encoding::base58::decode<CryptoPP::Z> (u);
        if (!decoded) throw exception {} << "try to read integer from invalid base58 string " << u;
        return *decoded;
    }

    division<CryptoPP::Z, CryptoPP::Z> inline
    divmod<CryptoPP::Z, CryptoPP::Z>::operator () (
        const CryptoPP::Z &a,
        const nonzero<CryptoPP::Z> &b
    ) {
        CryptoPP::Z q;
        CryptoPP::Z r;

        CryptoPP::Integer::Divide (r, q, a, b.Value);

        return {q, r};
    }
}

namespace CryptoPP {

    std::ostream inline &operator << (std::ostream &o, const Z &n) {
        return o << export_dec (n);
    }

    Z inline operator + (const Z &a, const Z &b) {
        return Z {static_cast<const CryptoPP::Integer &>(a)
            + static_cast<const CryptoPP::Integer &>(b)};
    }

    Z inline operator - (const Z &a, const Z &b) {
        return Z {static_cast<const CryptoPP::Integer &> (a)
            - static_cast<const CryptoPP::Integer &>(b)};
    }

    Z inline operator - (const Z &a) {
        return Z {-static_cast<const CryptoPP::Integer &>(a)};
    }

    Z inline operator * (const Z &a, const Z &b) {
        return Z {static_cast<const CryptoPP::Integer &>(a)
            * static_cast<const CryptoPP::Integer &>(b)};
    }

    Z inline &operator ++ (Z &a) {
        static_cast<CryptoPP::Integer &>(a) += CryptoPP::Integer::One ();
        return a;
    }

    Z inline &operator -- (Z &a) {
        static_cast<CryptoPP::Integer &>(a) -= CryptoPP::Integer::One ();
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

    template <Word U> Integer import_bin (
        slice<const U> input,
        // the ordering of the overall array.
        endian word_order,
        // the ordering of each value in the array.
        endian byte_order,
        negativity neg
    ) {

        if (input.size () == 0) return Integer ();

        // for certain values, we can directly translate into
        // the cryptopp input function.
        if ((neg == negativity::nones || neg == negativity::twos) &&
            (sizeof (U) == sizeof (byte) || word_order == byte_order)
        ) return Integer (
            reinterpret_cast<const byte *> (input.data ()),
            input.size () * sizeof (U),
            neg == negativity::nones ? Integer::UNSIGNED : Integer::SIGNED,
            word_order == endian::little ? LITTLE_ENDIAN_ORDER : BIG_ENDIAN_ORDER);

        data::bytestring<U> bytes (input.size ());

        // everything goes to native endian so that we
        // can use BC::negate below if we have to.

        // first put the whole thing in native word order.
        if (word_order == endian::native)
            std::copy (input.begin (), input.end (), bytes.begin ());
        else std::copy (input.rbegin (), input.rend (), bytes.begin ());

        // next swap the bytes if needed.
        if (byte_order != endian::native && sizeof (word) > 1)
            for (U &u : bytes) u = std::byteswap (u);

        // for negativity nones we don't have to worry about negative numbers and for
        // negativity twos they can be imported natively. For BC we have to take the
        // absolute value, import that, and then tegate it.
        bool bc_negate = neg == negativity::BC && NTL::is_negative (input, word_order, byte_order, neg);

        // negate
        if (bc_negate) data::arithmetic::BC::negate<endian::native, U> (bytes);

        Integer z (
            reinterpret_cast<const byte *> (bytes.data ()),
                input.size () * sizeof (U),
                neg == negativity::nones || neg == negativity::BC? Integer::UNSIGNED : Integer::SIGNED,
                endian::native == endian::big ? BIG_ENDIAN_ORDER : LITTLE_ENDIAN_ORDER);

        return bc_negate ? -z : z;
    }

    template <Word U> void export_bin (
        slice<U> output,
        const Integer &x,
        // the ordering of the overall array.
        endian word_order,
        // the ordering of each value in the array.
        endian byte_order,
        negativity neg) {

        if (x.IsZero ()) {
            std::fill (output.begin (), output.end (), U {0});
            return;
        }

        auto size = output.size () * sizeof (U);

        if (neg == negativity::nones) {
            if (x.IsNegative () ||
                x.MinEncodedSize (CryptoPP::Integer::UNSIGNED) > size)
                throw data::exception {} << "CryptoPP Integer does not fit into " << neg << " size " << size;

            x.Encode (
                reinterpret_cast<byte *> (output.data ()),
                size,
                CryptoPP::Integer::UNSIGNED);
        } else if (neg == negativity::twos) {
            if (x.MinEncodedSize (CryptoPP::Integer::SIGNED) > size)
                throw data::exception {} << "CryptoPP Integer does not fit into " << neg << " size " << size;

            x.Encode (
                reinterpret_cast<byte *> (output.data ()),
                size,
                CryptoPP::Integer::SIGNED);
        } else {
            auto a = data::abs (Z (x));

            if (data::bit_width (a) > size * CHAR_BIT)
                throw data::exception {"integer does not fit"};

            auto bytes = reinterpret_cast<byte *> (output.data ());

            a.Encode (
                bytes,
                size,
                CryptoPP::Integer::UNSIGNED);

            // set sign bit
            if (x.IsNegative ()) bytes[0] |= 0x80;

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
