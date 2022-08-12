// Copyright (c) 2019 Daniel Krawisz
// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_INTEGER
#define DATA_ENCODING_INTEGER

#include <ctre.hpp>

#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string.hpp>

#include <data/types.hpp>
#include <data/encoding/invalid.hpp>
#include <data/math/number/bytes/bytes.hpp>
#include <data/math/root.hpp>

namespace data::encoding {
    
    namespace decimal {
        static constexpr ctll::fixed_string pattern{"0|([1-9][0-9]*)"};
        
        std::string characters();
        char digit(char x);
        bool valid(string_view s);
        bool nonzero(string_view s);
        uint32 digits(string_view s);
        
        template <endian::order r> ptr<math::N_bytes<r>> read(string_view s);
        
        template <typename range> 
        std::ostream &write(std::ostream& o, range r);
        
        // a decimal string inherets from std::string but is 
        // a big number that supports standard numerical operations. 
        struct string;
        
        template <endian::order r> 
        string write(const math::number::N_bytes<r> &z);
        
        // all valid decimal strings are uniquely associated with
        // a natural number, so we can use a strong ordering. 
        std::strong_ordering operator<=>(const string &, const string &);
        
        string &operator++(string&);
        string &operator--(string&);
        
        string operator++(string&, int);
        string operator--(string&, int);
        
        string operator+(const string&, const string&);
        string operator-(const string&, const string&);
        string operator*(const string&, const string&);
            
        string operator<<(const string&, int);
        string operator>>(const string&, int);
        
        string operator|(const string&, const string&);
        string operator&(const string&, const string&);
        
        string operator/(const string&, const string&);
        string operator%(const string&, const string&);
        
    }
    
    namespace signed_decimal {
        
        static constexpr ctll::fixed_string pattern{"0|(-?[1-9][0-9]*)"};
        
        bool valid(string_view s);
        bool nonzero(string_view s);
        bool positive(string_view s);
        bool negative(string_view s);
        math::sign sign(string_view s);
        
        template <endian::order r> 
        ptr<math::Z_bytes<r>> read(string_view s);
    
        template <endian::order r> 
        std::ostream &write(std::ostream &o, const math::number::Z_bytes<r> &z);
        
        // a decimal string inherets from std::string but is 
        // a big number that supports standard numerical operations. 
        struct string;
    
        template <endian::order r> 
        string write(const math::number::Z_bytes<r> &z);
        
        // all valid decimal strings are uniquely associated with
        // a natural number, so we can use a strong ordering. 
        std::strong_ordering operator<=>(const string &, const string &);
        
        string &operator++(string&);
        string &operator--(string&);
        
        string operator++(string&, int);
        string operator--(string&, int);
    
        string operator-(const string &);
        
        string operator+(const string&, const string&);
        string operator-(const string&, const string&);
        string operator*(const string&, const string&);
            
        string operator<<(const string&, int);
        string operator>>(const string&, int);
        
        string operator|(const string&, const string&);
        string operator&(const string&, const string&);
        
        string operator/(const string&, const string&);
        
        string operator+(const string &n, const decimal::string &x);
        string operator-(const string &n, const decimal::string &x);
        string operator*(const string &n, const decimal::string &x);
        string operator|(const string &n, const decimal::string &x);
        string operator&(const string &n, const decimal::string &x);
        string operator/(const string &n, const decimal::string &x);
        
        decimal::string operator%(const string&, const decimal::string &x);
        
        string &operator+=(string &n, const decimal::string &x);
        string &operator-=(string &n, const decimal::string &x);
        string &operator*=(string &n, const decimal::string &x);
        string &operator|=(string &n, const decimal::string &x);
        string &operator&=(string &n, const decimal::string &x);
        string &operator/=(string &n, const decimal::string &x);
    }
    
    namespace hexidecimal {
        static constexpr ctll::fixed_string pattern{"0x((([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*))"};
        static constexpr ctll::fixed_string zero_pattern{"0x(00)*"};
        
        bool valid(string_view s);
        bool zero(string_view s);
        bool nonzero(string_view s);
        uint32 digits(string_view s);
        char digit(char x);
        
        hex::letter_case read_case(string_view s);
    
        template <endian::order r> 
        ptr<oriented<r, byte>> read(string_view s);
        
        template <endian::order r> 
        std::ostream inline &write(std::ostream &o, const oriented<r, byte> &d, hex::letter_case q);
        
        struct string;
        
        template <endian::order r> 
        string write(const oriented<r, byte> &z, hex::letter_case q);
        
    }
    
