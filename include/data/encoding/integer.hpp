// Copyright (c) 2019 Daniel Krawisz
// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_INTEGER
#define DATA_ENCODING_INTEGER

#include <ctre.hpp>

#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string.hpp>

#include <data/encoding/invalid.hpp>
#include <data/math/number/complement.hpp>
#include <data/math/number/bytes/bytes.hpp>
#include <data/math/root.hpp>

#include <iostream>

namespace data::encoding {
    
    namespace decimal {
        static constexpr ctll::fixed_string pattern {"0|([1-9][0-9]*)"};
        
        std::string characters ();
        char digit (char x);
        bool valid (string_view s);
        bool nonzero (string_view s);
        uint32 digits (string_view s);
        
        template <endian::order r> maybe<math::N_bytes<r>> read (string_view s);
        
        template <typename range> 
        std::ostream &write (std::ostream &o, range r);
        
        // a decimal string inherets from string but is
        // a big number that supports standard numerical operations. 
        struct string;
        
        template <endian::order r> 
        string write (const math::number::N_bytes<r> &z);
        
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
        
        string operator | (const string &, const string &);
        string operator & (const string &, const string &);
        
        string operator / (const string &, const string &);
        string operator % (const string &, const string &);
        
    }
    
    namespace signed_decimal {
        
        static constexpr ctll::fixed_string pattern {"0|(-?[1-9][0-9]*)"};
        
        bool valid (string_view s);
        bool nonzero (string_view s);
        bool positive (string_view s);
        bool negative (string_view s);
        math::sign sign (string_view s);
        
        template <endian::order r> 
        maybe<math::Z_bytes<r>> read (string_view s);
        
        using complement = math::number::complement;

        template <endian::order r>
        std::ostream &write (std::ostream &o, const math::number::Z_bytes<r, complement::ones> &z);

        template <endian::order r>
        std::ostream &write (std::ostream &o, const math::number::Z_bytes<r, complement::twos> &z);
        
        // a decimal string inherets from string but is
        // a big number that supports standard numerical operations. 
        struct string;

        template <endian::order r, complement c>
        string write (const math::number::Z_bytes<r, c> &z);
        
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
        
        string operator | (const string &, const string &);
        string operator & (const string &, const string &);
        
        string operator / (const string &, const string &);
        
        string operator + (const string &n, const decimal::string &x);
        string operator - (const string &n, const decimal::string &x);
        string operator * (const string &n, const decimal::string &x);
        string operator | (const string &n, const decimal::string &x);
        string operator & (const string &n, const decimal::string &x);
        string operator / (const string &n, const decimal::string &x);

        string operator + (const decimal::string &n, const string &x);
        string operator - (const decimal::string &n, const string &x);
        string operator * (const decimal::string &n, const string &x);
        string operator | (const decimal::string &n, const string &x);
        string operator & (const decimal::string &n, const string &x);
        string operator / (const decimal::string &n, const string &x);
        
        decimal::string operator % (const string &, const decimal::string &x);
        
        string &operator += (string &n, const decimal::string &x);
        string &operator -= (string &n, const decimal::string &x);
        string &operator *= (string &n, const decimal::string &x);
        string &operator |= (string &n, const decimal::string &x);
        string &operator &= (string &n, const decimal::string &x);
        string &operator /= (string &n, const decimal::string &x);
        
    }
    
    namespace hexidecimal {
        static constexpr ctll::fixed_string pattern {"0x((([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*))"};
        static constexpr ctll::fixed_string zero_pattern {"0x(00)*"};
        
        bool valid (string_view s);
        bool zero (string_view s);
        bool nonzero (string_view s);
        uint32 digits (string_view s);
        char digit (char x);
        
        hex::letter_case read_case (string_view s);
        
        template <endian::order r> 
        maybe<oriented<r, byte>> read (string_view s);
        
        template <endian::order r> 
        std::ostream inline &write (std::ostream &o, const oriented<r, byte> &d, hex::letter_case q);
        
        template <hex::letter_case cx> struct string : data::string {
            string () : string {"0x"} {}
            explicit string (const std::string &x) : data::string {hexidecimal::valid (x) ? x : ""} {}
            bool valid () const {
                return hexidecimal::valid (*this);
            }
        };
        
        template <hex::letter_case cx, endian::order r> 
        string<cx> write (const oriented<r, byte> &z);

        using complement = math::number::complement;

        template <complement c, hex::letter_case cx>
        bool is_minimal (const string<cx> &);

        template <complement c, hex::letter_case cx>
        bool is_negative (const string<cx> &);

        template <complement c, hex::letter_case cx>
        size_t minimal_size (const string<cx> &);

        template <complement c, hex::letter_case cx>
        string<cx> extend (const string<cx> &, size_t);

        template <complement c, hex::letter_case cx>
        string<cx> trim (const string<cx> &);
        
        // a hexidecimal integer inherets from string but is
        // a big number that supports standard numerical operations.
        template <complement, hex::letter_case cx> struct integer;

        template <complement cl, complement cr, hex::letter_case cx>
        bool operator == (const integer<cl, cx> &a, const integer<cr, cx> &b);
        
        template <hex::letter_case cx>
        std::weak_ordering operator <=> (const integer<complement::nones, cx> &, const integer<complement::nones, cx> &);
        
        template <hex::letter_case cx>
        std::weak_ordering operator <=> (const integer<complement::ones, cx> &, const integer<complement::ones, cx> &);
        
        template <hex::letter_case cx>
        std::weak_ordering operator <=> (const integer<complement::twos, cx> &, const integer<complement::twos, cx> &);

        template <complement c, hex::letter_case cx>
        std::weak_ordering operator <=> (const integer<complement::nones, cx> &, const integer<c, cx> &);

        template <complement c, hex::letter_case cx>
        std::weak_ordering operator <=> (const integer<c, cx> &, const integer<complement::nones, cx> &);

        template <complement cl, complement cr, hex::letter_case cx>
        std::weak_ordering operator <=> (const integer<cl, cx> &, const integer<cr, cx> &);

        template <hex::letter_case cx>
        bool operator == (const integer<complement::nones, cx> &, uint64);

        template <hex::letter_case cx>
        bool operator == (const integer<complement::ones, cx> &, int64);

        template <hex::letter_case cx>
        bool operator == (const integer<complement::twos, cx> &, int64);

        template <hex::letter_case cx>
        std::weak_ordering operator <=> (const integer<complement::nones, cx> &, uint64);

        template <hex::letter_case cx>
        std::weak_ordering operator <=> (const integer<complement::ones, cx> &, int64);

        template <hex::letter_case cx>
        std::weak_ordering operator <=> (const integer<complement::twos, cx> &, int64);
        
        template <hex::letter_case cx>
        integer<complement::nones, cx> &operator ++ (integer<complement::nones, cx> &);
        
        template <hex::letter_case cx>
        integer<complement::ones, cx> &operator ++ (integer<complement::ones, cx> &);
        
        template <hex::letter_case cx>
        integer<complement::twos, cx> &operator ++ (integer<complement::twos, cx> &);
        
        template <hex::letter_case cx>
        integer<complement::nones, cx> &operator -- (integer<complement::nones, cx> &);
        
        template <hex::letter_case cx>
        integer<complement::ones, cx> &operator -- (integer<complement::ones, cx> &);
        
        template <hex::letter_case cx>
        integer<complement::twos, cx> &operator -- (integer<complement::twos, cx> &);
        
        template <complement c, hex::letter_case cx>
        integer<c, cx> operator ++ (integer<c, cx> &, int);
        
        template <complement c, hex::letter_case cx>
        integer<c, cx> operator -- (integer<c, cx> &, int);
        
        template <complement c, hex::letter_case cx>
        integer<c, cx> operator + (const integer<c, cx> &, const integer<c, cx> &);
        
        template <complement c, hex::letter_case cx>
        integer<c, cx> operator - (const integer<c, cx> &, const integer<c, cx> &);
        
        template <complement c, hex::letter_case cx>
        integer<c, cx> operator * (const integer<c, cx> &, const integer<c, cx> &);
        
