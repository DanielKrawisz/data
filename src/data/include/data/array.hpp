// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ARRAY
#define DATA_ARRAY

#include <data/slice.hpp>
#include <data/valid.hpp>
#include <data/stream.hpp>
#include <data/integral.hpp>
#include <data/encoding/hex.hpp>
#include <data/arithmetic/arithmetic.hpp>
#include <data/arithmetic/words.hpp>

namespace data {

    template <typename X, size_t... > struct array;

    template <std::integral word, size_t ...sizes> using bytes_array = array<word, sizes...>;

    template <size_t size> using byte_array = bytes_array<byte, size>;

    template <size_t size> byte_array<size> read_byte_array (const hex_string &h);

    template <typename X, size_t... sizes>
    constexpr bool operator == (const array<X, sizes...> &a, const array<X, sizes... > &b);

    template <typename X, size_t... sizes>
    constexpr auto operator <=> (const array<X, sizes...> &a, const array<X, sizes... > &b) -> decltype (*a.begin () <=> *b.begin ());

    template <std::integral word, size_t size>
    constexpr bytes_array<word, size> operator ~ (const bytes_array<word, size> &b);

    template <std::integral word, size_t size>
    constexpr bytes_array<word, size> operator ^ (const bytes_array<word, size> &, const bytes_array<word, size> &);

    template <std::integral word, size_t size>
    constexpr bytes_array<word, size> operator & (const bytes_array<word, size> &, const bytes_array<word, size> &);

    template <std::integral word, size_t size>
    constexpr bytes_array<word, size> operator | (const bytes_array<word, size> &, const bytes_array<word, size> &);

    // bit shift
    template <std::integral word, size_t size>
    constexpr bytes_array<word, size> operator << (const bytes_array<word, size> &b, int32 i);

    template <std::integral word, size_t size>
    constexpr bytes_array<word, size> operator >> (const bytes_array<word, size> &b, int32 i);

    // write
    template <std::integral word, size_t... sizes>
    writer<word> &operator << (writer<word> &, const bytes_array<word, sizes...> &);

    template <std::integral word, size_t... sizes>
    reader<word> &operator >> (reader<word> &, bytes_array<word, sizes...> &);

    template <std::integral word, size_t size>
    std::ostream &operator << (std::ostream &o, const array<word, size> &s);

