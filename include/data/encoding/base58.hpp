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
#include <data/iterable.hpp>

namespace data::encoding::base58 {
    
    const std::string Format{"base58"};
    
    inline std::string characters() {return "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";}
    
    static constexpr auto pattern = ctll::fixed_string{"1|([2-9A-HJ-NP-Za-km-z][1-9A-HJ-NP-Za-km-z]*)"};
    
    inline bool valid(const string_view s) {
        return ctre::match<pattern>(s);
    }
    
    inline char digit(char c) {
        return c < '1' ? -1 : c <= '9' ?  c - '1' : c < 'A' ? -1 : c <= 'H' ? c - 'A' + 9 : c < 'J' ? -1 : c <= 'N' ? c - 'J' + 17 : c < 'P' ? -1 : c <= 'Z' ? c - 'P' + 22 : c < 'a' ? -1 : c <= 'k' ? c - 'a' + 33 : c < 'm' ? -1 : c <= 'z' ? c - 'm' + 44 : -1;
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
        string(const std::string&);
        string(uint64);
            
        bool valid() const {
            return base58::valid(*this);
        }
        
        bool operator<=(const string&) const;
        bool operator>=(const string&) const;
        bool operator<(const string&) const;
        bool operator>(const string&) const;
    
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
        return encoding::write_base<N>(n, characters());
    };
    
    string write(const bytes_view b);
    
}

#endif