        template <complement c, hex::letter_case cx>
        integer<c, cx> operator | (const integer<c, cx> &, const integer<c, cx> &);

        template <complement c, hex::letter_case cx>
        integer<c, cx> operator & (const integer<c, cx> &, const integer<c, cx> &);

        template <complement c, hex::letter_case cx>
        integer<c, cx> operator | (const integer<c, cx> &, uint64);

        template <complement c, hex::letter_case cx>
        integer<c, cx> operator & (const integer<c, cx> &, uint64);
        
        template <complement c, hex::letter_case cx>
        integer<c, cx> operator << (const integer<c, cx> &, int);
        
        template <complement c, hex::letter_case cx>
        integer<c, cx> operator >> (const integer<c, cx> &, int);

        template <hex::letter_case cx>
        integer<complement::ones, cx> operator - (const integer<complement::nones, cx> &);
        
        template <hex::letter_case cx> 
        integer<complement::ones, cx> operator - (const integer<complement::ones, cx> &);
        
        template <hex::letter_case cx> 
        integer<complement::twos, cx> operator - (const integer<complement::twos, cx> &);
        
        template <hex::letter_case cx>
        integer<complement::ones, cx> operator ~ (const integer<complement::ones, cx> &);
        
        template <hex::letter_case cx> integer<complement::ones, cx> operator ^
            (const integer<complement::ones, cx> &, const integer<complement::ones, cx> &);

        template <hex::letter_case cx>
        integer<complement::twos, cx> operator ! (const integer<complement::twos, cx> &);

        template <hex::letter_case cx> integer<complement::twos, cx> operator &&
            (const integer<complement::twos, cx> &, const integer<complement::twos, cx> &);

        template <hex::letter_case cx> integer<complement::twos, cx> operator ||
            (const integer<complement::twos, cx> &, const integer<complement::twos, cx> &);
        
        template <hex::letter_case cx> 
        integer<complement::ones, cx> operator + (const integer<complement::ones, cx> &n, const integer<complement::nones, cx> &x);
        
        template <hex::letter_case cx> 
        integer<complement::ones, cx> operator - (const integer<complement::ones, cx> &n, const integer<complement::nones, cx> &x);
        
        template <hex::letter_case cx> 
        integer<complement::ones, cx> operator * (const integer<complement::ones, cx> &n, const integer<complement::nones, cx> &x);

        template <hex::letter_case cx>
        integer<complement::ones, cx> operator + (const integer<complement::nones, cx> &n, const integer<complement::ones, cx> &x);

        template <hex::letter_case cx>
        integer<complement::ones, cx> operator - (const integer<complement::nones, cx> &n, const integer<complement::ones, cx> &x);

        template <hex::letter_case cx>
        integer<complement::ones, cx> operator * (const integer<complement::nones, cx> &n, const integer<complement::ones, cx> &x);
        
        template <hex::letter_case cx> 
        integer<complement::ones, cx> operator | (const integer<complement::ones, cx> &n, const integer<complement::nones, cx> &x);
        
        template <hex::letter_case cx> 
        integer<complement::ones, cx> operator & (const integer<complement::ones, cx> &n, const integer<complement::nones, cx> &x);
        
        template <hex::letter_case cx> 
        integer<complement::ones, cx> operator / (const integer<complement::ones, cx> &n, const integer<complement::nones, cx> &x);
        
        template <hex::letter_case cx> 
        integer<complement::ones, cx> &operator += (integer<complement::ones, cx> &n, const integer<complement::nones, cx> &x);
        
        template <hex::letter_case cx> 
        integer<complement::ones, cx> &operator -= (integer<complement::ones, cx> &n, const integer<complement::nones, cx> &x);
        
        template <hex::letter_case cx> 
        integer<complement::ones, cx> &operator *= (integer<complement::ones, cx> &n, const integer<complement::nones, cx> &x);
        
        template <hex::letter_case cx> 
        integer<complement::ones, cx> &operator |= (integer<complement::ones, cx> &n, const integer<complement::nones, cx> &x);
        
        template <hex::letter_case cx> 
        integer<complement::ones, cx> &operator &= (integer<complement::ones, cx> &n, const integer<complement::nones, cx> &x);
        
    }
    
    namespace natural {

        static constexpr ctll::fixed_string pattern {"0|([1-9][0-9]*)|(0x((([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*)))"};

        static constexpr ctll::fixed_string zero_pattern {"0|0x(00)*"};
        
        bool valid (string_view s);
        bool zero (string_view s);
        bool nonzero (string_view s);
        uint32 digits (string_view s);
        
        template <endian::order r> maybe<math::N_bytes<r>> read (string_view s);
        
    }
    
    namespace integer {

        static constexpr ctll::fixed_string pattern {"0|(-?[1-9][0-9]*)|(0x((([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*)))"};
        
        static constexpr ctll::fixed_string zero_pattern {"0|0x(00)*"};
        
        static constexpr ctll::fixed_string negative_pattern 
            {"(-(0*[1-9][0-9]*))|0x(([8-9a-f][0-9a-f]([0-9a-f][0-9a-f])*)|([8-9A-F][0-9A-F]([0-9A-F][0-9A-F])*))"};
        
        bool valid (string_view s);
        bool negative (string_view s);
        bool zero (string_view s);
        bool nonzero (string_view s);
        uint32 digits (string_view s);

        template <endian::order r, math::number::complement c> 
        maybe<math::number::Z_bytes<r, c>> read (string_view s);
        
    }
    
}

namespace data::hex {
    template <hex_case zz> using uint = encoding::hexidecimal::integer<math::number::complement::nones, zz>;
    template <hex_case zz> using int1 = encoding::hexidecimal::integer<math::number::complement::ones, zz>;
    template <hex_case zz> using int2 = encoding::hexidecimal::integer<math::number::complement::twos, zz>;
    template <math::number::complement c, hex_case zz> using integer = encoding::hexidecimal::integer<c, zz>;
}

namespace data {
    using dec_uint = encoding::decimal::string;
    using dec_int = encoding::signed_decimal::string;
    
    math::sign sign (const dec_uint &);
    math::sign sign (const dec_int &);
    
    template <hex_case zz> math::sign sign (const hex::uint<zz> &);
    template <hex_case zz> math::sign sign (const hex::int1<zz> &);
    template <hex_case zz> math::sign sign (const hex::int2<zz> &);
    
    dec_uint increment (const dec_uint &);
    dec_uint decrement (const dec_uint &);
    
    dec_int increment (const dec_int &);
    dec_int decrement (const dec_int &);
    
    template <math::number::complement c, hex_case zz>
    hex::integer<c, zz> increment (const hex::integer<c, zz> &);
    template <math::number::complement c, hex_case zz>
    hex::integer<c, zz> decrement (const hex::integer<c, zz> &);

    template<math::number::complement a, hex_case b, math::number::complement c, hex_case d>
    bool identical (const hex::integer<a, b> &, const hex::integer<c, d> &);
    
}

namespace data::math {
    
    template <> struct abs<dec_uint> {
        dec_uint operator () (const dec_uint &);
    };
    
    template <> struct abs<dec_int> {
        dec_uint operator () (const dec_int &);
    };
    
    template <hex_case zz> struct abs<hex::uint<zz>> {
        hex::uint<zz> operator () (const hex::uint<zz> &);
    };
    
    template <hex_case zz> struct abs<hex::int1<zz>> {
        hex::uint<zz> operator () (const hex::int1<zz> &);
    };
    
    template <hex_case zz> struct abs<hex::int2<zz>> {
        hex::int2<zz> operator () (const hex::int2<zz> &);
    };
    
    template <> struct commutative<plus<dec_uint>, dec_uint> {};
    template <> struct associative<plus<dec_uint>, dec_uint> {};
    template <> struct commutative<times<dec_uint>, dec_uint> {};
    template <> struct associative<times<dec_uint>, dec_uint> {};
    
    template <> struct commutative<plus<dec_int>, dec_int> {};
    template <> struct associative<plus<dec_int>, dec_int> {};
    template <> struct commutative<times<dec_int>, dec_int> {};
    template <> struct associative<times<dec_int>, dec_int> {};
    
