// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_WORDS
#define DATA_ENCODING_WORDS

#include <iterator>
#include <limits>
#include <type_traits>
#include <data/types.hpp>
#include <data/encoding/endian/endian.hpp>
#include <data/tools/index_iterator.hpp>
#include <data/slice.hpp>

namespace data::encoding {
    
    template <endian::order o, typename digit> struct words;
    
    template <typename digit> struct words<endian::little, digit> {
        slice<digit> Data;
        
        digit &operator[](int i);
        const digit &operator[](int i) const;
        
        using iterator = slice<digit>::iterator;
        using const_iterator = slice<digit>::const_iterator;
        using reverse_iterator = slice<digit>::reverse_iterator;
        using const_reverse_iterator = slice<digit>::const_reverse_iterator;
        
        iterator begin();
        iterator end();
        
        const_iterator begin() const;
        const_iterator end() const;
        
        reverse_iterator rbegin();
        reverse_iterator rend();
        
        const_reverse_iterator rbegin() const;
        const_reverse_iterator rend() const;
        
        words<endian::big, digit> reverse() const;
        
        size_t size() const {
            return end() - begin();
        }
        
    };
    
    template <typename digit> struct words<endian::big, digit> {
        slice<digit> Data;
        
        digit &operator[](int i);
        const digit &operator[](int i) const;
        
        using iterator = slice<digit>::reverse_iterator;
        using const_iterator = slice<digit>::const_reverse_iterator;
        using reverse_iterator = slice<digit>::iterator;
        using const_reverse_iterator = slice<digit>::const_iterator;
        
        iterator begin();
        iterator end();
        
        const_iterator begin() const;
        const_iterator end() const;
        
        reverse_iterator rbegin();
        reverse_iterator rend();
        
        const_reverse_iterator rbegin() const;
        const_reverse_iterator rend() const;
        
        words<endian::little, digit> reverse() const;
        
        size_t size() const {
            return end() - begin();
        }
        
    };
    
    template <typename digit> 
    digit inline &words<endian::little, digit>::operator[](int i) {
        return Data[i];
    }
    
    template <typename digit> 
    const digit inline &words<endian::little, digit>::operator[](int i) const {
        return Data[i];
    }
    
    template <typename digit> 
    digit inline &words<endian::big, digit>::operator[](int i) {
        return Data[-i - 1];
    }
    
    template <typename digit> 
    const digit inline &words<endian::big, digit>::operator[](int i) const {
        return Data[-i - 1];
    }
    
    template <typename digit> 
    words<endian::little, digit>::iterator inline words<endian::little, digit>::begin() {
        return Data.begin();
    }
    
    template <typename digit> 
    words<endian::little, digit>::iterator inline words<endian::little, digit>::end() {
        return Data.end();
    }
    
    template <typename digit> 
    words<endian::big, digit>::iterator inline words<endian::big, digit>::begin() {
        return Data.rbegin();
    }
    
    template <typename digit> 
    words<endian::big, digit>::iterator inline words<endian::big, digit>::end() {
        return Data.rend();
    }
    
    template <typename digit> 
    words<endian::little, digit>::const_iterator inline words<endian::little, digit>::begin() const {
        return Data.begin();
    }
    
    template <typename digit> 
    words<endian::little, digit>::const_iterator inline words<endian::little, digit>::end() const {
        return Data.end();
    }
    
    template <typename digit> 
    words<endian::big, digit>::const_iterator inline words<endian::big, digit>::begin() const {
        return Data.rbegin();
    }
    
    template <typename digit> 
    words<endian::big, digit>::const_iterator inline words<endian::big, digit>::end() const {
        return Data.rend();
    }
    
    template <typename digit> 
    words<endian::little, digit>::reverse_iterator inline words<endian::little, digit>::rbegin() {
        return Data.rbegin();
    }
    
    template <typename digit> 
    words<endian::little, digit>::reverse_iterator inline words<endian::little, digit>::rend() {
        return Data.rend();
    }
    
    template <typename digit> 
    words<endian::big, digit>::reverse_iterator inline words<endian::big, digit>::rbegin() {
        return Data.begin();
    }
    
    template <typename digit> 
    words<endian::big, digit>::reverse_iterator inline words<endian::big, digit>::rend() {
        return Data.end();
    }
    
    template <typename digit> 
    words<endian::little, digit>::const_reverse_iterator inline words<endian::little, digit>::rbegin() const {
        return Data.rbegin();
    }
    
    template <typename digit> 
    words<endian::little, digit>::const_reverse_iterator inline words<endian::little, digit>::rend() const {
        return Data.rend();
    }
    
    template <typename digit> 
    words<endian::big, digit>::const_reverse_iterator inline words<endian::big, digit>::rbegin() const {
        return Data.begin();
    }
    
    template <typename digit> 
    words<endian::big, digit>::const_reverse_iterator inline words<endian::big, digit>::rend() const {
        return Data.end();
    }
    
    template <typename digit> 
    words<endian::big, digit> inline words<endian::little, digit>::reverse() const {
        return {Data};
    }
    
    template <typename digit> 
    words<endian::little, digit> inline words<endian::big, digit>::reverse() const {
        return {Data};
    }
    
}

#endif
