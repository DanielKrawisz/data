// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <type_traits>
#include <iterator>
#include <data/math/number/bounded/bounded.hpp>
#include <data/math/number/bytes/Z.hpp>

#include <data/math/number/extended_euclidian.hpp>
#include <data/math/power.hpp>

#include <data/encoding/integer.hpp>
#include <data/encoding/digits.hpp>

#include <data/exception.hpp>

namespace data::math::number {

    template <endian r, size_t size, Word word>
    constexpr bounded<false, r, size, word>::bounded (string_view x) {

        if consteval {
            if (encoding::decimal::valid (x))
                // TODO it is possible that the number string is too big, but we won't know yet.
                *this = encoding::read_base<bounded<false, r, size, word>> (x, 10, encoding::decimal::digit);
            else if (encoding::hexidecimal::valid (x) && x.size () == size * sizeof (word) * 2 + 2)
                encoding::hex::decode (x.begin () + 2, x.end (), this->words ().rbegin ());
            else if (encoding::hex::valid (x) && x.size () == size * sizeof (word) * 2)
                encoding::hex::decode (x.begin (), x.end (), this->begin ());
            else throw data::exception {} << "invalid natural string \"" << x << "\"";
        } else {
            if (encoding::decimal::valid (x)) *this = bounded {N_bytes<r, word>::read (x)};
            else if (encoding::hexidecimal::valid (x) && x.size () == size * sizeof (word) * 2 + 2)
                encoding::hex::decode (x.begin () + 2, x.end (), this->words ().rbegin ());
            else if (encoding::hex::valid (x) && x.size () == size * sizeof (word) * 2)
                encoding::hex::decode (x.begin (), x.end (), this->begin ());
            else throw data::exception {} << "invalid natural string \"" << x << "\"";
        }
    }

    template <endian r, size_t size, Word word>
    constexpr bounded<true, r, size, word>::bounded (string_view x) {
        if consteval {
            if (encoding::signed_decimal::valid (x)) {
                // TODO it is possible that the number string is too big, but we won't know.
                if (encoding::decimal::valid (x)) {
                    *this = bounded<true, r, size, word> (
                        encoding::read_base<bounded<false, r, size, word>> (x, 10, encoding::decimal::digit));

                    // if the result was negative then clearly the string was too big, but
                    // we could have still strings that are too big that we don't catch this way.
                    if (*this < 0)
                        throw exception {} << "integer string \"" << x << "\" is too big for the given number type";
                } else {
                    *this = -bounded<true, r, size, word> (
                        encoding::read_base<bounded<false, r, size, word>> (x.substr (1), 10, encoding::decimal::digit));
                }
            } else if (encoding::hexidecimal::valid (x) && x.size () == size * sizeof (word) * 2 + 2)
                encoding::hex::decode (x.begin () + 2, x.end (), this->words ().rbegin ());
            else if (encoding::hex::valid (x) && x.size () == size * sizeof (word) * 2)
                encoding::hex::decode (x.begin (), x.end (), this->begin ());
        } else {
            if (encoding::signed_decimal::valid (x)) {
                auto z = Z_bytes<r, negativity::twos, word>::read (x);
                *this = convert<bounded<true, r, size, word>> (z);
            }
            else if (encoding::hexidecimal::valid (x) && x.size () == 2 * size * sizeof (word) + 2)
                encoding::hex::decode (x.begin () + 2, x.end (), this->words ().rbegin ());
            else if (encoding::hex::valid (x) && x.size () == size * sizeof (word) * 2)
                encoding::hex::decode (x.begin (), x.end (), this->begin ());
            else throw exception {} << "invalid integer string \"" << x << "\"";
        }
    }

    template <endian r, size_t size, Word word>
    std::istream &operator >> (std::istream &i, bounded<true, r, size, word> &z) {
        encoding::integer::string x;
        i >> x;
        if (i) z = bounded<true, r, size, word> {x};
        return i;
    }

    template <endian r, size_t size, Word word>
    std::istream &operator >> (std::istream &i, bounded<false, r, size, word> &n) {
        encoding::natural::string x;
        i >> x;
        if (i) n = bounded<false, r, size, word> {x};
        return i;
    }
}

namespace data::math::def {
    template <endian r, size_t x, Word word>
    constexpr division<ubound<r, x, word>, ubound<r, x, word>> inline divmod<ubound<r, x, word>, ubound<r, x, word>>::operator ()
        (const ubound<r, x, word> &v, const nonzero<ubound<r, x, word>> &z) {
        return number::divmod (v, z.Value);
    }

    template <endian r, size_t x, Word word>
    constexpr division<bound<r, x, word>, bound<r, x, word>> inline divmod<bound<r, x, word>, bound<r, x, word>>::operator ()
        (const bound<r, x, word> &v, const nonzero<bound<r, x, word>> &z) {
        return number::integer_divmod<number::TRUNCATE_TOWARD_ZERO> (v, z.Value);
    }

    template <endian r, size_t x, Word word>
    constexpr division<ubound<r, x, word>, ubound<r, x, word>> inline divmod<bound<r, x, word>, ubound<r, x, word>>::operator ()
        (const bound<r, x, word> &v, const nonzero<ubound<r, x, word>> &z) {
        return number::divmod (ubound<r, x, word> (v), z.Value);
    }