    namespace natural {
        static constexpr ctll::fixed_string pattern{"0|([1-9][0-9]*)|(0x((([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*)))"};

        static constexpr ctll::fixed_string zero_pattern{"0|0x(00)*"};
        
        bool valid(string_view s);
        bool zero(string_view s);
        bool nonzero(string_view s);
        uint32 digits(string_view s);
        
        template <endian::order r> ptr<math::N_bytes<r>> read(string_view s);
        
    }
    
    namespace integer {
        static constexpr ctll::fixed_string pattern{"0|(-?[1-9][0-9]*)|(0x((([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*)))"};
        
        static constexpr ctll::fixed_string zero_pattern {"0|0x(00)*"};
        
        static constexpr ctll::fixed_string negative_pattern{"(-(0*[1-9][0-9]*))|0x(([8-9a-f][0-9a-f]([0-9a-f][0-9a-f])*)|([8-9A-F][0-9A-F]([0-9A-F][0-9A-F])*))"};
        
        bool valid(string_view s);
        bool negative(string_view s);
        bool zero(string_view s);
        bool nonzero(string_view s);
        uint32 digits(string_view s);
        
        template <endian::order r> 
        ptr<math::number::Z_bytes<r>> read(string_view s);
        
    }
    
}

namespace data {
    using dec_uint = encoding::decimal::string;
    using dec_int = encoding::signed_decimal::string;
    using hex_uint = encoding::hexidecimal::string;
    
    math::sign sign(const dec_uint&);
    math::sign sign(const dec_int&);
    math::sign sign(const hex_uint&);
    
    dec_uint increment(const dec_uint&);
    dec_uint decrement(const dec_uint&);
    
    dec_int increment(const dec_int&);
    dec_int decrement(const dec_int&);
    
    hex_uint increment(const hex_uint&);
    hex_uint decrement(const hex_uint&);
    
}

namespace data::math {
    
    template <> struct abs<dec_uint> {
        dec_uint operator()(const dec_uint&);
    };
    
    template <> struct abs<dec_int> {
        dec_uint operator()(const dec_int&);
    };
    
    template <> struct abs<hex_uint> {
        hex_uint operator()(const hex_uint&);
    };
    
}

namespace data::math::number {
    
    bool is_zero(const hex_uint &);
    bool is_negative(const hex_uint &);
    bool is_positive(const hex_uint &);
    
    bool is_zero(const dec_uint &);
    bool is_negative(const dec_uint &);
    bool is_positive(const dec_uint &);
    
    bool is_zero(const dec_int &);
    bool is_negative(const dec_int &);
    bool is_positive(const dec_int &);
    
}

namespace data::encoding::decimal {
    
    struct string : std::string {
        string() : std::string{"0"} {};
        
        explicit string(const std::string &x) : std::string{decimal::valid(x) ? x : ""} {}
        explicit string(std::string &&x) : std::string{x} {}
        string(uint64);
        
        bool valid() const {
            return decimal::valid(*this);
        }
        
        static string read(string_view x);
        
        string &operator+=(const string&);
        string &operator-=(const string&);
        string &operator*=(const string&);
        
        string &operator<<=(int);
        string &operator>>=(int);
        
        string &operator|=(const string&) const;
        string &operator&=(const string&) const;
        
        math::division<string, uint64> divide(uint64) const;
        
        bool operator==(uint64) const;
        std::strong_ordering operator<=>(uint64) const;
        
        string operator+(uint64) const;
        string operator-(uint64) const;
        string operator*(uint64) const;
        
        string &operator+=(uint64);
        string &operator-=(uint64);
        string &operator*=(uint64);
        
        explicit operator double() const;
    };
    
    signed_decimal::string operator-(const string &);
    
}

namespace data::encoding::signed_decimal {
    
    struct string : std::string {
        string() : std::string{"0"} {};
        explicit string(const std::string &x) : std::string{signed_decimal::valid(x) ? x : ""} {}
        explicit string(std::string &&x) : std::string{x} {}
        string(int64);
        
        bool valid() const {
            return signed_decimal::valid(*this);
        }
        
        static string read(string_view x);
        
        string &operator+=(const string&);
        string &operator-=(const string&);
        string &operator*=(const string&);
        
        string &operator<<=(int);
        string &operator>>=(int);
        
        string &operator|=(const string&) const;
        string &operator&=(const string&) const;
        
        math::division<string, int64> divide(int64) const;
        
