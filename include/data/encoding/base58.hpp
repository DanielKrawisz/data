// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_BASE58
#define DATA_ENCODING_BASE58

#include <algorithm>

#include <ctre.hpp>

#include <data/types.hpp>
#include <data/encoding/invalid.hpp>
#include <data/math/number/bytes/bytes.hpp>
#include <data/math/number/gmp/N.hpp>
#include <data/encoding/digits.hpp>

// base 58 is a format for writing natural numbers using
// 58 digits that are easily distinguished by the human
// eye. Numbers are written in big endian. Somewhat
// confusingly, in base 58 '1' means zero. 
namespace data::encoding::base58 {
    
    const std::string Format{"base58"};
    
    std::string inline characters() {return "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";}
    
    static constexpr auto pattern = ctll::fixed_string{"1|([2-9A-HJ-NP-Za-km-z][1-9A-HJ-NP-Za-km-z]*)"};
    
    bool inline valid(const string_view s) {
        return ctre::match<pattern>(s);
    }
    
    char inline digit(char c) {
        return c < '1' ? -1 : c <= '9' ?  c - '1' : c < 'A' ? -1 : c <= 'H' ? c - 'A' + 9 : c < 'J' ? -1 : 
            c <= 'N' ? c - 'J' + 17 : c < 'P' ? -1 : c <= 'Z' ? c - 'P' + 22 : c < 'a' ? -1 : 
            c <= 'k' ? c - 'a' + 33 : c < 'm' ? -1 : c <= 'z' ? c - 'm' + 44 : -1;
    };
    
    template <math::natural N>
    N read(const string_view s) {
        if (s.size() == 0) return N{};
        
        N power{1};
        
        N n{0};
        
        for (int i = s.size() - 1; i >= 0; i--) {
            char v = digit(s[i]);
            if (v == -1) return N{};
            n += power * v;
            power *= 58;
        }
        
        return n;
    }
    
    class view : public string_view {
        bytes Bytes;
        bytes *ToBytes;
        
    public:
        explicit operator bytes_view() const {
            if (ToBytes == nullptr) throw invalid{Format, *this};
            return bytes_view(*ToBytes);
        }
        
        bool valid() const noexcept {
            return ToBytes != nullptr;
        }
        
        view(string_view);
    };
    
    struct string;
    
    template <typename N> string write(const N& n);
    
    string write(const bytes_view b);
    
    // base58 check encoding is a way to encode arbitrary bite strings 
    // using base 58. This is done by encoding every initial zero bite
    // as a '1', and encoding the rest of the string as a number, as 
    // in standard base50. There is a four byte checksum attached,
    // which is the first four bytes of the double SHA2_256 hash. 
    struct check : std::string {

        // A Bitcoin checksum takes the hash256 value of a string
        // and appends the last 4 bytes of the result. 
        static uint32_little sum(bytes_view b);

        static ptr<bytes> decode(string_view);
        static check encode(bytes_view);

        bool valid() const {
            return decode(*this) != nullptr;
        }

        check(string_view x) : std::string{x} {}
        
        // try all single letter replacements, insertions, and deletions
        // to see if we can find a valid base58 check encoded string. 
        static check recover(const string_view invalid);
        
    private:
        check() : std::string{} {}
    };
}

namespace data::math {
    encoding::base58::string increment(const encoding::base58::string&);
    encoding::base58::string decrement(const encoding::base58::string&);
    template <> struct abs<encoding::base58::string> {
        encoding::base58::string operator()(const encoding::base58::string&);
    };
}

namespace data::math::number {
    bool is_zero(const encoding::base58::string &);
    bool is_negative(const encoding::base58::string &);
    bool is_positive(const encoding::base58::string &);
}

namespace data::encoding::base58 {
    
    // base58 strings are really natural numbers, so we 
    // can define standard math operations on them. 
    struct string : std::string {
        string();
        explicit string(string_view);
        string(uint64);
            
        bool valid() const {
            return base58::valid(*this);
        }
        
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
        math::division<string> divide(const string&) const;
        
        string operator/(const string &x) {
            return divide(x).Quotient;
        }
        
        string operator%(const string &x) {
            return divide(x).Remainder;
        }
        
        bool operator==(int64 x) const {
            if (x < 0) return false;
            return *this == string{static_cast<uint64>(x)};
        }
        
        std::strong_ordering operator<=>(int64 x) const {
            if (x < 0) return std::strong_ordering::greater;
            return *this <=> string{static_cast<uint64>(x)};
        }
        
        string operator+(uint64) const;
        string operator-(uint64) const;
        string operator*(uint64) const;
        
        string &operator+=(uint64);
        string &operator-=(uint64);
        string &operator*=(uint64);
            
        explicit operator double() const;
        
        math::sign sign() const {
            if (!valid()) throw std::invalid_argument{"invalid base 58 string"};
            return math::number::is_zero(*this) ? math::zero : math::positive;
        }
    };
    
    template <math::natural N>
    string inline write(const N& n) {
        return string{encoding::write_base<N>(n, characters())};
    };

    string inline &string::operator++() {
        return *this = *this + 1;
    }
    
    string inline &string::operator--() {
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

    inline string::string() : std::string{"1"} {}
    
    inline string::string(string_view x) : std::string{base58::valid(x) ? x : ""} {}
    
}

namespace data::math {
    
    encoding::base58::string inline increment(const encoding::base58::string &x) {
        auto z = x;
        return z++;
    }
    
    encoding::base58::string inline decrement(const encoding::base58::string &x) {
        auto z = x;
        return z++;
    }
    
    encoding::base58::string inline abs<encoding::base58::string>::operator()(const encoding::base58::string &x) {
        if (!x.valid()) throw std::invalid_argument{"invalid base 58 string"};
        return x;
    }
}

namespace data::math::number {
    
    bool inline is_zero(const encoding::base58::string &x) {
        return x == string{"1"};
    }
    
    bool inline is_negative(const encoding::base58::string &x) {
        return false;
    }
    
    bool inline is_positive(const encoding::base58::string &x) {
        return !is_zero(x);
    }
}

#endif
