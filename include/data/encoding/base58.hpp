// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_BASE58
#define DATA_ENCODING_BASE58

#include <algorithm>

#include <ctre.hpp>

#include <data/types.hpp>
#include <data/encoding/digits.hpp>
#include <data/encoding/invalid.hpp>
#include <data/math/division.hpp>
#include <data/math/abs.hpp>
#include <data/math/root.hpp>
#include <data/cross.hpp>

namespace data::encoding::base58 {
    
    const std::string Format{"base58"};
    
    std::string inline characters() {return "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";}
    
    static constexpr auto pattern = ctll::fixed_string{"1|([2-9A-HJ-NP-Za-km-z][1-9A-HJ-NP-Za-km-z]*)"};
    
    bool inline valid(const string_view s) {
        return ctre::match<pattern>(s);
    }
    
    bool inline nonzero(const string_view s) {
        return valid(s) && s[0] != '1';
    }
    
    char inline digit(char c) {
        return c < '1' ? -1 : c <= '9' ?  c - '1' : c < 'A' ? -1 : c <= 'H' ? c - 'A' + 9 : 
            c < 'J' ? -1 : c <= 'N' ? c - 'J' + 17 : c < 'P' ? -1 : c <= 'Z' ? c - 'P' + 22 : 
            c < 'a' ? -1 : c <= 'k' ? c - 'a' + 33 : c < 'm' ? -1 : c <= 'z' ? c - 'm' + 44 : -1;
    };
    
    template <typename N>
    N read(const string_view s) {
        if (s.size() == 0) return N{};
        
        N power{1};
        
        N n{0};
        
        for (int i = s.size() - 1; i >= 0; i--) {
            char v = digit(s[i]);
            if (v == -1) return N{};
            n += power * uint64(v);
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
    
    struct string : std::string {
        string();
        explicit string(string_view);
        explicit string(std::string &&x): std::string{x} {};
        string(uint64);
        
        bool valid() const {
            return base58::valid(*this);
        }
        
        static string read(string_view x);
        
        std::strong_ordering operator<=>(const string&) const;
        
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
    
    template <typename N>
    string write(N n) {
        return string{encoding::write_base<N>(n, characters())};
    };
    
    string write(const bytes_view b);
    
}

namespace data {
    using base58_uint = encoding::base58::string;
    
    math::sign sign(const base58_uint&);
}

namespace data::math {
    
    template <> struct abs<base58_uint> {
        base58_uint operator()(const base58_uint&);
    };
    
    template <uint64 pow> 
    struct root<base58_uint, pow> {
        set<base58_uint> operator()(const base58_uint& n);
    };
}

namespace data::math::number {
    
    base58_uint increment(const base58_uint&);
    base58_uint decrement(const base58_uint&);
    
    bool is_zero(const base58_uint &);
    bool is_negative(const base58_uint &);
    bool is_positive(const base58_uint &);
    
}

namespace data {
    
    math::sign inline sign(const base58_uint &n) {
        if (!encoding::base58::valid(n)) throw std::invalid_argument{std::string{"invalid base 58 string: "} + std::string{n}};
        return encoding::base58::nonzero(n) ? math::positive : math::zero;
    }
}

namespace data::math::number {
    
    base58_uint inline increment(const base58_uint &n) {
        auto x = n;
        return ++x;
    }
    
    base58_uint inline decrement(const base58_uint &n) {
        auto x = n;
        return --x;
    }
    
    bool inline is_zero(const base58_uint &n) {
        if (encoding::base58::valid(n)) throw std::invalid_argument{std::string{"invalid base 58 string: "} + std::string{n}};
        return !encoding::base58::nonzero(n);
    }
    
    bool inline is_negative(const base58_uint &n) {
        if (encoding::base58::valid(n)) throw std::invalid_argument{std::string{"invalid base 58 string: "} + std::string{n}};
        return false;
    }
    
    bool inline is_positive(const base58_uint &n) {
        if (encoding::base58::valid(n)) throw std::invalid_argument{std::string{"invalid base 58 string: "} + std::string{n}};
        return encoding::base58::nonzero(n);
    }
    
}

#endif
