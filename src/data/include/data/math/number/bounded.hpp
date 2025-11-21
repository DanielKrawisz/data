// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BOUNDED
#define DATA_MATH_NUMBER_BOUNDED

#include <type_traits>
#include <iterator>
#include <data/math/number/bounded/bounded.hpp>
//#include <data/math/number/gmp/mpz.hpp>
#include <data/math/number/extended_euclidian.hpp>
#include <data/encoding/integer.hpp>
#include <data/encoding/digits.hpp>
#include <data/io/unimplemented.hpp>

namespace data::math::number {


    template <endian::order r, size_t size, std::unsigned_integral word>
    constexpr bounded<false, r, size, word>::bounded (string_view x) {
        if consteval {
            if (encoding::decimal::valid (x))
                // TODO it is possible that the number string is too big, but we won't know.
                *this = encoding::read_base<bounded<false, r, size, word>> (x, 10, encoding::decimal::digit);
            else if (encoding::hexidecimal::valid (x) && x.size () == size * sizeof (word) * 2 + 2)
                encoding::hex::decode (x.end (), x.begin () + 2, this->words ().rbegin ());
            else if (encoding::hex::valid (x) && x.size () == size * sizeof (word) * 2)
                encoding::hex::decode (x.end (), x.begin (), this->begin ());
        } else {
            if (encoding::decimal::valid (x)) *this = bounded {N_bytes<r, word>::read (x)};
            else if (encoding::hexidecimal::valid (x) && x.size () == size * sizeof (word) * 2 + 2)
                encoding::hex::decode (x.end (), x.begin () + 2, this->words ().rbegin ());
            else if (encoding::hex::valid (x) && x.size () == size * sizeof (word) * 2)
                encoding::hex::decode (x.end (), x.begin (), this->begin ());
            else throw data::exception {} << "invalid natural string \"" << x << "\"";
        }
    }

