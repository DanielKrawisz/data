// Copyright (c) 2019 Katrina Swales
// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_INTEGER
#define DATA_ENCODING_INTEGER

#include <ctre.hpp>

#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string.hpp>

#include <data/arithmetic/negativity.hpp>
#include <data/math/number/gmp/mpz.hpp>

#include <data/string.hpp>

#include <iostream>

namespace data::encoding {
    
    namespace decimal {
        // decimal strings must begin with 1 - 9 unless they are "0"
        static constexpr ctll::fixed_string pattern {"0|([1-9][0-9]*)"};
        
        const std::string &characters ();
        constexpr char digit (char x);
        constexpr bool valid (string_view s);
        constexpr bool nonzero (string_view s);
        constexpr uint32 digits (string_view s);
        
        template <endian::order r, std::unsigned_integral word>
        maybe<math::N_bytes<r, word>> read (string_view s);

        template <std::unsigned_integral I>
        std::ostream &write (std::ostream &, I);
        
        template <endian::order r, std::unsigned_integral word>
        std::ostream &write (std::ostream &, const math::N_bytes<r, word> &);

        template <endian::order r, size_t x, std::unsigned_integral word>
        std::ostream &write (std::ostream &o, const math::uint<r, x, word> &);
        
        // a decimal string inherets from string but is
        // a big number that supports standard numerical operations. 
        struct string;

        template <std::unsigned_integral I>
        string write (I);
        
        template <endian::order r, std::unsigned_integral word>
        string write (const math::N_bytes<r, word> &);

        template <endian::order r, size_t x, std::unsigned_integral word>
        string write (const math::uint<r, x, word> &);
        
    }
    
    namespace signed_decimal {

        static constexpr ctll::fixed_string pattern {"0|(-?[1-9][0-9]*)"};

        constexpr bool valid (string_view);
        constexpr bool nonzero (string_view);
        constexpr bool positive (string_view);
        constexpr bool negative (string_view);
        constexpr math::signature sign (string_view);

        template <endian::order r, std::unsigned_integral word>
        maybe<math::Z_bytes<r, word>> read (string_view);

        using negativity = math::negativity;

        template <std::signed_integral I>
        std::ostream &write (std::ostream &, I);

        template <endian::order r, std::unsigned_integral word>
        std::ostream &write (std::ostream &o, const math::number::Z_bytes<r, negativity::twos, word> &);

        template <endian::order r, std::unsigned_integral word>
        std::ostream &write (std::ostream &o, const math::number::Z_bytes<r, negativity::BC, word> &);

        template <endian::order r, size_t x, std::unsigned_integral word>
        std::ostream &write (std::ostream &o, const math::sint<r, x, word> &);

        // a signed decimal string inherets from string but is
        // a big number that supports standard numerical operations.
        struct string;

        template <std::signed_integral I>
        string write (I);

        template <endian::order r, negativity c, std::unsigned_integral word>
        string write (const math::number::Z_bytes<r, c, word> &);

        template <endian::order r, size_t x, std::unsigned_integral word>
        string write (const math::sint<r, x, word> &);
        
    }
    
    namespace hexidecimal {

        static constexpr ctll::fixed_string pattern {"0x((([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*))"};
        static constexpr ctll::fixed_string zero_pattern {"0x(00)*"};
        static constexpr ctll::fixed_string lower_case_pattern {"0x([0-9a-f][0-9a-f])*"};
        static constexpr ctll::fixed_string upper_case_pattern {"0x([0-9A-F][0-9A-F])*"};

        constexpr bool valid (string_view s);
        constexpr bool zero (string_view s);
        constexpr bool nonzero (string_view s);
        constexpr uint32 digits (string_view s);
        constexpr char digit (char x);

        constexpr hex::letter_case read_case (string_view s);

        template <endian::order r, std::unsigned_integral word>
        maybe<oriented<r, word>> read (string_view s);

        template <endian::order r, std::unsigned_integral word>
        std::ostream inline &write (std::ostream &o, const oriented<r, word> &d, hex::letter_case q);

        template <hex::letter_case cx> struct string : data::string {
            string () : string {"0x"} {}
            explicit string (string_view x) : data::string {hexidecimal::valid (x) ? x : string_view {}} {}
            constexpr bool valid () const {
                return hexidecimal::valid (*this);
            }
        };

        template <hex::letter_case cx, endian::order r, std::unsigned_integral word>
        string<cx> write (const oriented<r, word> &z);

        using negativity = math::negativity;

        template <negativity c, hex::letter_case cx>
        bool is_minimal (const string<cx> &);

        template <negativity c, hex::letter_case cx>
        bool is_negative (const string<cx> &);

        template <negativity c, hex::letter_case cx>
        size_t minimal_size (const string<cx> &);

        template <negativity c, hex::letter_case cx>
        string<cx> extend (const string<cx> &, size_t);

        template <negativity c, hex::letter_case cx>
        string<cx> trim (const string<cx> &);
        
    }
    
    namespace natural {

        static constexpr ctll::fixed_string pattern {"0|([1-9][0-9]*)|(0x((([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*)))"};
        
        static constexpr ctll::fixed_string zero_pattern {"0|0x(00)*"};
        
        constexpr bool valid (string_view s);
        constexpr bool zero (string_view s);
        constexpr bool nonzero (string_view s);
        constexpr uint32 digits (string_view s);
        
        template <endian::order r, std::unsigned_integral word> maybe<math::N_bytes<r, word>> read (string_view s);
        
    }
    
    namespace integer {

        static constexpr ctll::fixed_string pattern {"0|(-?[1-9][0-9]*)|(0x((([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*)))"};
        
        static constexpr ctll::fixed_string zero_pattern {"0|0x(00)*"};
        
        static constexpr ctll::fixed_string negative_pattern 
            {"(-(0*[1-9][0-9]*))|0x(([8-9a-f][0-9a-f]([0-9a-f][0-9a-f])*)|([8-9A-F][0-9A-F]([0-9A-F][0-9A-F])*))"};
        
        constexpr bool valid (string_view s);
        constexpr bool negative (string_view s);
        constexpr bool zero (string_view s);
        constexpr bool nonzero (string_view s);
        constexpr uint32 digits (string_view s);
        
        template <endian::order r, math::negativity c, std::unsigned_integral word>
        maybe<math::number::Z_bytes<r, c, word>> read (string_view s);
        
    }
    
    namespace decimal {
        
        // all valid decimal strings are uniquely associated with
        // a natural number, so we can use a strong ordering. 
        std::strong_ordering operator <=> (const string &, const string &);
        
        string &operator ++ (string &);
        string &operator -- (string &);
        
        string operator ++ (string &, int);
        string operator -- (string &, int);
        
        string operator + (const string &, const string &);
        string operator - (const string &, const string &);
        string operator * (const string &, const string &);

        string operator << (const string &, int);
        string operator >> (const string &, int);
        
        signed_decimal::string operator ~ (const string &);
        string operator | (const string &, const string &);
        string operator & (const string &, const string &);
        signed_decimal::string operator ^ (const string &, const string &);
        
        string operator / (const string &, const string &);
        string operator % (const string &, const string &);
        
    }
    
    namespace signed_decimal {
        
        // all valid decimal strings are uniquely associated with
        // a natural number, so we can use a strong ordering. 
        std::strong_ordering operator <=> (const string &, const string &);
        
        string &operator ++ (string &);
        string &operator -- (string &);
        
        string operator ++ (string &, int);
        string operator -- (string &, int);
    
        string operator - (const string &);
        
        string operator + (const string &, const string &);
        string operator - (const string &, const string &);
        string operator * (const string &, const string &);
            
        string operator << (const string &, int);
        string operator >> (const string &, int);
        
        string operator ~ (const string &);
        string operator | (const string &, const string &);
        string operator & (const string &, const string &);
        string operator ^ (const string &, const string &);
        
        string operator / (const string &, const string &);
        
        string operator + (const string &n, const decimal::string &x);
        string operator - (const string &n, const decimal::string &x);
        string operator * (const string &n, const decimal::string &x);
        string operator | (const string &n, const decimal::string &x);
        string operator ^ (const string &n, const decimal::string &x);
        string operator & (const string &n, const decimal::string &x);
        string operator / (const string &n, const decimal::string &x);

        string operator + (const decimal::string &n, const string &x);
        string operator - (const decimal::string &n, const string &x);
        string operator * (const decimal::string &n, const string &x);
        string operator | (const decimal::string &n, const string &x);
        string operator ^ (const decimal::string &n, const string &x);
        string operator & (const decimal::string &n, const string &x);
        string operator / (const decimal::string &n, const string &x);
        
        decimal::string operator % (const string &, const decimal::string &x);
        
        string &operator += (string &n, const decimal::string &x);
        string &operator -= (string &n, const decimal::string &x);
        string &operator *= (string &n, const decimal::string &x);
        string &operator |= (string &n, const decimal::string &x);
        string &operator &= (string &n, const decimal::string &x);
        string &operator ^= (string &n, const string &x);
        string &operator /= (string &n, const decimal::string &x);
        string &operator %= (string &n, const decimal::string &x);
        
    }
    
    namespace hexidecimal {

        // a hexidecimal integer inherets from string but is
        // a big number that supports standard numerical operations.
        template <negativity, hex::letter_case cx> struct integer;

        // comparison
        template <negativity cl, negativity cr, hex::letter_case cx>
        bool operator == (const integer<cl, cx> &a, const integer<cr, cx> &b);
        
        template <hex::letter_case cx>
        std::weak_ordering operator <=> (const integer<negativity::nones, cx> &, const integer<negativity::nones, cx> &);
        
        template <hex::letter_case cx>
        std::weak_ordering operator <=> (const integer<negativity::twos, cx> &, const integer<negativity::twos, cx> &);
        
        template <hex::letter_case cx>
        std::weak_ordering operator <=> (const integer<negativity::BC, cx> &, const integer<negativity::BC, cx> &);

        template <negativity c, hex::letter_case cx>
        std::weak_ordering operator <=> (const integer<negativity::nones, cx> &, const integer<c, cx> &);

        template <negativity c, hex::letter_case cx>
        std::weak_ordering operator <=> (const integer<c, cx> &, const integer<negativity::nones, cx> &);

        template <negativity cl, negativity cr, hex::letter_case cx>
        std::weak_ordering operator <=> (const integer<cl, cx> &, const integer<cr, cx> &);

        template <hex::letter_case cx>
        bool operator == (const integer<negativity::nones, cx> &, uint64);

        template <hex::letter_case cx>
        bool operator == (const integer<negativity::twos, cx> &, int64);

        template <hex::letter_case cx>
        bool operator == (const integer<negativity::BC, cx> &, int64);

        template <hex::letter_case cx>
        std::weak_ordering operator <=> (const integer<negativity::nones, cx> &, uint64);

        template <hex::letter_case cx>
        std::weak_ordering operator <=> (const integer<negativity::twos, cx> &, int64);

        template <hex::letter_case cx>
        std::weak_ordering operator <=> (const integer<negativity::BC, cx> &, int64);
        
        // increment and decrement
        template <hex::letter_case cx>
        integer<negativity::nones, cx> &operator ++ (integer<negativity::nones, cx> &);
        
        template <hex::letter_case cx>
        integer<negativity::twos, cx> &operator ++ (integer<negativity::twos, cx> &);
        
        template <hex::letter_case cx>
        integer<negativity::BC, cx> &operator ++ (integer<negativity::BC, cx> &);
        
        template <hex::letter_case cx>
        integer<negativity::nones, cx> &operator -- (integer<negativity::nones, cx> &);
        
        template <hex::letter_case cx>
        integer<negativity::twos, cx> &operator -- (integer<negativity::twos, cx> &);
        
        template <hex::letter_case cx>
        integer<negativity::BC, cx> &operator -- (integer<negativity::BC, cx> &);
        
        template <negativity c, hex::letter_case cx>
        integer<c, cx> operator ++ (integer<c, cx> &, int);
        
        template <negativity c, hex::letter_case cx>
        integer<c, cx> operator -- (integer<c, cx> &, int);
        
        // basic arithmetic
        template <negativity c, hex::letter_case cx>
        integer<c, cx> operator + (const integer<c, cx> &, const integer<c, cx> &);
        
        template <negativity c, hex::letter_case cx>
        integer<c, cx> operator - (const integer<c, cx> &, const integer<c, cx> &);
        
        template <negativity c, hex::letter_case cx>
        integer<c, cx> operator * (const integer<c, cx> &, const integer<c, cx> &);

        // power
        template <hex::letter_case cx>
        integer<negativity::BC, cx> operator ^
            (const integer<negativity::BC, cx> &, const integer<negativity::BC, cx> &);

        template <hex::letter_case cx>
        integer<negativity::BC, cx> &operator ^=
            (const integer<negativity::BC, cx> &, const integer<negativity::BC, cx> &);