    template <number::complement c, hex_case zz> 
    struct commutative<plus<hex::integer<c, zz>>, hex::integer<c, zz>> {};
    
    template <number::complement c, hex_case zz> 
    struct associative<plus<hex::integer<c, zz>>, hex::integer<c, zz>> {};
    
    template <number::complement c, hex_case zz> 
    struct commutative<times<hex::integer<c, zz>>, hex::integer<c, zz>> {};
    
    template <number::complement c, hex_case zz> 
    struct associative<times<hex::integer<c, zz>>, hex::integer<c, zz>> {};
    
    bool is_zero (const dec_uint &);
    bool is_negative (const dec_uint &);
    bool is_positive (const dec_uint &);
    
    bool is_zero (const dec_int &);
    bool is_negative (const dec_int &);
    bool is_positive (const dec_int &);
    
    template <hex_case zz> bool is_zero (const hex::uint<zz> &);
    template <hex_case zz> bool is_zero (const hex::int1<zz> &);
    template <hex_case zz> bool is_zero (const hex::int2<zz> &);
    
    template <hex_case zz> bool is_negative (const hex::uint<zz> &);
    template <hex_case zz> bool is_negative (const hex::int1<zz> &);
    template <hex_case zz> bool is_negative (const hex::int2<zz> &);
    
    template <number::complement c, hex_case zz>
    bool is_positive (const hex::integer<c, zz> &);
    
    template <hex_case cx> 
    bool is_positive_zero (const hex::int2<cx> &);
    
    template <hex_case cx> 
    bool is_negative_zero (const hex::int2<cx> &);
    
}

namespace data::math::number {
    
    template <hex_case cx> 
    bool sign_bit_set (const hex::int2<cx> &);
    
    template <hex_case cx> 
    bool is_minimal (const hex::uint<cx> &);
    
    template <hex_case cx> 
    bool is_minimal (const hex::int1<cx> &);
    
    template <hex_case cx> 
    bool is_minimal (const hex::int2<cx> &);
    
    template <hex_case cx> 
    size_t minimal_size (const hex::uint<cx> &);
    
    template <hex_case cx> 
    size_t minimal_size (const hex::int1<cx> &);
    
    template <hex_case cx> 
    size_t minimal_size (const hex::int2<cx> &);
    
    template <hex_case cx> 
    hex::uint<cx> extend (const hex::uint<cx> &, size_t);
    
    template <hex_case cx> 
    hex::int1<cx> extend (const hex::int1<cx> &, size_t);
    
    template <hex_case cx> 
    hex::int2<cx> extend (const hex::int2<cx> &, size_t);
    
    template <hex_case cx> 
    hex::uint<cx> trim (const hex::uint<cx> &);
    
    template <hex_case cx> 
    hex::int1<cx> trim (const hex::int1<cx> &);
    
    template <hex_case cx> 
    hex::int2<cx> trim (const hex::int2<cx> &);
    
}

namespace data::encoding::decimal {
    
    struct string : data::string {
        string () : data::string {"0"} {};
        
        explicit string (const std::string &x) : data::string {decimal::valid (x) ? x : ""} {}
        explicit string (std::string &&x) : data::string {x} {}
        string (uint64);
        
        bool valid () const {
            return decimal::valid (*this);
        }
        
        static string read (string_view x);
        
        string &operator += (const string &);
        string &operator -= (const string &);
        string &operator *= (const string &);
        
        string &operator <<= (int);
        string &operator >>= (int);
        
        string &operator |= (const string &) const;
        string &operator &= (const string &) const;
        
        math::division<string, uint64> divide (uint64) const;
        
        bool operator == (uint64) const;
        std::strong_ordering operator <=> (uint64) const;
        
        string operator + (uint64) const;
        string operator - (uint64) const;
        string operator * (uint64) const;
        
        string &operator += (uint64);
        string &operator -= (uint64);
        string &operator *= (uint64);
        
        explicit operator double () const;
        explicit operator uint64 () const;
    };
    
    signed_decimal::string operator - (const string &);
    
}

namespace data::encoding::signed_decimal {
    
    struct string : data::string {
        string () : data::string {"0"} {};
        explicit string (const std::string &x) : data::string {signed_decimal::valid (x) ? x : ""} {}
        explicit string (std::string &&x) : data::string {x} {}
        string (int64);
        
        bool valid () const {
            return signed_decimal::valid (*this);
        }
        
        static string read (string_view x);
        
        string &operator += (const string &);
        string &operator -= (const string &);
        string &operator *= (const string &);
        
        string &operator <<= (int);
        string &operator >>= (int);
        
        string &operator |= (const string &) const;
        string &operator &= (const string &) const;
        
        math::division<string, int64> divide (int64) const;
        
        bool operator == (int64) const;
        std::strong_ordering operator <=> (int64) const;
        
        string operator + (int64) const;
        string operator - (int64) const;
        string operator * (int64) const;
        
        string &operator += (int64);
        string &operator -= (int64);
        string &operator *= (int64);
        
        explicit operator double () const;
        explicit operator int64 () const;
    };
    
    template <endian::order r>
    std::ostream &write (std::ostream &w, const math::number::Z_bytes<r, complement::ones> &z) {
        if (math::is_negative (z)) w << "-";
        return decimal::write (w, data::abs (z));
    }

    template <endian::order r>
    std::ostream &write (std::ostream &w, const math::number::Z_bytes<r, complement::twos> &z) {
        if (math::is_negative (z)) w << "-";
        return decimal::write (w, math::number::N_bytes<r>::read (data::abs (z)));
    }
    
    template <endian::order r, complement n> 
    string inline write (const math::number::Z_bytes<r, n> &z) {
        std::stringstream ss;
        write (ss, z);
        return string {ss.str ()};
    }
}

namespace data::encoding::hexidecimal {

    // we need signed versions of integer to have constructors for
    // both uint64 and int64 whereas the unsigned version can only
    // support uint64. That's the only point of complemented_string
    template <complement c, hex::letter_case cx>
    struct complemented_string : string<cx> {
        using string<cx>::string;
        complemented_string (const string<cx> &x): string<cx> {x} {}
        complemented_string (int64);

        explicit operator int64 () const;
        explicit operator integer<complement (-int (c) + 3), cx> () const;

        static integer<c, cx> zero (size_t size = 0, bool negative = false);
    };

    template <hex::letter_case cx>
    struct complemented_string<complement::nones, cx> : string<cx> {
        using string<cx>::string;
        complemented_string (const string<cx> &x): string<cx> {x} {}
        complemented_string (int64);

        explicit operator uint64 () const;
        explicit operator integer<complement::ones, cx> () const;
        explicit operator integer<complement::twos, cx> () const;
    };
    
    template <complement c, hex::letter_case cx>
    struct integer : complemented_string<c, cx> {

        using complemented_string<c, cx>::complemented_string;
        
        static integer read (const std::string &);
        
        integer &operator += (const integer &);
        integer &operator -= (const integer &);
        integer &operator *= (const integer &);
        
        integer &operator <<= (int i);
        integer &operator >>= (int i);
        
        math::division<integer> divide (const integer &) const;
        
        integer operator / (const integer &x) const;
        integer operator % (const integer &x) const;
        
        integer operator + (int64) const;
        integer operator - (int64) const;
        integer operator * (int64) const;
        
        integer &operator += (int64);
        integer &operator -= (int64);
        integer &operator *= (int64);
        
        explicit operator double () const;
        explicit operator bool () const;
        
        integer &trim ();
        integer trim () const;
        
    };

    template <complement c, hex::letter_case cx>
    bool inline is_minimal (const string<cx> &x) {
        return data::math::number::is_minimal (integer<c, cx> {x});
    }

    template <complement c, hex::letter_case cx>
    bool inline is_negative (const string<cx> &x) {
        return data::math::is_negative (integer<c, cx> {x});
    }

    template <complement c, hex::letter_case cx>
    size_t inline minimal_size (const string<cx> &x) {
        return data::math::number::minimal_size (integer<c, cx> {x});
    }

    template <complement c, hex::letter_case cx>
    string<cx> inline extend (const string<cx> &x, size_t z) {
        return data::math::number::extend (integer<c, cx> {x}, z);
    }

