// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <data/bytes.hpp>
#include <data/float.hpp>
#include <data/math/number/numbers.hpp>
#include <data/arithmetic/complementary.hpp>

#include <data/exception.hpp>

namespace data {

    namespace math::number {

        template <endian r, size_t size, Word word>
        struct bounded<false, r, size, word> : public oriented<r, word, size> {

            constexpr bounded () : oriented<r, word, size> () {}

            constexpr static const size_t size_in_bytes = size * sizeof (word);

            // Implicitly convert from a smaller or equal size number of any sign.
            // This means that we may convert negative numbers to positive.
            // Bizarre, but this is how c++ works.

            template <std::integral I> requires (sizeof (I) <= size_in_bytes)
            constexpr bounded (I x): bounded {convert<bounded> (x)} {}

            template <bool x, endian o, size_t u, Word w>
            requires (u * sizeof (w) <= size_in_bytes)
            constexpr bounded (const bounded<x, o, u, w> &n): bounded {convert<bounded> (n)} {}

            template <data::endian Order, class T, std::size_t n_bits, alignment Align>
            requires (n_bits / CHAR_BIT <= size_in_bytes)
            constexpr bounded (const endian_arithmetic<Order, T, n_bits, Align> &x): bounded {convert<bounded> (x)} {}

            template <std::integral I> requires (size_in_bytes <= sizeof (I))
            constexpr operator I () const {
                return convert<I> (*this);
            }

            template <data::endian Order, class T, std::size_t n_bits, alignment Align>
            requires (size_in_bytes <= n_bits / CHAR_BIT)
            constexpr operator endian_arithmetic<Order, T, n_bits, Align> () const;

            // explicitly convert from a larger number.

            template <bool x, endian o, size_t u, Word w>
            requires (u * sizeof (w) > size_in_bytes)
            constexpr explicit bounded (const bounded<x, o, u, w> &n): bounded {convert<bounded> (n)} {}

            template <data::endian Order, class T, std::size_t n_bits, alignment Align>
            requires (n_bits / CHAR_BIT > size_in_bytes)
            constexpr explicit bounded (const endian_arithmetic<Order, T, n_bits, Align> &x): bounded {convert<bounded> (x)} {}

            template <std::integral I> requires (sizeof (I) > size_in_bytes)
            constexpr explicit bounded (I x): bounded {convert<bounded> (x)} {}

            // throw if out of bounds.
            template <endian o, negativity neg, Word w>
            explicit bounded (const Z_bytes<o, neg, w> &z): bounded {convert<bounded> (z)} {}

            template <std::integral I> requires (size_in_bytes > sizeof (I))
            constexpr explicit operator I () const {
                return convert<I> (*this);
            }

            template <data::endian Order, class T, std::size_t n_bits, alignment Align>
            requires (size_in_bytes > n_bits / CHAR_BIT)
            constexpr explicit operator endian_arithmetic<Order, T, n_bits, Align> () const;

            // convert to double
            constexpr explicit operator double () const;

            // import from sequence of bytes
            constexpr explicit bounded (slice<const word, size> x);

            // The string can be a hex string or a representation of a number.
            // explicit bounded (const data::string &s): bounded {read (s)} {}
            constexpr explicit bounded (const char *x): bounded {string_view {x, std::strlen (x)}} {}
            constexpr explicit bounded (const string_view);
            constexpr static bounded read (string_view x) {
                return bounded {x};
            }

            // TODO we shouldn't need this.
            division<bounded> divmod (const bounded &) const;

            constexpr static bounded max ();
            constexpr static bounded min ();

            // TODO do we need this?
            static N_bytes<r, word> modulus ();

        };

        template <endian r, size_t size, Word word>
        struct bounded<true, r, size, word> : public oriented<r, word, size> {

            constexpr bounded () : oriented<r, word, size> {} {}

            constexpr static const size_t size_in_bytes = size * sizeof (word);

            // implicitly convert from a smaller or equal size signed number.

            template <bool is_signed, endian o, size_t u, Word w>
            requires (u * sizeof (w) < size_in_bytes || u * sizeof (w) == size_in_bytes && is_signed)
            constexpr bounded (const bounded<is_signed, o, u, w> &n): bounded {convert<bounded> (n)} {}

            template <endian o, class T, std::size_t n_bits>
            requires (n_bits / CHAR_BIT < size_in_bytes || n_bits / CHAR_BIT == size_in_bytes && std::signed_integral<T>)
            constexpr bounded (const endian_arithmetic<o, T, n_bits, alignment::no> &n): bounded {convert<bounded> (n)} {}

            template <std::integral I>
            requires (sizeof (I) < size_in_bytes || sizeof (I) == size_in_bytes && std::signed_integral<I>)
            constexpr bounded (I x): bounded {convert<bounded> (x)} {}

            // Explicitly convert from a greater size or equal size unsigned number.

            template <bool is_signed, endian o, size_t u, Word w>
            requires (u * sizeof (w) > size_in_bytes || u * sizeof (w) == size_in_bytes && !is_signed)
            constexpr explicit bounded (const bounded<is_signed, o, u, w> &n): bounded {convert<bounded> (n)} {}

