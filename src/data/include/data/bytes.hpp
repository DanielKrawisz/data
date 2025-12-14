// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_BYTES
#define DATA_BYTES

#include <data/cross.hpp>
#include <data/array.hpp>
#include <data/arithmetic/arithmetic.hpp>
#include <data/arithmetic/words.hpp>
#include <data/encoding/hex.hpp>

namespace data {

    template <std::integral word> struct bytestring;

    using bytes = bytestring<byte>;

    template <std::integral word>
    std::ostream &operator << (std::ostream &o, const bytestring<byte> &s);

    template <std::integral word>
    bytestring<word> operator ~ (const bytestring<word> &b);

    template <std::integral word>
    bytestring<word> operator << (const bytestring<word> &b, int32 i);

    template <std::integral word>
    bytestring<word> operator >> (const bytestring<word> &b, int32 i);

    template <std::integral word>
    struct bytestring : public cross<word> {
        using cross<word>::cross;
        bytestring (slice<const word> v);

        operator slice<const word> () const;

        operator slice<word> ();

        bytestring &bit_negate ();

        void bit_shift_left (uint32 x, bool fill = false);
        void bit_shift_right (uint32 x, bool fill = false);
    };

    namespace {
        template <typename X>
        writer<byte> &write_to_writer (writer<byte> &w, X x) {
            return w << x;
        }

        template <typename X, typename... P>
        writer<byte> &write_to_writer (writer<byte> &w, X x, P... p) {
           return write_to_writer (write_to_writer (w, x), p...);
        }
    }

    template <typename X, typename... P>
    bytes write_bytes (size_t size, X x, P... p) {
        bytes b (size);
        iterator_writer w (b.begin (), b.end ());
        write_to_writer (w, x, p...);
        return b;
    }

    template <endian::order r, typename word, size_t ... sizes> struct oriented;

    template <endian::order r, std::unsigned_integral word>
    struct oriented<r, word> : public bytestring<word> {
        using bytestring<word>::bytestring;

        using words_type = arithmetic::Words<r, word>;

        words_type words () {
            return words_type {slice<word> (*this)};
        }

        const words_type words () const {
            return words_type {slice<word> (*const_cast<oriented*> (this))};
        }
    };

    // all constructors constexpr
    template <endian::order r, std::unsigned_integral word, size_t size>
    struct oriented<r, word, size> : public bytes_array<word, size> {
        using bytes_array<word, size>::bytes_array;
        constexpr oriented (const bytes_array<word, size> &x) : bytes_array<word, size> {x} {}

        using words_type = arithmetic::Words<r, word>;

        constexpr words_type words () {
            return words_type {slice<word> (*this)};
        }

        constexpr const words_type words () const {
            return words_type {slice<word> (*const_cast<oriented*> (this))};
        }
    };

    template <std::integral word>
    inline bytestring<word>::bytestring (slice<const word> v) : cross<word> (v.size ()) {
        std::copy (v.begin (), v.end (), this->begin ());
    }

    template <std::integral word>
    inline bytestring<word>::operator slice<const word> () const {
        return slice<const word> {this->data (), this->size ()};
    }

    template <std::integral word>
    inline bytestring<word>::operator slice<word> () {
        return {this->data (), this->size ()};
    }

    template <std::integral word>
    bytestring<word> inline &bytestring<word>::bit_negate () {
        arithmetic::bit_negate<word> (this->end (), this->begin (), this->begin ());
        return *this;
    }

    template <std::integral word>
    writer<word> inline &operator << (writer<word> &w, const bytestring<word> &x) {
        w.write (x.data (), x.size ());
        return w;
    }

    template <std::integral word>
    reader<word> inline &operator >> (reader<word> &r, bytestring<word> &x) {
        r.read (x.data (), x.size ());
        return r;
    }

    template <std::unsigned_integral word>
    bytestring<word> operator ~ (const bytestring<word> &b) {
        bytestring n (b);
        n.bit_negate ();
        return n;
    }

    template <std::integral word>
    bytestring<word> operator << (const bytestring<word> &b, int32 i) {
        bytestring n (b);
        if (i < 0) n.bit_shift_right (-i);
        else n.bit_shift_left (i);
        return n;
    }

    template <std::integral word>
    bytestring<word> operator >> (const bytestring<word> &b, int32 i) {
        bytestring n (b);
        if (i < 0) n.bit_shift_left (-i);
        else n.bit_shift_right (i);
        return n;
    }

    template <std::integral word>
    void inline bytestring<word>::bit_shift_left (uint32 x, bool fill) {
        arithmetic::Words<endian::big, word> (slice<word> (*this)).bit_shift_left (x, fill);
    }

    template <std::integral word>
    void inline bytestring<word>::bit_shift_right (uint32 x, bool fill) {
        arithmetic::Words<endian::big, word> (slice<word> (*this)).bit_shift_right (x, fill);
    }

    template <std::integral word>
    std::ostream inline &operator << (std::ostream &o, const bytestring<word> &s) {
        return o << "\"" << encoding::hex::write (slice<const word> (s)) << "\"";
    }

}

namespace data::encoding::hex {

    template <std::integral word> string::operator bytestring<word> () const {
        if (!this->valid ()) throw invalid {} << ": " << *this;
        if ((this->size () / 2) % sizeof (word) != 0)
            throw invalid {} << ": invalid hex string size " << this->size () << " for word size " << sizeof (word);
        bytestring<word> z (this->size () / (sizeof (word) * 2));
        decode (this->end (), this->begin (), z.data ());
        return z;
    }
}

#endif