        // division
        template <negativity c, hex::letter_case cx>
        integer<c, cx> operator / (const integer<c, cx> &, const integer<c, cx> &);

        template <negativity c, hex::letter_case cx>
        integer<c, cx> &operator /= (const integer<c, cx> &, const integer<c, cx> &);

        // mod
        template <hex::letter_case cx>
        integer<negativity::nones, cx> operator % (const integer<negativity::nones, cx> &n, const integer<negativity::nones, cx> &x);

        template <hex::letter_case cx>
        integer<negativity::nones, cx> &operator %= (integer<negativity::nones, cx> &n, const integer<negativity::nones, cx> &x);

        template <hex::letter_case cx>
        integer<negativity::BC, cx> &operator % (const integer<negativity::BC, cx> &n, const integer<negativity::BC, cx> &x);

        template <hex::letter_case cx>
        integer<negativity::BC, cx> &operator %= (integer<negativity::BC, cx> &n, const integer<negativity::BC, cx> &x);

        // bit operations
        template <hex::letter_case cx>
        integer<negativity::twos, cx> operator ~ (const integer<negativity::twos, cx> &);

        template <hex::letter_case cx>
        integer<negativity::twos, cx> operator ~ (const integer<negativity::nones, cx> &);

        template <hex::letter_case cx>
        integer<negativity::twos, cx> operator ^
        (const integer<negativity::nones, cx> &, const integer<negativity::nones, cx> &);

        template <hex::letter_case cx>
        integer<negativity::twos, cx> operator ^
            (const integer<negativity::twos, cx> &, const integer<negativity::twos, cx> &);

        template <hex::letter_case cx>
        integer<negativity::twos, cx> &operator ^=
            (const integer<negativity::twos, cx> &, const integer<negativity::twos, cx> &);
        
        template <negativity c, hex::letter_case cx>
        integer<c, cx> operator | (const integer<c, cx> &, const integer<c, cx> &);

        template <negativity c, hex::letter_case cx>
        integer<c, cx> operator & (const integer<c, cx> &, const integer<c, cx> &);

        template <negativity c, hex::letter_case cx>
        integer<c, cx> operator | (const integer<c, cx> &, uint64);

        template <negativity c, hex::letter_case cx>
        integer<c, cx> operator & (const integer<c, cx> &, uint64);

        template <negativity c, hex::letter_case cx>
        integer<c, cx> &operator |= (const integer<c, cx> &, const integer<c, cx> &);

        template <negativity c, hex::letter_case cx>
        integer<c, cx> &operator &= (const integer<c, cx> &, const integer<c, cx> &);
        
        // bit shift
        template <negativity c, hex::letter_case cx>
        integer<c, cx> operator << (const integer<c, cx> &, int);
        
        template <negativity c, hex::letter_case cx>
        integer<c, cx> operator >> (const integer<c, cx> &, int);

        template <hex::letter_case cx>
        integer<negativity::twos, cx> operator - (const integer<negativity::nones, cx> &);
        
        template <hex::letter_case cx> 
        integer<negativity::twos, cx> operator - (const integer<negativity::twos, cx> &);
        
        template <hex::letter_case cx> 
        integer<negativity::BC, cx> operator - (const integer<negativity::BC, cx> &);

        // bool operations
        template <hex::letter_case cx>
        integer<negativity::BC, cx> operator ! (const integer<negativity::BC, cx> &);

        template <hex::letter_case cx> integer<negativity::BC, cx> operator &&
            (const integer<negativity::BC, cx> &, const integer<negativity::BC, cx> &);

        template <hex::letter_case cx> integer<negativity::BC, cx> operator ||
            (const integer<negativity::BC, cx> &, const integer<negativity::BC, cx> &);
        
        // special cases
        template <hex::letter_case cx> 
        integer<negativity::twos, cx> operator + (const integer<negativity::twos, cx> &n, const integer<negativity::nones, cx> &x);
        
        template <hex::letter_case cx> 
        integer<negativity::twos, cx> operator - (const integer<negativity::twos, cx> &n, const integer<negativity::nones, cx> &x);
        
        template <hex::letter_case cx> 
        integer<negativity::twos, cx> operator * (const integer<negativity::twos, cx> &n, const integer<negativity::nones, cx> &x);

        template <hex::letter_case cx>
        integer<negativity::twos, cx> operator + (const integer<negativity::nones, cx> &n, const integer<negativity::twos, cx> &x);

        template <hex::letter_case cx>
        integer<negativity::twos, cx> operator - (const integer<negativity::nones, cx> &n, const integer<negativity::twos, cx> &x);

        template <hex::letter_case cx>
        integer<negativity::twos, cx> operator * (const integer<negativity::nones, cx> &n, const integer<negativity::twos, cx> &x);
        
        template <hex::letter_case cx> 
        integer<negativity::twos, cx> operator | (const integer<negativity::twos, cx> &n, const integer<negativity::nones, cx> &x);
        
        template <hex::letter_case cx> 
        integer<negativity::twos, cx> operator & (const integer<negativity::twos, cx> &n, const integer<negativity::nones, cx> &x);
        
        template <hex::letter_case cx> 
        integer<negativity::twos, cx> operator / (const integer<negativity::twos, cx> &n, const integer<negativity::nones, cx> &x);

        template <hex::letter_case cx>
        integer<negativity::nones, cx> operator % (const integer<negativity::twos, cx> &n, const integer<negativity::nones, cx> &x);
        
        template <hex::letter_case cx> 
        integer<negativity::twos, cx> &operator += (integer<negativity::twos, cx> &n, const integer<negativity::nones, cx> &x);
        
        template <hex::letter_case cx> 
        integer<negativity::twos, cx> &operator -= (integer<negativity::twos, cx> &n, const integer<negativity::nones, cx> &x);
        
        template <hex::letter_case cx> 
        integer<negativity::twos, cx> &operator *= (integer<negativity::twos, cx> &n, const integer<negativity::nones, cx> &x);
        
        template <hex::letter_case cx> 
        integer<negativity::twos, cx> &operator |= (integer<negativity::twos, cx> &n, const integer<negativity::nones, cx> &x);
        
        template <hex::letter_case cx> 
        integer<negativity::twos, cx> &operator &= (integer<negativity::twos, cx> &n, const integer<negativity::nones, cx> &x);
        
    }

    namespace natural {
        // a string representation of a natural number (dec or hexidecimal)
        struct string : data::string {
            using data::string::string;
        };

        std::istream &operator >> (std::istream &, string &);
    }

    namespace integer {
        // a string representation of an integer (hexidecimal or dec with an optional -);
        struct string : data::string {
            using data::string::string;
        };

        std::istream &operator >> (std::istream &, string &);
    }
}

namespace data::hex {
    template <hex_case zz> using uint = encoding::hexidecimal::integer<math::negativity::nones, zz>;
    template <hex_case zz> using int2 = encoding::hexidecimal::integer<math::negativity::twos, zz>;
    template <hex_case zz> using intBC = encoding::hexidecimal::integer<math::negativity::BC, zz>;
    template <math::negativity c, hex_case zz> using integer = encoding::hexidecimal::integer<c, zz>;
}

namespace data {
    using dec_uint = encoding::decimal::string;
    using dec_int = encoding::signed_decimal::string;

    template<math::negativity a, hex_case b, math::negativity c, hex_case d>
    bool identical (const hex::integer<a, b> &, const hex::integer<c, d> &);
    
}

namespace data::math {

    template <> struct sign<dec_uint> {
        math::signature operator () (const dec_uint &);
    };

    template <> struct sign<dec_int> {
        math::signature operator () (const dec_int &);
    };

    template <hex_case zz> struct sign<hex::uint<zz>> {
        math::signature operator () (const hex::uint<zz> &);
    };

    template <hex_case zz> struct sign<hex::int2<zz>> {
        math::signature operator () (const hex::int2<zz> &);
    };

    template <hex_case zz> struct sign<hex::intBC<zz>> {
        math::signature operator () (const hex::intBC<zz> &);
    };
    
    template <> struct abs<dec_uint> {
        dec_uint operator () (const dec_uint &);
    };
    
    template <> struct abs<dec_int> {
        dec_uint operator () (const dec_int &);
    };
    
    template <hex_case zz> struct abs<hex::uint<zz>> {
        hex::uint<zz> operator () (const hex::uint<zz> &);
    };
    
    template <hex_case zz> struct abs<hex::int2<zz>> {
        hex::uint<zz> operator () (const hex::int2<zz> &);
    };
    
    template <hex_case zz> struct abs<hex::intBC<zz>> {
        hex::intBC<zz> operator () (const hex::intBC<zz> &);
    };

    template <> struct times<dec_int> {
        dec_int operator () (const dec_int &, const dec_int &);
        nonzero<dec_int> operator () (const nonzero<dec_int> &, const nonzero<dec_int> &);
    };

    template <negativity c, hex_case zz> struct times<hex::integer<c, zz>> {
        hex::integer<c, zz> operator () (const hex::integer<c, zz> &, const hex::integer<c, zz> &);
        nonzero<hex::integer<c, zz>> operator () (const nonzero<hex::integer<c, zz>> &, const nonzero<hex::integer<c, zz>> &);
    };

    template <> struct is_zero<dec_uint> {
        bool operator () (const dec_uint &);
    };

    template <> struct is_negative<dec_uint> {
        bool operator () (const dec_uint &);
    };

    template <> struct is_positive<dec_uint> {
        bool operator () (const dec_uint &);
    };

    template <> struct is_zero<dec_int> {
        bool operator () (const dec_int &);
    };

    template <> struct is_negative<dec_int> {
        bool operator () (const dec_int &);
    };

    template <> struct is_positive<dec_int> {
        bool operator () (const dec_int &);
    };
    
    template <hex_case zz> struct is_zero<hex::uint<zz>> {
        bool operator () (const hex::uint<zz> &);
    };

    template <hex_case zz> struct is_zero<hex::int2<zz>> {
        bool operator () (const hex::int2<zz> &);
    };

    template <hex_case zz> struct is_zero<hex::intBC<zz>> {
        bool operator () (const hex::intBC<zz> &);
    };
    
    template <hex_case zz> struct is_negative<hex::uint<zz>> {
        bool operator () (const hex::uint<zz> &);
    };

    template <hex_case zz> struct is_negative<hex::int2<zz>> {
        bool operator () (const hex::int2<zz> &);
    };

    template <hex_case zz> struct is_negative<hex::intBC<zz>> {
        bool operator () (const hex::intBC<zz> &);
    };
    
    template <negativity c, hex_case zz> struct is_positive<hex::integer<c, zz>> {
        bool operator () (const hex::integer<c, zz> &);
    };
    
    template <hex_case cx> struct is_positive_zero<hex::intBC<cx>> {
        bool operator () (const hex::intBC<cx> &);
    };
    
    template <hex_case cx> struct is_negative_zero<hex::intBC<cx>> {
        bool operator () (const hex::intBC<cx> &);
    };

    template <> struct divide<dec_uint, dec_uint> {
        division<dec_uint, dec_uint> operator () (const dec_uint &, const nonzero<dec_uint> &);
    };

    template <> struct divide<dec_int, dec_int> {
        division<dec_int, dec_uint> operator () (const dec_int &, const nonzero<dec_int> &);
    };

    template <> struct divide<dec_int, dec_uint> {
        division<dec_int, dec_uint> operator () (const dec_int &, const nonzero<dec_uint> &);
    };

    template <hex_case zz>
    struct divide<hex::uint<zz>, hex::uint<zz>> {
        division<hex::uint<zz>, hex::uint<zz>> operator () (const hex::uint<zz> &, const nonzero<hex::uint<zz>> &);
    };

    template <hex_case zz>
    struct divide<hex::int2<zz>, hex::int2<zz>> {
        division<hex::int2<zz>, hex::uint<zz>> operator () (const hex::int2<zz> &, const nonzero<hex::int2<zz>> &);
    };

    template <hex_case zz>
    struct divide<hex::int2<zz>, hex::uint<zz>> {
        division<hex::int2<zz>, hex::uint<zz>> operator () (const hex::int2<zz> &, const nonzero<hex::uint<zz>> &);
    };

    template <hex_case zz>
    struct divide<hex::intBC<zz>, hex::intBC<zz>> {
        division<hex::intBC<zz>, hex::intBC<zz>> operator () (const hex::intBC<zz> &, const nonzero<hex::intBC<zz>> &);
    };

    // We have special cases for converting from string to N.
    template <> struct divide<dec_uint, int> {
        division<dec_uint, unsigned int> operator () (const dec_uint &, const nonzero<int> &);
    };

    template <> struct divide<dec_int, int> {
        division<dec_int, unsigned int> operator () (const dec_int &, const nonzero<int> &);
    };

    template <hex_case zz>
    struct divide<hex::uint<zz>, int> {
        division<hex::uint<zz>, unsigned int> operator () (const hex::uint<zz> &, const nonzero<int> &);
    };