            template <endian o, class T, std::size_t n_bits>
            requires (n_bits / CHAR_BIT > size_in_bytes || n_bits / CHAR_BIT == size_in_bytes && std::unsigned_integral<T>)
            constexpr explicit bounded (const endian_arithmetic<o, T, n_bits, alignment::no> &n): bounded {convert<bounded> (n)} {}

            template <std::integral I>
            requires (sizeof (I) > size_in_bytes || sizeof (I) == size_in_bytes && std::unsigned_integral<I>)
            constexpr explicit bounded (I x): bounded {convert<bounded> (x)} {}

            template <endian o, negativity neg, Word w>
            explicit bounded (const Z_bytes<o, neg, w> &z): bounded {convert<bounded> (z)} {}

            constexpr explicit bounded (slice<const word, size> &x) : oriented<r, word, size> {x} {}

            constexpr explicit bounded (const char *x): bounded {string_view {x, std::strlen (x)}} {}
            explicit bounded (const std::string &s): bounded {read (s)} {}
            constexpr explicit bounded (string_view s);
            constexpr static bounded read (string_view x) {
                return bounded {x};
            }

            division<bounded> divmod (const bounded &) const;

            explicit bounded (slice<word, size>);

            constexpr explicit operator double () const;

            template <std::integral I> constexpr explicit operator I () const;

            constexpr static bounded max ();
            constexpr static bounded min ();

        };
    }

    namespace math {

        template <bool is_signed, endian r, size_t size, Word word>
        struct numeric_limits<number::bounded<is_signed, r, size, word>> {
            constexpr static const number::bounded<is_signed, r, size, word> Max = number::bounded<is_signed, r, size, word>::max ();
            constexpr static const number::bounded<is_signed, r, size, word> Min = number::bounded<is_signed, r, size, word>::min ();

            constexpr static const number::bounded<is_signed, r, size, word> &max () {
                return Max;
            }

            constexpr static const number::bounded<is_signed, r, size, word> &min () {
                return Min;
            }
        };

    }

    namespace math::number {

        template <bool u, endian r, size_t x, Word word>
        constexpr bounded<u, r, x, word> inline operator ~ (const bounded<u, r, x, word> &n) {
            bounded<u, r, x, word> z;
            arithmetic::bit_negate<word> (z.begin (), z.end (), n.begin ());
            return z;
        }

        template <std::integral I, bool u, endian r, size_t size, Word word>
        constexpr std::strong_ordering inline operator <=> (I x, const bounded<u, r, size, word> &n) {
            return bounded<u, r, size, word> {x} <=> n;
        }

        template <bool u, endian r, size_t size, Word word, std::integral I>
        constexpr std::strong_ordering inline operator <=> (const bounded<u, r, size, word> &n, I x) {
            return n <=> bounded<u, r, size, word> {x};
        }

        template <endian r, size_t size, endian o, Word word>
        std::weak_ordering inline operator <=> (const bound<r, size, word> &a, const Z_bytes<o, negativity::twos, word> &b) {
            return Z_bytes<r, negativity::twos, word> (a) <=> b;
        }

        template <endian r, size_t size, endian o, Word word>
        std::weak_ordering inline operator <=> (const ubound<r, size, word> &a, const N_bytes<o, word> &b) {
            return Z_bytes<r, negativity::twos, word> (a) <=> b;
        }

        template <bool x, endian r, size_t n, bool y, endian o, size_t z, Word word>
        constexpr std::weak_ordering inline operator <=> (const bounded<x, r, n, word> &a, const endian_integral<y, o, z> &b) {
            return a <=> bounded<y, o, z, word> (b);
        }

        // Something strange is going on here. It seems that we have an infinite loop.
        template <bool u, data::endian r, size_t x, Word word>
        std::ostream &operator << (std::ostream &o, const bounded<u, r, x, word> &n) {
            if (o.flags () & std::ios::hex)
                return encoding::hexidecimal::write (o, n);
            if constexpr (u) {
                if (o.flags () & std::ios::dec)
                    return encoding::signed_decimal::write (o, n);
            } else {
                if (o.flags () & std::ios::dec)
                    return encoding::decimal::write (o, n);
            }

            throw exception {} << "write a bounded number with invalid flags";
        }

        template <bool u, data::endian r, size_t x, Word word>
        writer<word> inline &operator << (writer<word> &o, const bounded<u, r, x, word> &n) {
            o.write (n.data (), x);
            return o;
        }

        template <bool u, data::endian r, size_t x, Word word>
        reader<word> inline &operator >> (reader<word> &o, bounded<u, r, x, word> &n) {
            o.read (n.data (), x);
            return o;
        }

        template <bool u, endian r, size_t size, Word word>
        constexpr bounded<u, r, size, word> inline operator ++ (bounded<u, r, size, word> &x, int) {
            bounded n = x;
            ++x;
            return n;
        }

        template <bool u, endian r, size_t size, Word word>
        constexpr bounded<u, r, size, word> inline operator -- (bounded<u, r, size, word> &x, int) {
            bounded n = x;
            ++x;
            return n;
        }

        template <endian r, size_t z, Word word, std::unsigned_integral I>
        constexpr ubound<r, z, word> inline &operator /= (ubound<r, z, word> &a, I b) {
            return a = a / b;
        }

        template <endian r, size_t z, Word word, std::integral I>
        constexpr bound<r, z, word> inline &operator /= (bound<r, z, word> &a, I b) {
            return a = a / b;
        }

