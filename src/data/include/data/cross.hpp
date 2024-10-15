// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CROSS
#define DATA_CROSS

#include <data/sequence.hpp>
#include <data/slice.hpp>
#include <data/valid.hpp>
#include <data/arithmetic/words.hpp>
#include <data/stream.hpp>
#include <data/math/abs.hpp>
#include <data/encoding/hex.hpp>

namespace data {
    using hex_string = encoding::hex::string;
    
    // The cartesian product. 
    // it is the same as a vector with some slight improvements. 
    template <typename X> struct cross : std::vector<X> {
        constexpr cross ();
        explicit cross (size_t size);
        explicit cross (size_t size, X fill);
        
        cross (std::initializer_list<X> x);
        
        template<sequence list>
        explicit cross (list l);
        
        bool valid () const {
            for (const X &x : *this) if (!data::valid (x)) return false;
            return true;
        }
        
        X &operator [] (int i) {
            size_t size = this->size ();
            if (size == 0) throw std::out_of_range {"cross size 0"};
            if (i < 0 || i >= size) return this->operator [] ((i + size) % size);
            return std::vector<X>::operator [] (i);
        }
        
        const X &operator [] (int i) const {
            size_t size = this->size ();
            if (size == 0) throw std::out_of_range {"cross size 0"};
            if (i < 0 || i >= size) return this->operator [] ((i + size) % size);
            return std::vector<X>::operator [] (i);
        }
        
        explicit operator slice<X> ();
        explicit operator slice<const X> () const;
        
        slice<X> range (int);
        slice<X> range (int, int);

        slice<const X> range (int) const;
        slice<const X> range (int, int) const;
        
        cross (std::vector<X> &&v) : std::vector<X> {v} {}
        
    protected:
        void fill (const X &x) {
            for (X& z : *this) z = x;
        }
        
    };
    
    template <typename X>
    std::ostream &operator << (std::ostream &o, const cross<X> &s);

    template <typename X, size_t... > struct array;
    
    template <typename X, size_t size> struct array<X, size> : public std::array<X, size> {
        constexpr static size_t Size = size;
        using std::array<X, size>::array;

        constexpr array (std::initializer_list<X> x);
        // make an array filled with a particular value.
        static array filled (const X &x);

        bool valid () const;

        X &operator [] (size_t i);
        const X &operator [] (size_t i) const;

        explicit operator slice<X, size> ();

        slice<X> range (int);
        slice<X> range (int, int);

        explicit operator slice<const X, size> () const;

        slice<const X> range (int) const;
        slice<const X> range (int, int) const;
        
    protected:
        template <typename iterator> array (iterator it) : array () {
            for (X &x : *this) {
                x = *it; 
                it++;
            }
        }
    };

    template <typename X, size_t size, size_t... sizes> struct array<X, size, sizes...> : public array<X, size * array<X, sizes...>::Size> {
        constexpr static size_t Size = size * array<X, sizes...>::Size;
        array ();
        array (std::initializer_list<array<X, sizes...>>);

        slice<X, sizes...> &operator [] (size_t i);
        slice<const X, sizes...> &operator [] (size_t i) const;

    };

