// Copyright (c) 2019 Katrina Swales
// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_INTEGER
#define DATA_ENCODING_INTEGER

#include <ctre.hpp>

#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string.hpp>

#include <data/types.hpp>
#include <data/encoding/invalid.hpp>
#include <data/math/number/complement.hpp>
#include <data/math/number/bytes/bytes.hpp>

namespace data::encoding {
    
    namespace decimal {
        // decimal strings must begin with 1 - 9 unless they are "0"
        static constexpr ctll::fixed_string pattern{"0|([1-9][0-9]*)"};
        
        bool valid(string_view s);
        
        const std::string &characters();
        
        char digit(char x);
        
        uint32 count_digits(string_view s);
        
        template <endian::order r> ptr<math::number::N_bytes<r>> read(string_view s);
        
        template <endian::order r> std::ostream &write(std::ostream &, const math::number::N_bytes<r> &);
        
        // a decimal string inherets from std::string but is 
        // a big number that supports standard numerical operations. 
        struct string;
        
        template <endian::order r> 
        string write(const math::number::N_bytes<r> &z);
        
    }
    
    namespace signed_decimal {
        
        static constexpr ctll::fixed_string pattern{"0|(-?[1-9][0-9]*)"};
        
        bool inline valid(string_view s) {
            return ctre::match<pattern>(s);
        }
        
        template <endian::order r> 
        ptr<math::Z_bytes<r>> read(string_view s);
    
        template <endian::order r> 
        std::ostream inline &write(std::ostream &o, const math::number::Z_bytes<r> &z) {
            if (math::number::is_negative(z)) o << '-';
            return decimal::write(o, data::abs(z));
        }
        
        // a decimal string inherets from std::string but is 
        // a big number that supports standard numerical operations. 
        struct string;
    
        template <endian::order r> 
        string inline write(const math::number::Z_bytes<r> &z);
    }
    
    namespace hexidecimal {
        static constexpr ctll::fixed_string pattern{"0x((([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*))"};
        static constexpr ctll::fixed_string zero_pattern{"0x(00)*"};
        static constexpr ctll::fixed_string lower_case_pattern{"0x([0-9a-f][0-9a-f])*"};
        static constexpr ctll::fixed_string upper_case_pattern{"0x([0-9A-F][0-9A-F])*"};
        
        bool inline valid(string_view s, hex::letter_case cx = hex::unknown) {
            return cx == hex::unknown ? 
                ctre::match<pattern>(s) : 
                cx == hex::lower ? 
                    ctre::match<lower_case_pattern>(s) : 
                    ctre::match<upper_case_pattern>(s);
        } 
        
        bool inline zero(string_view s) {
            return ctre::match<zero_pattern>(s);
        }
        
        bool inline nonzero(string_view s) {
            return valid(s) && !ctre::match<zero_pattern>(s);
        }
        
        uint32 inline count_digits(string_view s) {
            return valid(s) ? s.size() - 2 : 0;
        }
        
        char inline digit(char x) {
            if (x >= '0' && x <= '9') return x - '0';
            if (x >= 'A' && x <= 'F') return x - 'A' + 10;
            if (x >= 'a' && x <= 'f') return x - 'a' + 10;
            return -1;
        }
        
        hex::letter_case read_case(string_view s);
    
        template <endian::order r> 
        ptr<oriented<r, byte>> read(string_view s) {
            if (!valid(s)) return nullptr;
            
            ptr<oriented<r, byte>> n = std::make_shared<oriented<r, byte>>();
            n->resize((s.size() - 2) / 2);
            boost::algorithm::unhex(s.begin() + 2, s.end(), n->words().rbegin());
            
            return n;
        }
        
        hex::letter_case read_case(string_view s);
    
        template <endian::order r> 
        ptr<oriented<r, byte>> read(string_view s) {
            if (!valid(s)) return nullptr;
            
            ptr<oriented<r, byte>> n = std::make_shared<oriented<r, byte>>();
            n->resize((s.size() - 2) / 2);
            boost::algorithm::unhex(s.begin() + 2, s.end(), n->words().rbegin());
            
            return n;
        }
        
        template <endian::order r> 
        std::ostream inline &write(std::ostream &o, const oriented<r, byte> &d, hex::letter_case q = hex::lower) {
            o << "0x"; 
            return encoding::hex::write(o, d.words().reverse(), q);
        }
        
        template <hex::letter_case cx> struct string : std::string {
            string() : string{"0x"} {}
            explicit string(const std::string &x) : std::string{hexidecimal::valid(x) ? x : ""} {}
            explicit string(std::string &&x) : string{x} {}
            bool valid() const {
                return hexidecimal::valid(*this, cx);
            }
        };
    
        template <endian::order r> 
        std::ostream &write(std::ostream &, const math::number::Z_bytes<r>&) {
            throw method::unimplemented{"string hexidecimal::write(Z_bytes)"};
        }
        
        template <hex::letter_case cx, endian::order r> 
        string<cx> inline write(const math::number::Z_bytes<r> &z) {
            std::stringstream ss;
            write(ss, z);
            return {ss.str()};
        }
        
        template <hex::letter_case cx, endian::order r> 
        string<cx> write(const oriented<r, byte> &z, hex::letter_case q = hex::lower) {
            std::stringstream ss;
            write(ss, z, q);
            return ss.str();
        }
        
    }
    
    namespace natural {
        static constexpr ctll::fixed_string pattern{"0|([1-9][0-9]*)|(0x((([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*)))"};
        
        static constexpr ctll::fixed_string zero_pattern{"0|0x(00)*"};
        
        bool inline valid(string_view s) {
            return ctre::match<pattern>(s);
        } 
        
        bool inline zero(string_view s) {
            return ctre::match<zero_pattern>(s);
        }
        
        bool inline nonzero(string_view s) {
            return valid(s) && ! ctre::match<zero_pattern>(s);
        }
        
        uint32 inline count_digits(string_view s) {
            return std::max(decimal::count_digits(s), hexidecimal::count_digits(s));
        }
        
        template <endian::order r> ptr<math::N_bytes<r>> read(string_view s);
        
    }
    
    namespace integer {
        static constexpr ctll::fixed_string pattern{"0|(-?[1-9][0-9]*)|(0x((([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*)))"};
        
        static constexpr ctll::fixed_string zero_pattern {"0|0x(00)*"};
        
        static constexpr ctll::fixed_string negative_pattern{"(-(0*[1-9][0-9]*))|0x(([8-9a-f][0-9a-f]([0-9a-f][0-9a-f])*)|([8-9A-F][0-9A-F]([0-9A-F][0-9A-F])*))"};
        
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
        
        uint32 inline count_digits(string_view s) {
            return negative(s) ? natural::count_digits(s.substr(1, s.size() - 1)) : natural::count_digits(s);
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
    
}

namespace data::encoding::decimal {
    
    struct string : std::string {
        string();
        explicit string(const std::string&);
        explicit string(std::string &&x) : std::string{x} {}
        string(uint64);
        
        bool valid() const {
            return decimal::valid(*this);
        }
        
        // all valid decimal strings are uniquely associated with
        // a natural number, so we can use a strong ordering. 
        std::strong_ordering operator<=>(const string&) const;
        
        string &operator++();
        string &operator--();
        
        string operator++(int);
        string operator--(int);
        
        string operator+(const string&) const;
        string operator-(const string&) const;
        string operator*(const string&) const;
            
        string operator<<(int) const;
        string operator>>(int) const;
        