    template <complement c, hex::letter_case cx>
    string<cx> inline trim (const string<cx> &x) {
        return data::math::number::trim (integer<c, cx> {x});
    }

    template <complement c, hex::letter_case zz>
    inline integer<c, zz>::operator bool () const {
        return !math::is_zero (*this);
    }
    
}

namespace data::math {
    
    template <uint64 pow> 
    struct root<dec_uint, pow> {
        set<dec_uint> operator () (const dec_uint &n);
    };
    
    template <uint64 pow> 
    struct root<dec_int, pow> {
        set<dec_int> operator () (const dec_int &n);
    };
    
    template <hex_case zz, uint64 pow> 
    struct root<hex::uint<zz>, pow> {
        set<hex::uint<zz>> operator () (const hex::uint<zz> &n);
    };
    
    template <hex_case zz, uint64 pow> 
    struct root<hex::int1<zz>, pow> {
        set<hex::int1<zz>> operator () (const hex::int1<zz> &n);
    };
    
    template <hex_case zz, uint64 pow> 
    struct root<hex::int2<zz>, pow> {
        set<hex::int2<zz>> operator () (const hex::int2<zz> &n);
    };
    
}

namespace data::encoding::decimal {
    
    std::string inline characters () {
        return "0123456789";
    }
    
    char inline digit (char x) {
        return x < '0' || x > '9' ? -1 : x - '0';
    }
        
    bool inline valid (string_view s) {
        return ctre::match<pattern> (s);
    } 
    
    bool inline nonzero (string_view s) {
        return valid (s) && s[0] != '0';
    }
    
    uint32 inline digits (string_view s) {
        return valid (s) ? s.size () : 0;
    }
    
    string inline &string::operator += (const string &x) {
        return *this = *this + x;
    }
    
    string inline &string::operator -= (const string &x) {
        return *this = *this - x;
    }
    
    string inline &string::operator *= (const string &x) {
        return *this = *this * x;
    }
    
    string inline &string::operator <<= (int x) {
        return *this = *this << x;
    }
    
    string inline &string::operator >>= (int x) {
        return *this = *this >> x;
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
    
}

namespace data::encoding::signed_decimal {
    
    bool inline valid (string_view s) {
        return ctre::match<pattern>(s);
    }
    
    bool inline nonzero (string_view s) {
        return valid (s) && s[0] != '0';
    }
    
    bool inline negative (string_view s) {
        return valid (s) && s[0] == '-';
    }
    
    bool inline positive (string_view s) {
        return valid (s) && s[0] != '-' && s[0] != '0';
    }
    
    math::sign inline sign (string_view s) {
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
    
    string inline operator / (const string &n, const decimal::string &x) {
        return n / string {static_cast<const std::string> (x)};
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
    
    bool inline valid (string_view s) {
        return ctre::match<pattern> (s);
    } 
    
    bool inline zero (string_view s) {
        return ctre::match<zero_pattern> (s);
    }
    
    bool inline nonzero (string_view s) {
        return valid (s) && !ctre::match<zero_pattern> (s);
    }
    
    uint32 inline digits (string_view s) {
        return valid (s) ? s.size () - 2 : 0;
    }
    
    char inline digit (char x) {
        if (x >= '0' && x <= '9') return x - '0';
        if (x >= 'A' && x <= 'F') return x - 'A' + 10;
        if (x >= 'a' && x <= 'f') return x - 'a' + 10;
        return -1;
    }
    
    template <endian::order r> 
    maybe<oriented<r, byte>> read (string_view s) {

        if (!valid (s)) return {};
        
        oriented<r, byte> n = {};
        size_t new_size = (s.size () - 2) / 2;
        n.resize (new_size);
        boost::algorithm::unhex (s.begin () + 2, s.end (), n.words ().rbegin ());
        
        return {n};
    }
    
    template <endian::order r> 
    std::ostream inline &write (std::ostream &o, const oriented<r, byte> &d, hex::letter_case q) {
        o << "0x"; 
        return encoding::hex::write (o, d.words ().reverse (), q);
    }
    
    template <hex::letter_case cx, endian::order r> 
    string<cx> write (const oriented<r, byte> &z) {
        std::stringstream ss;
        write (ss, z, cx);
        return string<cx> {ss.str ()};
    }

    template <complement cl, complement cr, hex::letter_case cx>
    bool inline operator == (const integer<cl, cx> &a, const integer<cr, cx> &b) {
        return (a <=> b) == 0;
    }

    template <complement c, hex::letter_case cx>
    std::weak_ordering inline operator <=> (const integer<complement::nones, cx> &a, const integer<c, cx> &b) {
        return integer<c, cx> (a) <=> b;
    }

    template <complement c, hex::letter_case cx>
    std::weak_ordering inline operator <=> (const integer<c, cx> &a, const integer<complement::nones, cx> &b) {
        return a <=> integer<c, cx> (b);
    }

    template <complement cl, complement cr, hex::letter_case cx>
    std::weak_ordering inline operator <=> (const integer<cl, cx> &a, const integer<cr, cx> &b) {
        return a <=> integer<cl, cx> (b);
    }

    template <hex::letter_case cx>
    bool inline operator == (const integer<complement::nones, cx> &x, uint64 u) {
        return x == integer<complement::nones, cx> {u};
    }

    template <hex::letter_case cx>
    bool inline operator == (const integer<complement::ones, cx> &x, int64 u) {
        return x == integer<complement::ones, cx> {u};
    }

    template <hex::letter_case cx>
    bool inline operator == (const integer<complement::twos, cx> &x, int64 u) {
        return x == integer<complement::twos, cx> {u};
    }

    template <hex::letter_case cx>
    std::weak_ordering inline operator <=> (const integer<complement::nones, cx> &x, uint64 u) {
        return x <=> integer<complement::nones, cx> {u};
    }

    template <hex::letter_case cx>
    std::weak_ordering inline operator <=> (const integer<complement::ones, cx> &x, int64 u) {
        return x <=> integer<complement::ones, cx> {u};
    }

    template <hex::letter_case cx>
    std::weak_ordering inline operator <=> (const integer<complement::twos, cx> &x, int64 u) {
        return x <=> integer<complement::twos, cx> {u};
    }
    
    template <hex::letter_case cx> 
    integer<complement::ones, cx> inline operator +
        (const integer<complement::ones, cx> &n, const integer<complement::nones, cx> &m) {
        return n + integer<complement::ones, cx> {math::number::extend (m, m.size () + 2)};
    }
    
    template <hex::letter_case cx> 
    integer<complement::ones, cx> inline operator -
        (const integer<complement::ones, cx> &n, const integer<complement::nones, cx> &m) {
        return n - integer<complement::ones, cx> {math::number::extend (m, m.size () + 2)};
    }
    
    template <hex::letter_case cx> 
    integer<complement::ones, cx> inline operator *
        (const integer<complement::ones, cx> &n, const integer<complement::nones, cx> &m) {
        return n * integer<complement::ones, cx> {math::number::extend (m, m.size () + 2)};
    }
    
    template <hex::letter_case cx> 
    integer<complement::ones, cx> inline operator |
        (const integer<complement::ones, cx> &n, const integer<complement::nones, cx> &m) {
        return n | integer<complement::ones, cx> {math::number::extend (m, m.size () + 1)};
    }
    
    template <hex::letter_case cx> 
    integer<complement::ones, cx> inline operator &
        (const integer<complement::ones, cx> &n, const integer<complement::nones, cx> &m) {
        return n & integer<complement::ones, cx> {math::number::extend (m, m.size () + 1)};
    }
    
    template <hex::letter_case cx> 
    integer<complement::ones, cx> inline operator /
        (const integer<complement::ones, cx> &n, const integer<complement::nones, cx> &m) {
        return n / integer<complement::ones, cx> {math::number::extend (m, m.size () + 1)};
    }
    
    template <hex::letter_case cx> 
    integer<complement::ones, cx> inline &operator +=
        (integer<complement::ones, cx> &n, const integer<complement::nones, cx> &m) {
        return n += integer<complement::ones, cx> {math::number::extend (m, m.size () + 1)};
    }
    
    template <hex::letter_case cx> 
    integer<complement::ones, cx> inline &operator -=
        (integer<complement::ones, cx> &n, const integer<complement::nones, cx> &m) {
        return n -= integer<complement::ones, cx> {math::number::extend (m, m.size () + 1)};
    }
    
    template <hex::letter_case cx> 
    integer<complement::ones, cx> inline &operator *=
        (integer<complement::ones, cx> &n, const integer<complement::nones, cx> &m) {
        return n *= integer<complement::ones, cx> {math::number::extend (m, m.size () + 1)};
    }
    
    template <hex::letter_case cx> 
    integer<complement::ones, cx> inline &operator |=
        (integer<complement::ones, cx> &n, const integer<complement::nones, cx> &m) {
        return n |= integer<complement::ones, cx> {math::number::extend (m, m.size () + 1)};
    }
    
    template <hex::letter_case cx> 
    integer<complement::ones, cx> inline &operator &=
        (integer<complement::ones, cx> &n, const integer<complement::nones, cx> &m) {
        return n &= integer<complement::ones, cx> {math::number::extend (m, m.size () + 1)};
    }

    template <complement c, hex::letter_case cx>
    integer<c, cx> inline operator | (const integer<c, cx> &x, uint64 u) {
        return x | integer<c, cx> (integer<complement::nones, cx> {u});
    }

    template <complement c, hex::letter_case cx>
    integer<c, cx> inline operator & (const integer<c, cx> &x, uint64 u) {
        return x & integer<c, cx> (integer<complement::nones, cx> {u});
    }

    template <hex::letter_case cx>
    integer<complement::ones, cx> inline operator - (const integer<complement::nones, cx> &x) {
        return -integer<complement::ones, cx> (x);
    }
    
} 

namespace data::encoding::natural {
    
    bool inline valid (string_view s) {
        return ctre::match<pattern> (s);
    } 
    
    bool inline zero (string_view s) {
        return ctre::match<zero_pattern> (s);
    }
    
    bool inline nonzero (string_view s) {
        return valid (s) && ! ctre::match<zero_pattern> (s);
    }
    
    uint32 inline digits (string_view s) {
        return std::max (decimal::digits (s), hexidecimal::digits (s));
    }
    
}

namespace data::encoding::integer {
    
    bool inline valid (string_view s) {
        return ctre::match<pattern> (s);
    } 
    
    bool inline negative (string_view s) {
        return ctre::match<negative_pattern> (s);
    }
    
    bool inline zero (string_view s) {
        return ctre::match<zero_pattern> (s);
    }
    
    bool inline nonzero (string_view s) {
        return valid (s) && ! ctre::match<zero_pattern> (s);
    }
    
    uint32 inline digits (string_view s) {
        return negative (s) ? natural::digits (s.substr (1, s.size () - 1)) : natural::digits (s);
    }
    
    template <endian::order r, math::number::complement c> 
    maybe<math::number::Z_bytes<r, c>> read (string_view s) {

        if (!valid (s)) return {};
        
        if (hexidecimal::valid (s)) {
            auto z = hexidecimal::read<r> (s);
            if (!z) return {};
            auto x = math::number::Z_bytes<r, c> {};
            x.resize (z->size ());
            std::copy (z->begin (), z->end (), x.begin ());
            return {x};
        }
        
        auto z = signed_decimal::read<r, c> (s);
        if (!z) return {};
        return {math::number::Z_bytes<r, c> (*z)};
    }
    
}

namespace data::math::number {
    
    template <hex_case cx> 
    bool inline is_minimal (const hex::uint<cx> &x) {
        if (x.size () < 4) return true;
        return x[2] != '0' || x[3] != '0';
    }
}

namespace data {
    
    math::sign inline sign (const dec_uint &n) {
        if (!encoding::decimal::valid (n)) throw exception {} << "invalid decimal string: " << n;
        
        return encoding::decimal::nonzero (n) ? math::positive : math::zero;
    }
    
    math::sign inline sign (const dec_int &n) {
        return encoding::signed_decimal::sign (n);
    }
    
    template <hex_case cx>
    math::sign inline sign (const hex::uint<cx> &x) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        
        return math::is_zero (x) ? math::zero : math::positive;
    }
    
    template <hex_case cx>
    math::sign inline sign (const hex::int1<cx> &x) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        
        return math::is_negative (x) ? math::negative : math::is_zero (x) ? math::zero : math::positive;
    }
    