    template <hex_case zz>
    struct divide<hex::int2<zz>, int> {
        division<hex::int2<zz>, unsigned int> operator () (const hex::int2<zz> &, const nonzero<int> &);
    };

    template <hex_case zz>
    struct divide<hex::intBC<zz>, int> {
        division<hex::intBC<zz>, int> operator () (const hex::intBC<zz> &, const nonzero<int> &);
    };

    template <> struct bit_xor<dec_uint> {
        dec_int operator () (const dec_uint &, const dec_uint &);
    };

    template <> struct bit_xor<dec_int> {
        dec_int operator () (const dec_int &, const dec_int &);
    };

    template <hex_case zz>
    struct bit_xor<hex::uint<zz>> {
        hex::int2<zz> operator () (const hex::uint<zz> &, const hex::uint<zz> &);
    };

    template <hex_case zz>
    struct bit_xor<hex::int2<zz>> {
        hex::int2<zz> operator () (const hex::int2<zz> &, const hex::int2<zz> &);
    };

    template <hex_case zz>
    struct bit_xor<hex::intBC<zz>> {
        hex::intBC<zz> operator () (const hex::intBC<zz> &, const hex::intBC<zz> &);
    };

    template <hex_case zz>
    struct bit_not<hex::intBC<zz>> {
        hex::intBC<zz> operator () (const hex::intBC<zz> &);
    };
    
}

namespace data::math::number {

    template <> struct increment<dec_uint> {
        nonzero<dec_uint> operator () (const dec_uint &);
    };

    template <> struct decrement<dec_uint> {
        dec_uint operator () (const nonzero<dec_uint> &);
        dec_uint operator () (const dec_uint &);
    };

    template <> struct increment<dec_int> {
        dec_int operator () (const dec_int &);
    };

    template <> struct decrement<dec_int> {
        dec_int operator () (const dec_int &);
    };

    template <hex_case zz> struct increment<hex::uint<zz>> {
        nonzero<hex::uint<zz>> operator () (const hex::uint<zz> &);
    };

    template <hex_case zz> struct decrement<hex::uint<zz>> {
        hex::uint<zz> operator () (const nonzero<hex::uint<zz>> &);
        hex::uint<zz> operator () (const hex::uint<zz> &);
    };

    template <math::negativity c, hex_case zz> struct increment<hex::integer<c, zz>> {
        hex::integer<c, zz> operator () (const hex::integer<c, zz> &);
    };

    template <math::negativity c, hex_case zz> struct decrement<hex::integer<c, zz>> {
        hex::integer<c, zz> operator () (const hex::integer<c, zz> &);
    };

    template <hex_case cx> 
    bool sign_bit_set (const hex::intBC<cx> &);
    
    template <hex_case cx> 
    bool is_minimal (const hex::uint<cx> &);
    
    template <hex_case cx> 
    bool is_minimal (const hex::int2<cx> &);
    
    template <hex_case cx> 
    bool is_minimal (const hex::intBC<cx> &);
    
    template <hex_case cx> 
    size_t minimal_size (const hex::uint<cx> &);
    
    template <hex_case cx> 
    size_t minimal_size (const hex::int2<cx> &);
    
    template <hex_case cx> 
    size_t minimal_size (const hex::intBC<cx> &);
    
    template <hex_case cx> 
    hex::uint<cx> extend (const hex::uint<cx> &, size_t);
    
    template <hex_case cx> 
    hex::int2<cx> extend (const hex::int2<cx> &, size_t);
    
    template <hex_case cx> 
    hex::intBC<cx> extend (const hex::intBC<cx> &, size_t);
    
    template <hex_case cx> 
    hex::uint<cx> trim (const hex::uint<cx> &);
    
    template <hex_case cx> 
    hex::int2<cx> trim (const hex::int2<cx> &);
    
    template <hex_case cx> 
    hex::intBC<cx> trim (const hex::intBC<cx> &);
    
}

namespace data::encoding::decimal {
    
    struct string : data::string {
        string ();
        
        explicit string (const std::string &x);
        explicit string (std::string &&x);
        explicit string (const char *x): string {string_view {x, std::strlen (x)}} {}
        explicit string (string_view x):
            data::string {decimal::valid (x) ? x : string_view {nullptr, 0}} {}

        // We need these to ensure that we can accept
        // any number literal.
        template <std::signed_integral I> string (I);
        template <std::unsigned_integral I> string (I);

        explicit string (const math::N &);
        
        bool valid () const {
            return decimal::valid (*this);
        }
        
        static string read (string_view x);
        
        string &operator += (const string &);
        string &operator -= (const string &);
        string &operator *= (const string &);
        
        string &operator <<= (int);
        string &operator >>= (int);
        
        string &operator |= (const string &);
        string &operator &= (const string &);

        string &operator /= (const string &);
        string &operator %= (const string &);
        
        bool operator == (uint64) const;
        std::strong_ordering operator <=> (uint64) const;
        
        string operator + (uint64) const;
        string operator - (uint64) const;
        string operator * (uint64) const;
        
        string &operator += (uint64);
        string &operator -= (uint64);
        string &operator *= (uint64);

        //string operator & (uint64 u) const;

        string operator | (uint64) const;

        string &operator &= (uint64);
        string &operator |= (uint64);

        string &operator /= (uint64);
        
        explicit operator double () const;
        explicit operator uint64 () const;
    };
    
    signed_decimal::string operator - (const string &);

    template <std::unsigned_integral I>
    string inline write (I x) {
        std::stringstream ss;
        write (ss, x);
        return string {ss.str ()};
    }

    template <std::unsigned_integral I>
    std::ostream inline &write (std::ostream &o, I x) {
        return o << std::dec << x;
    }

    template <endian::order r, size_t x, std::unsigned_integral word>
    std::ostream inline &write (std::ostream &o, const math::uint<r, x, word> &n) {
        return write (o, math::N_bytes<r, word> (n));
    }

    template <endian::order r, size_t x, std::unsigned_integral word>
    string inline write (const math::uint<r, x, word> &n) {
        std::stringstream ss;
        write (ss, n);
        return string {ss.str ()};
    }
    
}

namespace data::encoding::signed_decimal {
    
    struct string : data::string {
        string () : data::string {"0"} {};
        explicit string (const std::string &x) : data::string {signed_decimal::valid (x) ? x : ""} {}
        explicit string (std::string &&x) : data::string {x} {}
        explicit string (const char *x): string {string_view {x, std::strlen (x)}} {}
        explicit string (string_view x):
            data::string {signed_decimal::valid (x) ? x : string_view {nullptr, 0}} {}

        // We need these to ensure that we can accept
        // any number literal.
        template <std::signed_integral I> string (I x): data::string {write (x)} {}
        template <std::unsigned_integral I> string (I x): data::string {decimal::string {x}} {}

        explicit string (const math::Z &);
        
        bool valid () const {
            return signed_decimal::valid (*this);
        }
        
        static string read (string_view x);
        
        string &operator += (const string &x) {
            return *this = *this + x;
        }

        string &operator -= (const string &x) {
            return *this = *this - x;
        }

        string &operator *= (const string &);
        
        string &operator <<= (int i) {
            return *this = *this << i;
        }

        string &operator >>= (int i) {
            return *this = *this >> i;
        }
        
        string &operator |= (const string &) const;
        string &operator &= (const string &) const;

        string &operator /= (const string &);
        
        bool operator == (int64) const;
        std::strong_ordering operator <=> (int64) const;
        
        string operator + (int64) const;
        string operator - (int64) const;
        string operator * (int64) const;
        string operator / (int64) const;
        
        string &operator += (int64);
        string &operator -= (int64);
        string &operator *= (int64);

        string operator & (int64) const;
        string operator | (int64) const;

        string &operator &= (int64);
        string &operator |= (int64);

        string &operator /= (int64);
        
        explicit operator double () const;
        explicit operator int64 () const;
        //explicit operator math::Z () const;
    };

    template <std::signed_integral I>
    string inline write (I x) {
        std::stringstream ss;
        write (ss, x);
        return string {ss.str ()};
    }

    template <std::signed_integral I>
    std::ostream inline &write (std::ostream &o, I x) {
        return o << std::dec << x;
    }
    
    template <endian::order r, std::unsigned_integral word>
    std::ostream &write (std::ostream &w, const math::number::Z_bytes<r, negativity::twos, word> &z) {
        if (data::is_negative (z)) w << "-";
        return decimal::write (w, data::abs (z));
    }

    template <endian::order r, std::unsigned_integral word>
    std::ostream &write (std::ostream &w, const math::number::Z_bytes<r, negativity::BC, word> &z) {
        if (is_zero (z)) return w << "0";
        if (is_negative (z)) w << "-";
        return decimal::write (w, math::N_bytes<r, word>::read (abs (z)));
    }
    
    template <endian::order r, negativity n, std::unsigned_integral word>
    string inline write (const math::number::Z_bytes<r, n, word> &z) {
        std::stringstream ss;
        write (ss, z);
        return string {ss.str ()};
    }

    template <endian::order r, size_t x, std::unsigned_integral word>
    std::ostream inline &write (std::ostream &o, const math::sint<r, x, word> &z) {
        return write (o, math::number::Z_bytes<r, negativity::twos, word> (z));
    }

    template <endian::order r, size_t x, std::unsigned_integral word>
    string inline write (const math::sint<r, x, word> &n) {
        std::stringstream ss;
        write (ss, n);
        return string {ss.str ()};
    }
}

namespace data::encoding::hexidecimal {

    template <negativity c, hex::letter_case cx>
    struct complemented_string : string<cx> {
        using string<cx>::string;

        template <std::integral I> complemented_string (I);

        complemented_string (const string<cx> &x): string<cx> {x} {}
        explicit complemented_string (const math::Z &);

        explicit operator int64 () const;
        explicit operator integer<negativity (-int (c) + 5), cx> () const;

        static integer<c, cx> zero (size_t size = 0, bool negative = false);

        integer<c, cx> operator + (int64) const;
        integer<c, cx> operator - (int64) const;
        integer<c, cx> operator * (int64) const;

        integer<c, cx> &operator += (int64);
        integer<c, cx> &operator -= (int64);
        integer<c, cx> &operator *= (int64);

        integer<c, cx> operator & (int64) const;
        integer<c, cx> operator | (int64) const;

        integer<c, cx> &operator &= (int64);
        integer<c, cx> &operator |= (int64);

        integer<c, cx> &operator /= (int64);

        explicit operator math::Z () const;
    };

    template <hex::letter_case cx>
    struct complemented_string<negativity::nones, cx> : string<cx> {
        using string<cx>::string;

        template <std::integral I> complemented_string (I);

        explicit complemented_string (const math::N &);

        explicit operator integer<negativity::twos, cx> () const;
        explicit operator integer<negativity::BC, cx> () const;
        explicit operator uint64 () const {
            return uint64 (math::N (*this));
        }

        data::hex::uint<cx> operator + (uint64) const;
        data::hex::uint<cx> operator - (uint64) const;
        data::hex::uint<cx> operator * (uint64) const;

        data::hex::uint<cx> &operator += (uint64);
        data::hex::uint<cx> &operator -= (uint64);
        data::hex::uint<cx> &operator *= (uint64);

        data::hex::uint<cx> operator & (uint64) const;
        data::hex::uint<cx> operator | (uint64) const;

        data::hex::uint<cx> &operator &= (uint64);
        data::hex::uint<cx> &operator |= (uint64);

        data::hex::uint<cx> &operator /= (uint64);
        data::hex::uint<cx> &operator %= (uint64);

    };
    
    template <negativity c, hex::letter_case cx>
    struct integer : complemented_string<c, cx> {

        using complemented_string<c, cx>::complemented_string;

        // try to read either a hex string or a dec string.
        static integer read (string_view);
        
        integer &operator += (const integer &);
        integer &operator -= (const integer &);
        integer &operator *= (const integer &);
        
        integer &operator <<= (int i);
        integer &operator >>= (int i);
        
        integer operator / (const integer &x) const;
        integer operator % (const integer &x) const;
        
        explicit operator double () const;
        explicit operator bool () const;
        
        integer &trim ();
        integer trim () const;
        
    };

    template <negativity c, hex::letter_case cx>
    bool inline is_minimal (const string<cx> &x) {
        return math::number::is_minimal (integer<c, cx> {x});
    }

    template <negativity c, hex::letter_case cx>
    bool inline is_negative (const string<cx> &x) {
        return is_negative (integer<c, cx> {x});
    }

    template <negativity c, hex::letter_case cx>
    size_t inline minimal_size (const string<cx> &x) {
        return math::number::minimal_size (integer<c, cx> {x});
    }

    template <negativity c, hex::letter_case cx>
    string<cx> inline extend (const string<cx> &x, size_t z) {
        return math::number::extend (integer<c, cx> {x}, z);
    }

    template <negativity c, hex::letter_case cx>
    string<cx> inline trim (const string<cx> &x) {
        return math::number::trim (integer<c, cx> {x});
    }