        template <bool u, endian r, size_t x, Word word>
        constexpr bounded<u, r, x, word> inline &operator /= (bounded<u, r, x, word> &a, const bounded<u, r, x, word> &b) {
            return a = a / b;
        }

        template <bool u, endian r, size_t z, Word word>
        constexpr bounded<u, r, z, word> inline &operator %= (bounded<u, r, z, word> &a, const bounded<u, r, z, word> &b) {
            return a = a % b;
        }

        template <endian r, size_t size, Word word, std::integral I>
        constexpr ubound<r, size, word> inline operator & (const ubound<r, size, word> &z, I x) {
            return z & ubound<r, size, word> {static_cast<std::make_unsigned_t<I>> (x)};
        }

        template <std::integral I, endian r, size_t size, Word word>
        constexpr ubound<r, size, word> inline operator & (I x, const ubound<r, size, word> &z) {
            return z & ubound<r, size, word> {static_cast<std::make_unsigned_t<I>> (x)};
        }

        template <endian r, size_t size, Word word, std::signed_integral I>
        constexpr bound<r, size, word> inline operator & (const bound<r, size, word> &z, I x) {
            return z & bound<r, size, word> {x};
        }

        template <std::signed_integral I, endian r, size_t size, Word word>
        constexpr bound<r, size, word> inline operator & (I x, const bound<r, size, word> &z) {
            return z & bound<r, size, word> {x};
        }

        template <endian r, size_t size, Word word, std::unsigned_integral I>
        constexpr ubound<r, size, word> inline operator & (const bound<r, size, word> &z, I x) {
            return ubound<r, size, word> (z) & x;
        }

        template <std::unsigned_integral I, endian r, size_t size, Word word>
        constexpr ubound<r, size, word> inline operator & (I x, const bound<r, size, word> &z) {
            return x & ubound<r, size, word> (z);
        }

        template <endian r, size_t size, Word word, std::integral I>
        constexpr ubound<r, size, word> inline &operator &= (ubound<r, size, word> &z, I x) {
            return z &= ubound<r, size, word> {static_cast<std::make_unsigned_t<I>> (x)};
        }

        template <endian r, size_t size, Word word, std::signed_integral I>
        constexpr bound<r, size, word> inline &operator &= (bound<r, size, word> &z, I x) {
            return z &= bound<r, size, word> {x};
        }

        template <endian r, size_t size, Word word, std::integral I>
        constexpr ubound<r, size, word> inline operator ^ (const ubound<r, size, word> &z, I x) {
            return z ^ ubound<r, size, word> {static_cast<std::make_unsigned_t<I>> (x)};
        }

        template <std::integral I, endian r, size_t size, Word word>
        constexpr ubound<r, size, word> inline operator ^ (I x, const ubound<r, size, word> &z) {
            return z ^ ubound<r, size, word> {static_cast<std::make_unsigned_t<I>> (x)};
        }

        template <endian r, size_t size, Word word, std::signed_integral I>
        constexpr bound<r, size, word> inline operator ^ (const bound<r, size, word> &z, I x) {
            return z ^ bound<r, size, word> {x};
        }

        template <std::signed_integral I, endian r, size_t size, Word word>
        constexpr bound<r, size, word> inline operator ^ (I x, const bound<r, size, word> &z) {
            return z ^ bound<r, size, word> {x};
        }

        template <endian r, size_t size, Word word, std::unsigned_integral I>
        constexpr ubound<r, size, word> inline operator ^ (const bound<r, size, word> &z, I x) {
            return ubound<r, size, word> (z) ^ x;
        }

        template <std::unsigned_integral I, endian r, size_t size, Word word>
        constexpr ubound<r, size, word> inline operator ^ (I x, const bound<r, size, word> &z) {
            return x ^ ubound<r, size, word> (z);
        }

        template <endian r, size_t size, Word word, std::integral I>
        constexpr ubound<r, size, word> inline &operator ^= (ubound<r, size, word> &z, I x) {
            return z ^= ubound<r, size, word> {static_cast<std::make_unsigned_t<I>> (x)};
        }

        template <endian r, size_t size, Word word, std::signed_integral I>
        constexpr bound<r, size, word> inline &operator ^= (bound<r, size, word> &z, I x) {
            return z ^= bound<r, size, word> {x};
        }

        template <endian r, size_t size, Word word, std::integral I>
        constexpr ubound<r, size, word> inline operator | (const ubound<r, size, word> &z, I x) {
            return z | ubound<r, size, word> {static_cast<std::make_unsigned_t<I>> (x)};
        }

        template <std::integral I, endian r, size_t size, Word word>
        constexpr ubound<r, size, word> inline operator | (I x, const ubound<r, size, word> &z) {
            return z | ubound<r, size, word> {static_cast<std::make_unsigned_t<I>> (x)};
        }

        template <endian r, size_t size, Word word, std::signed_integral I>
        constexpr bound<r, size, word> inline operator | (const bound<r, size, word> &z, I x) {
            return z | bound<r, size, word> {x};
        }

        template <std::signed_integral I, endian r, size_t size, Word word>
        constexpr bound<r, size, word> inline operator | (I x, const bound<r, size, word> &z) {
            return z | bound<r, size, word> {x};
        }

        template <endian r, size_t size, Word word, std::unsigned_integral I>
        constexpr ubound<r, size, word> inline operator | (const bound<r, size, word> &z, I x) {
            return ubound<r, size, word> (z) | x;
        }