        string operator|(const string&) const;
        string operator&(const string&) const;
        
        string &operator+=(const string&);
        string &operator-=(const string&);
        string &operator*=(const string&);
        
        string &operator<<=(int);
        string &operator>>=(int);
        
        string &operator|=(const string&) const;
        string &operator&=(const string&) const;
        
        math::division<string, uint64> divide(uint64) const;
        
        string operator/(const string&) const;
        string operator%(const string&) const;
        
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
}
    
namespace data::encoding::signed_decimal {
    
    struct string : std::string {
        string();
        explicit string(const std::string&);
        explicit string(std::string &&x) : std::string{x} {}
        string(int64);
        
        bool valid() const {
            return signed_decimal::valid(*this);
        }
        
        // all valid decimal strings are uniquely associated with
        // a natural number, so we can use a strong ordering. 
        std::strong_ordering operator<=>(const string&) const;
        
        string &operator++();
        string &operator--();
        
        string operator++(int);
        string operator--(int);
        
        string operator+(const string&) const;
        string operator-(const string&) const;
        string operator*(const string&) const;
        
        string operator<<(int) const;
        string operator>>(int) const;
        
        string operator|(const string&) const;
        string operator&(const string&) const;
        
        string &operator+=(const string&);
        string &operator-=(const string&);
        string &operator*=(const string&);
        
        string &operator<<=(int);
        string &operator>>=(int);
        
        string &operator|=(const string&) const;
        string &operator&=(const string&) const;
        
        math::division<string, uint64> divide(uint64) const;
        
        string operator/(const string&) const;
        string operator%(const string&) const;
        
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
    
    template <endian::order r> 
    string inline write(const math::number::Z_bytes<r> &z) {
        std::stringstream ss;
        write(ss, z);
        return string{ss.str()};
    }
}

namespace data::encoding::hexidecimal {
    template <math::number::complement, hex::letter_case cx> struct integer;
    
    template <hex::letter_case cx>
    std::weak_ordering operator<=>(const integer<math::number::nones, cx>&, const integer<math::number::nones, cx>&);
    
    template <hex::letter_case cx>
    std::weak_ordering operator<=>(const integer<math::number::ones, cx>&, const integer<math::number::ones, cx>&);
    
    template <hex::letter_case cx>
    std::weak_ordering operator<=>(const integer<math::number::twos, cx>&, const integer<math::number::twos, cx>&);
    
    template <hex::letter_case cx>
    integer<math::number::nones, cx> &operator++(integer<math::number::nones, cx>&);
    
    template <hex::letter_case cx>
    integer<math::number::ones, cx> &operator++(integer<math::number::ones, cx>&);
    
    template <hex::letter_case cx>
    integer<math::number::twos, cx> &operator++(integer<math::number::twos, cx>&);
    
    template <hex::letter_case cx>
    integer<math::number::nones, cx> &operator--(integer<math::number::nones, cx>&);
    
    template <hex::letter_case cx>
    integer<math::number::ones, cx> &operator--(integer<math::number::ones, cx>&);
    
    template <hex::letter_case cx>
    integer<math::number::twos, cx> &operator--(integer<math::number::twos, cx>&);
    
    template <math::number::complement c, hex::letter_case cx>
    integer<c, cx> operator++(integer<c, cx>&, int);
    
    template <math::number::complement c, hex::letter_case cx>
    integer<c, cx> operator--(integer<c, cx>&, int);
    
    template <math::number::complement c, hex::letter_case cx>
    integer<c, cx> operator+(const integer<c, cx>&, const integer<c, cx>&);
    
    template <math::number::complement c, hex::letter_case cx>
    integer<c, cx> operator-(const integer<c, cx>&, const integer<c, cx>&);
    
    template <math::number::complement c, hex::letter_case cx>
    integer<c, cx> operator*(const integer<c, cx>&, const integer<c, cx>&);
    
    template <math::number::complement c, hex::letter_case cx>
    integer<c, cx> operator|(const integer<c, cx>&, const integer<c, cx>&);
    
    template <math::number::complement c, hex::letter_case cx>
    integer<c, cx> operator&(const integer<c, cx>&, const integer<c, cx>&);
    
    template <math::number::complement c, hex::letter_case cx>
    integer<c, cx> operator<<(const integer<c, cx>&, int);
    
    template <math::number::complement c, hex::letter_case cx>
    integer<c, cx> operator>>(const integer<c, cx>&, int);
    
    template <hex::letter_case cx> 
    integer<math::number::ones, cx> operator-(const integer<math::number::ones, cx>&);
    
    template <hex::letter_case cx> 
    integer<math::number::twos, cx> operator-(const integer<math::number::twos, cx>&);
    
    template <hex::letter_case cx> 
    integer<math::number::ones, cx> operator~(const integer<math::number::ones, cx>&);
    
    template <hex::letter_case cx> integer<math::number::ones, cx> operator^(
        const integer<math::number::ones, cx>&, 
        const integer<math::number::ones, cx>&);
    
    template <math::number::complement, hex::letter_case cx>
    struct integer : string<cx> {
        using string<cx>::string;
        
        integer() : string<cx>{"0x"} {}
        integer(int64);
        
        integer &operator+=(const integer&);
        integer &operator-=(const integer&);
        integer &operator*=(const integer&);
        
        integer &operator<<=(int);
        integer &operator>>=(int);
        
        math::division<integer, uint64> divide(uint64) const;
        
        integer operator/(const integer&) const;
        integer operator%(const integer&) const;
        
        bool operator==(int64) const;
        std::weak_ordering operator<=>(int64) const;
        
        integer operator+(int64) const;
        integer operator-(int64) const;
        integer operator*(int64) const;
        
        integer &operator+=(int64);
        integer &operator-=(int64);
        integer &operator*=(int64);
        
        explicit operator double() const;
        
        integer &trim();
        integer trim() const;
        
    };
}

namespace data {
    
    template <encoding::hex::letter_case zz> math::sign sign(const encoding::hexidecimal::integer<math::number::nones, zz>&);
    template <encoding::hex::letter_case zz> math::sign sign(const encoding::hexidecimal::integer<math::number::ones, zz>&);
    template <encoding::hex::letter_case zz> math::sign sign(const encoding::hexidecimal::integer<math::number::twos, zz>&);

    math::sign sign(const encoding::decimal::string&);
}

namespace data::math {
    template <number::complement c, encoding::hex::letter_case zz>
    encoding::hexidecimal::integer<c, zz> increment(const encoding::hexidecimal::integer<c, zz>&);
    template <number::complement c, encoding::hex::letter_case zz>
    encoding::hexidecimal::integer<c, zz> decrement(const encoding::hexidecimal::integer<c, zz>&);
    
    template <encoding::hex::letter_case cx> struct abs<encoding::hexidecimal::integer<number::nones, cx>> {
        encoding::hexidecimal::integer<number::nones, cx> operator()(const encoding::hexidecimal::integer<number::nones, cx>&);
    };
    
    template <encoding::hex::letter_case cx> struct abs<encoding::hexidecimal::integer<number::ones, cx>> {
        encoding::hexidecimal::integer<number::ones, cx> operator()(const encoding::hexidecimal::integer<number::ones, cx>&);
    };
    