    template <negativity c, hex::letter_case zz>
    inline integer<c, zz>::operator bool () const {
        return !is_zero (*this);
    }
    
}

namespace data::encoding::decimal {
        
    const std::string inline &characters () {
        static std::string Dec {"0123456789"};
        return Dec;
    }
    
    constexpr char inline digit (char x) {
        return x < '0' || x > '9' ? -1 : x - '0';
    }
        
    constexpr bool inline valid (string_view s) {
        return ctre::match<pattern> (s);
    } 
    
    constexpr bool inline nonzero (string_view s) {
        return valid (s) && s[0] != '0';
    }
    
    constexpr uint32 inline digits (string_view s) {
        return valid (s) ? s.size () : 0;
    }

    template <std::signed_integral I>
    inline string::string (I x): string {write (static_cast <std::make_unsigned_t<I>> (x))} {
        if (x < 0) throw exception {} << "decimal string cannot be negative";
    }

    template <std::unsigned_integral I>
    inline string::string (I x) : string {write (x)} {}
    
    inline string::string () : data::string {"0"} {}
        
    inline string::string (const std::string &x) : data::string {decimal::valid (x) ? x : ""} {}

    inline string::string (std::string &&x) : data::string {x} {}
    
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
    
    string inline string::operator + (uint64 x) const {
        return *this + string {x};
    }
    
    string inline string::operator - (uint64 x) const {
        return *this - string {x};
    }
    
    string inline string::operator * (uint64 x) const {
        return *this * string {x};
    }
    
    string inline &string::operator += (uint64 x) {
        return *this += string {x};
    }
    
    string inline &string::operator -= (uint64 x) {
        return *this -= string {x};
    }
    
    string inline &string::operator *= (uint64 x) {
        return *this *= string {x};
    }
    
    string inline operator ++ (string &x, int) {
        auto n = x;
        ++x;
        return n;
    }
    
    string inline operator -- (string &x, int) {
        auto n = x;
        --x;
        return n;
    }

    string inline &string::operator |= (const string &z) {
        return *this = *this | z;
    }

    string inline &string::operator &= (const string &z) {
        return *this = *this & z;
    }

    string inline &string::operator /= (const string &z) {
        return *this = *this / z;
    }

    string inline &string::operator %= (const string &z) {
        return *this = *this % z;
    }
    
}

namespace data::encoding::signed_decimal {
    
    constexpr bool inline valid (string_view s) {
        return ctre::match<pattern> (s);
    }
    
    constexpr bool inline nonzero (string_view s) {
        return valid (s) && s[0] != '0';
    }
    
    constexpr bool inline negative (string_view s) {
        return valid (s) && s[0] == '-';
    }
    
    constexpr bool inline positive (string_view s) {
        return valid (s) && s[0] != '-' && s[0] != '0';
    }
    
    constexpr math::signature inline sign (string_view s) {
        if (!valid (s)) throw exception {} << "invalid decimal string: " << s;
        return s[0] == '-' ? math::negative : s[0] == '0' ? math::zero : math::positive;
    }
    
    string inline string::operator + (int64 x) const {
        return *this + string {x};
    }
    
    string inline string::operator - (int64 x) const {
        return *this - string {x};
    }
    
    string inline string::operator * (int64 x) const {
        return *this * string {x};
    }
    
    string inline &string::operator += (int64 x) {
        return *this += string {x};
    }
    
    string inline &string::operator -= (int64 x) {
        return *this += string {x};
    }
    
    string inline &string::operator *= (int64 x) {
        return *this += string {x};
    }
    
    string inline operator + (const string &n, const decimal::string &x) {
        return n + string {static_cast<const std::string> (x)};
    }
    
    string inline operator - (const string &n, const decimal::string &x) {
        return n - string {static_cast<const std::string> (x)};
    }
    
    string inline operator * (const string &n, const decimal::string &x) {
        return n * string {static_cast<const std::string> (x)};
    }
    
    string inline operator | (const string &n, const decimal::string &x) {
        return n | string {static_cast<const std::string> (x)};
    }
    
    string inline operator & (const string &n, const decimal::string &x) {
        return n & string {static_cast<const std::string> (x)};
    }
    
    string inline &operator += (string &n, const decimal::string &x) {
        return n += string {static_cast<const std::string> (x)};
    }
    
    string inline &operator -= (string &n, const decimal::string &x) {
        return n -= string {static_cast<const std::string> (x)};
    }
    
    string inline &operator *= (string &n, const decimal::string &x) {
        return n *= string {static_cast<const std::string> (x)};
    }
    
    string inline &operator |= (string &n, const decimal::string &x) {
        return n |= string {static_cast<const std::string> (x)};
    }
    
    string inline &operator &= (string &n, const decimal::string &x) {
        return n &= string {static_cast<const std::string> (x)};
    }

    string inline operator / (const string &v, const decimal::string &z) {
        return math::divide<string, decimal::string> {} (v, math::nonzero<decimal::string> {z}).Quotient;
    }

    string inline operator / (const string &v, const string &z) {
        return math::divide<string, string> {} (v, math::nonzero<string> {z}).Quotient;
    }

    decimal::string inline operator % (const string &v, const decimal::string &z) {
        return math::divide<string, decimal::string> {} (v, math::nonzero<decimal::string> {z}).Remainder;
    }
    
    string inline operator ++ (string &x, int) {
        auto n = x;
        ++x;
        return n;
    }
    
    string inline operator -- (string &x, int) {
        auto n = x;
        --x;
        return n;
    }
    
}

namespace data::encoding::hexidecimal {
    
    constexpr bool inline valid (string_view s) {
        return ctre::match<pattern> (s);
    } 
    
    constexpr bool inline zero (string_view s) {
        return ctre::match<zero_pattern> (s);
    }
    
    constexpr bool inline nonzero (string_view s) {
        return valid (s) && !ctre::match<zero_pattern> (s);
    }
    
    constexpr uint32 inline digits (string_view s) {
        return valid (s) ? s.size () - 2 : 0;
    }
        
    constexpr hex::letter_case inline read_case (string_view s) {
        return ctre::match<upper_case_pattern> (s) ? hex_case::upper : ctre::match<lower_case_pattern> (s) ? hex_case::lower : hex_case::unknown;
    }
    
    constexpr char inline digit (char x) {
        if (x >= '0' && x <= '9') return x - '0';
        if (x >= 'A' && x <= 'F') return x - 'A' + 10;
        if (x >= 'a' && x <= 'f') return x - 'a' + 10;
        return -1;
    }

    uint8_t inline hex_digit_to_value (char c) {
        if ('0' <= c && c <= '9') return c - '0';
        else if ('a' <= c && c <= 'f') return 10 + (c - 'a');
        else if ('A' <= c && c <= 'F') return 10 + (c - 'A');
        else throw std::invalid_argument("Invalid hex digit");
    }
    
    template <endian::order r, std::unsigned_integral word>
    maybe<oriented<r, word>> read (string_view s) {
        if (!valid (s)) return {};

        if ((((s.size () - 2) / 2) % sizeof (word)) != 0) return {};

        oriented<r, word> n;
        n.resize ((s.size () - 2) / (2 * sizeof (word)));

        auto x = n.words ().rbegin ();
        auto y = s.begin () + 2;
        while (x != n.words ().rend ()) {
            *x = 0;
            for (int i = sizeof (word) - 1; i >= 0; i--) {
                *x |= static_cast<word> ((hex_digit_to_value (*y) << 4) | (hex_digit_to_value (*(y + 1)))) << (i * 8);
                y += 2;
            }

            x++;
        }

        return n;
    }
    
    template <endian::order r, std::unsigned_integral word>
    std::ostream inline &write (std::ostream &o, const oriented<r, word> &d, hex::letter_case q) {
        o << "0x";
        return encoding::hex::write (o, d.words ().reverse (), q);
    }
    
    template <hex::letter_case cx, endian::order r, std::unsigned_integral word>
    string<cx> write (const oriented<r, word> &z) {
        std::stringstream ss;
        write (ss, z, cx);
        return string<cx> {ss.str ()};
    }

    template <negativity cl, negativity cr, hex::letter_case cx>
    bool inline operator == (const integer<cl, cx> &a, const integer<cr, cx> &b) {
        return (a <=> b) == 0;
    }

    template <negativity c, hex::letter_case cx>
    std::weak_ordering inline operator <=> (const integer<negativity::nones, cx> &a, const integer<c, cx> &b) {
        return integer<c, cx> (a) <=> b;
    }

    template <negativity c, hex::letter_case cx>
    std::weak_ordering inline operator <=> (const integer<c, cx> &a, const integer<negativity::nones, cx> &b) {
        return a <=> integer<c, cx> (b);
    }

    template <negativity cl, negativity cr, hex::letter_case cx>
    std::weak_ordering inline operator <=> (const integer<cl, cx> &a, const integer<cr, cx> &b) {
        return a <=> integer<cl, cx> (b);
    }

    template <hex::letter_case cx>
    bool inline operator == (const integer<negativity::nones, cx> &x, uint64 u) {
        return x == integer<negativity::nones, cx> {u};
    }

    template <hex::letter_case cx>
    bool inline operator == (const integer<negativity::twos, cx> &x, int64 u) {
        return x == integer<negativity::twos, cx> {u};
    }

    template <hex::letter_case cx>
    bool inline operator == (const integer<negativity::BC, cx> &x, int64 u) {
        return x == integer<negativity::BC, cx> {u};
    }

    template <hex::letter_case cx>
    std::weak_ordering inline operator <=> (const integer<negativity::nones, cx> &x, uint64 u) {
        return x <=> integer<negativity::nones, cx> {u};
    }

    template <hex::letter_case cx>
    std::weak_ordering inline operator <=> (const integer<negativity::twos, cx> &x, int64 u) {
        return x <=> integer<negativity::twos, cx> {u};
    }

    template <hex::letter_case cx>
    std::weak_ordering inline operator <=> (const integer<negativity::BC, cx> &x, int64 u) {
        return x <=> integer<negativity::BC, cx> {u};
    }
    
    template <hex::letter_case cx> 
    integer<negativity::twos, cx> inline operator +
        (const integer<negativity::twos, cx> &n, const integer<negativity::nones, cx> &m) {
        return n + integer<negativity::twos, cx> {math::number::extend (m, m.size () + 2)};
    }
    
    template <hex::letter_case cx> 
    integer<negativity::twos, cx> inline operator -
        (const integer<negativity::twos, cx> &n, const integer<negativity::nones, cx> &m) {
        return n - integer<negativity::twos, cx> {math::number::extend (m, m.size () + 2)};
    }
    
    template <hex::letter_case cx> 
    integer<negativity::twos, cx> inline operator *
        (const integer<negativity::twos, cx> &n, const integer<negativity::nones, cx> &m) {
        return n * integer<negativity::twos, cx> {math::number::extend (m, m.size () + 2)};
    }
    
    template <hex::letter_case cx> 
    integer<negativity::twos, cx> inline operator |
        (const integer<negativity::twos, cx> &n, const integer<negativity::nones, cx> &m) {
        return n | integer<negativity::twos, cx> {math::number::extend (m, m.size () + 1)};
    }
    
    template <hex::letter_case cx> 
    integer<negativity::twos, cx> inline operator &
        (const integer<negativity::twos, cx> &n, const integer<negativity::nones, cx> &m) {
        return n & integer<negativity::twos, cx> {math::number::extend (m, m.size () + 1)};
    }
    
    template <hex::letter_case cx> 
    integer<negativity::twos, cx> inline operator /
        (const integer<negativity::twos, cx> &n, const integer<negativity::nones, cx> &m) {
        return n / integer<negativity::twos, cx> {math::number::extend (m, m.size () + 1)};
    }
    
    template <hex::letter_case cx> 
    integer<negativity::twos, cx> inline &operator +=
        (integer<negativity::twos, cx> &n, const integer<negativity::nones, cx> &m) {
        return n += integer<negativity::twos, cx> {math::number::extend (m, m.size () + 1)};
    }
    
    template <hex::letter_case cx> 
    integer<negativity::twos, cx> inline &operator -=
        (integer<negativity::twos, cx> &n, const integer<negativity::nones, cx> &m) {
        return n -= integer<negativity::twos, cx> {math::number::extend (m, m.size () + 1)};
    }
    
    template <hex::letter_case cx> 
    integer<negativity::twos, cx> inline &operator *=
        (integer<negativity::twos, cx> &n, const integer<negativity::nones, cx> &m) {
        return n *= integer<negativity::twos, cx> {math::number::extend (m, m.size () + 1)};
    }
    
    template <hex::letter_case cx> 
    integer<negativity::twos, cx> inline &operator |=
        (integer<negativity::twos, cx> &n, const integer<negativity::nones, cx> &m) {
        return n |= integer<negativity::twos, cx> {math::number::extend (m, m.size () + 1)};
    }
    