        bool operator==(int64) const;
        std::strong_ordering operator<=>(int64) const;
        
        string operator+(int64)const;
        string operator-(int64) const;
        string operator*(int64) const;
        
        string &operator+=(int64);
        string &operator-=(int64);
        string &operator*=(int64);
        
        explicit operator double() const;
    };
    
}

namespace data::encoding::hexidecimal {
    
    struct string : std::string {
        
        string() : std::string{"0x"} {};
        explicit string(string_view x) : std::string{hexidecimal::valid(x) ? x : ""} {}
        explicit string(std::string &&x) : std::string{x} {}
        string(uint64);
        
        static string read(string_view x);
        
        bool valid() const {
            return hexidecimal::valid(*this);
        }
        
        bool operator==(const string&) const;
        std::weak_ordering operator<=>(const string&) const;
        
        string& operator++();
        string& operator--();
        
        string operator++(int);
        string operator--(int);
        
        string operator+(const string&) const;
        string operator-(const string&) const;
        string operator*(const string&) const;
        
        string operator<<(int) const;
        string operator>>(int) const;
        
        string& operator+=(const string&);
        string& operator-=(const string&);
        string& operator*=(const string&);
        
        string& operator<<=(int);
        string& operator>>=(int);
        
        math::division<string, uint64> divide(uint64) const;
        
    };
    
}

namespace data::math {
    
    template <uint64 pow> 
    struct root<dec_uint, pow> {
        set<dec_uint> operator()(const dec_uint& n);
    };
    
    template <uint64 pow> 
    struct root<dec_int, pow> {
        set<dec_int> operator()(const dec_int& n);
    };
    
    template <uint64 pow> 
    struct root<hex_uint, pow> {
        set<hex_uint> operator()(const hex_uint& n);
    };
    
}

namespace data::encoding::decimal {
    
    std::string inline characters() {
        return "0123456789";
    }
    
    char inline digit(char x) {
        return x < '0' || x > '9' ? -1 : x - '0';
    }
        
    bool inline valid(string_view s) {
        return ctre::match<pattern>(s);
    } 
    
    bool inline nonzero(string_view s) {
        return valid(s) && s[0] != '0';
    }
    
    uint32 inline digits(string_view s) {
        return valid(s) ? s.size() : 0;
    }
    
    string inline &string::operator+=(const string &x) {
        return *this = *this + x;
    }
    
    string inline &string::operator-=(const string &x) {
        return *this = *this - x;
    }
    
    string inline &string::operator*=(const string &x) {
        return *this = *this * x;
    }
    
    string inline &string::operator<<=(int x) {
        return *this = *this << x;
    }
    
    string inline &string::operator>>=(int x) {
        return *this = *this >> x;
    }
    
    string inline string::operator+(uint64 x) const {
        return *this + string{x};
    }
    
    string inline string::operator-(uint64 x) const {
        return *this - string{x};
    }
    
    string inline string::operator*(uint64 x) const {
        return *this * string{x};
    }
    
    string inline &string::operator+=(uint64 x) {
        return *this += string{x};
    }
    
    string inline &string::operator-=(uint64 x) {
        return *this -= string{x};
    }
    
    string inline &string::operator*=(uint64 x) {
        return *this *= string{x};
    }
    
}

namespace data::encoding::signed_decimal {
    
    bool inline valid(string_view s) {
        return ctre::match<pattern>(s);
    }
    
    bool inline nonzero(string_view s) {
        return valid(s) && s[0] != '0';
    }
    
    bool inline negative(string_view s) {
        return valid(s) && s[0] == '-';
    }
    
    bool inline positive(string_view s) {
        return valid(s) && s[0] != '-' && s[0] != '0';
    }
    
    math::sign inline sign(string_view s) {
        if (!valid(s)) throw std::invalid_argument{std::string{"invalid decimal string: "} + std::string{s}};
        return s[0] == '-' ? math::negative : s[0] == '0' ? math::zero : math::positive;
    }
    
    template <endian::order r> 
    std::ostream inline &write(std::ostream &o, const math::number::Z_bytes<r> &z) {
        if (math::number::is_negative(z)) o << '-';
        return decimal::write(o, data::abs(z));
    }
    
    string inline string::operator+(int64 x) const {
        return *this + string{x};
    }
    
    string inline string::operator-(int64 x) const {
        return *this - string{x};
    }
    
    string inline string::operator*(int64 x) const {
        return *this * string{x};
    }
    
    string inline &string::operator+=(int64 x) {
        return *this += string{x};
    }
    