    template <endian::order r, size_t size, std::unsigned_integral word>
    constexpr bounded<true, r, size, word>::bounded (string_view x) {
        if consteval {
            if (encoding::signed_decimal::valid (x)) {
                // TODO it is possible that the number string is too big, but we won't know.
                if (encoding::decimal::valid (x)) {
                    *this = bounded<true, r, size, word> (
                        encoding::read_base<bounded<false, r, size, word>> (x, 10, encoding::decimal::digit));
                } else {
                    *this = -bounded<true, r, size, word> (
                        encoding::read_base<bounded<false, r, size, word>> (x.substr (1), 10, encoding::decimal::digit));
                }
            } else if (encoding::hexidecimal::valid (x) && x.size () == size * sizeof (word) * 2 + 2)
                encoding::hex::decode (x.end (), x.begin () + 2, this->words ().rbegin ());
            else if (encoding::hex::valid (x) && x.size () == size * sizeof (word) * 2)
                encoding::hex::decode (x.end (), x.begin (), this->begin ());
        } else {
            if (encoding::signed_decimal::valid (x)) *this = bounded {Z_bytes<r, neg::twos, word>::read (x)};
            else if (encoding::hexidecimal::valid (x) && x.size () == 2 * size * sizeof (word) + 2)
                encoding::hex::decode (x.end (), x.begin () + 2, this->words ().rbegin ());
            else if (encoding::hex::valid (x) && x.size () == size * sizeof (word) * 2)
                encoding::hex::decode (x.end (), x.begin (), this->begin ());
            else throw exception {} << "invalid integer string \"" << x << "\"";
        }
    }

    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    constexpr bounded<u, r, x, word> inline operator / (const bounded<u, r, x, word> &a, const bounded<u, r, x, word> &b) {
        return def::divmod<bounded<u, r, x, word>> {} (a, nonzero<bounded<u, r, x, word>> {b}).Quotient;
    }
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    constexpr uint<r, x, word> inline operator / (const uint<r, x, word> &a, uint64 b) {
        return a / uint<r, x, word> (b);
    }
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    constexpr sint<r, x, word> inline operator / (const sint<r, x, word> &a, int64 b) {
        return a / sint<r, x, word> (b);
    }

    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    constexpr bounded<u, r, x, word> inline operator % (const bounded<u, r, x, word> &a, const bounded<u, r, x, word> &b) {
        return data::divmod<bounded<u, r, x, word>> (a, nonzero<bounded<u, r, x, word>> {b}).Remainder;
    }
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    constexpr uint64 inline operator % (const uint<r, x, word> &a, uint64 b) {
        return uint64 (a % uint<r, x, word> (b));
    }
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    constexpr uint64 inline operator % (const sint<r, x, word> &a, uint64 b) {
        return uint64 (a % uint<r, x, word> (b));
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    inline bounded<false, r, size, word>::operator double () const {
        return double (N (N_bytes<r, word> (*this)));
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    inline bounded<true, r, size, word>::operator double () const {
        return double (Z (Z_bytes<r, neg::twos, word> (*this)));
    }

    template <endian::order r, size_t size, std::unsigned_integral word>
    std::istream &operator >> (std::istream &i, bounded<true, r, size, word> &z) {
        encoding::integer::string x;
        i >> x;
        if (i) z = bounded<true, r, size, word> {x};
        return i;
    }

    template <endian::order r, size_t size, std::unsigned_integral word>
    std::istream &operator >> (std::istream &i, bounded<false, r, size, word> &n) {
        encoding::natural::string x;
        i >> x;
        if (i) n = bounded<false, r, size, word> {x};
        return i;
    }
}

namespace data::math::def {
    template <endian::order r, size_t x, std::unsigned_integral word>
    constexpr division<uint<r, x, word>, uint<r, x, word>> inline divmod<uint<r, x, word>, uint<r, x, word>>::operator ()
        (const uint<r, x, word> &v, const nonzero<uint<r, x, word>> &z) {
        return number::natural_divmod (v, z.Value);
    }

    template <endian::order r, size_t x, std::unsigned_integral word>
    constexpr division<sint<r, x, word>, sint<r, x, word>> inline divmod<sint<r, x, word>, sint<r, x, word>>::operator ()
        (const sint<r, x, word> &v, const nonzero<sint<r, x, word>> &z) {
        return number::integer_divmod<number::TRUNCATE_TOWARD_ZERO> (v, z.Value);
    }

    template <endian::order r, size_t x, std::unsigned_integral word>
    constexpr division<uint<r, x, word>, uint<r, x, word>> inline divmod<sint<r, x, word>, uint<r, x, word>>::operator ()
        (const sint<r, x, word> &v, const nonzero<uint<r, x, word>> &z) {
        return number::natural_divmod (uint<r, x, word> (v), z.Value);
    }

    template <bool a, endian::order r, size_t x, std::unsigned_integral word>
    constexpr uint<r, x, word> square_mod<bounded<a, r, x, word>, uint<r, x, word>>::operator () (
        const bounded<a, r, x, word> &m,
        const nonzero<uint<r, x, word>> &q) {
        return times_mod<bounded<a, r, x, word>, bounded<a, r, x, word>, uint<r, x, word>> {} (m, m, q);
    }

    template <bool a, bool b, endian::order r, size_t x, std::unsigned_integral word>
    constexpr uint<r, x, word> inline plus_mod<bounded<a, r, x, word>, bounded<b, r, x, word>, uint<r, x, word>>::operator () (
        const bounded<a, r, x, word> &m,
        const bounded<b, r, x, word> &n,
        const nonzero<uint<r, x, word>> &q) {
        return uint<r, x, word> ((bounded<a, r, x + 1, word> (m) +
            bounded<b, r, x + 1, word> (n)) % uint<r, x + 1, word> (q.Value));
    }

    template <bool a, bool b, endian::order r, size_t x, std::unsigned_integral word>
    constexpr uint<r, x, word> inline times_mod<bounded<a, r, x, word>, bounded<b, r, x, word>, uint<r, x, word>>::operator () (
        const bounded<a, r, x, word> &m,
        const bounded<b, r, x, word> &n,
        const nonzero<uint<r, x, word>> &q) {
        return uint<r, x, word> (binary_accumulate_times_mod (
            bounded<a, r, x + 1, word> (m),
            bounded<b, r, x + 1, word> (n),
            nonzero {uint<r, x + 1, word> (q.Value)}));
    }

    template <bool a, bool b, endian::order r, size_t x, std::unsigned_integral word>
    constexpr uint<r, x, word> inline pow_mod<bounded<a, r, x, word>, bounded<b, r, x, word>, uint<r, x, word>>::operator () (
        const bounded<a, r, x, word> &m,
        const bounded<b, r, x, word> &n,
        const nonzero<uint<r, x, word>> &q) {
        return uint<r, x, word> (binary_accumulate_pow_mod (
            bounded<a, r, x * 2, word> (m),
            bounded<b, r, x * 2, word> (n),
            nonzero {uint<r, x * 2, word> (q.Value)}));
    }

    template <bool a, endian::order r, size_t x, std::unsigned_integral word>
    constexpr maybe<uint<r, x, word>> invert_mod<bounded<a, r, x, word>, uint<r, x, word>>::operator () (
        const bounded<a, r, x, word> &q,
        const nonzero<uint<r, x, word>> &mod) {
        auto invt = math::number::natural_invert_mod (
            bounded<a, r, x + 1, word> (x),
            nonzero {uint<r, x + 1, word> (mod.Value)});
        if (!bool (invt)) return {};
        return static_cast<uint<r, x, word>> (*invt);
    }
}

namespace data::encoding::hexidecimal {

    template <endian::order r, std::unsigned_integral word, size_t x>
    std::ostream &write (std::ostream &o, const oriented<r, word, x> &z, hex_case q) {
        o << "0x";
        return encoding::hex::write (o, z.words ().reverse (), q);
    }

    template <hex_case cx, endian::order r, std::unsigned_integral word, size_t x>
    string<cx> write (const oriented<r, word, x> &z) {
        std::stringstream ss;
        write (ss, z, cx);
        return string<cx> {ss.str ()};
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

#endif