    template <hex::letter_case cx> 
    integer<negativity::twos, cx> inline &operator &=
        (integer<negativity::twos, cx> &n, const integer<negativity::nones, cx> &m) {
        return n &= integer<negativity::twos, cx> {math::number::extend (m, m.size () + 1)};
    }

    template <hex::letter_case cx> integer<negativity::nones, cx> inline
    &operator %= (integer<negativity::nones, cx> &n, const integer<negativity::nones, cx> &x) {
        return n = n % x;
    }

    template <negativity c, hex::letter_case cx>
    integer<c, cx> inline operator | (const integer<c, cx> &x, uint64 u) {
        return x | integer<c, cx> (integer<negativity::nones, cx> {u});
    }

    template <negativity c, hex::letter_case cx>
    integer<c, cx> inline operator & (const integer<c, cx> &x, uint64 u) {
        return x & integer<c, cx> (integer<negativity::nones, cx> {u});
    }

    template <hex::letter_case cx>
    integer<negativity::twos, cx> inline operator - (const integer<negativity::nones, cx> &x) {
        return -integer<negativity::twos, cx> (x);
    }
    
} 

namespace data::encoding::natural {
    
    constexpr bool inline valid (string_view s) {
        return ctre::match<pattern> (s);
    } 
    
    constexpr bool inline zero (string_view s) {
        return ctre::match<zero_pattern> (s);
    }
    
    constexpr bool inline nonzero (string_view s) {
        return valid (s) && ! ctre::match<zero_pattern> (s);
    }
    
    constexpr uint32 inline digits (string_view s) {
        return std::max (decimal::digits (s), hexidecimal::digits (s));
    }
    
}

namespace data::encoding::integer {
    
    constexpr bool inline valid (string_view s) {
        return ctre::match<pattern> (s);
    } 
    
    constexpr bool inline negative (string_view s) {
        return ctre::match<negative_pattern> (s);
    }
    
    constexpr bool inline zero (string_view s) {
        return ctre::match<zero_pattern> (s);
    }
    
    constexpr bool inline nonzero (string_view s) {
        return valid (s) && ! ctre::match<zero_pattern> (s);
    }
    
    constexpr uint32 inline digits (string_view s) {
        return negative (s) ? natural::digits (s.substr (1, s.size () - 1)) : natural::digits (s);
    }
    
    template <endian::order r, math::negativity c, std::unsigned_integral word>
    maybe<math::number::Z_bytes<r, c, word>> read (string_view s) {

        if (!valid (s)) return {};
        
        if (hexidecimal::valid (s)) {
            auto z = hexidecimal::read<r, word> (s);
            if (!z) return {};
            auto x = math::number::Z_bytes<r, c, word> {};
            x.resize (z->size ());
            std::copy (z->begin (), z->end (), x.begin ());
            return {x};
        }
        
        auto z = signed_decimal::read<r, c, word> (s);
        if (!z) return {};
        return {math::number::Z_bytes<r, c, word> (*z)};
    }
    
}

namespace data::math::number {

    nonzero<dec_uint> inline increment<dec_uint>::operator () (const dec_uint &n) {
        return nonzero<dec_uint> {n + 1};
    }

    dec_uint inline decrement<dec_uint>::operator () (const nonzero<dec_uint> &n) {
        auto x = n.Value;
        return --x;
    }

    dec_uint inline decrement<dec_uint>::operator () (const dec_uint &n) {
        if (data::is_zero (n)) return n;
        auto x = n;
        return --x;
    }

    dec_int inline increment<dec_int>::operator () (const dec_int &n) {
        auto x = n;
        return ++x;
    }

    dec_int inline decrement<dec_int>::operator () (const dec_int &n) {
        auto x = n;
        return --x;
    }

    template <hex_case zz>
    nonzero<hex::uint<zz>> inline increment<hex::uint<zz>>::operator () (const hex::uint<zz> &n) {
        return nonzero<hex::uint<zz>> {n + 1};
    }

    template <hex_case zz>
    hex::uint<zz> inline decrement<hex::uint<zz>>::operator () (const nonzero<hex::uint<zz>> &n) {
        auto x = n.Value;
        return --x;
    }

    template <hex_case zz>
    hex::uint<zz> inline decrement<hex::uint<zz>>::operator () (const hex::uint<zz> &n) {
        if (data::is_zero (n)) return n;
        auto x = n;
        return --x;
    }

    template <math::negativity c, hex_case zz>
    hex::integer<c, zz> inline increment<hex::integer<c, zz>>::operator () (const hex::integer<c, zz> &n) {
        auto x = n;
        return ++x;
    }

    template <math::negativity c, hex_case zz>
    hex::integer<c, zz> inline decrement<hex::integer<c, zz>>::operator () (const hex::integer<c, zz> &n) {
        auto x = n;
        return --x;
    }
    
    template <hex_case cx> 
    bool inline is_minimal (const hex::uint<cx> &x) {
        if (x.size () < 4) return true;
        return x[2] != '0' || x[3] != '0';
    }
}

namespace data {

    template<math::negativity a, hex_case b, math::negativity c, hex_case d>
    bool inline identical (const hex::integer<a, b> &x, const hex::integer<c, d> &y) {
        return static_cast<std::string> (x) == static_cast<std::string> (y);
    }
}

namespace data::math {

    signature inline sign<dec_uint>::operator () (const dec_uint &n) {
        if (!encoding::decimal::valid (n)) throw exception {} << "invalid decimal string: " << n;

        return encoding::decimal::nonzero (n) ? math::positive : math::zero;
    }

    signature inline sign<dec_int>::operator () (const dec_int &n) {
        return encoding::signed_decimal::sign (n);
    }

    template <hex_case cx>
    signature inline sign<hex::uint<cx>>::operator () (const hex::uint<cx> &x) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;

        return data::is_zero (x) ? math::zero : math::positive;
    }

    template <hex_case cx>
    signature inline sign<hex::int2<cx>>::operator () (const hex::int2<cx> &x) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;

        return data::is_negative (x) ? math::negative : data::is_zero (x) ? math::zero : math::positive;
    }

    template <hex_case cx>
    math::signature inline sign<hex::intBC<cx>>::operator () (const hex::intBC<cx> &x) {
        if (!x.valid ()) throw exception{} << "invalid hexidecimal string: " << x;

        return data::is_zero (x) ? math::zero : math::number::sign_bit_set (x) ? math::negative : math::positive;
    }
    
    bool inline is_zero<dec_uint>::operator () (const dec_uint &n) {
        if (!encoding::decimal::valid (n)) throw exception {} << "invalid decimal string: " << n;
        return !encoding::decimal::nonzero (n);
    }
    
    bool inline is_negative<dec_uint>::operator () (const dec_uint &n) {
        if (!encoding::decimal::valid (n)) throw exception {} << "invalid decimal string: " << n;
        return false;
    }
    
    bool inline is_positive<dec_uint>::operator () (const dec_uint &n) {
        if (!encoding::decimal::valid (n)) throw exception {} << "invalid decimal string: " << n;
        return encoding::decimal::nonzero (n);
    }
    
    bool inline is_zero<dec_int>::operator () (const dec_int &n) {
        if (!encoding::signed_decimal::valid (n)) throw exception {} << "invalid decimal string: " << n;
        return !encoding::signed_decimal::nonzero (n);
    }
    
    bool inline is_negative<dec_int>::operator () (const dec_int &n) {
        if (!encoding::signed_decimal::valid (n)) throw exception {} << "invalid decimal string: " << n;
        return encoding::signed_decimal::negative (n);
    }
    
    bool inline is_positive<dec_int>::operator () (const dec_int &n) {
        if (!encoding::signed_decimal::valid (n)) throw exception {} << "invalid decimal string: " << n;
        return encoding::signed_decimal::positive (n);
    }

    template <negativity c, hex_case zz>
    bool inline is_positive<hex::integer<c, zz>>::operator () (const hex::integer<c, zz> &z) {
        return !is_negative<hex::integer<c, zz>> {} (z) && !is_zero<hex::integer<c, zz>> {} (z);
    }
    
    template <hex_case cx> 
    bool inline is_zero<hex::uint<cx>>::operator () (const hex::uint<cx> &z) {
        if (!z.valid ()) throw exception {} << "invalid hex integer: " << z;
        
        for (auto digit = z.begin () + 2; digit != z.end (); digit++) if (*digit != '0') return false;
        return true;
    }
    
    template <hex_case cx> 
    bool inline is_zero<hex::int2<cx>>::operator () (const hex::int2<cx> &z) {
        if (!z.valid ()) throw exception {} << "invalid hex integer: " << z;
        
        for (auto digit = z.begin () + 2; digit != z.end (); digit++) if (*digit != '0') return false;
        return true;
    }
    
    template <hex_case cx> 
    bool inline is_negative<hex::uint<cx>>::operator () (const hex::uint<cx> &) {
        return false;
    }
    
    template <hex_case cx> 
    bool inline is_negative<hex::int2<cx>>::operator () (const hex::int2<cx> &x) {
        if (!x.valid ()) throw exception {} << "invalid hex integer: " << x;
        
        if (x.size () < 3) return false;
        return encoding::hexidecimal::digit (x[2]) > 7;
    }
    
    dec_uint inline abs<dec_uint>::operator () (const dec_uint &x) {
        if (!x.valid ()) throw exception {} << "invalid dec string: " << x;
        return x;
    }
    
    dec_uint inline abs<dec_int>::operator () (const dec_int &x) {
        if (is_negative<dec_int> {} (x)) return dec_uint {string_view (x).substr (1)};
        return dec_uint {x};
    }
    
    template <hex_case zz>
    hex::uint<zz> inline abs<hex::uint<zz>>::operator () (const hex::uint<zz> &x) {
        if (!x.valid ()) throw exception {} << "invalid hex string: " << x;
        return x;
    }
    
    template <hex_case zz>
    hex::uint<zz> inline abs<hex::int2<zz>>::operator () (const hex::int2<zz> &x) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        if (data::is_negative (x)) return hex::uint<zz> {-x};
        return hex::uint<zz> {x};
    }
    
    template <hex_case zz>
    hex::intBC<zz> inline abs<hex::intBC<zz>>::operator () (const hex::intBC<zz> &x) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        return data::is_negative (x) ? -x : x;
    }

    template <> struct identity<plus<dec_uint>, dec_uint> {
        dec_uint operator () () {
            return 0;
        }
    };

    template <> struct identity<plus<dec_int>, dec_int> {
        dec_int operator () () {
            return 0;
        }
    };

    template <> struct identity<times<dec_uint>, dec_uint> {
        dec_uint operator () () {
            return 1;
        }
    };

    template <> struct identity<times<dec_int>, dec_int> {
        dec_int operator () () {
            return 1;
        }
    };

    template <negativity c, hex_case zz>
    struct identity<plus<hex::integer<c, zz>>, hex::integer<c, zz>> {
        hex::integer<c, zz> operator () () {
            return 0;
        }
    };

    template <negativity c, hex_case zz>
    struct identity<times<hex::integer<c, zz>>, hex::integer<c, zz>> {
        hex::integer<c, zz> operator () () {
            return 1;
        }
    };

    template <> struct inverse<plus<dec_int>, dec_int> {
        dec_int operator () (const dec_int &a, const dec_int &b) {
            return b - a;
        }
    };

    template <hex_case zz>
    struct inverse<plus<hex::int2<zz>>, hex::int2<zz>> {
        hex::int2<zz> operator () (const hex::int2<zz> &a, const hex::int2<zz> &b) {
            return b - a;
        }
    };

    template <hex_case zz>
    struct inverse<plus<hex::intBC<zz>>, hex::intBC<zz>> {
        hex::intBC<zz> operator () (const hex::intBC<zz> &a, const hex::intBC<zz> &b) {
            return b - a;
        }
    };

}


namespace data::encoding::hexidecimal {
    
    template <negativity c, hex::letter_case cx>
    integer<c, cx> inline operator ++ (integer<c, cx> &x, int) {
        integer n = x;
        ++x;
        return n;
    }
    
    template <negativity c, hex::letter_case cx>
    integer<c, cx> inline operator -- (integer<c, cx> &x, int) {
        integer n = x;
        --x;
        return n;
    }
    
    template <hex::letter_case cx>
    integer<negativity::twos, cx> inline operator - (const integer<negativity::twos, cx> &n) {
        if (!n.valid ()) throw exception {} << "invalid hex string: " << n;
        auto x = ~n;
        return ++x;
    }
    
    template <negativity c, hex::letter_case zz>
    integer<c, zz> inline &integer<c, zz>::operator += (const integer &i) {
        return *this = *this + i;
    }
    
    template <negativity c, hex::letter_case zz>
    integer<c, zz> inline &integer<c, zz>::operator -= (const integer &i) {
        return *this = *this - i;
    }
    