    template <encoding::hex::letter_case cx> struct abs<encoding::hexidecimal::integer<number::twos, cx>> {
        encoding::hexidecimal::integer<number::twos, cx> operator()(const encoding::hexidecimal::integer<number::twos, cx>&);
    };
    
    
    encoding::decimal::string increment(const encoding::decimal::string&);
    encoding::decimal::string decrement(const encoding::decimal::string&);
    
    template <> struct abs<encoding::decimal::string> {
        encoding::decimal::string operator()(const encoding::decimal::string&);
    };
}

namespace data::math::number {
    bool is_zero(const encoding::decimal::string &);
    bool is_negative(const encoding::decimal::string &);
    bool is_positive(const encoding::decimal::string &);
    
    template <encoding::hex::letter_case cx> 
    bool is_zero(const encoding::hexidecimal::integer<nones, cx> &);
    
    template <encoding::hex::letter_case cx> 
    bool is_zero(const encoding::hexidecimal::integer<ones, cx> &);
    
    template <encoding::hex::letter_case cx> 
    bool is_zero(const encoding::hexidecimal::integer<twos, cx> &);
    
    template <encoding::hex::letter_case cx> 
    bool is_negative(const encoding::hexidecimal::integer<nones, cx> &);
    
    template <encoding::hex::letter_case cx> 
    bool is_negative(const encoding::hexidecimal::integer<ones, cx> &);
    
    template <encoding::hex::letter_case cx> 
    bool is_negative(const encoding::hexidecimal::integer<twos, cx> &);
    
    template <complement c, encoding::hex::letter_case cx> 
    bool is_positive(const encoding::hexidecimal::integer<c, cx> &);
    
    template <encoding::hex::letter_case cx> 
    bool is_positive_zero(const encoding::hexidecimal::integer<twos, cx> &);
    
    template <encoding::hex::letter_case cx> 
    bool is_negative_zero(const encoding::hexidecimal::integer<twos, cx> &);
    
    template <encoding::hex::letter_case cx> 
    bool sign_bit_set(const encoding::hexidecimal::integer<twos, cx> &);
    
    template <encoding::hex::letter_case cx> 
    bool is_minimal(const encoding::hexidecimal::integer<nones, cx> &);
    
    template <encoding::hex::letter_case cx> 
    bool is_minimal(const encoding::hexidecimal::integer<ones, cx> &);
    
    template <encoding::hex::letter_case cx> 
    bool is_minimal(const encoding::hexidecimal::integer<twos, cx> &);
    
    template <encoding::hex::letter_case cx> 
    size_t minimal_size(const encoding::hexidecimal::integer<nones, cx> &);
    
    template <encoding::hex::letter_case cx> 
    size_t minimal_size(const encoding::hexidecimal::integer<ones, cx> &);
    
    template <encoding::hex::letter_case cx> 
    size_t minimal_size(const encoding::hexidecimal::integer<twos, cx> &);
    
    template <encoding::hex::letter_case cx> 
    encoding::hexidecimal::integer<nones, cx> extend(const encoding::hexidecimal::integer<nones, cx> &, size_t);
    
    template <encoding::hex::letter_case cx> 
    encoding::hexidecimal::integer<ones, cx> extend(const encoding::hexidecimal::integer<ones, cx> &, size_t);
    
    template <encoding::hex::letter_case cx> 
    encoding::hexidecimal::integer<twos, cx> extend(const encoding::hexidecimal::integer<twos, cx> &, size_t);
    
    template <encoding::hex::letter_case cx> 
    encoding::hexidecimal::integer<nones, cx> trim(const encoding::hexidecimal::integer<nones, cx> &);
    
    template <encoding::hex::letter_case cx> 
    encoding::hexidecimal::integer<ones, cx> trim(const encoding::hexidecimal::integer<ones, cx> &);
    
    template <encoding::hex::letter_case cx> 
    encoding::hexidecimal::integer<twos, cx> trim(const encoding::hexidecimal::integer<twos, cx> &);
}

namespace data::math::number {
    bool inline is_zero(const encoding::decimal::string &x) {
        return x == string{"0"};
    }
    
    bool inline is_negative(const encoding::decimal::string &x) {
        return false;
    }
    
    bool inline is_positive(const encoding::decimal::string &x) {
        return !is_zero(x);
    }
    
    template <complement c, encoding::hex::letter_case cx> 
    bool inline is_positive(const encoding::hexidecimal::integer<c, cx> &z) {
        return !is_negative(z) && !is_zero(z);
    }
    
    template <encoding::hex::letter_case cx> 
    bool inline is_zero(const encoding::hexidecimal::integer<nones, cx> &z) {
        if (!z.valid()) throw std::invalid_argument{"invalid hex integer"};
        for (auto digit = z.begin() + 2; digit != z.end(); digit++) if (*digit != '0') return false;
        return true;
    }
    
    template <encoding::hex::letter_case cx> 
    bool inline is_zero(const encoding::hexidecimal::integer<ones, cx> &z) {
        if (!z.valid()) throw std::invalid_argument{"invalid hex integer"};
        for (auto digit = z.begin() + 2; digit != z.end(); digit++) if (*digit != '0') return false;
        return true;
    }
    
    template <encoding::hex::letter_case cx> 
    bool inline is_negative(const encoding::hexidecimal::integer<nones, cx> &) {
        return false;
    }
    
    template <encoding::hex::letter_case cx> 
    bool inline is_negative(const encoding::hexidecimal::integer<ones, cx> &x) {
        if (!x.valid()) throw std::invalid_argument{"invalid hex integer"};
        if (x.size() < 3) return false;
        return encoding::hexidecimal::digit(x[2]) > 7;
    }
    
    template <encoding::hex::letter_case cx> 
    bool inline is_minimal(const encoding::hexidecimal::integer<nones, cx> &x) {
        if (x.size() < 4) return true;
        return x[2] != '0' || x[3] != '0';
    }
}

namespace data::encoding::decimal {
        
    const std::string inline &characters() {
        static string Dec{"0123456789"};
        return Dec;
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
    
    uint32 inline count_digits(string_view s) {
        return valid(s) ? s.size() : 0;
    }
    
    inline string::string() : std::string{"0"} {}
        
    inline string::string(const std::string& x) : std::string{decimal::valid(x) ? x : ""} {}
    
    string inline &string::operator++() {
        if (!this->valid()) throw std::invalid_argument{"invalid decimal number"};
        return *this = *this + 1;
    }
    
    string inline &string::operator--() {
        if (!this->valid()) throw std::invalid_argument{"invalid decimal number"};
        return *this = *this - 1;
    }
    
    string inline string::operator++(int) {
        string n = *this;
        ++(*this);
        return n;
    }
    
    string inline string::operator--(int) {
        string n = *this;
        --(*this);
        return n;
    }
    
    string inline &string::operator+=(const string& n) {
        return *this = *this + n;
    }
    
    string inline &string::operator-=(const string& n) {
        return *this = *this - n;
    }
    
    string inline &string::operator*=(const string& n) {
        return *this = *this * n;
    }
    
    string inline &string::operator<<=(int i) {
        return *this = *this << i;
    }
    
    string inline &string::operator>>=(int i) {
        return *this = *this >> i;
    }
}

namespace data::encoding::hexidecimal {
    template <math::number::complement c, hex::letter_case cx> 
    integer<c, cx> inline operator++(integer<c, cx> &x, int) {
        integer n = x;
        ++x;
        return n;
    }
    
    template <math::number::complement c, hex::letter_case cx> 
    integer<c, cx> inline operator--(integer<c, cx> &x, int) {
        integer n = x;
        --x;
        return n;
    }
    