        template <std::unsigned_integral I, endian r, size_t size, Word word>
        constexpr ubound<r, size, word> inline operator | (I x, const bound<r, size, word> &z) {
            return x | ubound<r, size, word> (z);
        }

        template <endian r, size_t size, Word word, std::integral I>
        constexpr ubound<r, size, word> inline &operator |= (ubound<r, size, word> &z, I x) {
            return z |= ubound<r, size, word> {static_cast<std::make_unsigned_t<I>> (x)};
        }

        template <endian r, size_t size, Word word, std::signed_integral I>
        constexpr bound<r, size, word> inline &operator |= (bound<r, size, word> &z, I x) {
            return z |= bound<r, size, word> {x};
        }

        template <endian r, size_t size, Word word, std::integral I>
        constexpr ubound<r, size, word> inline operator + (const ubound<r, size, word> &z, I x) {
            return z + ubound<r, size, word> {static_cast<std::make_unsigned_t<I>> (x)};
        }

        template <std::integral I, endian r, size_t size, Word word>
        constexpr ubound<r, size, word> inline operator + (I x, const ubound<r, size, word> &z) {
            return z + ubound<r, size, word> {static_cast<std::make_unsigned_t<I>> (x)};
        }

        template <endian r, size_t size, Word word, std::signed_integral I>
        constexpr bound<r, size, word> inline operator + (const bound<r, size, word> &z, I x) {
            return z + bound<r, size, word> {x};
        }

        template <std::signed_integral I, endian r, size_t size, Word word>
        constexpr bound<r, size, word> inline operator + (I x, const bound<r, size, word> &z) {
            return z + bound<r, size, word> {x};
        }

        template <endian r, size_t size, Word word, std::unsigned_integral I>
        constexpr ubound<r, size, word> inline operator + (const bound<r, size, word> &z, I x) {
            return ubound<r, size, word> (z) + x;
        }

        template <std::unsigned_integral I, endian r, size_t size, Word word>
        constexpr ubound<r, size, word> inline operator + (I x, const bound<r, size, word> &z) {
            return x + ubound<r, size, word> (z);
        }

        template <endian r, size_t size, Word word, std::integral I>
        constexpr ubound<r, size, word> inline &operator += (ubound<r, size, word> &z, I x) {
            return z += ubound<r, size, word> {static_cast<std::make_unsigned_t<I>> (x)};
        }

        template <endian r, size_t size, Word word, std::signed_integral I>
        constexpr bound<r, size, word> inline &operator += (bound<r, size, word> &z, I x) {
            return z += bound<r, size, word> {x};
        }

        template <endian r, size_t size, Word word, std::integral I>
        constexpr ubound<r, size, word> inline operator - (const ubound<r, size, word> &z, I x) {
            return z - ubound<r, size, word> {static_cast<std::make_unsigned_t<I>> (x)};
        }

        template <std::integral I, endian r, size_t size, Word word>
        constexpr ubound<r, size, word> inline operator - (I x, const ubound<r, size, word> &z) {
            return ubound<r, size, word> {static_cast<std::make_unsigned_t<I>> (x)} - z;
        }

        template <endian r, size_t size, Word word, std::signed_integral I>
        constexpr bound<r, size, word> inline operator - (const bound<r, size, word> &z, I x) {
            return z - bound<r, size, word> {x};
        }

        template <std::signed_integral I, endian r, size_t size, Word word>
        constexpr bound<r, size, word> inline operator - (I x, const bound<r, size, word> &z) {
            return bound<r, size, word> {x} - z;
        }

        template <endian r, size_t size, Word word, std::unsigned_integral I>
        constexpr ubound<r, size, word> inline operator - (const bound<r, size, word> &z, I x) {
            return ubound<r, size, word> (z) - x;
        }

        template <std::unsigned_integral I, endian r, size_t size, Word word>
        constexpr ubound<r, size, word> inline operator - (I x, const bound<r, size, word> &z) {
            return x - ubound<r, size, word> (z);
        }

        template <endian r, size_t size, Word word, std::integral I>
        constexpr ubound<r, size, word> inline &operator -= (ubound<r, size, word> &z, I x) {
            return z -= ubound<r, size, word> {static_cast<std::make_unsigned_t<I>> (x)};
        }

        template <endian r, size_t size, Word word, std::signed_integral I>
        constexpr bound<r, size, word> inline &operator -= (bound<r, size, word> &z, I x) {
            return z -= bound<r, size, word> {x};
        }

        template <endian r, size_t size, Word word, std::integral I>
        constexpr ubound<r, size, word> inline operator * (const ubound<r, size, word> &z, I x) {
            return z * ubound<r, size, word> {static_cast<std::make_unsigned_t<I>> (x)};
        }

        template <std::integral I, endian r, size_t size, Word word>
        constexpr ubound<r, size, word> inline operator * (I x, const ubound<r, size, word> &z) {
            return z * ubound<r, size, word> {static_cast<std::make_unsigned_t<I>> (x)};
        }

        template <endian r, size_t size, Word word, std::signed_integral I>
        constexpr bound<r, size, word> inline operator * (const bound<r, size, word> &z, I x) {
            return z * bound<r, size, word> {x};
        }

