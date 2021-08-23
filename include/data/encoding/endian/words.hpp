// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_WORDS
#define DATA_ENCODING_WORDS

#include <type_traits>
#include <data/types.hpp>
#include <data/encoding/endian/endian.hpp>
#include <data/tools/index_iterator.hpp>
#include <data/math/sign.hpp>
#include <data/meta/equal.hpp>
#include <data/slice.hpp>

namespace data::arithmetic {
    
    template <endian::order o> struct digits;
    
    template <> struct digits<endian::little> {
        slice<byte> Data;
        
        byte &operator[](int i);
        const byte &operator[](int i) const;
        
        using iterator = slice<byte>::iterator;
        using const_iterator = slice<byte>::const_iterator;
        using reverse_iterator = slice<byte>::reverse_iterator;
        using const_reverse_iterator = slice<byte>::const_reverse_iterator;
        
        iterator begin();
        iterator end();
        
        const_iterator begin() const;
        const_iterator end() const;
        
        reverse_iterator rbegin();
        reverse_iterator rend();
        
        const_reverse_iterator rbegin() const;
        const_reverse_iterator rend() const;
        
    };
    
    template <> struct digits<endian::big> {
        slice<byte> Data;
        
        byte &operator[](int i);
        const byte &operator[](int i) const;
        
        using iterator = slice<byte>::reverse_iterator;
        using const_iterator = slice<byte>::const_reverse_iterator;
        using reverse_iterator = slice<byte>::iterator;
        using const_reverse_iterator = slice<byte>::const_iterator;
        
        iterator begin();
        iterator end();
        
        const_iterator begin() const;
        const_iterator end() const;
        
        reverse_iterator rbegin();
        reverse_iterator rend();
        
        const_reverse_iterator rbegin() const;
        const_reverse_iterator rend() const;
        
    };
    
    template <size_t size> struct word;
    
    template <> struct word<1> {
        using type = byte;
    };
    
    template <> struct word<2> {
        using type = uint16;
    };
    
    template <> struct word<4> {
        using type = uint32;
    };
    
    template <size_t size> using word_type = typename word<size>::type;
    
    template <typename bit32, endian::order o> struct words;
    
    byte inline &digits<endian::little>::operator[](int i) {
        return Data[i];
    }
    
    const byte inline &digits<endian::little>::operator[](int i) const {
        return Data[i];
    }
    
    byte inline &digits<endian::big>::operator[](int i) {
        return Data[Data.size() - i - 1];
    }
    
    const byte inline &digits<endian::big>::operator[](int i) const {
        return Data[Data.size() - i - 1];
    }
    
    digits<endian::little>::iterator inline digits<endian::little>::begin() {
        return Data.begin();
    }
    
    digits<endian::little>::iterator inline digits<endian::little>::end() {
        return Data.end();
    }
    
    digits<endian::big>::iterator inline digits<endian::big>::begin() {
        return Data.rbegin();
    }
    
    digits<endian::big>::iterator inline digits<endian::big>::end() {
        return Data.rend();
    }
    
    digits<endian::little>::const_iterator inline digits<endian::little>::begin() const {
        return Data.begin();
    }
    
    digits<endian::little>::const_iterator inline digits<endian::little>::end() const {
        return Data.end();
    }
    
    digits<endian::big>::const_iterator inline digits<endian::big>::begin() const {
        return Data.rbegin();
    }
    
    digits<endian::big>::const_iterator inline digits<endian::big>::end() const {
        return Data.rend();
    }
    
    digits<endian::little>::reverse_iterator inline digits<endian::little>::rbegin() {
        return Data.rbegin();
    }
    
    digits<endian::little>::reverse_iterator inline digits<endian::little>::rend() {
        return Data.rend();
    }
    
    digits<endian::big>::reverse_iterator inline digits<endian::big>::rbegin() {
        return Data.begin();
    }
    
    digits<endian::big>::reverse_iterator inline digits<endian::big>::rend() {
        return Data.end();
    }
    
    digits<endian::little>::const_reverse_iterator inline digits<endian::little>::rbegin() const {
        return Data.rbegin();
    }
    
    digits<endian::little>::const_reverse_iterator inline digits<endian::little>::rend() const {
        return Data.rend();
    }
    
    digits<endian::big>::const_reverse_iterator inline digits<endian::big>::rbegin() const {
        return Data.begin();
    }
    
    digits<endian::big>::const_reverse_iterator inline digits<endian::big>::rend() const {
        return Data.end();
    }
        
    
}

#endif
