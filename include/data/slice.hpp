// Copyright (c) 2019-202[ Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_SLICE
#define DATA_SLICE

#include <data/tools/index_iterator.hpp>
#include <data/meta/greater.hpp>
#include <data/meta/unsigned_minus.hpp>
#include <data/io/unimplemented.hpp>

namespace data {
    // Slice is an indexed section of an array which
    // can create smaller slices. 
    template <typename X, size_t ...> struct slice;
    
    // Slice is an indexed section of an array which
    // can create smaller slices. 
    template <typename X> struct slice<X> {
        using value_type = X;
        using iterator = X*;
        using const_iterator = const X*;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = const std::reverse_iterator<iterator>;
    private:
        X* Data;
        size_t Size;
    public:
        slice(iterator b, size_t size) : Data{b}, Size{size} {}
        slice() : Data{nullptr}, Size{0} {}
        
        slice(std::vector<X>& x) : slice{x.data(), x.size()} {}
        
        /*template <typename A>
        slice(A& x) : slice(x.data(), x.size()) {} */
        
        virtual const size_t size() const {
            return Size;
        }
        
        X& operator[](size_t n) const {
            if (n >= size() || n < 0) throw std::out_of_range{"index out of range"};
            return Data[n];
        }

        /// Selects a range from the current slice
        /// \param b range begins from this index inclusive
        /// \param e range ends at this index excluisive
        /// \return a slice containing the requested range
        [[nodiscard]] slice<X> range(int b, int e) const {
            if (b < 0) b = Size + b;
            if (e < 0) e = Size + e;
            if (e > Size || b > e || b < 0 || e < 0) return slice{};

            return slice{Data + b, static_cast<size_t>(e - b)};
        }

        /// Selects a range from the current slice up to end of slice
        /// \param b  range begins from this index inclusive
        /// \return a slice containing the requested range
        [[nodiscard]] slice<X> range(int32 b) const {
            return range(0, b);
        }
        
        template <int b, int e>
        slice<X, meta::unsigned_minus<e, b>::result> range() const;
        
        slice& operator=(const slice<X>& s) {
            Data = s.Data;
            Size = s.Size;
            return *this;
        }
        
        bool operator==(const slice<X>& s) const {
            if(Data == s.Data && Size == s.Size) return true;
            if(this->Size != s.Size) return false;
            for(int i=0;i<size();i++)
                if(operator[](i)!=s[i])
                    return false;
            return true;
        }
        
        bool operator!=(const slice<X>& s) const {
            return !operator==(s);
        }
        
        X* data() {
            return Data;
        }
        
        const X* data() const {
            return Data;
        }
        
        iterator begin() {
            return Data;
        }
        
        iterator end() {
            return Data + Size * sizeof(X);
        }
        
        const_iterator begin() const {
            return Data;
        }
        
        const_iterator end() const {
            return Data + Size * sizeof(X);
        }
        
        reverse_iterator rbegin() {
            return reverse_iterator{Data + Size * sizeof(X)};
        }
        
        reverse_iterator rend() {
            return reverse_iterator{Data};
        }
        
        const_reverse_iterator rbegin() const {
            return reverse_iterator{Data + Size * sizeof(X)};
        }
        
        const_reverse_iterator rend() const {
            return reverse_iterator{Data};
        }
        
        operator std::basic_string_view<X>() const {
            return std::basic_string_view<X>{Data, Size * sizeof(X)};
        }
        
    };
    
    template <typename X, size_t n> struct slice<X, n> : public slice<X> {
        const size_t size() const final override {
            return n;
        }
        
        slice(X* x) : slice<X>{x, n} {}
    };
    
    template <typename X> 
    template <int b, int e>
    inline slice<X, meta::unsigned_minus<e, b>::result> slice<X>::range() const {
        static meta::greater<e, b> requirement{};
        return slice<X, meta::unsigned_minus<e, b>::result>{slice<X>::Data + b};
    }

}

#endif