        template <std::signed_integral I, endian r, size_t size, Word word>
        constexpr bound<r, size, word> inline operator * (I x, const bound<r, size, word> &z) {
            return z * bound<r, size, word> {x};
        }

        template <endian r, size_t size, Word word, std::unsigned_integral I>
        constexpr ubound<r, size, word> inline operator * (const bound<r, size, word> &z, I x) {
            return ubound<r, size, word> (z) * x;
        }

        template <std::unsigned_integral I, endian r, size_t size, Word word>
        constexpr ubound<r, size, word> inline operator * (I x, const bound<r, size, word> &z) {
            return x * ubound<r, size, word> (z);
        }

        template <endian r, size_t size, Word word, std::integral I>
        constexpr ubound<r, size, word> inline &operator *= (ubound<r, size, word> &z, I x) {
            return z *= ubound<r, size, word> {static_cast<std::make_unsigned_t<I>> (x)};
        }

        template <endian r, size_t size, Word word, std::signed_integral I>
        constexpr bound<r, size, word> inline &operator *= (bound<r, size, word> &z, I x) {
            return z *= bound<r, size, word> {x};
        }

        template <endian r, size_t x, Word word, std::integral I>
        constexpr ubound<r, x, word> inline operator / (const ubound<r, x, word> &n, I u) {
            return n / ubound<r, x, word> (static_cast<std::make_unsigned_t<I>> (u));
        }

        template <endian r, size_t x, Word word, std::signed_integral I>
        constexpr bound<r, x, word> inline operator / (const bound<r, x, word> &n, I u) {
            return n / bound<r, x, word> (u);
        }

        template <endian r, size_t x, Word word, std::unsigned_integral I>
        constexpr ubound<r, x, word> inline operator / (const bound<r, x, word> &n, I u) {
            return n / ubound<r, x, word> (u);
        }

        template <endian r, size_t size, Word word>
        constexpr inline bounded<false, r, size, word>::operator double () const {
            return import_float<double> (slice<const word> (*this), r, endian::native, negativity::nones);
        }

        template <endian r, size_t size, Word word>
        constexpr inline bounded<true, r, size, word>::operator double () const {
            return import_float<double> (slice<const word> (*this), r, endian::native, negativity::twos);
        }

    }

    namespace encoding::decimal {

        template <bool is_signed, endian::order r, size_t size, Word word>
        std::string inline write (const math::number::bounded<is_signed, r, size, word> &n) {
            std::stringstream ss;
            ss << std::dec << n;
            return ss.str ();
        }

    }

    namespace math::number {

        template <endian r, size_t x, Word word>
        constexpr nonzero<ubound<r, x, word>> inline increment<ubound<r, x, word>>::operator () (const ubound<r, x, word> &n) {
            if (n == ubound<r, x, word>::max ()) throw exception {} << "cannot increment";
            return nonzero<ubound<r, x, word>> {n + 1};
        }

        template <endian r, size_t x, Word word>
        constexpr ubound<r, x, word> inline decrement<ubound<r, x, word>>::operator () (const nonzero<ubound<r, x, word>> &n) {
            auto z = n.Value;
            return --z;
        }

        template <endian r, size_t x, Word word>
        constexpr ubound<r, x, word> inline decrement<ubound<r, x, word>>::operator () (const ubound<r, x, word> &n) {
            if (data::is_zero (n)) return n;
            auto z = n;
            return --z;
        }

        template <endian r, size_t x, Word word>
        constexpr bound<r, x, word> inline increment<bound<r, x, word>>::operator () (const bound<r, x, word> &n) {
            if (n == bound<r, x, word>::max ()) throw exception {} << "cannot increment";
            auto z = n;
            return ++z;
        }

        template <endian r, size_t x, Word word>
        constexpr bound<r, x, word> inline decrement<bound<r, x, word>>::operator () (const bound<r, x, word> &n) {
            if (n == bound<r, x, word>::min ()) throw exception {} << "cannot decrement";
            auto z = n;
            return --z;
        }

        template <endian r, size_t size, Word word>
        constexpr std::strong_ordering operator <=> (const bound<r, size, word> &a, const bound<r, size, word> &b) {
            bool na = data::is_negative (a);
            bool nb = data::is_negative (b);
            if (na == nb) return arithmetic::compare<negativity::nones> (a.words (), b.words ());
            return na ? std::strong_ordering::less : std::strong_ordering::greater;
        }

        template <endian r, size_t size, Word word>
        constexpr std::strong_ordering inline operator <=> (const ubound<r, size, word> &a, const ubound<r, size, word> &b) {
            return arithmetic::compare<negativity::nones> (a.words (), b.words ());
        }

        template <bool x, endian r, size_t n, bool y, endian o, size_t z, Word word>
        constexpr std::strong_ordering inline operator <=> (const bounded<x, r, n, word> &a, const bounded<y, o, z, word> &b) {
            if constexpr (x && !y) {
                if (a < 0) return std::strong_ordering::less;
                return arithmetic::compare<negativity::nones> (a.words (), b.words ());
            } else if constexpr (y && !x) {
                if (b < 0) return std::strong_ordering::greater;
                return arithmetic::compare<negativity::nones> (a.words (), b.words ());
            } else throw exception {} << "undefined method bounded <=> bounded";
        }