    // vector addition and subtraction.
    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x + y} -> ImplicitlyConvertible<X>;
    } constexpr array<X, sizes...> operator + (const array<X, sizes...> &, const array<X, sizes...> &);

    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x + y} -> ImplicitlyConvertible<X>;
    } constexpr array<X, sizes...> operator - (const array<X, sizes...> &, const array<X, sizes...> &);

    template <typename X, size_t... sizes> requires requires (const X &x) {
        {-x} -> ImplicitlyConvertible<X>;
    } constexpr array<X, sizes...> operator - (const array<X, sizes...> &);

    // scalar multiplication and division.
    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x * y} -> ImplicitlyConvertible<X>;
    } constexpr array<X, sizes...> operator * (const array<X, sizes...> &, const X &);

    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x / y} -> ImplicitlyConvertible<X>;
    } constexpr array<X, sizes...> operator / (const array<X, sizes...> &, const X &);

    // this array is a structural type and therefore can be
    // used as a non-type template parameter.
    template <typename X, size_t z> struct array<X, z> {
        constexpr static size_t Size = z;

        X Values[Size];

        constexpr array ();
        constexpr array (std::initializer_list<X> x);
        constexpr array (slice<const X, z> x) {
            std::copy (x.begin (), x.end (), this->begin ());
        }

        // make an array filled with a particular value.
        constexpr static array filled (const X &x);

        constexpr bool valid () const;

        constexpr X &operator [] (size_t i);
        constexpr const X &operator [] (size_t i) const;

        constexpr slice<X> range (int);
        constexpr slice<X> range (int, int);

        constexpr slice<const X> range (int) const;
        constexpr slice<const X> range (int, int) const;

        constexpr operator slice<X, Size> () {
            return slice<X, Size> {Values};
        }

        constexpr operator slice<const X, Size> () const {
            return slice<const X, Size> {Values};
        }

        constexpr size_t size () const {
            return Size;
        }

        constexpr X *data () {
            return Values;
        }

        constexpr const X *data () const {
            return Values;
        }

        using iterator = slice<X, Size>::iterator;
        using const_iterator = slice<const X, Size>::iterator;
        using reverse_iterator = slice<X, Size>::reverse_iterator;
        using const_reverse_iterator = slice<const X, Size>::reverse_iterator;;

        iterator begin () {
            return slice<X, Size> (*this).begin ();
        }

        iterator end () {
            return slice<X, Size> (*this).end ();
        }

        const_iterator begin () const {
            return slice<const X, Size> (*this).begin ();
        }

        const_iterator end () const {
            return slice<const X, Size> (*this).end ();
        }

        reverse_iterator rbegin () {
            return slice<X, Size> (*this).rbegin ();
        }

        reverse_iterator rend () {
            return slice<X, Size> (*this).rend ();
        }

        const_reverse_iterator rbegin () const {
            return slice<const X, Size> (*this).rbegin ();
        }

        const_reverse_iterator rend () const {
            return slice<const X, Size> (*this).rend ();
        }
    };

    template <typename X, size_t size, size_t... sizes> struct array<X, size, sizes...> : public array<X, size * array<X, sizes...>::Size> {
        constexpr static size_t Size = size * array<X, sizes...>::Size;

        array ();
        array (std::initializer_list<array<X, sizes...>>);

        slice<X, sizes...> &operator [] (size_t i);
        slice<const X, sizes...> &operator [] (size_t i) const;

    };

    template <typename X> struct array<X> {
        constexpr static size_t Size = 1;

        X Value;

        constexpr array (const X &x): Value {x} {}

        array &operator = (const X &x) {
            Value = x;
            return *this;
        }

        constexpr array (X &&x): Value {std::move (x)} {}

        constexpr array &operator = (X &&x) {
            Value = std::move (x);
            return *this;
        }

        constexpr operator X () const;

        constexpr bool valid () const;

        explicit operator slice<X, 1> ();

        slice<X> range (int);
        slice<X> range (int, int);

        explicit operator slice<const X, 1> () const;

        slice<const X> range (int) const;
        slice<const X> range (int, int) const;

        constexpr slice<X>::iterator begin () {
            return slice<X> {&Value, 1}.begin ();
        }

        constexpr slice<X>::iterator end () {
            return slice<X> {&Value, 1}.end ();
        }

        constexpr slice<const X>::iterator begin () const {
            return slice<const X> {&Value, 1}.begin ();
        }

        constexpr slice<const X>::iterator end () const {
            return slice<const X> {&Value, 1}.end ();
        }

    };

    // multiplication operation good enough for an inner product and matrix multiplication
    template <typename X, size_t... A, size_t C, size_t... B> requires requires (const X &x, const X &y) {
        {x + y} -> ImplicitlyConvertible<X>;
        {x * inner (x, y)} -> ImplicitlyConvertible<X>;
    } && requires () {
        {X {}};
    } constexpr array<X, A..., B...> operator * (const array<X, A..., C> &a, const array<X, C, B...> &b);

    template <typename X, size_t size>
    constexpr inline array<X, size>::array () {
        for (size_t index = 0; index < Size; index++) Values[index] = X {};
    }

    template <typename X, size_t size>
    constexpr inline array<X, size>::array (std::initializer_list<X> x) {
        if (x.size () != Size) throw exception {} << "out of range";
        size_t index = 0;
        for (const X &xx : x) Values[index++] = xx;
    }

    template <typename X, size_t... sizes>
    constexpr bool inline operator == (const array<X, sizes...> &a, const array<X, sizes... > &b) {
        auto i = a.begin ();
        auto j = b.begin ();
        while (i != a.end ()) {
            if (*i != *j) return false;
            i++;
            j++;
        }
        return true;
    }

    template <typename X, size_t... sizes>
    constexpr auto operator <=> (const array<X, sizes...> &a, const array<X, sizes... > &b) -> decltype (*a.begin () <=> *b.begin ()) {
        auto i = a.begin ();
        auto j = b.begin ();

        // For certain types of comparison we can have equivalent
        // values that are not equal so we keep track of a running
        // value for that case.
        using comparison = decltype (*i <=> *j);
        comparison so_far = comparison::equal;

        while (i != a.end ()) {
            auto comp = *i <=> *j;
            if (comp == comparison::greater || comp == comparison::less) return comp;
            if (comp != comparison::equal) so_far = comp;
            i++;
            j++;
        }

        return so_far;
    }

    template <typename X, size_t size>
    constexpr array<X, size> inline array<X, size>::filled (const X &z) {
        array n {};
        for (X &x: n.Values) x = z;
        return n;
    }

    template <typename X, size_t size>
    constexpr bool inline array<X, size>::valid () const {
        for (const X &x : *this) if (!data::valid (x)) return false;
        return true;
    }

    template <typename X, size_t size>
    constexpr X inline &array<X, size>::operator [] (size_t i) {
        if (Size == 0) throw std::out_of_range {"cross size 0"};
        if (i < 0 || i >= Size) return this->operator [] ((i + Size) % Size);
        return Values[i];
    }

    template <typename X, size_t size>
    constexpr const X inline &array<X, size>::operator [] (size_t i) const {
        if (Size == 0) throw std::out_of_range {"cross size 0"};
        if (i < 0 || i >= Size) return this->operator [] ((i + Size) % Size);
        return Values[i];
    }

    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x + y} -> ImplicitlyConvertible<X>;
    } constexpr array<X, sizes...> operator + (const array<X, sizes...> &a, const array<X, sizes...> &b) {
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
        {x - y} -> ImplicitlyConvertible<X>;
    } constexpr array<X, sizes...> operator - (const array<X, sizes...> &a, const array<X, sizes...> &b) {
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
        {x * y} -> ImplicitlyConvertible<X>;
    } constexpr array<X, sizes...> operator * (const array<X, sizes...> &a, const X &b) {
        array<X, sizes...> x {};

        auto ai = a.begin ();
        for (auto xi = x.begin (); xi != x.end (); xi++) {
            *xi = *ai * b;
            ai++;
        }

        return x;
    }

    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x / y} -> ImplicitlyConvertible<X>;
    } constexpr array<X, sizes...> operator / (const array<X, sizes...> &a, const X &b) {
        array<X, sizes...> x {};

        auto ai = a.begin ();
        for (auto xi = x.begin (); xi != x.end (); xi++) {
            *xi = *ai / b;
            ai++;
        }

        return x;
    }

    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x + y} -> ImplicitlyConvertible<X>;
        {x * y} -> ImplicitlyConvertible<X>;
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

    template <std::integral word, size_t... sizes>
    writer<word> inline &operator << (writer<word> &w, const bytes_array<word, sizes...> &x) {
        w.write (x.data (), (1 * sizes)...);
        return w;
    }

    template <std::integral word, size_t... sizes>
    reader<word> &operator >> (reader<word> &r, bytes_array<word, sizes...> &x) {
        r.read (x.data (), (1 * sizes)...);
        return r;
    }

    template <std::integral word, size_t size>
    std::ostream inline &operator << (std::ostream &o, const bytes_array<word, size> &s) {
        return o << "\"" << encoding::hex::write (slice<const word> (s)) << "\"";
    }

    template <std::integral word, size_t size>
    constexpr bytes_array<word, size> operator << (const bytes_array<word, size> &b, int32 i) {
        bytes_array<word, size> n (b);
        if (i < 0) arithmetic::Words<endian::big, word> (slice<word> (n)).bit_shift_right (-i, false);
        else arithmetic::Words<endian::big, word> (slice<word> (n)).bit_shift_left (i, false);
        return n;
    }

    template <std::integral word, size_t size>
    constexpr bytes_array<word, size> operator >> (const bytes_array<word, size> &b, int32 i) {
        bytes_array<word, size> n (b);
        if (i < 0) arithmetic::Words<endian::big, word> (slice<word> (n)).bit_shift_left (-i, false);
        else arithmetic::Words<endian::big, word> (slice<word> (n)).bit_shift_right (i, false);
        return n;
    }

    template <std::integral word, size_t size>
    constexpr bytes_array<word, size> operator ~ (const bytes_array<word, size> &b) {
        bytes_array<word, size> n;
        arithmetic::bit_negate<word> (n.end (), n.begin (), b.begin ());
        return n;
    }

    template <std::integral word, size_t size>
    constexpr bytes_array<word, size> operator ^ (const bytes_array<word, size> &a, const bytes_array<word, size> &b) {
        bytes_array<word, size> n;
        arithmetic::bit_xor<word> (n.end (), n.begin (), a.begin (), b.begin ());
        return n;
    }

    template <std::integral word, size_t size>
    constexpr bytes_array<word, size> operator & (const bytes_array<word, size> &a, const bytes_array<word, size> &b) {
        bytes_array<word, size> n;
        arithmetic::bit_and<word> (n.end (), n.begin (), a.begin (), b.begin ());
        return n;
    }

    template <std::integral word, size_t size>
    constexpr bytes_array<word, size> operator | (const bytes_array<word, size> &a, const bytes_array<word, size> &b) {
        bytes_array<word, size> n;
        arithmetic::bit_or<word> (n.end (), n.begin (), a.begin (), b.begin ());
        return n;
    }

    template <size_t size> byte_array<size> read_byte_array (const hex_string &x) {
        if (!x.valid () || ((x.size () % 2) != 0)) throw encoding::invalid {encoding::hex::Format, x};
        if ((x.size () / 2) != size) throw exception {} << "invalid hex string size";
        byte_array<size> T;
        boost::algorithm::unhex (x.begin (), x.end (), T.data ());
        return T;
    }

}

#endif