    string inline &string::operator-=(int64 x) {
        return *this += string{x};
    }
    
    string inline &string::operator*=(int64 x) {
        return *this += string{x};
    }
    
    string inline operator+(const string &n, const decimal::string &x) {
        return n + string{static_cast<const std::string>(x)};
    }
    
    string inline operator-(const string &n, const decimal::string &x) {
        return n - string{static_cast<const std::string>(x)};
    }
    
    string inline operator*(const string &n, const decimal::string &x) {
        return n * string{static_cast<const std::string>(x)};
    }
    
    string inline operator|(const string &n, const decimal::string &x) {
        return n | string{static_cast<const std::string>(x)};
    }
    
    string inline operator&(const string &n, const decimal::string &x) {
        return n & string{static_cast<const std::string>(x)};
    }
    
    string inline &operator+=(string &n, const decimal::string &x) {
        return n += string{static_cast<const std::string>(x)};
    }
    
    string inline &operator-=(string &n, const decimal::string &x) {
        return n -= string{static_cast<const std::string>(x)};
    }
    
    string inline &operator*=(string &n, const decimal::string &x) {
        return n *= string{static_cast<const std::string>(x)};
    }
    
    string inline &operator|=(string &n, const decimal::string &x) {
        return n |= string{static_cast<const std::string>(x)};
    }
    
    string inline &operator&=(string &n, const decimal::string &x) {
        return n &= string{static_cast<const std::string>(x)};
    }
    
    string inline operator/(const string &n, const decimal::string &x) {
        return n / string{static_cast<const std::string>(x)};
    }
    
}

namespace data::encoding::hexidecimal {
    
    bool inline valid(string_view s) {
        return ctre::match<pattern>(s);
    } 
    
    bool inline zero(string_view s) {
        return ctre::match<zero_pattern>(s);
    }
    
    bool inline nonzero(string_view s) {
        return valid(s) && !ctre::match<zero_pattern>(s);
    }
    
    uint32 inline digits(string_view s) {
        return valid(s) ? s.size() - 2 : 0;
    }
    
    char inline digit(char x) {
        if (x >= '0' && x <= '9') return x - '0';
        if (x >= 'A' && x <= 'F') return x - 'A' + 10;
        if (x >= 'a' && x <= 'f') return x - 'a' + 10;
        return -1;
    }
    
    template <endian::order r> 
    ptr<oriented<r, byte>> read(string_view s) {
        if (!valid(s)) return nullptr;
        
        ptr<oriented<r, byte>> n = std::make_shared<oriented<r, byte>>();
        n->resize((s.size() - 2) / 2);
        boost::algorithm::unhex(s.begin() + 2, s.end(), n->words().rbegin());
        
        return n;
    }
    
    template <endian::order r> 
    std::ostream inline &write(std::ostream &o, const oriented<r, byte> &d, hex::letter_case q) {
        o << "0x"; 
        return encoding::hex::write(o, d.words().reverse(), q);
    }
    
    template <endian::order r> 
    string write(const oriented<r, byte> &z, hex::letter_case q) {
        std::stringstream ss;
        write(ss, z, q);
        return string{ss.str()};
    }
    
}

namespace data::encoding::natural {
    
    bool inline valid(string_view s) {
        return ctre::match<pattern>(s);
    } 
    
    bool inline zero(string_view s) {
        return ctre::match<zero_pattern>(s);
    }
    
    bool inline nonzero(string_view s) {
        return valid(s) && ! ctre::match<zero_pattern>(s);
    }
    
    uint32 inline digits(string_view s) {
        return std::max(decimal::digits(s), hexidecimal::digits(s));
    }
    
}

namespace data::encoding::integer {
    
    bool inline valid(string_view s) {
        return ctre::match<pattern>(s);
    } 
    
    bool inline negative(string_view s) {
        return ctre::match<negative_pattern>(s);
    }
    
    bool inline zero(string_view s) {
        return ctre::match<zero_pattern>(s);
    }
    
    bool inline nonzero(string_view s) {
        return valid(s) && ! ctre::match<zero_pattern>(s);
    }
    
    uint32 inline digits(string_view s) {
        return negative(s) ? natural::digits(s.substr(1, s.size() - 1)) : natural::digits(s);
    }
    