    template <hex::letter_case cx>
    integer<math::number::ones, cx> inline operator-(const integer<math::number::ones, cx> &x) {
        if (!x.valid()) throw std::invalid_argument{"invalid hex integer"};
        return math::increment(~x);
    }
    
    template <math::number::complement c, hex::letter_case zz> 
    integer<c, zz> inline &integer<c, zz>::operator+=(const integer &i) {
        return *this = *this + i;
    }
    
    template <math::number::complement c, hex::letter_case zz> 
    integer<c, zz> inline &integer<c, zz>::operator-=(const integer &i) {
        return *this = *this - i;
    }
    
    template <math::number::complement c, hex::letter_case zz> 
    integer<c, zz> inline &integer<c, zz>::operator*=(const integer &i) {
        return *this = *this * i;
    }
    
    template <math::number::complement c, hex::letter_case zz> 
    bool inline integer<c, zz>::operator==(int64 i) const {
        return *this == integer{i};
    }
    
    template <math::number::complement c, hex::letter_case zz> 
    std::weak_ordering inline integer<c, zz>::operator<=>(int64 i) const {
        return *this <=> integer{i};
    }
    
    template <math::number::complement c, hex::letter_case zz> 
    integer<c, zz> inline integer<c, zz>::operator+(int64 i) const {
        return *this + integer{i};
    }
    
    template <math::number::complement c, hex::letter_case zz> 
    integer<c, zz> inline integer<c, zz>::operator-(int64 i) const {
        return *this - integer{i};
    }
    
    template <math::number::complement c, hex::letter_case zz> 
    integer<c, zz> inline integer<c, zz>::operator*(int64 i) const {
        return *this * integer{i};
    }
    
    template <math::number::complement c, hex::letter_case zz> 
    integer<c, zz> inline &integer<c, zz>::operator+=(int64 i) {
        return *this += integer{i};
    }
    
    template <math::number::complement c, hex::letter_case zz> 
    integer<c, zz> inline &integer<c, zz>::operator-=(int64 i) {
        return *this -= integer{i};
    }
    
    template <math::number::complement c, hex::letter_case zz> 
    integer<c, zz> inline &integer<c, zz>::operator*=(int64 i) {
        return *this *= integer{i};
    }
    
    namespace {
        template <hex::letter_case cx, endian::order r, bool is_signed, size_t size> 
        string<cx> write_arith(const endian::arithmetic<r, is_signed, size> &z) {
            std::stringstream ss;
            ss << "0x";
            hex::write(ss, z, cx);
            return string<cx>{ss.str()};
        }
        
        template <math::number::complement c, hex::letter_case zz> struct write_int;
        
        template <hex::letter_case zz> struct write_int<math::number::nones, zz> {
            integer<math::number::nones, zz> operator()(int64 i) {
                if (i < 0) throw std::invalid_argument{"negative number"};
                return integer<math::number::nones, zz>{write_arith<zz>(endian::arithmetic<endian::big, false, 8>{static_cast<uint64>(i)})};
            }
        };
        
        template <hex::letter_case zz> struct write_int<math::number::ones, zz> {
            integer<math::number::ones, zz> operator()(int64 i) {
                return integer<math::number::ones, zz>{write_arith<zz>(endian::arithmetic<endian::big, true, 8>{i})};
            }
        };
        
        template <hex::letter_case zz> struct write_int<math::number::twos, zz> {
            integer<math::number::twos, zz> operator()(int64 i) {
                return i < 0 ? -write_uint(-i) : write_uint(i);
            }
            
        private:
            integer<math::number::twos, zz> write_uint(int64 i) {
                return integer<math::number::twos, zz>{write_arith<zz>(endian::arithmetic<endian::big, false, 8>{static_cast<uint64>(i)})};
            } 
        };
    }
    
    template <math::number::complement c, hex::letter_case zz> 
    inline integer<c, zz>::integer(int64 i) : integer{write_int<c, zz>{}(i)} {
        this->trim();
    }
    
    template <math::number::complement c, hex::letter_case zz> 
    integer<c, zz> inline &integer<c, zz>::trim() {
        return *this = math::number::trim(*this);
    }
    
    template <math::number::complement c, hex::letter_case zz> 
    integer<c, zz> inline integer<c, zz>::trim() const {
        return math::number::trim(*this);
    }
    
}

namespace data {
    
    template <encoding::hex::letter_case cx>
    math::sign inline sign(const encoding::hexidecimal::integer<math::number::nones, cx> &x) {
        if (!x.valid()) throw std::invalid_argument{std::string{"invalid hexidecimal string: \""} + (std::string)(x) + "\""}; 
        return math::number::is_zero(x) ? math::zero : math::positive;
    }
    
    template <encoding::hex::letter_case cx>
    math::sign inline sign(const encoding::hexidecimal::integer<math::number::ones, cx> &x) {
        if (!x.valid()) throw std::invalid_argument{std::string{"invalid hexidecimal string: \""} + (std::string)(x) + "\""}; 
        return math::number::is_negative(x) ? math::negative : math::number::is_zero(x) ? math::zero : math::positive;
    }
    
    template <encoding::hex::letter_case cx>
    math::sign inline sign(const encoding::hexidecimal::integer<math::number::twos, cx> &x) {
        if (!x.valid()) throw std::invalid_argument{std::string{"invalid hexidecimal string: \""} + (std::string)(x) + "\""}; 
        return math::number::is_zero(x) ? math::zero : math::number::sign_bit_set(x) ? math::negative : math::positive;
    }
    
    math::sign inline sign(const encoding::decimal::string &x) {
        if (!x.valid()) throw std::invalid_argument{"invalid dec string"};
        return math::number::is_zero(x) ? math::zero : math::positive;
    }
    
}

namespace data::math {
    
    template <number::complement c, encoding::hex::letter_case cx>
    encoding::hexidecimal::integer<c, cx> inline increment(const encoding::hexidecimal::integer<c, cx> &x) {
        auto z = x;
        return ++z;
    }
    
    template <number::complement c, encoding::hex::letter_case cx>
    encoding::hexidecimal::integer<c, cx> inline decrement(const encoding::hexidecimal::integer<c, cx> &x) {
        auto z = x;
        return --z;
    }
    
    template <encoding::hex::letter_case cx>
    encoding::hexidecimal::integer<number::nones, cx> inline 
    abs<encoding::hexidecimal::integer<number::nones, cx>>::operator()(const encoding::hexidecimal::integer<number::nones, cx> &x) {
        if (!x.valid()) throw std::invalid_argument{std::string{"invalid hexidecimal string: \""} + (std::string)(x) + "\""}; 
        return x;
    }
    
    template <encoding::hex::letter_case cx>
    encoding::hexidecimal::integer<number::ones, cx> inline 
    abs<encoding::hexidecimal::integer<number::ones, cx>>::operator()(const encoding::hexidecimal::integer<number::ones, cx> &x) {
        if (!x.valid()) throw std::invalid_argument{std::string{"invalid hexidecimal string: \""} + (std::string)(x) + "\""}; 
        return number::is_negative(x) ? -x : x;
    }
    
    template <encoding::hex::letter_case cx>
    encoding::hexidecimal::integer<number::twos, cx> inline 
    abs<encoding::hexidecimal::integer<number::twos, cx>>::operator()(const encoding::hexidecimal::integer<number::twos, cx> &x) {
        if (!x.valid()) throw std::invalid_argument{std::string{"invalid hexidecimal string: \""} + (std::string)(x) + "\""}; 
        return number::is_negative(x) ? -x : x;
    }
    
