// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_SLICE
#define DATA_SLICE

#include "list.hpp"
#include <data/tools/index_iterator.hpp>
#include <data/meta/greater.hpp>
#include <data/meta/unsigned_minus.hpp>

namespace data {
    // Slice is an indexed section of an array which
    // can create smaller slices. 
    template <typename X, size_t ...> struct slice;
    
    // Slice is an indexed section of an array which
    // can create smaller slices. 
    template <typename X> struct slice<X> {
        using iterator = X*;
        using const_iterator = const X*;
        using value_type = X;
    private:
        iterator Begin;
        iterator End;
    public:
        slice(iterator b, iterator e) : Begin{b}, End{e} {}
        slice(iterator b, size_t size) : Begin{b}, End{b + size} {}
        slice() : Begin{nullptr}, End{nullptr} {}
        slice(std::vector<X>& x) : slice(x.data(), x.size()) {}
        template <size_t n>
        slice(std::array<X, n>& x) : slice(x.data(), x.size()) {} 
        template <typename A>
        slice(A x) : slice(x.begin(), x.end()) {} 
        
        template <typename A>
        static const slice make(const A& x) {
            return slice{const_cast<A&>(x)};
        }
        
        virtual const size_t size() const {
            return End - Begin;
        }
        
        X& operator[](size_t n) const {
            if (n >= size()) throw std::out_of_range{"index out of range"};
            return *(Begin + n);
        }

        /// Selects a range from the current slice
        /// \param b range begins from this index inclusive
        /// \param e range ends at this index excluisive
        /// \return a slice containing the requested range
        [[nodiscard]] slice<X> range(int32 b, int32 e) const {
            size_t len = size();
            if(b < 0) b = len + b;
            if(e < 0) e = len + e;
            if (b >= len || e > len || b >= e || b < 0) return slice{};

            return slice{Begin + b, e - b};
        }

        /// Selects a range from the current slice up to end of slice
        /// \param b  range begins from this index inclusive
        /// \return a slice containing the requested range
        [[nodiscard]] slice<X> range(int32 b) const {
            return range(b, size());
        }
        
        template <size_t b, size_t e>
        slice<X, meta::unsigned_minus<e, b>::result> range() const;
        
        slice& operator=(const slice<X>& s) {
            Begin = s.Begin;
            End = s.End;
            return *this;
        }
        
        bool operator==(const slice<X>& s) const {
            if(Begin == s.Begin && End == s.End)
                return true;
            if(this->size()!=s.size())
                return false;
            for(int i=0;i<size();i++)
                if(operator[](i)!=s[i])
                    return false;
            return true;
        }
        
        bool operator!=(const slice<X>& s) const {
            return !operator==(s);
        }
        
        iterator begin() {
            return Begin;
        }
        
        iterator end() {
            return End;
        }
        
        const_iterator begin() const {
            return Begin;
        }
        
        const_iterator end() const {
            return End;
        }
        
    };
    
    template <typename X, size_t n> struct slice<X, n> : public slice<X> {
        const size_t size() const final override {
            return n;
        }
        
        slice(std::array<X, n>& x) : slice<X>{x} {}
        
        static const slice make(const std::array<X, n>& x) {
            return slice{const_cast<std::array<X, n>&>(x)};
        }
    };
    
    template <typename X> 
    template <size_t b, size_t e>
    inline slice<X, meta::unsigned_minus<e, b>::result> slice<X>::range() const {
        static meta::greater<e, b> requirement{};
        return slice<X, meta::unsigned_minus<end, begin>::result>{slice<X>::Begin + b, e - b};
    }

}

#endif
