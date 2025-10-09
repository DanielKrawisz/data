// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_BASE58
#define DATA_ENCODING_BASE58

#include <algorithm>

#include <ctre.hpp>

#include <data/divide.hpp>
#include <data/sign.hpp>
#include <data/abs.hpp>
#include <data/encoding/digits.hpp>
#include <data/math/number/gmp/mpz.hpp>
#include <data/string.hpp>

// base 58 is a format for writing natural numbers using
// 58 digits that are easily distinguished by the human
// eye. Numbers are written in big endian. Somewhat
// confusingly, in base 58 '1' means zero.
namespace data::encoding::base58 {

    data::string inline characters () {
        return "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
    }

    static constexpr auto pattern = ctll::fixed_string {"1|([2-9A-HJ-NP-Za-km-z][1-9A-HJ-NP-Za-km-z]*)"};

    bool inline valid (const string_view s) {
        return ctre::match<pattern> (s);
    }

    bool inline nonzero (const string_view s) {
        return valid (s) && s[0] != '1';
    }

    char inline digit (char c) {
        return c < '1' ? -1 : c <= '9' ? c - '1' : c < 'A' ? -1 : c <= 'H' ? c - 'A' + 9 :
            c < 'J' ? -1 : c <= 'N' ? c - 'J' + 17 : c < 'P' ? -1 : c <= 'Z' ? c - 'P' + 22 :
            c < 'a' ? -1 : c <= 'k' ? c - 'a' + 33 : c < 'm' ? -1 : c <= 'z' ? c - 'm' + 44 : -1;
    };

    template <typename N> maybe<N> inline decode (const string_view s) {
        if (!valid (s)) return {};
        return {read_base<N> (s, 58, digit)};
    }

    maybe<bytes> read (const string_view s);

    struct string;

    template <typename N> string encode (N n);

    string write (slice<const byte> b);

    // base58 strings are really natural numbers, so we
    // can define standard math operations on them.
    std::strong_ordering operator <=> (const string &, const string &);

    template <std::integral I> bool operator == (const string &, I);
    template <std::integral I> bool operator == (I, const string &);

    template <std::integral I> std::strong_ordering operator <=> (const string &, I);
    template <std::integral I> std::strong_ordering operator <=> (I, const string &);

    string operator ++ (string &, int);
    string operator -- (string &, int);

    string operator + (const string &, const string &);
    string operator - (const string &, const string &);
    string operator * (const string &, const string &);

    string operator << (const string &, int);
    string operator >> (const string &, int);

    string operator & (const string &, const string &);
    string operator | (const string &, const string &);
    string operator ^ (const string &, const string &);

    string operator + (const string &, uint64 x);
    string operator - (const string &, uint64 x);
    string operator * (const string &, uint64 x);

    string operator / (const string &, const string &);
    string operator % (const string &, const string &);

    // base58 strings are really natural numbers, so we
    // can define standard math operations on them.
    struct string : data::string {

        string &operator ++ ();
        string &operator -- ();

        string &operator <<= (int);
        string &operator >>= (int);

        string &operator |= (const string &);
        string &operator &= (const string &);
        string &operator ^= (const string &);

        string &operator += (uint64);
        string &operator -= (uint64);
        string &operator *= (uint64);

        string &operator += (const string &);
        string &operator -= (const string &);
        string &operator *= (const string &);

        string &operator /= (const string &);
        string &operator %= (const string &);

        string ();
        string (string_view x);

        template <std::integral I> string (I x);

        string (const char *lit): string {string_view {lit, std::strlen (lit)}} {}

        explicit string (const math::N &n): string {encode<math::N> (n)} {}

        bool valid () const {
            return base58::valid (*this);
        }

        static string read (string_view);

        explicit operator uint64 () const;
        
        explicit operator math::N () const {
            if (!valid ()) throw exception {} << "invalid base 58 number" << *this;
            return *decode<math::N> (*this);
        }

        friend string operator "" _b58 (const char*, size_t);
        friend string operator "" _b58 (unsigned long long int);

        //division<string, uint64> divide (uint64) const;
        //division<string> divide (const string&) const;

    private:
        string (std::string &&x): data::string {x} {};

        friend struct math::divide<string, int>;
    };

}

namespace data {
    using base58_uint = encoding::base58::string;
}

namespace data::math {

    template <> struct sign<base58_uint> {
        signature operator () (const base58_uint &);
    };

    template <> struct is_zero<base58_uint> {
        bool operator () (const base58_uint &);
    };

    template <> struct is_negative<base58_uint> {
        bool operator () (const base58_uint &);
    };

    template <> struct is_positive<base58_uint> {
        bool operator () (const base58_uint &);
    };

    template <> struct abs<base58_uint> {
        base58_uint operator () (const base58_uint &);
    };

    template <> struct divide<base58_uint> {
        division<base58_uint> operator () (const base58_uint &v, const nonzero<base58_uint> &z);
    };