    template <hex_case cx>
    math::sign inline sign (const hex::int2<cx> &x) {
        if (!x.valid ()) throw exception{} << "invalid hexidecimal string: " << x;
        
        return math::is_zero (x) ? math::zero : math::number::sign_bit_set (x) ? math::negative : math::positive;
    }
    
    dec_uint inline increment (const dec_uint &n) {
        auto x = n;
        return ++x;
    }
    
    dec_uint inline decrement (const dec_uint &n) {
        auto x = n;
        return --x;
    }
    
    dec_int inline increment (const dec_int &n) {
        auto x = n;
        return ++x;
    }
    
    dec_int inline decrement (const dec_int &n) {
        auto x = n;
        return --x;
    }
    
    template <math::number::complement c, hex_case zz>
    hex::integer<c, zz> inline increment (const hex::integer<c, zz> &n) {
        auto x = n;
        return ++x;
    }
    
    template <math::number::complement c, hex_case zz>
    hex::integer<c, zz> inline decrement (const hex::integer<c, zz> &n) {
        auto x = n;
        return --x;
    }

    template<math::number::complement a, hex_case b, math::number::complement c, hex_case d>
    bool inline identical (const hex::integer<a, b> &x, const hex::integer<c, d> &y) {
        return static_cast<std::string> (x) == static_cast<std::string> (y);
    }
}

namespace data::math {
    
    bool inline is_zero (const dec_uint &n) {
        if (!encoding::decimal::valid (n)) throw exception {} << "invalid decimal string: " << n;
        return !encoding::decimal::nonzero (n);
    }
    
    bool inline is_negative (const dec_uint &n) {
        if (!encoding::decimal::valid (n)) throw exception {} << "invalid decimal string: " << n;
        return false;
    }
    
    bool inline is_positive (const dec_uint &n) {
        if (!encoding::decimal::valid (n)) throw exception {} << "invalid decimal string: " << n;
        return encoding::decimal::nonzero (n);
    }
    
    bool inline is_zero (const dec_int &n) {
        if (!encoding::signed_decimal::valid (n)) throw exception {} << "invalid decimal string: " << n;
        return !encoding::signed_decimal::nonzero (n);
    }
    
    bool inline is_negative (const dec_int &n) {
        if (!encoding::signed_decimal::valid (n)) throw exception {} << "invalid decimal string: " << n;
        return encoding::signed_decimal::negative (n);
    }
    
    bool inline is_positive (const dec_int &n) {
        if (!encoding::signed_decimal::valid (n)) throw exception {} << "invalid decimal string: " << n;
        return encoding::signed_decimal::positive (n);
    }
    template <number::complement c, hex_case zz>
    bool inline is_positive (const hex::integer<c, zz> &z) {
        return !is_negative (z) && !is_zero (z);
    }
    
    template <hex_case cx> 
    bool inline is_zero (const hex::uint<cx> &z) {
        if (!z.valid ()) throw exception {} << "invalid hex integer: " << z;
        
        for (auto digit = z.begin () + 2; digit != z.end (); digit++) if (*digit != '0') return false;
        return true;
    }
    
    template <hex_case cx> 
    bool inline is_zero (const hex::int1<cx> &z) {
        if (!z.valid ()) throw exception {} << "invalid hex integer: " << z;
        
        for (auto digit = z.begin () + 2; digit != z.end (); digit++) if (*digit != '0') return false;
        return true;
    }
    
    template <hex_case cx> 
    bool inline is_negative (const hex::uint<cx> &) {
        return false;
    }
    
    template <hex_case cx> 
    bool inline is_negative (const hex::int1<cx> &x) {
        if (!x.valid ()) throw exception {} << "invalid hex integer: " << x;
        
        if (x.size () < 3) return false;
        return encoding::hexidecimal::digit (x[2]) > 7;
    }
    