    template <endian::order r> 
    ptr<math::number::Z_bytes<r>> read(string_view s) {
        if (!valid(s)) return nullptr;
        
        if (hexidecimal::valid(s)) {
            auto z = hexidecimal::read<r>(s);
            if (z == nullptr) return nullptr;
            auto x = std::make_shared<math::number::Z_bytes<r>>();
            x->resize(z->size());
            std::copy(z->begin(), z->end(), x->begin());
            return x;
        }
        
        auto z = signed_decimal::read<r>(s);
        if (z == nullptr) return nullptr;
        return std::make_shared<math::number::Z_bytes<r>>(math::number::Z_bytes<r>(*z));
    }
    
}

namespace data::math::number {
    
    bool inline is_zero(const hex_uint &n) {
        if (!encoding::hexidecimal::valid(n)) throw std::invalid_argument{std::string{"invalid hexidecimal string: "} + std::string{n}};
        return !encoding::hexidecimal::nonzero(n);
    }
    
    bool inline is_negative(const hex_uint &n) {
        if (!encoding::hexidecimal::valid(n)) throw std::invalid_argument{std::string{"invalid hexidecimal string: "} + std::string{n}};
        return false;
    }
    
    bool inline is_positive(const hex_uint &n) {
        if (!encoding::hexidecimal::valid(n)) throw std::invalid_argument{std::string{"invalid hexidecimal string: "} + std::string{n}};
        return encoding::hexidecimal::nonzero(n);
    }
    
    bool inline is_zero(const dec_uint &n) {
        if (!encoding::decimal::valid(n)) throw std::invalid_argument{std::string{"invalid decimal string: "} + std::string{n}};
        return !encoding::decimal::nonzero(n);
    }
    
    bool inline is_negative(const dec_uint &n) {
        if (!encoding::decimal::valid(n)) throw std::invalid_argument{std::string{"invalid decimal string: "} + std::string{n}};
        return false;
    }
    
    bool inline is_positive(const dec_uint &n) {
        if (!encoding::decimal::valid(n)) throw std::invalid_argument{std::string{"invalid decimal string: "} + std::string{n}};
        return encoding::decimal::nonzero(n);
    }
    
    bool inline is_zero(const dec_int &n) {
        if (!encoding::signed_decimal::valid(n)) throw std::invalid_argument{std::string{"invalid decimal string: "} + std::string{n}};
        return !encoding::signed_decimal::nonzero(n);
    }
    
    bool inline is_negative(const dec_int &n) {
        if (!encoding::signed_decimal::valid(n)) throw std::invalid_argument{std::string{"invalid decimal string: "} + std::string{n}};
        return encoding::signed_decimal::negative(n);
    }
    
    bool inline is_positive(const dec_int &n) {
        if (!encoding::signed_decimal::valid(n)) throw std::invalid_argument{std::string{"invalid decimal string: "} + std::string{n}};
        return encoding::signed_decimal::positive(n);
    }
    
}

namespace data {
    
    math::sign inline sign(const dec_uint &n) {
        if (!encoding::decimal::valid(n)) throw std::invalid_argument{std::string{"invalid decimal string: "} + std::string{n}};
        return encoding::decimal::nonzero(n) ? math::positive : math::zero;
    }
    
    math::sign inline sign(const dec_int &n) {
        return encoding::signed_decimal::sign(n);
    }
    
    math::sign inline sign(const hex_uint &n) {
        if (!encoding::hexidecimal::valid(n)) throw std::invalid_argument{std::string{"invalid hexidecimal string: "} + std::string{n}};
        return encoding::hexidecimal::nonzero(n) ? math::positive : math::zero;
    }
    
    dec_uint inline increment(const dec_uint &n) {
        auto x = n;
        return ++x;
    }
    
    dec_uint inline decrement(const dec_uint &n) {
        auto x = n;
        return --x;
    }
    
    dec_int inline increment(const dec_int &n) {
        auto x = n;
        return ++x;
    }
    
    dec_int inline decrement(const dec_int &n) {
        auto x = n;
        return --x;
    }
    
    hex_uint inline increment(const hex_uint &n) {
        auto x = n;
        return ++x;
    }
    
    hex_uint inline decrement(const hex_uint &n) {
        auto x = n;
        return --x;
    }
    
}

namespace data::math {
    
    dec_uint inline abs<dec_uint>::operator()(const dec_uint &x) {
        return x;
    }
    
    dec_uint inline abs<dec_int>::operator()(const dec_int &x) {
        if (encoding::signed_decimal::negative(x)) return dec_uint{x.substr(1)};
        return dec_uint{x};
    }
    
    hex_uint inline abs<hex_uint>::operator()(const hex_uint &x) {
        return x;
    }
    
}

#endif