    encoding::decimal::string inline abs<encoding::decimal::string>::operator()(const encoding::decimal::string &x) {
        if (!x.valid()) throw std::invalid_argument{"invalid dec string"};
        return x;
    }
    
}

namespace data::math::number {
    
    template <encoding::hex::letter_case cx> 
    encoding::hexidecimal::integer<nones, cx> 
    trim(const encoding::hexidecimal::integer<nones, cx> &x) {
        
        if (!x.valid()) throw std::invalid_argument{std::string{"cannot trim invalid hexidecimal string: "} + x};
        
        if (is_minimal(x)) return x;
        auto i = x.begin() + 2;
        while (i != x.end() && i[0] == '0' && i[1] == '0') i+=2;
        
        encoding::hexidecimal::integer<nones, cx> n{};
        n.resize(x.end() - i + 2);
        std::copy(i, x.end(), n.begin() + 2);
        return n;
    }
    
    template <encoding::hex::letter_case cx> 
    encoding::hexidecimal::integer<ones, cx> 
    trim(const encoding::hexidecimal::integer<ones, cx> &x) {
        
        if (!x.valid()) throw std::invalid_argument{std::string{"cannot trim invalid hexidecimal string: "} + x};
        
        size_t min_size = minimal_size(x);
        
        if (min_size == x.size()) return x;
        
        encoding::hexidecimal::integer<ones, cx> n{};
        n.resize(min_size);
        std::copy(x.end() - min_size + 2, x.end(), n.begin() + 2);
        return n;
    }
    
    template <encoding::hex::letter_case cx> 
    encoding::hexidecimal::integer<twos, cx> 
    trim(const encoding::hexidecimal::integer<twos, cx> &x) {
        
        if (!x.valid()) throw std::invalid_argument{std::string{"cannot trim invalid hexidecimal string: "} + x};
        if (is_minimal(x)) return x;
        
        encoding::hexidecimal::integer<twos, cx> n{};
        if (is_zero(x)) return n;
        
        auto i = x.begin() + 4;
        while (i != x.end() && i[0] == '0' && i[1] == '0') i+=2;
        
        if (i != x.end() && encoding::hexidecimal::digit(i[0]) >= 8) {
            n.resize(x.end() - i + 4);
            n[2] = (x)[2];
            n[3] = '0';
        } else {
            n.resize(x.end() - i + 2);
            n[2] = encoding::hex::characters(cx)[encoding::hexidecimal::digit(i[0]) + (x[2] == '8' ? 8 : 0)];
            n[3] = i[1];
            i += 2;
        }
        
        std::copy(i, x.end(), n.begin() + 4);
        return n;
    }
}

namespace data::encoding::hexidecimal {
    
    template <hex::letter_case zz> 
    integer<math::number::ones, zz> operator~(const integer<math::number::ones, zz> &x) {
        if (!x.valid()) throw std::invalid_argument{"invalid hex string"}; 
        if (x == std::string{"0x"}) return integer<math::number::ones, zz>{std::string{"0xff"}};
        
        auto characters = hex::characters(zz);
        integer<math::number::ones, zz> n{};
        n.resize(x.size());
        for (int i = 2; i < x.size(); i++) n[i] = characters[0x0f & ~digit(x[i])];
        
        return n.trim();
    }
    
    template <hex::letter_case zz> 
    integer<math::number::twos, zz> operator-(const integer<math::number::twos, zz> &x) {
        if (!x.valid()) throw std::invalid_argument{std::string{"invalid hexidecimal string: \""} + (std::string)(x) + "\""}; 
        if (x == std::string{"0x"}) return x;
        integer<math::number::twos, zz> n = x;
        auto d = digit(x[2]);
        n[2] = hex::characters(zz)[(d & 0x7) | (~d & 0x8)];
        return n.trim();
    }
        
    namespace {
    
        std::weak_ordering N_compare_same_size(string_view a, string_view b) {
            for (int i = 0; i < a.size(); i++) 
                if (digit(a[i]) > digit(b[i])) return std::weak_ordering::greater; 
                else if (digit(a[i]) < digit(b[i])) return std::weak_ordering::less;
            
            return std::weak_ordering::equivalent;
        }
        
        std::weak_ordering N_compare(string_view a, string_view b) {
            if (a.size() < b.size()) return 0 <=> N_compare(b, a);
            size_t size_difference = a.size() - b.size();
            for (int i = 0; i < size_difference; i++) if (a[i] != '0') return std::weak_ordering::greater; 
            return N_compare_same_size(a.substr(size_difference, a.size()), b);
        }
        
        template <hex::letter_case zz>
        char N_increment(string<zz> &x) {
            auto characters = hex::characters(zz);
            
            auto i = x.rbegin();
            auto e = x.rbegin() + x.size() - 2;
            char remainder = 1;
            while (i != e) {
                auto d = digit(*i) + remainder;
                *i = characters[d % 16];
                remainder = d >> 4;
                if (remainder == 0) return '0';
                i++;
            }
            return characters[remainder];
        }
        
        template <hex::letter_case zz>
        void N_decrement(string<zz> &x) {
            auto characters = hex::characters(zz);
            
            auto i = x.rbegin();
            auto e = x.rbegin() + x.size() - 2;
            while (i != e) {
                auto d = digit(*i);
                if (d != 0) {
                    *i = characters[d - 1];
                    break;
                }
                *i = characters[15];
                i++;
            }
        }
        
        // these strings will all be the same size. 
        template <hex::letter_case zz>
        void bit_and(string<zz> &out, const string<zz> &a, const string<zz> &b) {
            
            std::vector<byte> out_d((out.size() - 2) >> 1);
            std::vector<byte> a_d((out.size() - 2) >> 1);
            std::vector<byte> b_d((out.size() - 2) >> 1);
            
            boost::algorithm::unhex(a.begin() + 2, a.end(), a_d.begin());
            boost::algorithm::unhex(b.begin() + 2, b.end(), b_d.begin());
            
            arithmetic::bit_and<byte>(out_d.end(), out_d.begin(), a_d.begin(), b_d.begin());
            
            if (zz == hex::lower) {
                boost::algorithm::hex_lower(out_d.begin(), out_d.end(), out.begin() + 2);
            } else {
                boost::algorithm::hex(out_d.begin(), out_d.end(), out.begin() + 2);
            }
        }
        
        template <hex::letter_case zz>
        void bit_or(string<zz> &out, const string<zz> &a, const string<zz> &b) {
            
            std::vector<byte> out_d((out.size() - 2) >> 1);
            std::vector<byte> a_d((out.size() - 2) >> 1);
            std::vector<byte> b_d((out.size() - 2) >> 1);
            
            boost::algorithm::unhex(a.begin() + 2, a.end(), a_d.begin());
            boost::algorithm::unhex(b.begin() + 2, b.end(), b_d.begin());
            
            arithmetic::bit_or<byte>(out_d.end(), out_d.begin(), a_d.begin(), b_d.begin());
            
            if (zz == hex::lower) {
                boost::algorithm::hex_lower(out_d.begin(), out_d.end(), out.begin() + 2);
            } else {
                boost::algorithm::hex(out_d.begin(), out_d.end(), out.begin() + 2);
            }
        }
        