    template <> struct divide<base58_uint, int> {
        division<base58_uint, unsigned int> operator () (const base58_uint &w, nonzero<int> x);
    };

    template <> struct bit_xor<base58_uint> {
        base58_uint operator () (const base58_uint &, const base58_uint &);
    };

}

namespace data::math::number {

    template <> struct increment<base58_uint> {
        nonzero<base58_uint> operator () (const base58_uint &);
    };

    template <> struct decrement<base58_uint> {
        base58_uint operator () (const nonzero<base58_uint> &);
        base58_uint operator () (const base58_uint &);
    };

}

namespace data::math {

    signature inline sign<base58_uint>::operator () (const base58_uint &u) {
        if (!encoding::base58::valid (u)) throw exception {} << "invalid base 58 string provided to sign: " << u;
        return encoding::base58::nonzero (u) ? math::positive : math::zero;
    }

    base58_uint inline abs<base58_uint>::operator () (const base58_uint &u) {
        if (!encoding::base58::valid (u)) throw exception {} << "invalid base 58 string provided to abs: " << u;
        return u;
    }

    bool inline is_positive<base58_uint>::operator () (const encoding::base58::string &x) {
        return !data::is_zero (x);
    }

    bool inline is_zero<base58_uint>::operator () (const base58_uint &n) {
        if (!encoding::base58::valid (n)) throw exception {} << "invalid base 58 string provided to is_zero: " << n;
        return !encoding::base58::nonzero (n);
    }

    bool inline is_negative<base58_uint>::operator () (const base58_uint &n) {
        if (!encoding::base58::valid (n)) throw exception {} << "invalid base 58 string provided to is_negative: " << n;
        return false;
    }
    
    division<base58_uint> inline divide<base58_uint>::operator () (const base58_uint &v, const nonzero<base58_uint> &z) {
        // we have some extra lines here that shouldn't be necessary because the windows compiler gets confused here
        // for some reason. 
        N vn = v.operator N ();
        N zn = z.Value.operator N ();
        division<N> d = divide<N> {} (vn, nonzero<N> {zn});
        return {encoding::base58::encode<N> (d.Quotient), encoding::base58::encode<N> (d.Remainder)};
    }
    
}

namespace data::encoding::base58 {

    template <typename N>
    string inline encode (N n) {
        auto w = encoding::write_base<N> (n, characters ());
        if (w == "") return string {};
        return string {w};
    }

    template <std::integral I> bool inline operator == (const string &b, I x) {
        return b == string {x};
    }

    template <std::integral I> bool inline operator == (I x, const string &b) {
        return string {x} == b;
    }

    template <std::integral I> std::strong_ordering inline operator <=> (const string &b, I x) {
        return b <=> string {x};
    }

    template <std::integral I> std::strong_ordering inline operator <=> (I x, const string &b) {
        return string {x} <=> b;
    }

    string inline operator + (const string &n, uint64 x) {
        return n + string {x};
    }

    string inline operator - (const string &n, uint64 x) {
        return n - string {x};
    }

    string inline operator * (const string &n, uint64 x) {
        return n * string {x};
    }

    string inline operator ++ (string &m, int) {
        string n = m;
        ++m;
        return n;
    }

    string inline operator -- (string &m, int) {
        string n = m;
        --m;
        return n;
    }

    string inline &string::operator += (const string &n) {
        return *this = *this + n;
    }

    string inline &string::operator -= (const string &n) {
        return *this = *this - n;
    }

    string inline &string::operator *= (const string &n) {
        return *this = *this * n;
    }

    string inline &string::operator <<= (int i) {
        return *this = *this << i;
    }

    string inline &string::operator >>= (int i) {
        return *this = *this >> i;
    }

    inline string::string () : data::string {"1"} {}

    inline string::string (string_view x) : data::string {base58::valid (x) ? x : string_view {nullptr, 0}} {}

    template <std::integral I> string::string (I x): string {encode (math::N {x})} {}

    string inline operator / (const string &x, const string &y) {
        return math::divide<string, string> {} (x, math::nonzero {y}).Quotient;
    }

    string inline operator % (const string &x, const string &y) {
        return math::divide<string, string> {} (x, math::nonzero {y}).Remainder;
    }

    string inline &string::operator /= (const string &y) {
        return *this = *this / y;
    }

    string inline &string::operator %= (const string &y) {
        return *this = *this % y;
    }

}

namespace data::math::number {

    nonzero<base58_uint> inline increment<base58_uint>::operator () (const base58_uint &u) {
        nonzero<base58_uint> x {u};
        ++x.Value;
        return x;
    }

    base58_uint inline decrement<base58_uint>::operator () (const nonzero<base58_uint> &u) {
        auto x = u.Value;
        return --x;
    }

    base58_uint inline decrement<base58_uint>::operator () (const base58_uint &u) {
        if (data::is_zero (u)) return u;
        auto x = u;
        return --x;
    }

}

#endif
