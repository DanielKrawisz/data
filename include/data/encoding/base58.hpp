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

// base 58 is a format for writing natural numbers using
// 58 digits that are easily distinguished by the human
// eye. Numbers are written in big endian. Somewhat
// confusingly, in base 58 '1' means zero. 
namespace data::encoding::base58 {
    
    const std::string Format {"base58"};
    
    std::string inline characters () {
        return "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
    }
    
    static constexpr auto pattern = ctll::fixed_string {"([2-9A-HJ-NP-Za-km-z][1-9A-HJ-NP-Za-km-z]*)?"};
    
    bool inline valid (const string_view s) {
        return ctre::match<pattern> (s);
    }
    
    bool inline nonzero (const string_view s) {
        return valid (s) && s.size () > 0;
    }
    
    char inline digit (char c) {
        return c < '1' ? -1 : c <= '9' ?  c - '1' : c < 'A' ? -1 : c <= 'H' ? c - 'A' + 9 : 
            c < 'J' ? -1 : c <= 'N' ? c - 'J' + 17 : c < 'P' ? -1 : c <= 'Z' ? c - 'P' + 22 : 
            c < 'a' ? -1 : c <= 'k' ? c - 'a' + 33 : c < 'm' ? -1 : c <= 'z' ? c - 'm' + 44 : -1;
    };
    
    template <typename N> N inline decode (const string_view s) {
        return read_base<N> (s, 58, digit);
    }
    
    bytes inline read (const string_view s) {
        // we take two steps with different numbers because it's a lot faster. 
        return math::number::N_bytes<endian::big> (decode<math::N> (s));
    }
    
    struct string;
    
    template <typename N> string inline encode (N n);
    
    string inline write (const bytes_view b);
    
    // base58 strings are really natural numbers, so we
    // can define standard math operations on them.
    std::strong_ordering operator <=> (const string &, const string &);
    bool operator == (const string &, uint64);
    
    string& operator ++ (string &);
    string& operator -- (string &);
    
    string operator ++ (string &, int);
    string operator -- (string &, int);
    
    string operator + (const string &, const string &);
    string operator - (const string &, const string &);
    string operator * (const string &, const string &);
    
    string operator << (const string &, int);
    string operator >> (const string &, int);
    
    string operator & (const string &, const string &);
    string operator | (const string &, const string &);
    
    string operator + (const string &n, uint64 x);
    string operator - (const string &n, uint64 x);
    string operator * (const string &n, uint64 x);

    string operator / (const string &, const string &);
    string operator % (const string &, const string &);
    
    string &operator += (string &a, const string &);
    string &operator -= (string &a, const string &);
    string &operator *= (string &a, const string &);
    
    string &operator <<= (string &a, int);
    string &operator >>= (string &a, int);
    
    string &operator |= (string &a, const string&);
    string &operator &= (string &a, const string&);
    
    string &operator += (string &a, uint64);
    string &operator -= (string &a, uint64);
    string &operator *= (string &a, uint64);
    
    struct string : std::string {
        string ();
        explicit string (const std::string &);
        explicit string (std::string &&x): std::string {x} {};
        string (uint64);
        
        bool valid () const {
            return base58::valid (*this);
        }
        
        static string read (const std::string &);
        
        string& operator += (const string &);
        string& operator -= (const string &);
        string& operator *= (const string &);
        
        string& operator <<= (int);
        string& operator >>= (int);
        
        string operator &= (const string &) const;
        string operator |= (const string &) const;

        math::division<string, uint64> divide (uint64) const;

        explicit operator uint64 () const;
    };
    
    template <typename N>
    string inline encode (N n) {
        return string {encoding::write_base<N> (n, characters ())};
    }
    
    string inline write (const bytes_view b) {
        return encode<math::N> (math::N (math::number::N_bytes<endian::big>::read (b)));
    }
    
}

namespace data {
    using base58_uint = encoding::base58::string;
    
    math::sign sign (const base58_uint&);
    
    base58_uint increment (const base58_uint&);
    base58_uint decrement (const base58_uint&);
}

namespace data::math {
    
    template <> struct abs<base58_uint> {
        base58_uint operator () (const base58_uint&);
    };
    
    template <uint64 pow> 
    struct root<base58_uint, pow> {
        set<base58_uint> operator () (const base58_uint& n);
    };
    
    bool is_zero (const base58_uint &);
    bool is_negative (const base58_uint &);
    bool is_positive (const base58_uint &);
    
}

namespace data {
    
    math::sign inline sign (const base58_uint &u) {
        if (!encoding::base58::valid (u)) throw exception {} << "invalid base 58 string: \"" << u << "\"";
        return encoding::base58::nonzero (u) ? math::positive : math::zero;
    }
    
    base58_uint inline increment (const base58_uint &n) {
        auto x = n;
        return ++x;
    }
    
    base58_uint inline decrement (const base58_uint &n) {
        auto x = n;
        return --x;
    }
}

namespace data::math {
    
    base58_uint inline abs<base58_uint>::operator () (const base58_uint &u) {
        if (!encoding::base58::valid (u)) throw exception {} << "invalid base 58 string: \"" << u << "\"";
        return u;
    }
    
    bool inline is_positive (const encoding::base58::string &x) {
        return !is_zero(x);
    }
    
    bool inline is_zero (const base58_uint &n) {
        if (encoding::base58::valid (n)) throw exception {} << "invalid base 58 string: \"" << n << "\"";
        return !encoding::base58::nonzero (n);
    }
    
    bool inline is_negative (const base58_uint &n) {
        if (encoding::base58::valid (n)) throw exception {} << "invalid base 58 string: \"" << n << "\"";
        return false;
    }
}

namespace data::encoding::base58 {
    
    string inline operator + (const string& n, uint64 x) {
        return n + string {x};
    }
    
    string inline operator - (const string& n, uint64 x) {
        return n - string {x};
    }
    
    string inline operator * (const string& n, uint64 x) {
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

    inline string::string () : std::string{""} {}
    
    inline string::string (const std::string &x) : std::string {base58::valid (x) ? x : ""} {}
    
    inline string::string (uint64 x) : string {encode (math::N {x})} {}
    
}

#endif
