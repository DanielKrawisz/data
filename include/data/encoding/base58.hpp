// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_BASE58
#define DATA_ENCODING_BASE58

#include <data/types.hpp>
#include <data/encoding/invalid.hpp>
#include <data/math/number/gmp/gmp.hpp>
#include <data/math/number/bytes/N.hpp>
#include <ctre.hpp>
#include <algorithm>

namespace data::encoding::base58 {
    
    const std::string format{"base58"};
    
    const std::string characters{"123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz"};
    
    constexpr auto pattern = ctll::fixed_string{"[2-9A-HJ-NP-Za-km-z][1-9A-HJ-NP-Za-km-z]*"};
    
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
    
    template <typename N>
    std::string write(N n) {
        if (n == 0) return "1";
        
        N power{1};
        
        N x = n;
        std::string digits{};
        math::division<N> div;
        while(x > 0) {
            div = x.divide(58);
            digits += characters[(uint64)(div.Remainder)];
            x = div.Quotient;
        }
        
        std::reverse(digits.begin(), digits.end());
        return digits;
    };
    
    inline std::string write(const bytes_view b) {
        using namespace math::number;
        return write<N>(N{N_bytes<endian::big>{b}});
    }
    
    template<unsigned long size>
    inline std::string write(const std::array<byte, size>& x) {
        return write(bytes_view{x.data(), size});
    }
    
    class string {
        string_view String;
        bytes Bytes;
        bytes *ToBytes;
        
    public:
        explicit operator bytes() const {
            if (ToBytes == nullptr) throw invalid{format, String};
            return *ToBytes;
        }
        
        bool valid() const noexcept {
            return ToBytes != nullptr;
        }
        
        string(string_view s) : String{s}, Bytes{}, ToBytes{nullptr} {
            using namespace math::number;
            if (base58::valid(s)) {
                Bytes = N_bytes<endian::big>{read<N>(s)};
                ToBytes = &Bytes;
            }
        }
    };
    
}

#endif
