// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_BASE58
#define DATA_ENCODING_BASE58

#include <algorithm>

#include <ctre.hpp>

#include <data/encoding/invalid.hpp>
#include <data/math/division.hpp>
#include <data/math/sign.hpp>
#include <data/math/abs.hpp>
#include <data/math/root.hpp>
#include <data/math/number/bytes/Z.hpp>
#include <data/encoding/digits.hpp>
#include <data/string.hpp>

// base 58 is a format for writing natural numbers using
// 58 digits that are easily distinguished by the human
// eye. Numbers are written in big endian. Somewhat
// confusingly, in base 58 '1' means zero. 
namespace data::encoding::base58 {
    
    const std::string Format {"base58"};
    
    std::string inline characters () {
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
        return c < '1' ? -1 : c <= '9' ?  c - '1' : c < 'A' ? -1 : c <= 'H' ? c - 'A' + 9 : 
            c < 'J' ? -1 : c <= 'N' ? c - 'J' + 17 : c < 'P' ? -1 : c <= 'Z' ? c - 'P' + 22 : 
            c < 'a' ? -1 : c <= 'k' ? c - 'a' + 33 : c < 'm' ? -1 : c <= 'z' ? c - 'm' + 44 : -1;
    };
    
    template <typename N> maybe<N> inline decode (const string_view s) {
        if (!valid (s)) return {};
        return {read_base<N> (s, 58, digit)};
    }
    
    maybe<bytes> inline read (const string_view s) {
        // we take two steps with different numbers because it's a lot faster.
        auto n = decode<math::N> (s);
        if (!bool (n)) return {};
        return {bytes (math::number::N_bytes<endian::big, byte> (*n))};
    }
    
    struct string;
    
    template <typename N> string inline encode (N n);
    
    string inline write (const bytes_view b);
    
    // base58 strings are really natural numbers, so we
    // can define standard math operations on them.
    std::strong_ordering operator <=> (const string &, const string &);
    bool operator == (const string &, uint64);
    
    string &operator ++ (string &);
    string &operator -- (string &);
    
    string operator ++ (string &, int);
    string operator -- (string &, int);
    
    string operator + (const string &, const string &);
    string operator - (const string &, const string &);
    string operator * (const string &, const string &);
    
    string operator << (const string &, int);
    string operator >> (const string &, int);
    
    string operator & (const string &, const string &);
    string operator | (const string &, const string &);
    
    string operator + (const string &, uint64 x);
    string operator - (const string &, uint64 x);
    string operator * (const string &, uint64 x);

    string operator / (const string &, const string &);
    string operator % (const string &, const string &);
    
    string &operator <<= (string &, int);
    string &operator >>= (string &, int);
    
    string &operator |= (string &, const string &);
    string &operator &= (string &, const string &);
    
    string &operator += (string &, uint64);
    string &operator -= (string &, uint64);
    string &operator *= (string &, uint64);

    string &operator += (string &, const string &);
    string &operator -= (string &, const string &);
    string &operator *= (string &, const string &);

    string &operator /= (string &, const string &);
    string &operator %= (string &, const string &);

    // base58 strings are really natural numbers, so we
    // can define standard math operations on them.
    struct string : data::string {
        string ();
        string (const std::string &x);
        string (uint64);
        explicit string (const math::N &n): string {encode<math::N> (n)} {}
        
        bool valid () const {
            return base58::valid (*this);
        }
        
        static string read (const std::string &);

        explicit operator uint64 () const;
        explicit operator math::N () const {
            if (!valid ()) throw exception {} << "invalid base 58 number" << *this;
            return *decode<math::N> (*this);
        }

        friend string operator "" _b58 (const char*, size_t);
        friend string operator "" _b58 (unsigned long long int);

        math::division<string, uint64> divide (uint64) const;
        math::division<string> divide (const string&) const;
        
    };
    
    template <typename N>
    string inline encode (N n) {
        auto w = encoding::write_base<N> (n, characters ());
        if (w == "") return string {};
        return string {w};
    }
    
    string inline write (const bytes_view b) {
        return encode<math::N> (math::N (math::number::N_bytes<endian::big, byte>::read (b)));
    }
    
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
    
    template <uint64 pow> struct root<base58_uint, pow> {
        set<base58_uint> operator () (const base58_uint &n);
    };

    template <> struct divide<base58_uint> {
        division<base58_uint> operator () (const base58_uint &v, const nonzero<base58_uint> &z) {
            auto d = divide<N> {} (N (v), nonzero<N> {N (z.Value)});
            return {encoding::base58::encode (d.Quotient), encoding::base58::encode (d.Remainder)};
        }
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
}

namespace data::encoding::base58 {

    bool inline operator == (const string &n, uint64 x) {
        return n == string {x};
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
    
    string inline &operator += (string &a, const string& n) {
        return a = a + n;
    }
    
    string inline &operator -= (string &a, const string& n) {
        return a = a - n;
    }
    
    string inline &operator *= (string &a, const string& n) {
        return a = a * n;
    }
    
    string inline &operator <<= (string &a, int i) {
        return a = a << i;
    }
    
    string inline &operator >>= (string &a, int i) {
        return a = a >> i;
    }

    inline string::string () : data::string {"1"} {}
    
    inline string::string (const std::string &x) : data::string {base58::valid (x) ? x : ""} {}
    
    inline string::string (uint64 x) : string {encode (math::N {x})} {}

    string inline operator / (const string &x, const string &y) {
        return x.divide (y).Quotient;
    }

    string inline operator % (const string &x, const string &y) {
        return x.divide (y).Remainder;
    }

    string inline &operator /= (string &x, const string &y) {
        return x = x / y;
    }

    string inline &operator %= (string &x, const string &y) {
        return x = x % y;
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
