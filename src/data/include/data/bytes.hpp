// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_BYTES
#define DATA_BYTES

#include <data/cross.hpp>
#include <data/encoding/hex.hpp>

namespace data {

    template <std::integral word> struct bytestring;

    using bytes = bytestring<byte>;

    template <std::integral word>
    std::ostream &operator << (std::ostream &o, const bytestring<byte> &s);

    using hex_string = encoding::hex::string;

    template <std::integral word>
    struct bytestring : public cross<word> {
        using cross<word>::cross;
        bytestring (view<word> v);
        bytestring (const hex_string &);

        operator view<word> () const;

        operator slice<word> ();

        bytestring &bit_negate ();

        void bit_shift_left (uint32 x, bool fill = false);
        void bit_shift_right (uint32 x, bool fill = false);
    };

    template <std::integral word>
    bytestring<word> operator ~ (const bytestring<word> &b);

    template <std::integral word>
    bytestring<word> operator << (const bytestring<word> &b, int32 i);

    template <std::integral word>
    bytestring<word> operator >> (const bytestring<word> &b, int32 i);

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

    template <std::integral word, size_t...> struct bytes_array;

    template <std::integral word, size_t... sizes>
    writer<word> &operator << (writer<word> &, const bytes_array<word, sizes...> &);

    // all constructors constexpr
    template <std::integral word, size_t size> struct bytes_array<word, size> : public array<word, size> {
        using array<word, size>::array;

        constexpr bytes_array (slice<const word, size> v);

        bytes_array (const hex_string &);

        static bytes_array filled (const word &x) {
            bytes_array n {};
            n.fill (x);
            return n;
        }

        operator view<word> () const;
        operator slice<word, size> ();
        operator slice<const word, size> () const;

        bytes_array operator | (const slice<word, size> a) const {
            bytes_array n (*this);
            n.bit_or (a);
            return n;
        }

        bytes_array operator & (const slice<word, size> a) const {
            bytes_array n (*this);
            n.bit_and (a);
            return n;
        }

        bytes_array operator ^ (const slice<word, size> a) const {
            bytes_array n (*this);
            n.bit_xor (a);
            return n;
        }

        bytes_array &bit_negate () {
            arithmetic::bit_negate<word> (this->end (), this->begin (), this->begin ());
            return *this;
        }

    protected:
        void bit_and (const slice<word, size> a) {
            arithmetic::bit_and<word>
                (this->end (), this->begin (), slice<word, size> (*this).begin (), a.begin ());
        }

        void bit_or (const slice<word, size> a) {
            arithmetic::bit_or<word>
                (this->end (), this->begin (), slice<word, size> (*this).begin (), a.begin ());
        }

        void bit_xor (const slice<word, size> a) {
            arithmetic::bit_xor<word>
                (this->end (), this->begin (), slice<word, size> (*this).begin (), a.begin ());
        }

        void fill (byte b) {
            for (byte &z : *this) z = b;
        }

    };

    template <std::integral word, size_t size>
    bytes_array<word, size> operator ~ (const bytes_array<word, size> &b);

    template <std::integral word, size_t size>
    bytes_array<word, size> operator << (const bytes_array<word, size> &b, int32 i);

    template <std::integral word, size_t size>
    bytes_array<word, size> operator >> (const bytes_array<word, size> &b, int32 i);

    template <size_t size> using byte_array = bytes_array<byte, size>;

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

        words_type words () {
            return words_type {slice<word> (*this)};
        }

        const words_type words () const {
            return words_type {slice<word> (*const_cast<oriented*> (this))};
        }
    };

    template <std::integral word>
    inline bytestring<word>::bytestring (view<word> v) : cross<word> (v.size ()) {
        std::copy (v.begin (), v.end (), this->begin ());
    }

    template <std::integral word>
    inline bytestring<word>::operator view<word> () const {
        return view<word> {this->data (), this->size ()};
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

    template <std::integral word, size_t... sizes>
    writer<word> inline &operator << (writer<word> &w, const bytes_array<word, sizes...> &x) {
        w.write (x.data (), (1 * sizes)...);
        return w;
    }

    template <std::integral word>
    reader<word> inline &operator >> (reader<word> &r, bytestring<word> &x) {
        r.read (x.data (), x.size ());
        return r;
    }

    template <std::integral word, size_t... sizes>
    reader<word> inline &operator >> (reader<word> &r, bytes_array<word, sizes...> &x) {
        r.read (x.data (), (1 * sizes)...);
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

    template <std::integral word, size_t size>
    bytes_array<word, size> operator ~ (const bytes_array<word, size> &b) {
        bytes_array<word, size> n;
        arithmetic::bit_negate<word> (n.end (), n.begin (), b.begin ());
        return n;
    }

    template <std::integral word, size_t size>
    bytes_array<word, size> operator << (const bytes_array<word, size> &b, int32 i) {
        bytes_array<word, size> n (b);
        if (i < 0) n.bit_shift_right (-i);
        else n.bit_shift_left (i);
        return n;
    }

    template <std::integral word, size_t size>
    bytes_array<word, size> operator >> (const bytes_array<word, size> &b, int32 i) {
        bytes_array<word, size> n (b);
        if (i < 0) n.bit_shift_left (-i);
        else n.bit_shift_right (i);
        return n;
    }

    template <std::integral word, size_t size>
    inline bytes_array<word, size>::operator slice<word, size> () {
        return {this->data ()};
    }

    template <std::integral word, size_t size>
    inline bytes_array<word, size>::operator view<word> () const {
        return {this->data (), size};
    }

    template <std::integral word, size_t size>
    inline bytes_array<word, size>::operator slice<const word, size> () const {
        return {const_cast<word *> (this->data ())};
    }

    template <std::integral word>
    std::ostream inline &operator << (std::ostream &o, const bytestring<word> &s) {
        return o << "\"" << encoding::hex::write (slice<const word> (s)) << "\"";
    }

    template <std::integral word> bytestring<word>::bytestring (const hex_string &x) {
        if (!x.valid () || ((x.size () / 2) % sizeof (word) != 0)) throw encoding::invalid {encoding::hex::Format, x};
        if ((x.size () / 2) % sizeof (word) != 0) throw exception {} << "invalid hex string size";
        this->resize (x.size () / (sizeof (word) * 2));
        boost::algorithm::unhex (x.begin (), x.end (), static_cast<byte *> (this->data ()));
    }

    template <std::integral word, size_t size> bytes_array<word, size>::bytes_array (const hex_string &x) {
        if (!x.valid () || ((x.size () / 2) % sizeof (word) != 0)) throw encoding::invalid {encoding::hex::Format, x};
        if ((x.size () / 2) != sizeof (word) * size) throw exception {} << "invalid hex string size";
        boost::algorithm::unhex (x.begin (), x.end (), static_cast<byte *> (this->data ()));
    }

}

#endif