    // vector addition and subtraction.
    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x + y} -> implicitly_convertible_to<X>;
    } array<X, sizes...> operator + (const array<X, sizes...> &, const array<X, sizes...> &);

    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x + y} -> implicitly_convertible_to<X>;
    } array<X, sizes...> operator - (const array<X, sizes...> &, const array<X, sizes...> &);

    template <typename X, size_t... sizes> requires requires (const X &x) {
        {-x} -> implicitly_convertible_to<X>;
    } array<X, sizes...> operator - (const array<X, sizes...> &);

    // scalar multiplication and division.
    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x * y} -> implicitly_convertible_to<X>;
    } array<X, sizes...> operator * (const array<X, sizes...> &, const X &);

    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x / y} -> implicitly_convertible_to<X>;
    } array<X, sizes...> operator / (const array<X, sizes...> &, const X &);

    // multiplication operation good enough for an inner product and matrix multiplication
    template <typename X, size_t... A, size_t C, size_t... B> requires requires (const X &x, const X &y) {
        {x + y} -> implicitly_convertible_to<X>;
        {x * inner (x, y)} -> implicitly_convertible_to<X>;
    } && requires () {
        {X {}};
    } array<X, A..., B...> operator * (const array<X, A..., C> &a, const array<X, C, B...> &b);

    template <std::integral word> struct bytestring;

    template <std::integral word>
    writer<word> &operator << (writer<word> &, const bytestring<word> &);

    using bytes = bytestring<byte>;
    
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
    
    std::ostream &operator << (std::ostream &o, const bytes &s);
    
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
        
        explicit operator slice<byte> () {
            return slice<byte> {(byte*) this->data (), size * sizeof (word)};
        }
    };

    template <typename X, size_t size>
    constexpr inline array<X, size>::array (std::initializer_list<X> x) : std::array<X, size> {} {
        if (x.size () != size) throw exception {} << "out of range";
        size_t index = 0;
        for (const X &xx : x) std::array<X, size>::operator [] (index++) = xx;
    }

    template <typename X, size_t size>
    array<X, size> inline array<X, size>::filled (const X &x) {
        array n {};
        n.fill (x);
        return n;
    }

    template <typename X, size_t size>
    bool inline array<X, size>::valid () const {
        for (const X &x : *this) if (!data::valid (x)) return false;
        return true;
    }

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

    template <typename X, size_t size>
    X inline &array<X, size>::operator [] (size_t i) {
        if (size == 0) throw std::out_of_range {"cross size 0"};
        if (i < 0 || i >= size) return this->operator [] ((i + size) % size);
        return std::array<X, size>::operator [] (i);
    }

    template <typename X, size_t size>
    const X inline &array<X, size>::operator [] (size_t i) const {
        if (size == 0) throw std::out_of_range {"cross size 0"};
        if (i < 0 || i >= size) return this->operator [] ((i + size) % size);
        return std::array<X, size>::operator [] (i);
    }
    
    template <std::unsigned_integral word>
    writer<word> inline &operator << (writer<word> &w, const bytestring<word> &x) {
        w.write (x.data (), x.size ());
        return w;
    }
    
    template <std::unsigned_integral word, size_t... sizes>
    writer<word> inline &operator << (writer<word> &w, const bytes_array<word, sizes...> &x) {
        w.write (x.data (), (1 * sizes)...);
        return w;
    }
    
    template <std::unsigned_integral word>
    reader<word> inline &operator >> (reader<word> &r, bytestring<word> &x) {
        r.read (x.data (), x.size ());
        return r;
    }
    
    template <std::unsigned_integral word, size_t... sizes>
    reader<word> inline &operator >> (reader<word> &r, bytes_array<word, sizes...> &x) {
        r.read (x.data (), (1 * sizes)...);
        return r;
    }
    
    template <typename X>
    std::ostream &operator << (std::ostream &o, const cross<X> &s) {
        auto b = s.begin ();
        o << "[";
        while (true) {
            if (b == s.end ()) return o << "]";
            else if (b != s.begin ()) o << ", ";
            o << *b;
            b++;
        }
    }
    
    template <std::unsigned_integral word>
    bytestring<word> operator ~ (const bytestring<word> &b) {
        bytestring n (b);
        n.bit_negate ();
        return n;
    }
    
    template <std::unsigned_integral word>
    bytestring<word> operator << (const bytestring<word> &b, int32 i) {
        bytestring n (b);
        if (i < 0) n.bit_shift_right (-i);
        else n.bit_shift_left (i);
        return n;
    }
    
    template <std::unsigned_integral word>    
    bytestring<word> operator >> (const bytestring<word> &b, int32 i) {
        bytestring n (b);
        if (i < 0) n.bit_shift_left (-i);
        else n.bit_shift_right (i);
        return n;
    }
    
    template <std::unsigned_integral word>
    void inline bytestring<word>::bit_shift_left (uint32 x, bool fill) {
        arithmetic::Words<endian::big, word> (slice<word> (*this)).bit_shift_left (x, fill);
    }
    
    template <std::unsigned_integral word>
    void inline bytestring<word>::bit_shift_right (uint32 x, bool fill) {
        arithmetic::Words<endian::big, word> (slice<word> (*this)).bit_shift_right (x, fill);
    }
    
    template <std::unsigned_integral word, size_t size> 
    bytes_array<word, size> operator ~ (const bytes_array<word, size> &b) {
        bytes_array<word, size> n;
        arithmetic::bit_negate<word> (n.end (), n.begin (), b.begin ());
        return n;
    }
    
    template <std::unsigned_integral word, size_t size> 
    bytes_array<word, size> operator << (const bytes_array<word, size> &b, int32 i) {
        bytes_array<word, size> n (b);
        if (i < 0) n.bit_shift_right (-i);
        else n.bit_shift_left (i);
        return n;
    }
    
    template <std::unsigned_integral word, size_t size>
    bytes_array<word, size> operator >> (const bytes_array<word, size> &b, int32 i) {
        bytes_array<word, size> n (b);
        if (i < 0) n.bit_shift_left (-i);
        else n.bit_shift_right (i);
        return n;
    }
    
    template <typename X>
    constexpr inline cross<X>::cross () : std::vector<X> {} {}
    
    template <typename X>
    inline cross<X>::cross (size_t size) : std::vector<X> (size) {}
    
    template <typename X>
    inline cross<X>::cross (size_t size, X fill) : std::vector<X> (size) {
        for (auto it = std::vector<X>::begin (); it < std::vector<X>::end (); it++) *it = fill;
    }
    
    template <typename X>
    inline cross<X>::cross (std::initializer_list<X> x) : std::vector<X> {x} {}
    
    template <typename X>
    template<sequence list>
    cross<X>::cross (list l) : cross {} {
        std::vector<X>::resize (data::size (l));
        auto b = std::vector<X>::begin ();
        while (!l.empty ()) {
            *b = l.first ();
            l = l.rest ();
            b++;
        }
    }
    
    template <typename X>
    inline cross<X>::operator slice<X> () {
        return slice<X> (static_cast<std::vector<X> &> (*this));
    }
    
    template <typename X>
    inline slice<X> cross<X>::range (int e) {
        return operator slice<X> ().range (e);
    }
    
    template <typename X>
    inline slice<X> cross<X>::range (int b, int e) {
        return operator slice<X> ().range (e);
    }
    
    template <std::unsigned_integral word, size_t size>
    inline bytes_array<word, size>::operator slice<word, size> () {
        return {this->data ()};
    }
    
    template <std::unsigned_integral word, size_t size>
    inline bytes_array<word, size>::operator view<word> () const {
        return {this->data (), size};
    }

    template <std::unsigned_integral word, size_t size>
    inline bytes_array<word, size>::operator slice<const word, size> () const {
        return {const_cast<word *> (this->data ())};
    }

    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x + y} -> implicitly_convertible_to<X>;
    } array<X, sizes...> operator + (const array<X, sizes...> &a, const array<X, sizes...> &b) {
        array<X, sizes...> x {};
        auto ai = a.begin ();
        auto bi = b.begin ();

        for (auto xi = x.begin (); xi != x.end (); xi++) {
            *xi = *ai + *bi;
            ai++;
            bi++;
        }

        return x;
    }

    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x - y} -> implicitly_convertible_to<X>;
    } array<X, sizes...> operator + (const array<X, sizes...> &a, const array<X, sizes...> &b) {
        array<X, sizes...> x {};
        auto ai = a.begin ();
        auto bi = b.begin ();

        for (auto xi = x.begin (); xi != x.end (); xi++) {
            *xi = *ai - *bi;
            ai++;
            bi++;
        }

        return x;
    }

    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x * y} -> implicitly_convertible_to<X>;
    } array<X, sizes...> operator * (const array<X, sizes...> &a, const X &b) {
        array<X, sizes...> x {};

        auto ai = a.begin ();
        for (auto xi = x.begin (); xi != x.end (); xi++) {
            *xi = *ai * b;
            ai++;
        }

        return x;
    }

    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x / y} -> implicitly_convertible_to<X>;
    } array<X, sizes...> operator / (const array<X, sizes...> &a, const X &b) {
        array<X, sizes...> x {};

        auto ai = a.begin ();
        for (auto xi = x.begin (); xi != x.end (); xi++) {
            *xi = *ai / b;
            ai++;
        }

        return x;
    }

    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x + y} -> implicitly_convertible_to<X>;
        {x * y} -> implicitly_convertible_to<X>;
    } && requires () {
        {X {0}};
    } X operator * (const array<X, sizes...> &a, const array<X, sizes...> &b) {
        X x {0};
        auto bi = b.begin ();

        for (auto ai = a.begin (); ai != a.end (); ai++) {
            x += *ai * *bi;
            bi++;
        }

        return x;
    }

    std::ostream inline &operator << (std::ostream &o, const bytes &s) {
        return o << "\"" << encoding::hex::write (s) << "\"";
    }

    template <std::integral word>
    bytestring<word>::bytestring (const hex_string &x) {
        if (!x.valid () || ((x.size () / 2) % sizeof (word) != 0)) throw encoding::invalid {encoding::hex::Format, x};
        if ((x.size () / 2) % sizeof (word) != 0) throw exception {} << "invalid hex string size";
        this->resize (x.size () / (sizeof (word) * 2));
        boost::algorithm::unhex (x.begin (), x.end (), static_cast<byte *> (this->data ()));
    }
    
}

#endif


