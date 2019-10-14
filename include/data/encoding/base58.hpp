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
    
    const std::string format{"base58"};
    
    const std::string characters{"123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz"};
    
    constexpr auto pattern = ctll::fixed_string{"[2-9A-HJ-NP-Za-km-z][1-9A-HJ-NP-Za-km-z]*"};
    
    inline bool valid(const string_view s) {
        return ctre::match<pattern>(s);
    }
    
    inline char digit(char c) {
        return c < '1' ? -1 : c <= '9' ?  c - '1' : c < 'A' ? -1 : c <= 'H' ? c - 'A' + 9 : c < 'J' ? -1 : c <= 'N' ? c - 'J' + 17 : c < 'P' ? -1 : c <= 'Z' ? c - 'P' + 22 : c < 'a' ? -1 : c <= 'k' ? c - 'a' + 33 : c < 'm' ? -1 : c <= 'z' ? c - 'm' + 44 : -1;
    };
    
    // cache of powers of 58.
    template <typename N>
    class power {
        functional_queue<N, list::linked<N>>& Powers;
        N First;
        functional_queue<N, list::linked<N>> Rest;
    public:
        power(
        functional_queue<N, list::linked<N>>& p, N n, 
        functional_queue<N, list::linked<N>> r) : Powers{p}, First{n}, Rest{r} {}
        
        N first() const {
            return First;
        }
        
        power rest() const {
            if (Rest.empty()) {
                N next{First * 58};
                Powers = Powers.append(next);
                return power{Powers, next, Rest};
            }
            return power{Powers, Rest.first(), Rest.rest()};
        }
    };
    
    // note: not good for multithreading. 
    template <typename N>
    power<N> powers() {
        static functional_queue<N, list::linked<N>> Powers = functional_queue<N, list::linked<N>>::make(N{1});
        return power<N>{Powers, Powers.first(), Powers.rest()};
    }
    
    template <typename N>
    N read(const string_view s) {
        if (s.size() == 0) return N{};
        
        N power{1};
        
        N n{0};
        
        for (char x : s) {
            char v = digit(x);
            if (v == -1) return N{0};
            n += power * uint64(v);
            power *= 58;
        }
        
        return n;
    }
    
    template <typename N>
    string write(N n) {
        if (n == 0) return "1";
        
        N power{1};
        
        uint32 size = 0;
        while(power <= n) {
            size ++;
            power *= 58;
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