    dec_uint inline abs<dec_uint>::operator () (const dec_uint &x) {
        if (!x.valid ()) throw exception {} << "invalid dec string: " << x;
        return x;
    }
    
    dec_uint inline abs<dec_int>::operator () (const dec_int &x) {
        if (math::is_negative (x)) return dec_uint {x.substr (1)};
        return dec_uint {x};
    }
    
    template <hex_case zz>
    hex::uint<zz> inline abs<hex::uint<zz>>::operator () (const hex::uint<zz> &x) {
        if (!x.valid ()) throw exception {} << "invalid hex string: " << x;
        return x;
    }
    
    template <hex_case zz>
    hex::uint<zz> inline abs<hex::int1<zz>>::operator () (const hex::int1<zz> &x) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        if (math::is_negative (x)) return hex::uint<zz> {-x};
        return hex::uint<zz> {x};
    }
    
    template <hex_case zz>
    hex::int2<zz> inline abs<hex::int2<zz>>::operator () (const hex::int2<zz> &x) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        return math::number::sign_bit_set (x) ? -x : math::number::trim (x);
    }
    
}

namespace data::encoding::hexidecimal {
    
    template <complement c, hex::letter_case cx> 
    integer<c, cx> inline operator ++ (integer<c, cx> &x, int) {
        integer n = x;
        ++x;
        return n;
    }
    
    template <complement c, hex::letter_case cx> 
    integer<c, cx> inline operator -- (integer<c, cx> &x, int) {
        integer n = x;
        --x;
        return n;
    }
    
    template <hex::letter_case cx>
    integer<complement::ones, cx> inline operator - (const integer<complement::ones, cx> &x) {
        if (!x.valid ()) throw exception {} << "invalid hex string: " << x;
        return increment (~x);
    }
    
    template <complement c, hex::letter_case zz> 
    integer<c, zz> inline &integer<c, zz>::operator += (const integer &i) {
        return *this = *this + i;
    }
    
    template <complement c, hex::letter_case zz> 
    integer<c, zz> inline &integer<c, zz>::operator -= (const integer &i) {
        return *this = *this - i;
    }
    
    template <complement c, hex::letter_case zz> 
    integer<c, zz> inline &integer<c, zz>::operator *= (const integer &i) {
        return *this = *this * i;
    }

    template <complement c, hex::letter_case zz> 
    integer<c, zz> inline integer<c, zz>::operator + (int64 i) const {
        return *this + integer {i};
    }
    
    template <complement c, hex::letter_case zz> 
    integer<c, zz> inline integer<c, zz>::operator - (int64 i) const {
        return *this - integer {i};
    }

    template <complement c, hex::letter_case zz> 
    integer<c, zz> inline integer<c, zz>::operator * (int64 i) const {
        return *this * integer {i};
    }
    
    template <complement c, hex::letter_case zz> 
    integer<c, zz> inline &integer<c, zz>::operator += (int64 i) {
        return *this += integer {i};
    }
    
    template <complement c, hex::letter_case zz> 
    integer<c, zz> inline &integer<c, zz>::operator -= (int64 i) {
        return *this -= integer {i};
    }
    
    template <complement c, hex::letter_case zz> 
    integer<c, zz> inline &integer<c, zz>::operator *= (int64 i) {
        return *this *= integer {i};
    }
    
    template <complement c, hex::letter_case zz> 
    integer<c, zz> inline &integer<c, zz>::operator <<= (int i) {
        return *this = *this << i;
    }
    
    template <complement c, hex::letter_case zz> 
    integer<c, zz> inline &integer<c, zz>::operator >>= (int i) {
        return *this = *this >> i;
    }
        
    template <complement c, hex::letter_case zz> 
    integer<c, zz> inline integer<c, zz>::operator / (const integer &x) const {
        return divide (x).Quotient;
    }
    
    template <complement c, hex::letter_case zz> 
    integer<c, zz> inline integer<c, zz>::operator % (const integer &x) const {
        return divide (x).Remainder;
    }
    
    namespace {
        template <hex::letter_case cx, bool is_signed, endian::order r, size_t size> 
        string<cx> write_arith (const endian::arithmetic<is_signed, r, size> &z) {
            std::stringstream ss;
            ss << "0x";
            hex::write (ss, z, cx);
            return string<cx> {ss.str ()};
        }
        
        template <complement c, hex::letter_case zz> struct write_int;
        
        template <hex::letter_case zz> struct write_int<complement::nones, zz> {
            integer<complement::nones, zz> operator () (int64 i) {
                if (i < 0) throw exception {} << "attempt to construct natural number from negative int " << i;
                return operator () (static_cast<uint64> (i));
            }

            integer<complement::nones, zz> operator () (uint64 i) {
                return integer<complement::nones, zz>
                    {write_arith<zz> (endian::arithmetic<false, endian::big, 8> {i})};
            }
        };
        
        template <hex::letter_case zz> struct write_int<complement::ones, zz> {
            integer<complement::ones, zz> operator () (uint64 i) {
                return integer<complement::nones, zz>
                    {write_arith<zz> (endian::arithmetic<false, endian::big, 8> {i})};
            }

            integer<complement::ones, zz> operator () (int64 i) {
                return integer<complement::ones, zz>
                    {write_arith<zz> (endian::arithmetic<true, endian::big, 8> {i})};
            }
        };
        
        template <hex::letter_case zz> struct write_int<complement::twos, zz> {
            integer<complement::twos, zz> operator () (uint64 i) {
                return write_uint (i);
            }

            integer<complement::twos, zz> operator () (int64 i) {
                return i < 0 ? -write_uint (static_cast<uint64> (-i)) : write_uint (static_cast<uint64> (i));
            }
            
        private:
            integer<complement::twos, zz> write_uint (uint64 i) {
                return integer<complement::twos, zz>
                    {write_arith<zz> (endian::arithmetic<false, endian::big, 8> {i})};
            } 
        };

        template <hex::letter_case zz, complement from, complement to> struct cast_complement;

        template <hex::letter_case zz> struct cast_complement<zz, complement::ones, complement::twos> {
            integer<complement::twos, zz> operator () (const integer<complement::ones, zz> &x) {
                return math::is_negative (x) ?
                    -integer<complement::twos, zz> (static_cast<string<zz>> (-x)):
                    integer<complement::twos, zz> (static_cast<string<zz>> (x));
            }
        };

        template <hex::letter_case zz> struct cast_complement<zz, complement::twos, complement::ones> {
            integer<complement::ones, zz> operator () (const integer<complement::twos, zz> &x) {
                return math::number::sign_bit_set (x) ?
                    -integer<complement::ones, zz> (static_cast<string<zz>> (-x)):
                    integer<complement::ones, zz> (static_cast<string<zz>> (x));
            }
        };
    }

    template <complement c, hex::letter_case zz>
    inline complemented_string<c, zz>::complemented_string (int64 x): complemented_string {write_int<c, zz> {} (x)} {
        *this = trim<c, zz> (*this);
    }

    template <hex::letter_case zz>
    inline complemented_string<complement::nones, zz>::complemented_string (int64 x):
        complemented_string {write_int<complement::nones, zz> {} (x)} {
        *this = trim<complement::nones, zz> (*this);
    }
    
    template <complement c, hex::letter_case cx>
    complemented_string<c, cx>::operator integer<complement (-int (c) + 3), cx> () const {
        return math::number::trim (cast_complement<cx, c, complement (-int (c) + 3)> {} (integer<c, cx> (*this)));
    }

    template <hex::letter_case cx>
    inline complemented_string<complement::nones, cx>::operator integer<complement::ones, cx> () const {
        return is_negative<complement::ones, cx> (*this) ?
            integer<complement::ones, cx> (extend<complement::nones, cx> (*this, this->size () + 2)):
            integer<complement::ones, cx> (static_cast<string<cx>> (*this));
    }

    template <hex::letter_case cx>
    inline complemented_string<complement::nones, cx>::operator integer<complement::twos, cx> () const {
        return is_negative<complement::twos, cx> (*this) ?
            integer<complement::twos, cx> (extend<complement::nones, cx> (*this, this->size () + 2)):
            integer<complement::twos, cx> (static_cast<string<cx>> (*this));
    }