        template <bool is_signed, endian r, size_t size, Word word>
        constexpr bounded<is_signed, r, size, word> inline operator << (const bounded<is_signed, r, size, word> &z, int i) {
            auto n = z;
            return n <<= i;
        }

        template <bool is_signed, endian r, size_t size, Word word>
        constexpr bounded<is_signed, r, size, word> inline operator >> (const bounded<is_signed, r, size, word> &z, int i) {
            auto n = z;
            return n >>= i;
        }

    }

    namespace math::def {

        template <endian r, size_t x, Word word>
        constexpr math::sign inline sign<ubound<r, x, word>>::operator () (const ubound<r, x, word> &z) {
            return arithmetic::sign<negativity::nones> (z.words ());
        }

        template <endian r, size_t x, Word word>
        constexpr math::sign inline sign<bound<r, x, word>>::operator () (const bound<r, x, word> &z) {
            return arithmetic::sign<negativity::twos> (z.words ());
        }

        template <endian r, size_t x, Word word>
        constexpr bool inline is_positive<ubound<r, x, word>>::operator () (const ubound<r, x, word> &n) {
            return !data::is_zero (n);
        }

        template <endian r, size_t x, Word word>
        constexpr bool inline is_positive<bound<r, x, word>>::operator () (const bound<r, x, word> &n) {
            return !data::is_negative (n) && !data::is_zero (n);
        }

        template <endian r, size_t x, Word word>
        constexpr bool inline is_negative<ubound<r, x, word>>::operator () (const ubound<r, x, word> &n) {
            return false;
        }

        template <endian r, size_t x, Word word>
        constexpr bool inline is_negative<bound<r, x, word>>::operator () (const bound<r, x, word> &z) {
            return arithmetic::sign_bit (z.words ());
        }

        template <bool u, endian r, size_t x, Word word>
        constexpr bool inline is_zero<nbound<u, r, x, word>>::operator () (const nbound<u, r, x, word> &z) {
            return arithmetic::is_zero<u ? negativity::twos : negativity::nones> (z.words ());
        }

        template <endian r, size_t x, Word word>
        constexpr ubound<r, x, word> inline abs<ubound<r, x, word>>::operator () (const ubound<r, x, word> &z) {
            return z;
        }

        template <endian r, size_t x, Word word>
        constexpr bound<r, x, word> inline abs<bound<r, x, word>>::operator () (const bound<r, x, word> &z) {
            return z < 0 ? -z : z;
        }

        template <bool u, endian r, size_t x, Word word>
        constexpr nbound<u, r, x, word> inline inverse<plus<nbound<u, r, x, word>>, nbound<u, r, x, word>>::operator () (
            const nbound<u, r, x, word> &a,
            const nbound<u, r, x, word> &b) {
            return b - a;
        }

        template <endian r, size_t x, Word word>
        constexpr ubound<r, x, word> inline div_2_pow<ubound<r, x, word>>::operator () (const ubound<r, x, word> &u, uint32 exp) {
            return bit_div_2_pow (u, exp);
        }

        template <endian r, size_t x, Word word>
        constexpr ubound<r, x, word> inline mod_2<ubound<r, x, word>>::operator () (const ubound<r, x, word> &u) {
            return bit_mod_2_positive_mod (u);
        }

        template <endian r, size_t x, Word word>
        constexpr bound<r, x, word> inline div_2_pow<bound<r, x, word>>::operator () (const bound<r, x, word> &u, uint32 exp) {
            return bit_div_2_pow_signed (u, exp);
        }

        template <endian r, size_t x, Word word>
        constexpr bound<r, x, word> inline mod_2<bound<r, x, word>>::operator () (const bound<r, x, word> &u) {
            return bit_mod_2_negative_mod (u);
        }

        template <bool is_signed, endian r, size_t size, Word word>
        constexpr size_t inline size_in_base<number::bounded<is_signed, r, size, word>, 2>::operator () (const number::bounded<is_signed, r, size, word> &x) {
            return arithmetic::bit_width<is_signed ? negativity::twos : negativity::nones> (x.words ());
        }

        template <bool is_signed, endian r, size_t size, Word word>
        constexpr size_t inline digit<number::bounded<is_signed, r, size, word>>::operator () (const number::bounded<is_signed, r, size, word> &u, int32 i) {
            return arithmetic::get_digit<is_signed ? negativity::twos: negativity::nones> (u.words (), i);
        }
    }

    namespace math::number {

        template <endian o, size_t size, Word word>
        constexpr ubound<o, size, word> bounded<false, o, size, word>::min () {
            bounded b {};
            for (int i = 0; i <= size; i++) b[i] = 0x00;
            return b;
        }

        template <bool u, endian r, size_t x, Word word>
        constexpr bounded<u, r, x, word> inline operator ^ (const bounded<u, r, x, word> &a, const bounded<u, r, x, word> &b) {
            bounded<u, r, x, word> z {};
            arithmetic::bit_xor<word> (z.begin (), z.end (), a.begin (), b.begin ());
            return z;
        }

        template <bool u, endian r, size_t size, Word word>
        constexpr bounded<u, r, size, word> inline operator & (const bounded<u, r, size, word> &a, const bounded<u, r, size, word> &b) {
            bounded<u, r, size, word> x;
            arithmetic::bit_and<word>
                (x.words ().begin (), x.words ().end (), a.words ().begin (), b.words ().begin ());
            return x;
        }