    template <negativity c, hex::letter_case zz>
    integer<c, zz> inline &integer<c, zz>::operator *= (const integer &i) {
        return *this = *this * i;
    }

    template <negativity c, hex::letter_case zz>
    integer<c, zz> inline complemented_string<c, zz>::operator + (int64 i) const {
        return integer<c, zz> {*this} + integer<c, zz> {i};
    }
    
    template <negativity c, hex::letter_case zz>
    integer<c, zz> inline complemented_string<c, zz>::operator - (int64 i) const {
        return integer<c, zz> {*this} - integer<c, zz> {i};
    }

    template <negativity c, hex::letter_case zz>
    integer<c, zz> inline complemented_string<c, zz>::operator * (int64 i) const {
        return integer<c, zz> {*this} * integer<c, zz> {i};
    }

    template <hex::letter_case cx>
    data::hex::uint<cx> inline complemented_string<negativity::nones, cx>::operator + (uint64 i) const {
        return data::hex::uint<cx> {*this} + data::hex::uint<cx> {i};
    }

    template <hex::letter_case cx>
    data::hex::uint<cx> inline complemented_string<negativity::nones, cx>::operator - (uint64 i) const {
        return data::hex::uint<cx> {*this} - data::hex::uint<cx> {i};
    }

    template <hex::letter_case cx>
    data::hex::uint<cx> inline complemented_string<negativity::nones, cx>::operator * (uint64 i) const {
        return data::hex::uint<cx> {*this} * data::hex::uint<cx> {i};
    }
    
    template <negativity c, hex::letter_case zz>
    integer<c, zz> inline &integer<c, zz>::operator <<= (int i) {
        return *this = *this << i;
    }
    
    template <negativity c, hex::letter_case zz>
    integer<c, zz> inline &integer<c, zz>::operator >>= (int i) {
        return *this = *this >> i;
    }
        
    template <negativity c, hex::letter_case zz>
    integer<c, zz> inline integer<c, zz>::operator / (const integer &x) const {
        return math::divide<integer<c, zz>> {} (*this, math::nonzero {x}).Quotient;
    }
    
    template <negativity c, hex::letter_case zz>
    integer<c, zz> inline integer<c, zz>::operator % (const integer &x) const {
        return math::divide<integer<c, zz>> {} (*this, math::nonzero {x}).Remainder;
    }
    
    namespace {
        // NOTE: we write a built-in type to a hex string number by converting to
        // an endian::integral type, writing with extra zeros, possibly negating it
        // and then trimming the result. This would be much more efficient if we
        // just figured out how big the number would be beforehand. Do we really
        // need to bother making this work better though? I don't think so.
        template <hex::letter_case cx, bool is_signed, size_t size>
        string<cx> write_arith (const endian::integral<is_signed, endian::big, size> &z) {
            std::stringstream ss;
            if constexpr (is_signed) {
                if (z < 0) ss << "0xff";
                else ss << "0x00";
            } else {
                ss << "0x00";
            }
            hex::write (ss, z, cx);
            return string<cx> {ss.str ()};
        }
        
        template <negativity c, hex::letter_case zz> struct write_int;
        
        template <hex::letter_case zz> struct write_int<negativity::nones, zz> {
            template <std::signed_integral I>
            integer<negativity::nones, zz> operator () (I i) {
                if (i < 0) throw exception {} << "attempt to construct natural number from negative int " << i;
                return operator () (static_cast<std::make_unsigned_t<I>> (i));
            }

            template <std::unsigned_integral I>
            integer<negativity::nones, zz> operator () (I i) {
                return integer<negativity::nones, zz>
                    {write_arith<zz, false, sizeof (I)>
                        (endian::integral<false, endian::big, sizeof (I)> {i})};
            }
        };
        
        template <hex::letter_case zz> struct write_int<negativity::twos, zz> {
            template <std::signed_integral I>
            integer<negativity::twos, zz> operator () (I i) {
                return integer<negativity::twos, zz>
                    {write_arith<zz, true, sizeof (I)>
                        (endian::integral<true, endian::big, sizeof (I)> {i})};
            }

            template <std::unsigned_integral I>
            integer<negativity::twos, zz> operator () (I i) {
                return integer<negativity::twos, zz>
                {write_arith<zz, false, sizeof (I)>
                    (endian::integral<false, endian::big, sizeof (I)> {i})};
            }
        };
        
        template <hex::letter_case zz> struct write_int<negativity::BC, zz> {
            template <std::signed_integral I>
            integer<negativity::BC, zz> operator () (I i) {
                using U = std::make_unsigned_t<I>;
                if (i == std::numeric_limits<int64>::min ())
                    return -operator () (static_cast<U> (i));
                return i < 0 ?
                    -operator () (static_cast<U> (-i)) :
                    operator () (static_cast<U> (i));
            }
            
            template <std::unsigned_integral I>
            integer<negativity::BC, zz> operator () (I i) {
                return integer<negativity::BC, zz>
                    {write_arith<zz, false, sizeof (I)>
                        (endian::integral<false, endian::big, sizeof (I)> {i})};
            } 
        };

        template <hex::letter_case zz, negativity from, negativity to> struct cast_negativity;

        template <hex::letter_case zz> struct cast_negativity<zz, negativity::twos, negativity::BC> {
            integer<negativity::BC, zz> operator () (const integer<negativity::twos, zz> &x) {
                return is_negative (x) ?
                    -integer<negativity::BC, zz> (static_cast<string<zz>> (-x)):
                    integer<negativity::BC, zz> (static_cast<string<zz>> (x));
            }
        };

        template <hex::letter_case zz> struct cast_negativity<zz, negativity::BC, negativity::twos> {
            integer<negativity::twos, zz> operator () (const integer<negativity::BC, zz> &x) {
                return math::number::sign_bit_set (x) ?
                    -integer<negativity::twos, zz> (static_cast<string<zz>> (-x)):
                    integer<negativity::twos, zz> (static_cast<string<zz>> (x));
            }
        };
    }

    template <negativity c, hex::letter_case zz>
    template <std::integral I>
    inline complemented_string<c, zz>::complemented_string (I x): complemented_string {write_int<c, zz> {} (x)} {
        static_cast<string<zz> &> (*this) = trim<c, zz> (*this);
    }

    template <hex::letter_case zz>
    template <std::integral I>
    inline complemented_string<negativity::nones, zz>::complemented_string (I x):
        complemented_string {write_int<negativity::nones, zz> {} (x)} {
        static_cast<string<zz> &> (*this) = trim<negativity::nones, zz> (*this);
    }
    
    template <negativity c, hex::letter_case cx>
    complemented_string<c, cx>::operator integer<negativity (-int (c) + 5), cx> () const {
        return math::number::trim (cast_negativity<cx, c, negativity (-int (c) + 5)> {} (integer<c, cx> (*this)));
    }

    template <hex::letter_case cx>
    inline complemented_string<negativity::nones, cx>::operator integer<negativity::twos, cx> () const {
        return is_negative<negativity::twos, cx> (*this) ?
            integer<negativity::twos, cx> (extend<negativity::nones, cx> (*this, this->size () + 2)):
            integer<negativity::twos, cx> (static_cast<string<cx>> (*this));
    }

    template <hex::letter_case cx>
    inline complemented_string<negativity::nones, cx>::operator integer<negativity::BC, cx> () const {
        return is_negative<negativity::BC, cx> (*this) ?
            integer<negativity::BC, cx> (extend<negativity::nones, cx> (*this, this->size () + 2)):
            integer<negativity::BC, cx> (static_cast<string<cx>> (*this));
    }

    template <negativity c, hex::letter_case cx>
    integer<c, cx> complemented_string<c, cx>::zero (size_t size, bool negative) {
        if constexpr (c == negativity::twos) {
            if (negative) throw exception {} << "negativity ones has no negative zero";
        } else {
            if (negative && size == 0) throw exception {} << "can't do negative zero with size zero";
        }

        std::string x {};
        x.resize (2 + size * 2);
        x[0] = '0';
        x[1] = 'x';

        if (size > 0) {
            x[2] = negative ? '8' : '0';
            for (auto i = x.begin () + 3; i != x.end (); i++) *i = '0';
        }

        return integer<c, cx> {x};
    }

    template <negativity c, hex::letter_case zz>
    integer<c, zz> inline &integer<c, zz>::trim () {
        return *this = math::number::trim (*this);
    }
    
    template <negativity c, hex::letter_case zz>
    integer<c, zz> inline integer<c, zz>::trim () const {
        return math::number::trim (*this);
    }
    
}

namespace data::math::number {
    
    template <hex_case zz> hex::uint<zz> trim (const hex::uint<zz> &x) {
        
        if (!x.valid ()) throw exception {} << "cannot trim invalid hexidecimal string: " << x;
        
        if (is_minimal (x)) return x;
        auto i = x.begin () + 2;
        while (i != x.end () && i[0] == '0' && i[1] == '0') i += 2;
        
        hex::uint<zz> n {};
        n.resize (x.end () - i + 2);
        std::copy (i, x.end (), n.begin () + 2);
        return n;
    }
    
    template <hex_case cx> hex::int2<cx> trim (const hex::int2<cx> &x) {

        if (!x.valid ()) throw exception {} << "cannot trim invalid hexidecimal string: " << x;
        
        size_t min_size = minimal_size (x);
        
        if (min_size == x.size ()) return x;
        
        hex::int2<cx> n {};
        n.resize (min_size);
        std::copy (x.end () - min_size + 2, x.end (), n.begin () + 2);
        return n;

    }
    
    template <hex_case cx> hex::intBC<cx> trim (const hex::intBC<cx> &x) {
        
        if (!x.valid ()) throw exception {} << "cannot trim invalid hexidecimal string: " << x;
        if (is_minimal (x)) return x;

        hex::intBC<cx> n {};
        if (data::is_zero (x)) return n;
        
        auto i = x.begin () + 4;
        while (i != x.end () && i[0] == '0' && i[1] == '0') i += 2;
        
        if (i != x.end () && encoding::hexidecimal::digit (i[0]) >= 8) {
            n.resize (x.end () - i + 4);
            n[2] = (x)[2];
            n[3] = '0';
        } else {
            n.resize (x.end () - i + 2);
            n[2] = encoding::hex::characters (cx)[encoding::hexidecimal::digit (i[0]) + (x[2] == '8' ? 8 : 0)];
            n[3] = i[1];
            i += 2;
        }
        
        std::copy (i, x.end (), n.begin () + 4);
        return n;
    }
}

namespace data::encoding::hexidecimal {