    template <complement c, hex::letter_case cx>
    integer<c, cx> complemented_string<c, cx>::zero (size_t size, bool negative) {
        if constexpr (c == complement::ones) {
            if (negative) throw exception {} << "complement ones has no negative zero";
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

    template <complement c, hex::letter_case zz> 
    integer<c, zz> inline &integer<c, zz>::trim () {
        return *this = math::number::trim (*this);
    }
    
    template <complement c, hex::letter_case zz> 
    integer<c, zz> inline integer<c, zz>::trim () const {
        return math::number::trim (*this);
    }
    
}

namespace data::math::number {
    
    template <hex_case cx>
    encoding::hexidecimal::integer<number::complement::nones, cx> 
    trim (const encoding::hexidecimal::integer<number::complement::nones, cx> &x) {
        
        if (!x.valid ()) throw exception {} << "cannot trim invalid hexidecimal string: " << x;
        
        if (is_minimal (x)) return x;
        auto i = x.begin () + 2;
        while (i != x.end () && i[0] == '0' && i[1] == '0') i += 2;
        
        encoding::hexidecimal::integer<number::complement::nones, cx> n {};
        n.resize (x.end () - i + 2);
        std::copy (i, x.end (), n.begin () + 2);
        return n;
    }
    
    template <hex_case cx>
    encoding::hexidecimal::integer<number::complement::ones, cx> 
    trim (const encoding::hexidecimal::integer<number::complement::ones, cx> &x) {

        if (!x.valid ()) throw exception {} << "cannot trim invalid hexidecimal string: " << x;
        
        size_t min_size = minimal_size (x);
        
        if (min_size == x.size ()) return x;
        
        encoding::hexidecimal::integer<number::complement::ones, cx> n {};
        n.resize (min_size);
        std::copy (x.end () - min_size + 2, x.end (), n.begin () + 2);
        return n;
    }
    
    template <hex_case cx>
    encoding::hexidecimal::integer<number::complement::twos, cx> 
    trim (const encoding::hexidecimal::integer<number::complement::twos, cx> &x) {

        if (!x.valid ()) throw exception {} << "cannot trim invalid hexidecimal string: " << x;
        if (is_minimal (x)) return x;
        
        encoding::hexidecimal::integer<number::complement::twos, cx> n {};
        if (is_zero (x)) return n;
        
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
    integer<complement::twos, zz> operator ! (const integer<complement::twos, zz> &x) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        return bool (x) ?
            integer<complement::twos, zz> {std::string {"0x"}} :
            integer<complement::twos, zz> {std::string {"0x01"}};
    }

    template <hex::letter_case zz>
    integer<complement::twos, zz> operator &&
        (const integer<complement::twos, zz> &x, const integer<complement::twos, zz> &y) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        if (!y.valid ()) throw exception {} << "invalid hexidecimal string: " << y;
        return bool (x) && bool (y) ?
            integer<complement::twos, zz> {std::string {"0x01"}} :
            integer<complement::twos, zz> {std::string {"0x"}};
    }

    template <hex::letter_case zz>
    integer<complement::twos, zz> operator ||
        (const integer<complement::twos, zz> &x, const integer<complement::twos, zz> &y) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        if (!y.valid ()) throw exception {} << "invalid hexidecimal string: " << y;
        return bool (x) || bool (y) ?
            integer<complement::twos, zz> {std::string {"0x01"}} :
            integer<complement::twos, zz> {std::string {"0x"}};
    }
    
    template <hex::letter_case zz>
    integer<complement::ones, zz> operator ~ (const integer<complement::ones, zz> &x) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        if (x == std::string {"0x"}) return integer<complement::ones, zz> {std::string {"0xff"}};
        
        auto characters = hex::characters (zz);
        integer<complement::ones, zz> n {};
        n.resize (x.size ());
        for (int i = 2; i < x.size (); i++) n[i] = characters[0x0f & ~digit (x[i])];
        