        template <hex::letter_case zz>
        void bit_xor(string<zz> &out, const string<zz> &a, const string<zz> &b) {
            
            std::vector<byte> out_d((out.size() - 2) >> 1);
            std::vector<byte> a_d((out.size() - 2) >> 1);
            std::vector<byte> b_d((out.size() - 2) >> 1);
            
            boost::algorithm::unhex(a.begin() + 2, a.end(), a_d.begin());
            boost::algorithm::unhex(b.begin() + 2, b.end(), b_d.begin());
            
            arithmetic::bit_xor<byte>(out_d.end(), out_d.begin(), a_d.begin(), b_d.begin());
            
            if (zz == hex::lower) {
                boost::algorithm::hex_lower(out_d.begin(), out_d.end(), out.begin() + 2);
            } else {
                boost::algorithm::hex(out_d.begin(), out_d.end(), out.begin() + 2);
            }
        }
        
        // the out string will always be 2 characters longer than the other two. 
        template <hex::letter_case zz>
        char plus(string<zz> &out, const string<zz> &a, const string<zz> &b) {
            throw method::unimplemented{"plus"};
        }
        
        template <hex::letter_case zz>
        char minus(string<zz> &out, const string<zz> &a, const string<zz> &b) {
            throw method::unimplemented{"minus"};
        }
        
        // the out string will always be the size of the sum of the two inputs, which won't necessarily be equal size. 
        template <hex::letter_case zz>
        void times(string<zz> &out, const string<zz> &a, const string<zz> &b) {
            throw method::unimplemented{"times"};
        }
        
        template <math::number::complement c, hex::letter_case zz> 
        integer<c, zz> bit_and(const integer<c, zz> &a, const integer<c, zz> &b) {
            if (a.size() < b.size()) return bit_and(b, a);
            integer<c, zz> n{};
            n.resize(a.size());
            bit_and(n, a, math::number::extend(b, a.size()));
            return n;
        }
        
        template <math::number::complement c, hex::letter_case zz> 
        integer<c, zz> bit_or(const integer<c, zz> &a, const integer<c, zz> &b) {
            if (a.size() < b.size()) return bit_or(b, a);
            integer<c, zz> n{};
            n.resize(a.size());
            bit_or(n, a, math::number::extend(b, a.size()));
            return n;
        }
        
        template <hex::letter_case zz> 
        integer<math::number::ones, zz> bit_xor(const integer<math::number::ones, zz> &a, const integer<math::number::ones, zz> &b) {
            if (a.size() < b.size()) return bit_xor(b, a);
            integer<math::number::ones, zz> n{};
            n.resize(a.size());
            bit_xor(n, a, math::number::extend(b, a.size()));
            return n;
        }
        
        template <hex::letter_case zz> 
        string<zz> shift_left(const string<zz> &x, uint32 i) {
            throw method::unimplemented{"shift left"};
        }
        
        template <hex::letter_case zz> 
        string<zz> shift_right(const string<zz> &x, uint32 i) {
            throw method::unimplemented{"shift left"};
        }
        
        template <hex::letter_case zz> 
        string<zz> inline shift(const string<zz> &x, int i) {
            return i < 0 ? shift_right(x, -i) : shift_left(x, i);
        }
        
        template <math::number::complement c, hex::letter_case zz> 
        integer<c, zz> inline bit_shift(const integer<c, zz> &x, int i) {
            return integer<c, zz>{shift(x, i)};
        }
        
        template <hex::letter_case zz> 
        integer<math::number::twos, zz> inline bit_shift(const integer<math::number::twos, zz> &x, int i) {
            return math::number::is_negative(x) ? -integer<math::number::twos, zz>{shift(-x, i)} : integer<math::number::twos, zz>{shift(x, i)};
        }
        
        template <hex::letter_case zz> 
        integer<math::number::nones, zz> plus(const integer<math::number::nones, zz> &a, const integer<math::number::nones, zz> &b) {
            if (a.size() < b.size()) return plus(b, a);
            integer<math::number::nones, zz> n{};
            n.resize(a.size() + 1);
            plus(n, a, math::number::extend(b, a.size()));
            return n;
        }
        
        template <hex::letter_case zz> 
        integer<math::number::ones, zz> plus(const integer<math::number::ones, zz> &a, const integer<math::number::ones, zz> &b) {
            if (a.size() < b.size()) return plus(b, a);
            integer<math::number::ones, zz> n{};
            n.resize(a.size() + 1);
            plus(n, a, math::number::extend(b, a.size()));
            return n;
        }
        
        template <hex::letter_case zz> 
        integer<math::number::nones, zz> minus(const integer<math::number::nones, zz> &a, const integer<math::number::nones, zz> &b) {
            if (b > a) return integer<math::number::nones, zz>{};
            // these numbers are both trimmed, so we can expect that the size of b is less than that of a. 
            integer<math::number::nones, zz> n{};
            n.resize(a.size());
            minus(n, a, math::number::extend(b, a.size()));
            return n;
        }
        
        template <hex::letter_case zz> 
        integer<math::number::ones, zz> minus(const integer<math::number::ones, zz> &a, const integer<math::number::ones, zz> &b) {
            throw method::unimplemented{"minus"};
        }
        
        template <hex::letter_case zz> 
        integer<math::number::twos, zz> minus(const integer<math::number::twos, zz> &a, const integer<math::number::twos, zz> &b) {
            throw method::unimplemented{"minus"};
        }
        
        template <hex::letter_case zz> 
        integer<math::number::twos, zz> plus(const integer<math::number::twos, zz> &a, const integer<math::number::twos, zz> &b) {
            bool an = math::number::is_negative(a);
            bool bn = math::number::is_negative(b);
            
            if (an && bn) return -plus(-a, -b);
            
            if (an) return minus(b, -a);
            
            if (bn) return minus(a, -b);
            
            if (a.size() < b.size()) return plus(b, a);
            
            integer<math::number::twos, zz> n{};
            n.resize(a.size() + 1);
            plus(n, a, math::number::extend(b, a.size()));
            return n;
        }
        
        template <hex::letter_case zz> 
        integer<math::number::nones, zz> times(const integer<math::number::nones, zz> &a, const integer<math::number::nones, zz> &b) {
            throw method::unimplemented{"times"};
        }
        
        template <hex::letter_case zz> 
        integer<math::number::ones, zz> times(const integer<math::number::ones, zz> &a, const integer<math::number::ones, zz> &b) {
            throw method::unimplemented{"times"};
        }
        
        template <hex::letter_case zz> 
        integer<math::number::twos, zz> times(const integer<math::number::twos, zz> &a, const integer<math::number::twos, zz> &b) {
            throw method::unimplemented{"times"};
        }
    }
    
    template <math::number::complement c, hex::letter_case zz> 
    bool inline operator==(const integer<c, zz> &a, const integer<c, zz> &b) {
        return (a <=> b) == 0;
    }
    
    template <hex::letter_case zz> 
    std::weak_ordering inline operator<=>(const integer<math::number::nones, zz> &a, const integer<math::number::nones, zz> &b) {
        if (!a.valid() || !b.valid()) throw std::invalid_argument{"invalid hexidecimal string"};
        return N_compare(string_view{a.data() + 2, a.size() - 2}, string_view{b.data() + 2, b.size() - 2});
    }
    