    template <hex::letter_case zz>
    integer<negativity::BC, zz> operator ! (const integer<negativity::BC, zz> &x) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        return bool (x) ?
            integer<negativity::BC, zz> {std::string {"0x"}} :
            integer<negativity::BC, zz> {std::string {"0x01"}};
    }

    template <hex::letter_case zz>
    integer<negativity::BC, zz> operator &&
        (const integer<negativity::BC, zz> &x, const integer<negativity::BC, zz> &y) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        if (!y.valid ()) throw exception {} << "invalid hexidecimal string: " << y;
        return bool (x) && bool (y) ?
            integer<negativity::BC, zz> {std::string {"0x01"}} :
            integer<negativity::BC, zz> {std::string {"0x"}};
    }

    template <hex::letter_case zz>
    integer<negativity::BC, zz> operator ||
        (const integer<negativity::BC, zz> &x, const integer<negativity::BC, zz> &y) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        if (!y.valid ()) throw exception {} << "invalid hexidecimal string: " << y;
        return bool (x) || bool (y) ?
            integer<negativity::BC, zz> {std::string {"0x01"}} :
            integer<negativity::BC, zz> {std::string {"0x"}};
    }
    
    template <hex::letter_case zz>
    integer<negativity::twos, zz> operator ~ (const integer<negativity::twos, zz> &x) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        if (x == std::string {"0x"}) return integer<negativity::twos, zz> {std::string {"0xff"}};
        
        auto characters = hex::characters (zz);
        integer<negativity::twos, zz> n {};
        n.resize (x.size ());
        for (int i = 2; i < x.size (); i++) n[i] = characters[0x0f & ~digit (x[i])];
        
        return n.trim ();
    }
    
    template <hex::letter_case zz> 
    integer<negativity::BC, zz> operator - (const integer<negativity::BC, zz> &x) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        if (data::is_zero (x)) return integer<negativity::BC, zz> {"0x"};

        integer<negativity::BC, zz> n = x;
        auto d = digit (x[2]);
        n[2] = hex::characters (zz)[(d & 0x7) | (~d & 0x8)];

        return n.trim ();
    }
    
    namespace {
    
        std::weak_ordering N_compare_same_size (string_view a, string_view b) {
            for (int i = 0; i < a.size (); i++)
                if (digit (a[i]) > digit (b[i])) return std::weak_ordering::greater;
                else if (digit (a[i]) < digit (b[i])) return std::weak_ordering::less;
            
            return std::weak_ordering::equivalent;
        }
        
        std::weak_ordering N_compare (string_view a, string_view b) {
            if (a.size () < b.size ()) return 0 <=> N_compare (b, a);
            size_t size_difference = a.size () - b.size ();
            for (int i = 0; i < size_difference; i++) if (a[i] != '0') return std::weak_ordering::greater; 
            return N_compare_same_size (a.substr (size_difference, a.size ()), b);
        }
        
        template <hex::letter_case zz>
        char N_increment (string<zz> &x) {
            auto characters = hex::characters (zz);
            
            auto i = x.rbegin ();
            auto e = x.rbegin () + x.size () - 2;
            char remainder = 1;
            while (i != e) {
                auto d = digit (*i) + remainder;
                *i = characters[d % 16];
                remainder = d >> 4;
                if (remainder == 0) return '0';
                i++;
            }
            return characters[remainder];
        }
        
        template <hex::letter_case zz>
        void N_decrement (string<zz> &x) {
            auto characters = hex::characters (zz);
            
            auto i = x.rbegin ();
            auto e = x.rbegin () + x.size () - 2;
            while (i != e) {
                auto d = digit (*i);
                if (d != 0) {
                    *i = characters[d - 1];
                    break;
                }
                *i = characters[15];
                i++;
            }
        }
        
        // these strings will all be the same size. 
        template <hex::letter_case zz> void bit_and (string<zz> &out, const string<zz> &a, const string<zz> &b) {
            
            std::vector<byte> out_d ((out.size () - 2) >> 1);
            std::vector<byte> a_d ((out.size () - 2) >> 1);
            std::vector<byte> b_d ((out.size () - 2) >> 1);
            
            boost::algorithm::unhex (a.begin () + 2, a.end (), a_d.begin ());
            boost::algorithm::unhex (b.begin () + 2, b.end (), b_d.begin ());
            
            arithmetic::bit_and<byte> (out_d.end (), out_d.begin (), a_d.begin (), b_d.begin ());
            
            if (zz == hex_case::lower) boost::algorithm::hex_lower (out_d.begin (), out_d.end (), out.begin () + 2);
            else boost::algorithm::hex (out_d.begin (), out_d.end (), out.begin () + 2);

        }
        
        template <hex::letter_case zz>
        void bit_or (string<zz> &out, const string<zz> &a, const string<zz> &b) {
            
            std::vector<byte> out_d ((out.size() - 2) >> 1);
            std::vector<byte> a_d ((out.size () - 2) >> 1);
            std::vector<byte> b_d ((out.size () - 2) >> 1);
            
            boost::algorithm::unhex (a.begin () + 2, a.end (), a_d.begin ());
            boost::algorithm::unhex (b.begin () + 2, b.end (), b_d.begin ());
            
            arithmetic::bit_or<byte> (out_d.end (), out_d.begin (), a_d.begin (), b_d.begin ());
            
            if (zz == hex_case::lower) boost::algorithm::hex_lower (out_d.begin (), out_d.end (), out.begin() + 2);
            else boost::algorithm::hex (out_d.begin (), out_d.end (), out.begin () + 2);

        }
        
        template <hex::letter_case zz>
        void bit_xor (string<zz> &out, const string<zz> &a, const string<zz> &b) {
            
            std::vector<byte> out_d ((out.size () - 2) >> 1);
            std::vector<byte> a_d ((out.size () - 2) >> 1);
            std::vector<byte> b_d ((out.size () - 2) >> 1);
            
            boost::algorithm::unhex (a.begin () + 2, a.end (), a_d.begin ());
            boost::algorithm::unhex (b.begin () + 2, b.end (), b_d.begin ());
            
            arithmetic::bit_xor<byte> (out_d.end (), out_d.begin (), a_d.begin (), b_d.begin ());
            
            if (zz == hex_case::lower) boost::algorithm::hex_lower (out_d.begin (), out_d.end (), out.begin () + 2);
            else boost::algorithm::hex (out_d.begin (), out_d.end (), out.begin () + 2);

        }
        
        // the out string will always be 2 characters longer than the other two. 
        template <hex::letter_case zz>
        void plus (string<zz> &out, const string<zz> &a, const string<zz> &b) {
            auto characters = hex::characters (zz);
            
            auto ai = a.rbegin ();
            auto ae = a.rbegin () + a.size () - 2;
            
            auto bi = b.rbegin ();
            
            auto oi = out.rbegin ();
            auto oe = out.rbegin () + out.size () - 2;
            
            int remainder = 0;
            
            while (ai != ae) {
                auto d = int (digit (*ai)) + int (digit (*bi)) + remainder;
                *oi = characters[d % 16];
                remainder = d >> 4;
                
                ai++;
                bi++;
                oi++;
            }
            
            while (oi != oe) {
                *oi = characters[remainder % 16];
                remainder = remainder >> 4;
                oi++;
            }
        }
        
        // the out string will always be 2 characters longer than the other two. 
        template <hex::letter_case zz>
        void minus (string<zz> &out, const string<zz> &a, const string<zz> &b) {
            auto characters = hex::characters (zz);
            
            auto ai = a.rbegin ();
            auto ae = a.rbegin () + a.size () - 2;
            
            auto bi = b.rbegin ();
            
            auto oi = out.rbegin ();
            auto oe = out.rbegin () + out.size () - 2;
            
            int remainder = 0;
            
            while (ai != ae) {
                auto d = int (digit (*ai)) - int (digit (*bi)) + remainder;
                *oi = characters[(d + 16) % 16];
                remainder = d >> 4;
                
                ai++;
                bi++;
                oi++;
            }
            
            while (oi != oe) {
                *oi = characters[(remainder + 16) % 16];
                remainder = remainder >> 4;
                oi++;
            }
        }
        
        // the out string will always be the size of the sum of the two inputs, which won't necessarily be equal size. 
        template <hex::letter_case zz>
        void times (string<zz> &out, const string<zz> &a, const string<zz> &b);
        
        template <negativity c, hex::letter_case zz>
        integer<c, zz> bit_and (const integer<c, zz> &a, const integer<c, zz> &b) {
            if (a.size () < b.size ()) return bit_and (b, a);
            integer<c, zz> n {};
            n.resize (a.size ());
            bit_and (n, a, math::number::extend (b, a.size ()));
            return n;
        }
        
        template <negativity c, hex::letter_case zz>
        integer<c, zz> bit_or(const integer<c, zz> &a, const integer<c, zz> &b) {
            if (a.size () < b.size ()) return bit_or (b, a);
            integer<c, zz> n {};
            n.resize (a.size ());
            bit_or (n, a, math::number::extend (b, a.size ()));
            return n;
        }
        
        template <hex::letter_case zz> 
        integer<negativity::twos, zz> bit_xor (const integer<negativity::twos, zz> &a, const integer<negativity::twos, zz> &b) {
            if (a.size () < b.size ()) return bit_xor (b, a);
            integer<negativity::twos, zz> n {};
            n.resize (a.size ());
            bit_xor (n, a, math::number::extend (b, a.size ()));
            return n;
        }
        
        template <hex::letter_case zz> 
        string<zz> shift (const string<zz> &x, int i);
        
        template <hex::letter_case zz> 
        integer<negativity::nones, zz> inline bit_shift (const integer<negativity::nones, zz> &x, int i) {
            return integer<negativity::nones, zz> {shift (x, i)};
        }
        
        template <hex::letter_case zz> 
        integer<negativity::twos, zz> bit_shift (const integer<negativity::twos, zz> &x, int i);
        
        template <hex::letter_case zz> 
        integer<negativity::BC, zz> inline bit_shift (const integer<negativity::BC, zz> &x, int i) {
            return integer<negativity::BC, zz> (bit_shift (integer<negativity::twos, zz> (x), i));
        }
        
        template <negativity c, hex::letter_case zz> struct add;
        
        template <hex::letter_case zz> struct add<negativity::nones, zz> {
            integer<negativity::nones, zz> operator () (
                const integer<negativity::nones, zz> &a,
                const integer<negativity::nones, zz> &b) {
                if (a.size () < b.size ()) return add<negativity::nones, zz> {} (b, a);
                integer<negativity::nones, zz> n {};
                n.resize (a.size () + 2);
                plus (n, a, math::number::extend (b, a.size ()));
                return n;
            }
        };
        
        template <hex::letter_case zz> 
        integer<negativity::nones, zz> minus (const integer<negativity::nones, zz> &a, const integer<negativity::nones, zz> &b) {
            if (b > a) return integer<negativity::nones, zz> {};
            // these numbers are both trimmed, so we can expect that the size of b is less than that of a. 
            integer<negativity::nones, zz> n {};
            n.resize (a.size ());
            minus (n, a, math::number::extend (b, a.size ()));
            return n;
        }
        
        template <negativity c, hex::letter_case zz> struct add {
            integer<c, zz> operator () (
                const integer<c, zz> &a, 
                const integer<c, zz> &b) {

                if (a.size () < b.size ()) return add<c, zz> {} (b, a);
                
                bool an = is_negative (a);
                bool bn = is_negative (b);
                
                if (an && bn) return -add<c, zz> {} (-a, -b);
                
                integer<c, zz> n {};
                n.resize (a.size () + 2);
                
                if (an || bn) {
                    auto ab = data::abs (a);
                    auto bb = data::abs (b);
                    
                    if (ab > bb) {
                        if (an) {
                            auto am = -a;
                            size_t max_size = std::max (am.size (), b.size ());
                            minus (n, math::number::extend (am, max_size), math::number::extend (b, max_size));
                            return -n;
                        } else {
                            auto bm = -b;
                            size_t max_size = std::max (a.size (), bm.size ());
                            minus (n, math::number::extend (a, max_size), math::number::extend (bm, max_size));
                            return n;
                        }
                    } else {
                        if (an) {
                            auto am = -a;
                            size_t max_size = std::max (am.size (), b.size ());
                            minus (n, math::number::extend (b, max_size), math::number::extend (am, max_size));
                            return n;
                        } else {
                            auto bm = -b;
                            size_t max_size = std::max (a.size (), bm.size ());
                            minus (n, math::number::extend (bm, max_size), math::number::extend (a, max_size));
                            return -n;
                        }
                    }
                    
                } 
                
                plus (n, a, math::number::extend (b, a.size ()));
                return n;
            }
        };
        
        template <hex::letter_case zz> 
        integer<negativity::twos, zz> inline
        minus (const integer<negativity::twos, zz> &a, const integer<negativity::twos, zz> &b) {
            return add<negativity::twos, zz> {} (a, -b);
        }
        
        template <hex::letter_case zz> 
        integer<negativity::BC, zz> inline
        minus (const integer<negativity::BC, zz> &a, const integer<negativity::BC, zz> &b) {
            return add<negativity::BC, zz> {} (a, -b);
        }
        
        template <negativity c, hex::letter_case zz>
        struct multiply {
            integer<c, zz> operator () (const integer<c, zz> &a, const integer<c, zz> &b) {
                auto ar = abs (a);
                auto br = abs (b);
                integer<c, zz> n;
                n.resize (ar.size () + br.size () - 2);
                times (n, ar, br);
                return (sign (a) * sign (b) < 0) ? -n : n;
            }
        };
        
        template <hex::letter_case zz> 
        struct multiply<negativity::nones, zz> {
            integer<negativity::nones, zz> operator () (
                const integer<negativity::nones, zz> &a,
                const integer<negativity::nones, zz> &b) {
                integer<negativity::nones, zz> n;
                n.resize (a.size () + b.size () - 2);
                times (n, a, b);
                return n;
            }
        };
        
    }
    
    template <hex::letter_case zz> 
    std::weak_ordering inline operator <=> (const integer<negativity::nones, zz> &a, const integer<negativity::nones, zz> &b) {
        if (!a.valid ()) throw exception {} << "invalid hexidecimal string: " << a;
        if (!b.valid ()) throw exception {} << "invalid hexidecimal string: " << b;
        
        return N_compare (string_view {a.data () + 2, a.size () - 2}, string_view {b.data () + 2, b.size () - 2});
    }
    
    template <hex::letter_case zz> 
    std::weak_ordering operator <=> (const integer<negativity::twos, zz> &a, const integer<negativity::twos, zz> &b) {

        if (!a.valid ()) throw exception {} << "invalid hexidecimal string: " << a;
        if (!b.valid ()) throw exception {} << "invalid hexidecimal string: " << b;
        
        bool na = is_negative (a);
        bool nb = is_negative (b);
        
        if (na && nb) {
            auto ya = -b;
            auto yb = -a;
            return N_compare (string_view {ya.data () + 2, ya.size () - 2}, string_view {yb.data () + 2, yb.size () - 2});
        }
        
        if (!na && !nb) return N_compare (string_view {a.data () + 2, a.size () - 2}, string_view {b.data () + 2, b.size () - 2});
        return na ? std::weak_ordering::less : std::weak_ordering::greater;
    }
    
    template <hex::letter_case zz> 
    std::weak_ordering operator <=> (const integer<negativity::BC, zz> &a, const integer<negativity::BC, zz> &b) {

        math::signature na = sign (a);
        math::signature nb = sign (b);
        
        if (na == math::zero) switch (nb) {
            case math::positive: return std::weak_ordering::less;
            case math::negative: return std::weak_ordering::greater;
            default: return std::weak_ordering::equivalent;
        }
        
        if (nb == math::zero) return na == math::positive ? std::weak_ordering::greater : std::weak_ordering::less;
        
        if (na == math::positive && nb == math::positive) 
            return N_compare (string_view {a.data () + 2, a.size () - 2}, string_view {b.data () + 2, b.size () - 2});
        
        if (na == math::negative && nb == math::negative) {
            auto ya = -b;
            auto yb = -a;
            return N_compare (string_view {ya.data () + 2, ya.size () - 2}, string_view {yb.data () + 2, yb.size () - 2});
        }
        
        return na == math::negative ? std::weak_ordering::less : std::weak_ordering::greater;
    }
    
    template <hex::letter_case zz> 
    integer<negativity::nones, zz> &operator ++ (integer<negativity::nones, zz> &x) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        
        char remainder = N_increment (x);
        if (remainder != '0') {
            integer<negativity::nones, zz> n {};
            n.resize (x.size () + 2);
            std::copy (x.begin () + 2, x.end (), n.begin () + 4);
            n[2] = '0';
            n[3] = remainder;
            x = n;
        } 
        
        return x.trim ();
    }
    
    template <hex::letter_case zz> 
    integer<negativity::nones, zz> &operator -- (integer<negativity::nones, zz> &x) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        
        if (is_zero (x)) return x;
        N_decrement (x);
        return x.trim ();
    }
    
    template <hex::letter_case zz> 
    integer<negativity::twos, zz> &operator ++ (integer<negativity::twos, zz> &x) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        
        bool neg = data::is_negative (x);
        char remainder = N_increment (x);
        
        if (!neg && (remainder != '0' || data::is_negative (x))) {
            integer<negativity::twos, zz> n {};
            n.resize (x.size () + 2);
            std::copy (x.begin () + 2, x.end (), n.begin () + 4);
            n[2] = '0';
            n[3] = remainder;
            x = n;
        } 
        
        return x.trim ();
    }
    
    template <hex::letter_case zz> 
    integer<negativity::twos, zz> &operator -- (integer<negativity::twos, zz> &x) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        
        if (x == std::string ("0x")) return x = integer<negativity::twos, zz> {"0xff"};
        x = math::number::extend (x, x.size () + 2);
        N_decrement (x);
        return x.trim ();
    }
    
    template <hex::letter_case zz> 
    integer<negativity::BC, zz> &operator ++ (integer<negativity::BC, zz> &x) {

        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        
        if (is_negative (x)) return x = -math::number::decrement<integer<negativity::BC, zz>> {} (-x);
        if (is_negative_zero (x)) return x = integer<negativity::BC, zz> {"0x01"};
        
        char remainder = N_increment (x);
        if (remainder != '0') {
            integer<negativity::BC, zz> n;
            n.resize (x.size () + 2);
            std::copy (x.begin () + 2, x.end (), n.begin () + 4);

            n[2] = '0';
            n[3] = remainder;
            x = n;
        } else if (math::number::sign_bit_set (x)) {
            integer<negativity::BC, zz> n;
            n.resize (x.size () + 2);
            std::copy (x.begin () + 2, x.end (), n.begin () + 4);

            n[2] = '0';
            n[3] = '0';
            x = n;
        }
        
        return x.trim ();
    }
    
    template <hex::letter_case zz> 
    integer<negativity::BC, zz> &operator -- (integer<negativity::BC, zz> &x) {

        if (is_zero (x)) return x = integer<negativity::BC, zz> {"0x81"};
        if (is_negative (x)) return x = -math::number::increment<integer<negativity::BC, zz>> {} (-x);
        N_decrement (x);
        return x.trim ();
    }
    
    template <negativity c, hex::letter_case zz>
    integer<c, zz> inline operator | (const integer<c, zz> &a, const integer<c, zz> &b) {
        return math::number::trim (bit_or (a, b));
    }
    
    template <negativity c, hex::letter_case zz>
    integer<c, zz> inline operator & (const integer<c, zz> &a, const integer<c, zz> &b) {
        return math::number::trim (bit_and (a, b));
    }
    
    template <hex::letter_case zz> 
    integer<negativity::twos, zz> inline
    operator^(const integer<negativity::twos, zz> &a, const integer<negativity::twos, zz> &b) {
        return math::number::trim (bit_xor (a, b));
    }
    
    template <negativity c, hex::letter_case zz>
    integer<c, zz> inline operator << (const integer<c, zz> &x, int i) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        
        return math::number::trim (bit_shift (x, i));
    }
    
    template <negativity c, hex::letter_case zz>
    integer<c, zz> inline operator >> (const integer<c, zz> &x, int i) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        
        return math::number::trim (bit_shift (x, -i));
    }
    
    template <negativity c, hex::letter_case zz>
    integer<c, zz> inline operator + (const integer<c, zz> &a, const integer<c, zz> &b) {
        return math::number::trim (add<c, zz> {} (math::number::trim (a), math::number::trim (b)));
    }
    
    template <negativity c, hex::letter_case zz>
    integer<c, zz> inline operator - (const integer<c, zz> &a, const integer<c, zz> &b) {
        return math::number::trim (minus (math::number::trim (a), math::number::trim (b)));
    }
    
    template <negativity c, hex::letter_case zz>
    integer<c, zz> inline operator * (const integer<c, zz> &a, const integer<c, zz> &b) {
        return math::number::trim (multiply<c, zz> {} (math::number::trim (a), math::number::trim (b)));
    }
    
}

