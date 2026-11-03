// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/cross.hpp>
#include <data/array.hpp>
#include <data/arithmetic/arithmetic.hpp>
#include <data/arithmetic/words.hpp>
#include <data/encoding/hex.hpp>
#include <data/tools/lazy_writer.hpp>

namespace data {

    template <Word word> struct bytestring;

    using bytes = bytestring<byte>;

    template <Word word>
    std::ostream &operator << (std::ostream &o, const bytestring<byte> &s);

    template <Word word>
    bytestring<word> operator ~ (const bytestring<word> &b);

    template <Word word>
    bytestring<word> operator << (const bytestring<word> &b, int32 i);

    template <Word word>
    bytestring<word> operator >> (const bytestring<word> &b, int32 i);

    template <Word word>
    struct bytestring : public cross<word> {
        using cross<word>::cross;
        bytestring (slice<const word> v);

        operator slice<const word> () const;

        operator slice<word> ();

        bytestring &bit_negate ();

        void bit_shift_left (uint32 x, bool fill = false);
        void bit_shift_right (uint32 x, bool fill = false);
    };

    using lazy_bytes_writer = lazy_writer<bytes, byte>;

    using lazy_bytes_writer = lazy_writer<bytes, byte>;

    template <endian r, typename word, size_t ... sizes> struct oriented;

    template <endian r, Word word>
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
    template <endian r, Word word, size_t size>
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

    template <Word word>
    inline bytestring<word>::bytestring (slice<const word> v) : cross<word> (v.size ()) {
        std::copy (v.begin (), v.end (), this->begin ());
    }

    template <Word word>
    inline bytestring<word>::operator slice<const word> () const {
        return slice<const word> {this->data (), this->size ()};
    }

    template <Word word>
    inline bytestring<word>::operator slice<word> () {
        return {this->data (), this->size ()};
    }

    template <Word word>
    bytestring<word> inline &bytestring<word>::bit_negate () {
        arithmetic::bit_negate<word> (this->begin (), this->end (), this->begin ());
        return *this;
    }

    template <Word word>
    writer<word> inline &operator << (writer<word> &w, const bytestring<word> &x) {
        w.write (x.data (), x.size ());
        return w;
    }

    template <Word word>
    reader<word> inline &operator >> (reader<word> &r, bytestring<word> &x) {
        r.read (x.data (), x.size ());
        return r;
    }

    template <Word word>
    bytestring<word> operator ~ (const bytestring<word> &b) {
        bytestring n (b);
        n.bit_negate ();
        return n;
    }

    template <Word word>
    bytestring<word> operator << (const bytestring<word> &b, int32 i) {
        bytestring n (b);
        if (i < 0) n.bit_shift_right (-i);
        else n.bit_shift_left (i);
        return n;
    }

    template <Word word>
    bytestring<word> operator >> (const bytestring<word> &b, int32 i) {
        bytestring n (b);
        if (i < 0) n.bit_shift_left (-i);
        else n.bit_shift_right (i);
        return n;
    }

    template <Word word>
    void inline bytestring<word>::bit_shift_left (uint32 x, bool fill) {
        arithmetic::Words<endian::big, word> (slice<word> (*this)).bit_shift_left (x, fill);
    }

    template <Word word>
    void inline bytestring<word>::bit_shift_right (uint32 x, bool fill) {
        arithmetic::Words<endian::big, word> (slice<word> (*this)).bit_shift_right (x, fill);
    }

    template <Word word>
    std::ostream inline &operator << (std::ostream &o, const bytestring<word> &s) {
        return o << "\"" << encoding::hex::write (slice<const word> (s)) << "\"";
    }

}
