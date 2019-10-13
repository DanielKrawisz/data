// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_BASE58
#define DATA_ENCODING_BASE58

#include <data/types.hpp>
#include <data/slice.hpp>
#include <data/queue/functional_queue.hpp>
#include <data/list/linked.hpp>
#include <data/math/division.hpp>
#include <ctre.hpp>
#include <iostream>

namespace data::encoding::base58 {
    
    static const std::string format{"base58"};
    
    static const std::string characters{"123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz"};
    
    static constexpr auto pattern = ctll::fixed_string{"[2-9A-HJ-NP-Za-km-z][1-9A-HJ-NP-Za-km-z]*"};
    
    static bool valid(const string_view s) {
        return ctre::match<pattern>(s);
    }
    
    static char digit(char c) {
        return c < '1' ? -1 : c <= '9' ?  c - '1' : c < 'A' ? -1 : c <= 'H' ? c - 'A' + 9 : c < 'J' ? -1 : c <= 'N' ? c - 'J' + 17 : c < 'P' ? -1 : c <= 'Z' ? c - 'P' + 22 : c < 'a' ? -1 : c <= 'k' ? c - 'a' + 33 : c < 'm' ? -1 : c <= 'z' ? c - 'm' + 44 : -1;
    };
    
    template <typename N>
    static functional_queue<N, list::linked<N>>& powers() {
        static functional_queue<N, list::linked<N>> Powers = functional_queue<N, list::linked<N>>::make(N{1});
        return Powers;
    }
    
    template <typename N>
    static N read(const string_view s) {
        if (s.size() == 0) return N{};
        
        while (powers<N>().size() < s.size()) {
            powers<N>() = powers<N>().append(powers<N>().last() * 58);
        }
        
        functional_queue<N, list::linked<N>> Powers = powers<N>();
        
        N n{0};
        
        auto i = s.end();
        while(i != s.begin()) {
            i--;
            char v = digit(*i);
            if (v == -1) return N{0};
            
            n = n + (Powers.first() * uint64(digit));
        
            Powers = Powers.rest();
        }
        
        return n;
    }
    
    template <typename N>
    string write(N n) {
        if (n == 0) return "1";
        functional_queue<N, list::linked<N>> Powers = powers<N>();
        while(powers<N>().last() < n) {
            powers<N>() = powers<N>().append(powers<N>().last() * 58);
        }
        
        uint32 size = 0;
        Powers = powers<N>();
        while(Powers.first() <= n) {
            size ++;
            Powers = Powers.rest();
        }
        
        string s{static_cast<char>(size)};
        int i = size - 1;
        math::division<N> div;
        N x = n;
        while(true) {
            div = x.divide(58);
            s[i] = characters[(uint64)(div.Remainder)];
            i--;
            x = div.Quotient;
        }
        return s;
    };
    
    string write(const bytes&);

    template<unsigned long size>
    string write(const std::array<byte, size>&);
    bytes read(const std::string&);
    
    class string : public std::string {
        bytes Bytes;
        bytes* ToBytes;
        
    public:
        explicit operator bytes() const {
            if (ToBytes == nullptr) throw ;
            return *ToBytes;
        }
        
        bool valid() const {
            return ToBytes != nullptr;
        }
        
        string(std::string);
        
        string(std::string * sourceString):string(*sourceString){};
        
        string(const string& s) : 
            std::string{static_cast<const std::string&>(s)}, 
            Bytes{s.Bytes}, 
            ToBytes{s.ToBytes == nullptr ? nullptr : &Bytes} {}
        
        string(string&& s) : 
            std::string{static_cast<std::string&&>(s)}, 
            Bytes{s.Bytes}, 
            ToBytes{s.ToBytes == nullptr ? nullptr : &Bytes} {}
        
        string& operator=(string&);
    };
}

#endif