        return n.trim ();
    }
    
    template <hex::letter_case zz> 
    integer<complement::twos, zz> operator - (const integer<complement::twos, zz> &x) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        if (math::is_zero (x)) return integer<complement::twos, zz> {"0x"};

        integer<complement::twos, zz> n = x;
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
            
            math::number::arithmetic::bit_and<byte> (out_d.end (), out_d.begin (), a_d.begin (), b_d.begin ());
            
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
            
            math::number::arithmetic::bit_or<byte> (out_d.end (), out_d.begin (), a_d.begin (), b_d.begin ());
            
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
            
            math::number::arithmetic::bit_xor<byte> (out_d.end (), out_d.begin (), a_d.begin (), b_d.begin ());
            
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
        
        template <complement c, hex::letter_case zz> 
        integer<c, zz> bit_and (const integer<c, zz> &a, const integer<c, zz> &b) {
            if (a.size () < b.size ()) return bit_and (b, a);
            integer<c, zz> n {};
            n.resize (a.size ());
            bit_and (n, a, math::number::extend (b, a.size ()));
            return n;
        }
        
        template <complement c, hex::letter_case zz> 
        integer<c, zz> bit_or(const integer<c, zz> &a, const integer<c, zz> &b) {
            if (a.size () < b.size ()) return bit_or (b, a);
            integer<c, zz> n {};
            n.resize (a.size ());
            bit_or (n, a, math::number::extend (b, a.size ()));
            return n;
        }
        
        template <hex::letter_case zz> 
        integer<complement::ones, zz> bit_xor (const integer<complement::ones, zz> &a, const integer<complement::ones, zz> &b) {
            if (a.size () < b.size ()) return bit_xor (b, a);
            integer<complement::ones, zz> n {};
            n.resize (a.size ());
            bit_xor (n, a, math::number::extend (b, a.size ()));
            return n;
        }
        
        template <hex::letter_case zz> 
        string<zz> shift (const string<zz> &x, int i);
        
        template <hex::letter_case zz> 
        integer<complement::nones, zz> inline bit_shift (const integer<complement::nones, zz> &x, int i) {
            return integer<complement::nones, zz> {shift (x, i)};
        }
        
        template <hex::letter_case zz> 
        integer<complement::ones, zz> bit_shift (const integer<complement::ones, zz> &x, int i);
        
        template <hex::letter_case zz> 
        integer<complement::twos, zz> inline bit_shift (const integer<complement::twos, zz> &x, int i) {
            return integer<complement::twos, zz> (bit_shift (integer<complement::ones, zz> (x), i));
        }
        
        template <complement c, hex::letter_case zz> struct add;
        
        template <hex::letter_case zz> struct add<complement::nones, zz> {
            integer<complement::nones, zz> operator () (
                const integer<complement::nones, zz> &a, 
                const integer<complement::nones, zz> &b) {
                if (a.size () < b.size ()) return add<complement::nones, zz> {} (b, a);
                integer<complement::nones, zz> n {};
                n.resize (a.size () + 2);
                plus (n, a, math::number::extend (b, a.size ()));
                return n;
            }
        };
        
        template <hex::letter_case zz> 
        integer<complement::nones, zz> minus (const integer<complement::nones, zz> &a, const integer<complement::nones, zz> &b) {
            if (b > a) return integer<complement::nones, zz> {};
            // these numbers are both trimmed, so we can expect that the size of b is less than that of a. 
            integer<complement::nones, zz> n {};
            n.resize (a.size ());
            minus (n, a, math::number::extend (b, a.size ()));
            return n;
        }
        
        template <complement c, hex::letter_case zz> struct add {
            integer<c, zz> operator () (
                const integer<c, zz> &a, 
                const integer<c, zz> &b) {

                if (a.size () < b.size ()) return add<c, zz> {} (b, a);
                
                bool an = math::is_negative (a);
                bool bn = math::is_negative (b);
                
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
        integer<complement::ones, zz> inline
        minus (const integer<complement::ones, zz> &a, const integer<complement::ones, zz> &b) {
            return add<complement::ones, zz> {} (a, -b);
        }
        
        template <hex::letter_case zz> 
        integer<complement::twos, zz> inline
        minus (const integer<complement::twos, zz> &a, const integer<complement::twos, zz> &b) {
            return add<complement::twos, zz> {} (a, -b);
        }
        
        template <complement c, hex::letter_case zz> 
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
        struct multiply<complement::nones, zz> {
            integer<complement::nones, zz> operator () (
                const integer<complement::nones, zz> &a, 
                const integer<complement::nones, zz> &b) {
                integer<complement::nones, zz> n;
                n.resize (a.size () + b.size () - 2);
                times (n, a, b);
                return n;
            }
        };
        
    }
    
    template <hex::letter_case zz> 
    std::weak_ordering inline operator <=> (const integer<complement::nones, zz> &a, const integer<complement::nones, zz> &b) {
        if (!a.valid ()) throw exception {} << "invalid hexidecimal string: " << a;
        if (!b.valid ()) throw exception {} << "invalid hexidecimal string: " << b;
        
        return N_compare (string_view {a.data () + 2, a.size () - 2}, string_view {b.data () + 2, b.size () - 2});
    }
    
    template <hex::letter_case zz> 
    std::weak_ordering operator <=> (const integer<complement::ones, zz> &a, const integer<complement::ones, zz> &b) {

        if (!a.valid ()) throw exception {} << "invalid hexidecimal string: " << a;
        if (!b.valid ()) throw exception {} << "invalid hexidecimal string: " << b;
        
        bool na = math::is_negative (a);
        bool nb = math::is_negative (b);
        
        if (na && nb) {
            auto ya = -b;
            auto yb = -a;
            return N_compare (string_view {ya.data () + 2, ya.size () - 2}, string_view {yb.data () + 2, yb.size () - 2});
        }
        
        if (!na && !nb) return N_compare (string_view {a.data () + 2, a.size () - 2}, string_view {b.data () + 2, b.size () - 2});
        return na ? std::weak_ordering::less : std::weak_ordering::greater;
    }
    
    template <hex::letter_case zz> 
    std::weak_ordering operator <=> (const integer<complement::twos, zz> &a, const integer<complement::twos, zz> &b) {

        math::sign na = sign (a);
        math::sign nb = sign (b);
        
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
    integer<complement::nones, zz> &operator ++ (integer<complement::nones, zz> &x) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        
        char remainder = N_increment (x);
        if (remainder != '0') {
            integer<complement::nones, zz> n {};
            n.resize (x.size () + 2);
            std::copy (x.begin () + 2, x.end (), n.begin () + 4);
            n[2] = '0';
            n[3] = remainder;
            x = n;
        } 
        
        return x.trim ();
    }
    
    template <hex::letter_case zz> 
    integer<complement::nones, zz> &operator -- (integer<complement::nones, zz> &x) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        
        if (math::is_zero (x)) return x;
        N_decrement (x);
        return x.trim ();
    }
    
    template <hex::letter_case zz> 
    integer<complement::ones, zz> &operator ++ (integer<complement::ones, zz> &x) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        
        bool neg = math::is_negative (x);
        char remainder = N_increment (x);
        
        if (!neg && (remainder != '0' || math::is_negative (x))) {
            integer<complement::ones, zz> n {};
            n.resize (x.size () + 2);
            std::copy (x.begin () + 2, x.end (), n.begin () + 4);
            n[2] = '0';
            n[3] = remainder;
            x = n;
        } 
        
        return x.trim ();
    }
    
    template <hex::letter_case zz> 
    integer<complement::ones, zz> &operator -- (integer<complement::ones, zz> &x) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        
        if (x == std::string ("0x")) return x = integer<complement::ones, zz> {"0xff"};
        x = math::number::extend (x, x.size () + 2);
        N_decrement (x);
        return x.trim ();
    }
    
    template <hex::letter_case zz> 
    integer<complement::twos, zz> &operator ++ (integer<complement::twos, zz> &x) {

        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        
        if (math::is_negative (x)) return x = -decrement (-x);
        if (math::is_negative_zero (x)) return x = integer<complement::twos, zz> {"0x01"};
        
        char remainder = N_increment (x);
        if (remainder != '0') {
            integer<complement::twos, zz> n;
            n.resize (x.size () + 2);
            std::copy (x.begin () + 2, x.end (), n.begin () + 4);

            n[2] = '0';
            n[3] = remainder;
            x = n;
        } else if (math::number::sign_bit_set (x)) {
            integer<complement::twos, zz> n;
            n.resize (x.size () + 2);
            std::copy (x.begin () + 2, x.end (), n.begin () + 4);

            n[2] = '0';
            n[3] = '0';
            x = n;
        }
        
        return x.trim ();
    }
    
    template <hex::letter_case zz> 
    integer<complement::twos, zz> &operator -- (integer<complement::twos, zz> &x) {

        if (math::is_zero (x)) return x = integer<complement::twos, zz> {"0x81"};
        if (math::is_negative (x)) return x = -increment (-x);
        N_decrement (x);
        return x.trim ();
    }
    
    template <complement c, hex::letter_case zz> 
    integer<c, zz> inline operator | (const integer<c, zz> &a, const integer<c, zz> &b) {
        return math::number::trim (bit_or (a, b));
    }
    
    template <complement c, hex::letter_case zz> 
    integer<c, zz> inline operator & (const integer<c, zz> &a, const integer<c, zz> &b) {
        return math::number::trim (bit_and (a, b));
    }
    
    template <hex::letter_case zz> 
    integer<complement::ones, zz> inline 
    operator^(const integer<complement::ones, zz> &a, const integer<complement::ones, zz> &b) {
        return math::number::trim (bit_xor (a, b));
    }
    
    template <complement c, hex::letter_case zz> 
    integer<c, zz> inline operator << (const integer<c, zz> &x, int i) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        
        return math::number::trim (bit_shift (x, i));
    }
    
    template <complement c, hex::letter_case zz> 
    integer<c, zz> inline operator >> (const integer<c, zz> &x, int i) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        
        return math::number::trim (bit_shift (x, -i));
    }
    
    template <complement c, hex::letter_case zz> 
    integer<c, zz> inline operator + (const integer<c, zz> &a, const integer<c, zz> &b) {
        return math::number::trim (add<c, zz> {} (math::number::trim (a), math::number::trim (b)));
    }
    
    template <complement c, hex::letter_case zz> 
    integer<c, zz> inline operator - (const integer<c, zz> &a, const integer<c, zz> &b) {
        return math::number::trim (minus (math::number::trim (a), math::number::trim (b)));
    }
    
    template <complement c, hex::letter_case zz> 
    integer<c, zz> inline operator * (const integer<c, zz> &a, const integer<c, zz> &b) {
        return math::number::trim (multiply<c, zz> {} (math::number::trim (a), math::number::trim (b)));
    }
    
}

namespace data::math {
    
    template <hex_case zz>
    bool is_positive_zero (const hex::int2<zz> &z) {
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
        bool is_negative_zero (const hex::int2<zz> &z) {
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
    bool inline is_negative (const hex::int2<zz> &x) {
        if (!x.valid ()) throw exception {} << "invalid hexidecimal string: " << x;
        return number::sign_bit_set (x) && !is_negative_zero (x);
    }
    
    template <hex_case zz>
    bool is_zero (const hex::int2<zz> &z) {
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
    bool inline sign_bit_set (const hex::int2<zz> &x) {
        return x.size () > 2 && encoding::hexidecimal::digit (x[2]) > 7;
    }
    
    template <hex_case zz>
    bool is_minimal (const hex::int1<zz> &x) {
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
    bool inline is_minimal (const hex::int2<zz> &x) {
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
    hex::int1<zz> extend (const hex::int1<zz> &x, size_t size) {
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

        char fill = is_negative (x) ? (zz == hex_case::upper ? 'F' : 'f') : '0';

        for (int zeros = 0; zeros < size - x.size (); zeros ++) {
            *i = fill;
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
        
        if (x.size () == size) return x;
        
        hex::int2<zz> n;
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
    size_t minimal_size (const hex::int1<zz> &x) {
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
    size_t minimal_size (const hex::int2<zz> &x) {
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