    template <bool a, endian r, size_t x, Word word>
    constexpr ubound<r, x, word> square_mod<nbound<a, r, x, word>, ubound<r, x, word>>::operator () (
        const nbound<a, r, x, word> &m,
        const nonzero<ubound<r, x, word>> &q) {
        return times_mod<nbound<a, r, x, word>, nbound<a, r, x, word>, ubound<r, x, word>> {} (m, m, q);
    }

    template <bool a, bool b, endian r, size_t x, Word word>
    constexpr ubound<r, x, word> inline plus_mod<nbound<a, r, x, word>, nbound<b, r, x, word>, ubound<r, x, word>>::operator () (
        const nbound<a, r, x, word> &m,
        const nbound<b, r, x, word> &n,
        const nonzero<ubound<r, x, word>> &q) {
        return ubound<r, x, word> ((nbound<a, r, x + 1, word> (m) +
            nbound<b, r, x + 1, word> (n)) % ubound<r, x + 1, word> (q.Value));
    }

    template <bool a, bool b, endian r, size_t x, Word word>
    constexpr ubound<r, x, word> inline times_mod<nbound<a, r, x, word>, nbound<b, r, x, word>, ubound<r, x, word>>::operator () (
        const nbound<a, r, x, word> &m,
        const nbound<b, r, x, word> &n,
        const nonzero<ubound<r, x, word>> &q) {
        return ubound<r, x, word> (binary_accumulate_times_mod (
            nbound<a, r, x + 1, word> (m),
            nbound<b, r, x + 1, word> (n),
            nonzero {ubound<r, x + 1, word> (q.Value)}));
    }

}

namespace data::math::number {
    
    template struct bounded<false, endian::big, 10, byte>;
    template struct bounded<true, endian::big, 10, byte>;
    template struct bounded<false, endian::little, 10, byte>;
    template struct bounded<true, endian::little, 10, byte>;

    template struct bounded<false, endian::big, 5, uint16>;
    template struct bounded<true, endian::big, 5, uint16>;
    template struct bounded<false, endian::little, 5, uint16>;
    template struct bounded<true, endian::little, 5, uint16>;

    template struct bounded<false, endian::big, 16, byte>;
    template struct bounded<true, endian::big, 16, byte>;
    template struct bounded<false, endian::little, 16, byte>;
    template struct bounded<true, endian::little, 16, byte>;

    template struct bounded<false, endian::big, 4, uint32>;
    template struct bounded<true, endian::big, 4, uint32>;
    template struct bounded<false, endian::little, 4, uint32>;
    template struct bounded<true, endian::little, 4, uint32>;

    template struct bounded<false, endian::big, 20, byte>;
    template struct bounded<true, endian::big, 20, byte>;
    template struct bounded<false, endian::little, 20, byte>;
    template struct bounded<true, endian::little, 20, byte>;

    template struct bounded<false, endian::big, 5, uint32>;
    template struct bounded<true, endian::big, 5, uint32>;
    template struct bounded<false, endian::little, 5, uint32>;
    template struct bounded<true, endian::little, 5, uint32>;

    template struct bounded<false,endian::big, 28, byte>;
    template struct bounded<true, endian::big, 28, byte>;
    template struct bounded<false, endian::little, 28, byte>;
    template struct bounded<true, endian::little, 28, byte>;

    template struct bounded<false,endian::big, 7, uint32>;
    template struct bounded<true, endian::big, 7, uint32>;
    template struct bounded<false, endian::little, 7, uint32>;
    template struct bounded<true, endian::little, 7, uint32>;

    template struct bounded<false, endian::big, 32, byte>;
    template struct bounded<true, endian::big, 32, byte>;
    template struct bounded<false, endian::little, 32, byte>;
    template struct bounded<true, endian::little, 32, byte>;

    template struct bounded<false, endian::big, 8, uint32>;
    template struct bounded<true, endian::big, 8, uint32>;
    template struct bounded<false, endian::little, 8, uint32>;
    template struct bounded<true, endian::little, 8, uint32>;

    template struct bounded<false, endian::big, 40, byte>;
    template struct bounded<true, endian::big, 40, byte>;
    template struct bounded<false, endian::little, 40, byte>;
    template struct bounded<true, endian::little, 40, byte>;

    template struct bounded<false, endian::big, 10, uint32>;
    template struct bounded<true, endian::big, 10, uint32>;
    template struct bounded<false, endian::little, 10, uint32>;
    template struct bounded<true, endian::little, 10, uint32>;

    template struct bounded<false, endian::big, 56, byte>;
    template struct bounded<true, endian::big, 56, byte>;
    template struct bounded<false, endian::little, 56, byte>;
    template struct bounded<true, endian::little, 56, byte>;

    template struct bounded<false, endian::big, 14, uint32>;
    template struct bounded<true, endian::big, 14, uint32>;
    template struct bounded<false, endian::little, 14, uint32>;
    template struct bounded<true, endian::little, 14, uint32>;

    template struct bounded<false, endian::big, 64, byte>;
    template struct bounded<true, endian::big, 64, byte>;
    template struct bounded<false, endian::little, 64, byte>;
    template struct bounded<true, endian::little, 64, byte>;

    template struct bounded<false, endian::big, 16, uint32>;
    template struct bounded<true, endian::big, 16, uint32>;
    template struct bounded<false, endian::little, 16, uint32>;
    template struct bounded<true, endian::little, 16, uint32>;

}