        template <bool u, endian r, size_t size, Word word>
        constexpr bounded<u, r, size, word> inline operator | (const bounded<u, r, size, word> &a, const bounded<u, r, size, word> &b) {
            bounded<u, r, size, word> x;
            arithmetic::bit_or<word>
                (x.words ().begin (), x.words ().end (), a.words ().begin (), b.words ().begin ());
            return x;
        }

        template <bool u, endian r, size_t size, Word word>
        constexpr bounded<u, r, size, word> inline &operator &= (bounded<u, r, size, word> &a, const bounded<u, r, size, word> &b) {
            arithmetic::bit_and<word> (a.words ().begin (), a.words ().end (), a.words ().begin (), b.words ().begin ());
            return a;
        }

        template <bool u, endian r, size_t size, Word word>
        constexpr bounded<u, r, size, word> inline &operator |= (bounded<u, r, size, word> &a, const bounded<u, r, size, word> &b) {
            arithmetic::bit_or<word> (a.words ().begin (), a.words ().end (), a.words ().begin (), b.words ().begin ());
            return a;
        }

        template <bool u, endian r, size_t size, Word word>
        constexpr bounded<u, r, size, word> inline &operator ^= (bounded<u, r, size, word> &a, const bounded<u, r, size, word> &b) {
            arithmetic::bit_xor<word> (a.words ().begin (), a.words ().end (), a.words ().begin (), b.words ().begin ());
            return a;
        }

        template <endian r, size_t size, Word word>
        constexpr bounded<false, r, size, word>::bounded (slice<const word, size> x) {
            std::copy (x.begin (), x.end (), this->begin ());
        }

        template <bool u, endian r, size_t x, Word word>
        constexpr bounded<u, r, x, word> inline operator - (const bounded<u, r, x, word> &n) {
            bounded<u, r, x, word> z;
            arithmetic::bit_negate<word> (z.words ().begin (), z.words ().end (), n.words ().begin ());
            return ++z;
        }

        template <bool u, endian r, size_t x, Word word>
        constexpr bounded<u, r, x, word> operator + (const bounded<u, r, x, word> &a, const bounded<u, r, x, word> &b) {
            bounded<u, r, x, word> z {};
            auto i = z.words ().begin ();
            auto j = a.words ().begin ();
            auto k = b.words ().begin ();
            arithmetic::add_with_carry<word> (z.words ().end (), i, j, k);
            return z;
        }

        template <bool u, endian r, size_t x, Word word>
        constexpr bounded<u, r, x, word> operator - (const bounded<u, r, x, word> &a, const bounded<u, r, x, word> &b) {
            bounded<u, r, x, word> z {};
            auto i = z.words ().begin ();
            auto j = a.words ().begin ();
            auto k = b.words ().begin ();
            arithmetic::subtract_with_carry<word> (z.words ().end (), i, j, k);
            return z;
        }

        template <bool u, endian r, size_t x, Word word>
        constexpr bounded<u, r, x, word> &operator += (bounded<u, r, x, word> &a, const bounded<u, r, x, word> &b) {
            auto awb = a.words ().begin ();
            auto cawb = const_cast<const bounded<u, r, x, word> &> (a).words ().begin ();
            auto bwb = b.words ().begin ();
            arithmetic::add_with_carry<word> (a.words ().end (), awb, cawb, bwb);
            return a;
        }

        template <bool u, endian r, size_t x, Word word>
        constexpr bounded<u, r, x, word> &operator -= (bounded<u, r, x, word> &a, const bounded<u, r, x, word> &b) {
            auto awb = a.words ().begin ();
            auto cawb = const_cast<const bounded<u, r, x, word> &> (a).words ().begin ();
            auto bwb = b.words ().begin ();
            arithmetic::subtract_with_carry<word> (a.words ().end (), awb, cawb, bwb);
            return a;
        }

        template <bool u, endian r, size_t x, Word word>
        constexpr bounded<u, r, x, word> inline operator * (const bounded<u, r, x, word> &a, const bounded<u, r, x, word> &b) {
            bounded<u, r, x, word> z {};
            auto w = z.words ();
            arithmetic::times (w, a.words (), b.words ());
            return z;
        }

        template <bool u, endian r, size_t x, Word word>
        constexpr bounded<u, r, x, word> inline &operator *= (bounded<u, r, x, word> &a, const bounded<u, r, x, word> &b) {
            return a = a * b;
        }

        template <bool u, endian r, size_t x, Word word>
        constexpr bounded<u, r, x, word> inline &operator ++ (bounded<u, r, x, word> &n) {
            auto o = n.words ().begin ();
            auto i = n.words ().begin ();
            arithmetic::add_with_carry<word> (n.words ().end (), o, i, 1);
            return n;
        }

        template <bool u, endian r, size_t x, Word word>
        constexpr bounded<u, r, x, word> inline &operator -- (bounded<u, r, x, word> &n) {
            auto xx = n.words ().begin ();
            auto xy = n.words ().begin ();
            arithmetic::subtract_with_carry<word> (n.words ().end (), xx, xy, 1);
            return n;
        }

        template <endian r, size_t size, Word word>
        constexpr ubound<r, size, word> inline bounded<false, r, size, word>::max () {
            ubound<r, size, word> n;
            arithmetic::set_max_unsigned (n.words ());
            return n;
        }