    template <hex::letter_case zz> 
    std::weak_ordering operator<=>(const integer<math::number::ones, zz> &a, const integer<math::number::ones, zz> &b) {
        if (!a.valid() || !b.valid()) throw std::invalid_argument{"invalid hexidecimal string"};
        bool na = math::number::is_negative(a);
        bool nb = math::number::is_negative(b);
        
        if (na && nb) {
            auto ya = -b;
            auto yb = -a;
            return N_compare(string_view{ya.data() + 2, ya.size() - 2}, string_view{yb.data() + 2, yb.size() - 2});
        }
        
        if (!na && !nb) return N_compare(string_view{a.data() + 2, a.size() - 2}, string_view{b.data() + 2, b.size() - 2});
        return na ? std::weak_ordering::less : std::weak_ordering::greater;
    }
    
    template <hex::letter_case zz> 
    std::weak_ordering operator<=>(const integer<math::number::twos, zz> &a, const integer<math::number::twos, zz> &b) {
        
        math::sign na = sign(a);
        math::sign nb = sign(b);
        
        if (na == math::zero) switch (nb) {
            case math::positive: return std::weak_ordering::less;
            case math::negative: return std::weak_ordering::greater;
            default: return std::weak_ordering::equivalent;
        }
        
        if (nb == math::zero) return na == math::positive ? std::weak_ordering::greater : std::weak_ordering::less;
        
        if (na == math::positive && nb == math::positive) 
            return N_compare(string_view{a.data() + 2, a.size() - 2}, string_view{b.data() + 2, b.size() - 2});
        
        if (na == math::negative && nb == math::negative) {
            auto ya = -b;
            auto yb = -a;
            return N_compare(string_view{ya.data() + 2, ya.size() - 2}, string_view{yb.data() + 2, yb.size() - 2});
        }
        
        return na == math::negative ? std::weak_ordering::less : std::weak_ordering::greater;
    }
    
    template <hex::letter_case zz> 
    integer<math::number::nones, zz> &operator++(integer<math::number::nones, zz> &x) {
        if (!x.valid()) throw std::invalid_argument{std::string{"invalid hexidecimal string: \""} + (std::string)(x) + "\""};
        
        char remainder = N_increment(x);
        if (remainder != '0') {
            integer<math::number::nones, zz> n{};
            n.resize(x.size() + 2);
            std::copy(x.begin() + 2, x.end(), n.begin() + 4);
            n[2] = '0';
            n[3] = remainder;
            x = n;
        } 
        
        return x.trim();
    }
    
    template <hex::letter_case zz> 
    integer<math::number::nones, zz> &operator--(integer<math::number::nones, zz> &x) {
        if (!x.valid()) throw std::invalid_argument{std::string{"invalid hexidecimal string: \""} + (std::string)(x) + "\""}; 
        if (math::number::is_zero(x)) return x;
        N_decrement(x);
        return x.trim();
    }
    
    template <hex::letter_case zz> 
    integer<math::number::ones, zz> &operator++(integer<math::number::ones, zz> &x) {
        if (!x.valid()) throw std::invalid_argument{std::string{"invalid hexidecimal string: \""} + (std::string)(x) + "\""}; 
        
        bool neg = math::number::is_negative(x); 
        char remainder = N_increment(x);
        
        if (!neg && (remainder != '0' || math::number::is_negative(x))) {
            integer<math::number::ones, zz> n{};
            n.resize(x.size() + 2);
            std::copy(x.begin() + 2, x.end(), n.begin() + 4);
            n[2] = '0';
            n[3] = remainder;
            x = n;
        } 
        
        return x.trim();
    }
    
    template <hex::letter_case zz> 
    integer<math::number::ones, zz> &operator--(integer<math::number::ones, zz> &x) {
        if (!x.valid()) throw std::invalid_argument{"invalid hex string"};
        if (x == std::string("0x")) return x = integer<math::number::ones, zz>{"0xff"};
        N_decrement(x);
        return x.trim();
    }
    
    template <hex::letter_case zz> 
    integer<math::number::twos, zz> &operator++(integer<math::number::twos, zz> &x) {
        if (!x.valid()) throw std::invalid_argument{"invalid hex string"};
        if (math::number::is_negative(x)) return x = -math::decrement(-x);
        if (math::number::is_negative_zero(x)) return x = integer<math::number::twos, zz>{"0x01"};
        
        char remainder = N_increment(x);
        integer<math::number::twos, zz> n{};
        n.resize(x.size() + 2);
        std::copy(x.begin() + 2, x.end(), n.begin() + 4);
        if (remainder != '0') {
            n[2] = '0';
            n[3] = remainder;
            x = n;
        } else if (math::number::is_negative(x)) {
            n[2] = '0';
            n[3] = '0';
            x = n;
        }
        
        return x.trim();
    }
    
    template <hex::letter_case zz> 
    integer<math::number::twos, zz> &operator--(integer<math::number::twos, zz> &x) {
        if (math::number::is_negative(x)) return x = -data::math::increment(-x);
        if (math::number::is_zero(x)) return x = integer<math::number::twos, zz>{"0x81"};
        N_decrement(x);
        return x.trim();
    }
    
    template <math::number::complement c, hex::letter_case zz> 
    integer<c, zz> inline operator|(const integer<c, zz> &a, const integer<c, zz> &b) {
        return math::number::trim(bit_or(a, b));
    }
    
    template <math::number::complement c, hex::letter_case zz> 
    integer<c, zz> inline operator&(const integer<c, zz> &a, const integer<c, zz> &b) {
        return math::number::trim(bit_and(a, b));
    }
    
    template <hex::letter_case zz> 
    integer<math::number::ones, zz> inline operator^(const integer<math::number::ones, zz> &a, const integer<math::number::ones, zz> &b) {
        return math::number::trim(bit_xor(a, b));
    }
    
    template <math::number::complement c, hex::letter_case zz> 
    integer<c, zz> inline operator<<(const integer<c, zz> &a, int i) {
        return math::number::trim(bit_shift(a, i));
    }
    
    template <math::number::complement c, hex::letter_case zz> 
    integer<c, zz> inline operator>>(const integer<c, zz> &a, int i) {
        return math::number::trim(bit_shift(a, -i));
    }
    
    template <math::number::complement c, hex::letter_case zz> 
    integer<c, zz> inline operator+(const integer<c, zz> &a, const integer<c, zz> &b) {
        return math::number::trim(plus(math::number::trim(a), math::number::trim(b)));
    }
    
    template <math::number::complement c, hex::letter_case zz> 
    integer<c, zz> inline operator-(const integer<c, zz> &a, const integer<c, zz> &b) {
        return math::number::trim(minus(math::number::trim(a), math::number::trim(b)));
    }
    
    template <math::number::complement c, hex::letter_case zz> 
    integer<c, zz> inline operator*(const integer<c, zz> &a, const integer<c, zz> &b) {
        return math::number::trim(times(math::number::trim(a), math::number::trim(b)));
    }
    
}

namespace data::math::number {
    template <encoding::hex::letter_case zz> 
    bool inline sign_bit_set(const encoding::hexidecimal::integer<twos, zz> &x) {
        return x.size() > 2 && encoding::hexidecimal::digit(x[2]) > 7;
    }
    
    template <encoding::hex::letter_case zz> 
    bool is_positive_zero(const encoding::hexidecimal::integer<twos, zz> &z) {
        auto digit = z.begin() + 2;
        
        while (true) {
            if (digit == z.end()) return true;
            if (*digit != '0') return false;
            digit++;
        }
        
        // we can't really reach here. 
        return true;
    }
    