namespace data::math {
    
    template <hex_case zz>
    bool is_positive_zero<hex::intBC<zz>>::operator () (const hex::intBC<zz> &z) {
        auto digit = z.begin () + 2;
        
        while (true) {
            if (digit == z.end ()) return true;
            if (*digit != '0') return false;
            digit++;
        }
        
        // we can't really reach here. 
        return true;
    }
    
    template <hex_case zz>
        bool is_negative_zero<hex::intBC<zz>>::operator () (const hex::intBC<zz> &z) {
        auto digit = z.begin () + 2;
        if (*digit != '8') return false;
    
        while (true) {
            digit++;
            if (digit == z.end ()) return true;
            if (*digit != '0') return false;
        }
        
        // we can't really reach here. 
        return true;
    }
    
    template <hex_case zz>
    bool inline is_negative<hex::intBC<zz>>::operator () (const hex::intBC<zz> &x) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        return number::sign_bit_set (x) && !data::is_negative_zero (x);
    }
    
    template <hex_case zz>
    bool is_zero<hex::intBC<zz>>::operator () (const hex::intBC<zz> &z) {
        auto digit = z.begin () + 2;
        if (digit == z.end ()) return true;
        if (*digit != '0' && *digit != '8') return false;
        
        while (true) {
            digit++;
            if (digit == z.end ()) return true;
            if (*digit != '0') return false;
        }
        
        // we can't really reach here. 
        return true;
    }
}

namespace data::math::number {
    template <hex_case zz>
    bool inline sign_bit_set (const hex::intBC<zz> &x) {
        return x.size () > 2 && encoding::hexidecimal::digit (x[2]) > 7;
    }
    
    template <hex_case zz>
    bool is_minimal (const hex::int2<zz> &x) {
        // minimal zero. 
        if (x.size () == 2) return true;
        // numbers of one byte. 
        if (x.size () == 4) return x[2] != '0' || x[3] != '0';
        // numbers without an initial 00 or ff. 
        if ((x[2] != 'f' || x[3] != 'f') && (x[2] != '0' || x[3] != '0')) return true; 
        // numbers that would be interpreted with the wrong sign if they were shortened. 
        char d = encoding::hexidecimal::digit (x[4]);
        return x[2] == '0' && d >= 8 || x[2] == 'f' && d < 8;
    }
    
    template <hex_case zz>
    bool inline is_minimal (const hex::intBC<zz> &x) {
        // minimal zero. 
        return (x.size () == 2) ||
            // numbers without an initial 0x00 or 0x80
            ((x[2] != '0' && x[2] != '8') || x[3] != '0') || 
            // representations of zero. 
            (x.size () > 4 &&
                // numbers that would be interpreted as having the wrong sign 
                // if they were shortened. 
                encoding::hexidecimal::digit (x[4]) >= 8);
    }
    
    template <hex_case zz>
    hex::uint<zz> extend (const hex::uint<zz> &x, size_t size) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        
        if (size & 1 || size < 2) throw exception {} << "invalid size " << size;
        
        if (x.size () > size) {
            auto minimal = minimal_size (x);
            if (minimal > size) throw exception ("cannot extend below minimal size");
            return extend (trim (x), size);
        }
        
        hex::uint<zz> n;
        n.resize (size);
        auto i = n.begin () + 2;
        for (int zeros = 0; zeros < size - x.size (); zeros ++) {
            *i = '0';
            i ++;
        }
        
        std::copy (x.begin () + 2, x.end (), i);
        return n;
    }
    
    template <hex_case zz>
    hex::int2<zz> extend (const hex::int2<zz> &x, size_t size) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        
        if (size & 1 || size < 2) throw exception {} << "invalid size " << size;
        
        if (x.size () > size) {
            auto minimal = minimal_size (x);
            if (minimal > size) throw exception ("cannot extend below minimal size");
            return extend (trim (x), size);
        }
        
        hex::uint<zz> n;
        n.resize (size);
        auto i = n.begin () + 2;

        char fill = data::is_negative (x) ? (zz == hex_case::upper ? 'F' : 'f') : '0';

        for (int zeros = 0; zeros < size - x.size (); zeros ++) {
            *i = fill;
            i ++;
        }
        
        std::copy (x.begin () + 2, x.end (), i);
        return n;
    }
    
    template <hex_case zz>
    hex::intBC<zz> extend (const hex::intBC<zz> &x, size_t size) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        
        if (size & 1 || size < 2) throw exception {} << "invalid size " << size;
        
        if (x.size () > size) {
            auto minimal = minimal_size (x);
            if (minimal > size) throw exception ("cannot extend below minimal size");
            return extend (trim (x), size);
        }
        
        if (x.size () == size) return x;
        
        hex::intBC<zz> n;
        n.resize (size);
        auto i = n.begin () + 2;
        for (int zeros = 0; zeros < size - x.size (); zeros ++) {
            *i = '0';
            i++;
        }
        
        std::copy (x.begin () + 2, x.end (), i);
        if (x.size () > 2 && size > 2) {
            auto sign_digit = encoding::hexidecimal::digit (x[2]);
            if (sign_digit >= 8) {
                n[2] = '8';
                n[2 + size - x.size ()] = encoding::hex::characters (zz)[sign_digit - 8];
            }
        }
        
        return n;
        
    }
    
    template <hex_case zz>
    size_t minimal_size (const hex::uint<zz> &x) {
        int zeros = 0;
        for (auto i = x.begin () + 2; i != x.end (); i += 2)
            if (i[0] == '0' && i[1] == '0') zeros += 2;
            else break;
        return x.size () - zeros;
    }
    
    template <hex_case zz>
    size_t minimal_size (const hex::int2<zz> &x) {
        if (x.size () == 2) return 2;
        // numbers that don't begin with 00 or ff are minimal. 
        char z = x[2];
        if (z != 'f' && z != '0' || x[3] != z) return x.size ();
        // count the number of repeated bytes after the first.
        // All can be removed. 
        int repeated = 0;
        auto i = x.begin () + 4;

        while (true) {
            // if we reach the end, then the number is either 0 or -1.
            if (i == x.end ()) return z == '0' ? 2 : 4;
            if (i[0] == z && i[1] == z) repeated += 2;
            else {
                char d = encoding::hexidecimal::digit (*i);
                return x.size () - repeated - (d >= 8 && z == 'f' || d < 8 && z == '0' ? 2 : 0);
            }
            i += 2;
        }
    }
    
    template <hex_case zz>
    size_t minimal_size (const hex::intBC<zz> &x) {
        if (x.size () == 2) return 2;
        // numbers that don't begin with 00 or 80 are minimal. 
        char z = x[2];
        if (z != '8' && z != '0' || x[3] != '0') return x.size ();
        // count the number of zero bytes after the first. 
        int zeros = 0;
        auto i = x.begin () + 4;

        while (true) {
            // if we reach the end then this number is zero. 
            if (i == x.end ()) return 2;
            if (i[0] == '0' && i[1] == '0') zeros += 2;
            // if the first non-zero digit does not have the
            // sign bit set then we can remove an extra digit. 
            else return x.size () - zeros - (encoding::hexidecimal::digit (*i) < 8 ? 2 : 0);
            i += 2;
        }
    }
}

#endif