        template <endian r, size_t size, Word word>
        N_bytes<r, word> inline bounded<false, r, size, word>::modulus () {
            return N_bytes<r, word> (max ()) + 1u;
        }

        template <endian r, size_t size, Word word>
        constexpr bound<r, size, word> inline bounded<true, r, size, word>::max () {
            bound<r, size, word> n;
            arithmetic::set_max_signed_twos (n.words ());
            return n;
        }

        template <endian r, size_t size, Word word>
        constexpr bound<r, size, word> inline bounded<true, r, size, word>::min () {
            bound<r, size, word> n;
            arithmetic::set_min_signed_twos (n.words ());
            return n;
        }

        namespace {
            template <endian r, size_t size, Word word>
            constexpr void shift_right (bytes_array<word, size> &n, uint32 i, byte fill) {
                if (r == endian::big) arithmetic::bit_shift_right<word> (n.rbegin (), n.rend (), i, fill);
                else arithmetic::bit_shift_right<word> (n.begin (), n.end (), i, fill);
            }

            template <endian r, size_t size, Word word>
            constexpr void shift_left (bytes_array<word, size> &n, uint32 i, byte fill) {
                if (r == endian::big) arithmetic::bit_shift_left<word> (n.begin (), n.end (), i, fill);
                else arithmetic::bit_shift_left<word> (n.rbegin (), n.rend (), i, fill);
            }
        }

        template <bool is_signed, endian r, size_t size, Word word>
        constexpr bounded<is_signed, r, size, word> inline &operator <<= (bounded<is_signed, r, size, word> &n, int i) {
            (i < 0 ? shift_right<r, size, word> : shift_left<r, size, word>) (n, i, data::is_negative (n));
            return n;
        }

        template <bool is_signed, endian r, size_t size, Word word>
        constexpr bounded<is_signed, r, size, word> inline &operator >>= (bounded<is_signed, r, size, word> &n, int i) {
            (i < 0 ? shift_left<r, size, word> : shift_right<r, size, word>) (n, i, data::is_negative (n));
            return n;
        }

        namespace {
            // TODO it looks like we don't need this, as we have a compare function in arithmetic that ought to work.
            template <Word wb, Word wl, typename ita, typename sena, typename itb, typename senb>
            requires (sizeof (wb) >= sizeof (wl))
            std::weak_ordering uz_cmp (ita &a, const sena &ae, itb &b, const senb &be) {
                size_t current_pos = 0;
                int last_pos_a = -1;
                int last_pos_b = -1;
                wb last_val_a = 0;
                wb last_val_b = 0;

                constexpr int factor = sizeof (wb) / sizeof (wl);

                while (a != ae || b != be) {
                    wb val_a {0};
                    wb val_b {0};

                    if (a != ae) {
                        val_a = *a;
                        a++;
                    }

                    if (b != be) {
                        for (int i = 0; i < factor; i++) {
                            if (b == be) break;
                            val_b |= wb (*b) << (i * sizeof (wl) * 8);
                            b++;
                        }
                    }

                    if (val_a != 0) {
                        last_val_a = val_a;
                        last_pos_a = current_pos;
                    }

                    if (val_b != 0) {
                        last_val_b = val_b;
                        last_pos_b = current_pos;
                    }

                    current_pos++;
                }

                if (last_pos_a > last_pos_b) return std::weak_ordering::greater;
                if (last_pos_a < last_pos_b) return std::weak_ordering::less;
                if (last_val_a > last_val_b) return std::weak_ordering::greater;
                if (last_val_a < last_val_b) return std::weak_ordering::less;
                return std::weak_ordering::equivalent;
            }
        }

        template <bool u, endian r, size_t x, Word word>
        constexpr bounded<u, r, x, word> inline operator / (const bounded<u, r, x, word> &a, const bounded<u, r, x, word> &b) {
            return def::divmod<bounded<u, r, x, word>> {} (a, nonzero<bounded<u, r, x, word>> {b}).Quotient;
        }

        template <bool u, endian r, size_t x, Word word>
        constexpr bounded<u, r, x, word> inline operator % (const bounded<u, r, x, word> &a, const bounded<u, r, x, word> &b) {
            return data::mod<bounded<u, r, x, word>> (a, nonzero<bounded<u, r, x, word>> {b});
        }

        template <endian r, size_t x, Word word>
        constexpr uint64 inline operator % (const ubound<r, x, word> &a, uint64 b) {
            return uint64 (a % ubound<r, x, word> (b));
        }

        template <endian r, size_t x, Word word>
        constexpr uint64 inline operator % (const bound<r, x, word> &a, uint64 b) {
            return uint64 (a % ubound<r, x, word> (b));
        }

        template <endian r, size_t size, Word word, endian o, negativity neg, Word w>
        std::weak_ordering inline operator <=> (const ubound<r, size, word> &a, const Z_bytes<o, neg, w> &b) {
            if (b < 0) return std::weak_ordering::greater;

            auto ai = a.words ().begin ();
            auto bi = b.words ().begin ();

            if constexpr (sizeof (w) > sizeof (word))
                return 0 <=> uz_cmp<w, word> (bi, b.words ().end (), ai, a.words ().end ());

            else return uz_cmp<word, w> (ai, a.words ().end (), bi, b.words ().end ());

        }
    }
}