    template <encoding::hex::letter_case zz> 
        bool is_negative_zero(const encoding::hexidecimal::integer<twos, zz> &z) {
        auto digit = z.begin() + 2;
        if (*digit != '8') return false;
    
        while (true) {
            digit++;
            if (digit == z.end()) return true;
            if (*digit != '0') return false;
        }
        
        // we can't really reach here. 
        return true;
    }
    
    template <encoding::hex::letter_case cx> 
    bool inline is_negative(const encoding::hexidecimal::integer<twos, cx> &x) {
        if (!x.valid()) throw std::invalid_argument{"invalid hex integer"};
        return sign_bit_set(x) && !is_negative_zero(x);
    }
    
    template <encoding::hex::letter_case zz> 
    bool is_zero(const encoding::hexidecimal::integer<twos, zz> &z) {
        auto digit = z.begin() + 2;
        if (digit == z.end()) return true;
        if (*digit != '0' && *digit != '8') return false;
        
        while (true) {
            digit++;
            if (digit == z.end()) return true;
            if (*digit != '0') return false;
        }
        
        // we can't really reach here. 
        return true;
    }
    
    template <encoding::hex::letter_case cx> 
    bool is_minimal(const encoding::hexidecimal::integer<ones, cx> &x) {
        // minimal zero. 
        if (x.size() == 2) return true;
        // numbers of one byte. 
        if (x.size() == 4) return x[2] != '0' || x[3] != '0';
        // numbers without an initial 00 or ff. 
        if ((x[2] != 'f' || x[3] != 'f') && (x[2] != '0' || x[3] != '0')) return true; 
        // numbers that would be interpreted with the wrong sign if they were shortened. 
        char d = encoding::hexidecimal::digit(x[4]);
        return x[2] == '0' && d >= 8 || x[2] == 'f' && d < 8;
    }
    
    template <encoding::hex::letter_case zz> 
    bool inline is_minimal(const encoding::hexidecimal::integer<twos, zz> &x) {
        // minimal zero. 
        return (x.size() == 2) ||
            // numbers without an initial 0x00 or 0x80
            ((x[2] != '0' && x[2] != '8') || x[3] != '0') || 
            // representations of zero. 
            (x.size() > 4 && 
                // numbers that would be interpreted as having the wrong sign 
                // if they were shortened. 
                encoding::hexidecimal::digit(x[4]) >= 8);
    }
    
    template <encoding::hex::letter_case zz> 
    encoding::hexidecimal::integer<nones, zz> extend(const encoding::hexidecimal::integer<nones, zz> &x, size_t size) {
        if (!x.valid()) throw std::invalid_argument{"invalid hex number"};
        if (size & 1 || size < 2) throw std::invalid_argument{"invalid size"};
        if (x.size() > size) {
            auto minimal = minimal_size(x);
            if (minimal > size) throw std::logic_error("cannot extend below minimal size");
            return extend(trim(x), size);
        }
        
        encoding::hexidecimal::integer<nones, zz> n;
        n.resize(size);
        auto i = n.begin() + 2;
        for (int zeros = 0; zeros < size - x.size(); zeros ++) {
            *i = '0';
            i ++;
        }
        std::copy(x.begin() + 2, x.end(), i);
        return n;
    }
    
    template <encoding::hex::letter_case zz> 
    encoding::hexidecimal::integer<ones, zz> extend(const encoding::hexidecimal::integer<ones, zz> &x, size_t size) {
        if (!x.valid()) throw std::invalid_argument{"invalid hex number"};
        if (size & 1 || size < 2) throw std::invalid_argument{"invalid size"};
        
        if (x.size() > size) {
            auto minimal = minimal_size(x);
            if (minimal > size) throw std::logic_error("cannot extend below minimal size");
            return extend(trim(x), size);
        }
        
        encoding::hexidecimal::integer<ones, zz> n;
        n.resize(size);
        auto i = n.begin() + 2;
        char z = x.size() == 2 || encoding::hexidecimal::digit(x[2]) < 8 ? '0' : 'f';
        for (int zeros = 0; zeros < size - x.size(); zeros ++) {
            *i = z;
            i ++;
        }
        std::copy(x.begin() + 2, x.end(), i);
        return n;
    }
    
    template <encoding::hex::letter_case zz> 
    encoding::hexidecimal::integer<twos, zz> extend(const encoding::hexidecimal::integer<twos, zz> &x, size_t size) {
        
        if (!x.valid()) throw std::invalid_argument{"invalid hex number"};
        if (size & 1 || size < 2) throw std::invalid_argument{"invalid size"};
        
        if (x.size() > size) {
            auto minimal = minimal_size(x);
            if (minimal > size) throw std::logic_error("cannot extend below minimal size");
            return extend(trim(x), size);
        }
        
        if (x.size() == size) return x;
        
        encoding::hexidecimal::integer<twos, zz> n;
        n.resize(size);
        auto i = n.begin() + 2;
        for (int zeros = 0; zeros < size - x.size(); zeros ++) {
            *i = '0';
            i++;
        }
        
        std::copy(x.begin() + 2, x.end(), i);
        if (x.size() > 2 && size > 2) {
            auto sign_digit = encoding::hexidecimal::digit(x[2]);
            if (sign_digit >= 8) {
                n[2] = '8';
                n[2 + size - x.size()] = encoding::hex::characters(zz)[sign_digit - 8];
            }
        }
        
        return n;
        
    }
    
    template <encoding::hex::letter_case zz> 
    size_t minimal_size(const encoding::hexidecimal::integer<nones, zz> &x) {
        int zeros = 0;
        for (auto i = x.begin() + 2; i != x.end(); i += 2) 
            if (i[0] == '0' && i[1] == '0') zeros += 2;
            else break;
        return x.size() - zeros;
    }
    
    template <encoding::hex::letter_case zz> 
    size_t minimal_size(const encoding::hexidecimal::integer<ones, zz> &x) {
        if (x.size() == 2) return 2;
        // numbers that don't begin with 00 or ff are minimal. 
        char z = x[2];
        if (z != 'f' && z != '0' || x[3] != z) return x.size();
        // count the number of repeated bytes after the first.
        // All can be removed. 
        int repeated = 0;
        auto i = x.begin() + 4; 
        while (true) {
            // if we reach the end, then the number is either 0 or -1.
            if (i == x.end()) return z == '0' ? 2 : 4; 
            if (i[0] == z && i[1] == z) repeated += 2;
            else {
                char d = encoding::hexidecimal::digit(*i);
                return x.size() - repeated - (d >= 8 && z == 'f' || d < 8 && z == '0' ? 2 : 0);
            }
            i += 2;
        }
    }
    
    template <encoding::hex::letter_case zz> 
    size_t minimal_size(const encoding::hexidecimal::integer<twos, zz> &x) {
        if (x.size() == 2) return 2;
        // numbers that don't begin with 00 or 80 are minimal. 
        char z = x[2];
        if (z != '8' && z != '0' || x[3] != '0') return x.size();
        // count the number of zero bytes after the first. 
        int zeros = 0;
        auto i = x.begin() + 4; 
        while(true) {
            // if we reach the end then this number is zero. 
            if (i == x.end()) return 2;
            if (i[0] == '0' && i[1] == '0') zeros += 2;
            // if the first non-zero digit does not have the
            // sign bit set then we can remove an extra digit. 
            else return x.size() - zeros - (encoding::hexidecimal::digit(*i) < 8 ? 2 : 0);
            i += 2;
        }
    }
}

#endif

