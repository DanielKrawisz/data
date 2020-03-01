// Copyright (c) 2019-202[ Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_SLICE
#define DATA_SLICE

#include <data/tools/index_iterator.hpp>
#include <data/meta/greater.hpp>
#include <data/meta/unsigned_minus.hpp>
#include <data/io/unimplemented.hpp>

namespace data::meta {

    template <uint32 X, uint32 ceil>
    struct ceiling {
        constexpr static uint32 result = (X % ceil == 0 ? X : X % ceil);
    };
    
}

namespace data {
    
    struct range {
        int Begin;
        int End;
        
        range(int b, int e) : Begin{b}, End{e} {}
        
        int size() const {
            return End - Begin;
        }
        
        range operator%(size_t Size) const {
            range x(Begin % static_cast<int>(Size), End % static_cast<int>(Size));
            if (x.Begin < 0) x.Begin += Size;
            if (x.End < 0) x.End += Size;
            return x;
        }
    };
    
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
    protected:
        X* Data;
    public:
        size_t Size;
        slice() : Data{nullptr}, Size{0} {}
        slice(iterator b, size_t size) : Data{b}, Size{size} {}
        
        slice(std::vector<X>& x) : slice{x.data(), x.size()} {}
        
        const size_t size() const {
            return Size;
        }
        
        bool valid() const {
            return Data != nullptr && Size >= 0;
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
            data::range x = data::range{b, e} % Size;
            int new_size = x.size();
            return new_size < 0 ? slice{} : slice{Data + x.Begin, static_cast<size_t>(new_size)};
        }

        /// Selects a range from the current slice up to end of slice
        /// \param b  range begins from this index inclusive
        /// \return a slice containing the requested range
        [[nodiscard]] slice<X> range(int32 b) const {
            return range(0, b);
        }
        
        template <size_t b, size_t e>
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
        bool valid() const {
            return slice<X>::valid() && slice<X>::Size == n;
        }
        
        slice(X* x) : slice<X>{x, n} {}
        
        template <int b, int e>
        slice<X, meta::unsigned_minus<meta::ceiling<e, n>::value, meta::ceiling<b, n>::value>::result> range() const;
        
        using slice<X>::range;
    };
    
    template <typename X> 
    template <size_t b, size_t e>
    inline slice<X, meta::unsigned_minus<e, b>::result> slice<X>::range() const {
        static meta::greater<e, b> requirement{};
        return slice<X, meta::unsigned_minus<e, b>::result>{slice<X>::Data + b};
    }
    
    template <typename X, size_t n> 
    template <int b, int e>
    inline slice<X, meta::unsigned_minus<meta::ceiling<e, n>::value, meta::ceiling<b, n>::value>::result> 
    slice<X, n>::range() const {
        static meta::greater<meta::ceiling<e, n>::value, meta::ceiling<b, n>::value> requirement{};
        return slice<X, meta::unsigned_minus<meta::ceiling<e, n>::value, meta::ceiling<b, n>::value>::result>{
            slice<X>::Data + meta::ceiling<b, n>::value};
    }

}

#endif
