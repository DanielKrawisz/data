// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_WORDS
#define DATA_ENCODING_WORDS

#include <iterator>
#include <limits>
#include <type_traits>
#include <data/slice.hpp>
#include <data/encoding/halves.hpp>
#include <data/tools/index_iterator.hpp>
#include <iostream>
#include <sstream>

namespace data::arithmetic {
    
    template <typename sen, typename it>
    requires std::input_iterator<it> && std::sentinel_for<sen, it>
    bool equal (sen z, it i, it j) {
        while (i != z) {
            if (*i != *j) return false;
            i++;
            j++;
        }
        return true;
    }
    
    template <typename sen, typename it>
    requires std::input_iterator<it> && std::sentinel_for<sen, it>
    bool greater (sen z, it i, it j) {
        while (i != z) {
            if (*i > *j) return true;
            i++;
            j++;
        }
        return false;
    }
    
    template <typename sen, typename it>
    requires std::input_iterator<it> && std::sentinel_for<sen, it>
    bool less (sen z, it i, it j) {
        while (i != z) {
            if (*i < *j) return true;
            i++;
            j++;
        }
        return false;
    }
    
    template <typename sen, typename it>
    requires std::input_iterator<it> && std::sentinel_for<sen, it>
    std::weak_ordering compare (sen z, it i, it j) {
        while (i != z) {
            if (*i < *j) return std::weak_ordering::less;
            if (*i > *j) return std::weak_ordering::greater;
            i++;
            j++;
        }
        return std::weak_ordering::equivalent;
    }
    
    template <typename digit, typename sen, typename ito, typename iti>
    requires std::sentinel_for<sen, ito> && std::output_iterator<ito, digit> && std::input_iterator<iti> 
    void bit_negate (sen z, ito i, iti j) {
        while (i != z) {
            *i = ~ *j;
            i++;
            j++;
        }
    }
    
    template <typename digit, typename sen, typename ito, typename iti>
    requires std::sentinel_for<sen, ito> && std::output_iterator<ito, digit> && std::input_iterator<iti> 
    void bit_and (sen z, ito i, iti a, iti b) {
        while (i != z) {
            *i = *a & *b;
            i++;
            a++;
            b++;
        }
    }
    
    template <typename digit, typename sen, typename ito, typename iti>
    requires std::sentinel_for<sen, ito> && std::output_iterator<ito, digit> && std::input_iterator<iti> 
    void bit_or (sen z, ito i, iti a, iti b) {
        while (i != z) {
            *i = *a | *b;
            i++;
            a++;
            b++;
        }
    }
    
    template <typename digit, typename sen, typename ito, typename iti>
    requires std::sentinel_for<sen, ito> && std::output_iterator<ito, digit> && std::input_iterator<iti> 
    void bit_xor (sen z, ito i, iti a, iti b) {
        while (i != z) {
            *i = *a ^ *b;
            i++;
            a++;
            b++;
        }
    }
    
    template <typename digit, typename sen, typename ito, typename iti>
    requires std::sentinel_for<sen, ito> && std::output_iterator<ito, digit> && std::input_iterator<iti> 
    digit plus (sen z, ito &a, digit d, iti &b) {
        using two_digits = typename encoding::twice<digit>::type;
        
        digit remainder = d;
        while (a != z) {
            two_digits result = encoding::add<digit> (*b, 0, remainder);
            *a = encoding::lesser_half (result);
            remainder = encoding::greater_half (result);

            a++;
            b++;
        }
        
        return remainder;
    }
    
    template <typename digit, typename sen, typename ito, typename iti>
    requires std::sentinel_for<sen, ito> && std::output_iterator<ito, digit> && std::input_iterator<iti> 
    digit minus (sen z, ito &a, digit d, iti &b) {
        digit remainder = d;
        while (a != z) {
            if (*b >= remainder) {
                *a = *b - remainder; 
                remainder = 0;
            } else {
                *a = encoding::lesser_half<typename encoding::twice<digit>::type> (encoding::combine<digit> (1, *b) - remainder);
                remainder = 1;
            }

            a++;
            b++;
        }
        
        return remainder;
    }
    
    template <typename digit, typename sen, typename ito, typename iti>
    requires std::sentinel_for<sen, ito> && std::output_iterator<ito, digit> && std::input_iterator<iti> 
    digit plus (sen z, ito &i, iti &a, iti &b) {
        using two_digits = typename encoding::twice<digit>::type;
        
        digit remainder = 0;
        
        while (i != z) {
            two_digits result = encoding::add<digit> (*a, *b, remainder);
            remainder = encoding::greater_half (result);
            *i = encoding::lesser_half (result);
            i++;
            a++;
            b++;
        }
        
        return remainder;
    }
    
    template <typename digit, typename sen, typename ito, typename iti>
    requires std::sentinel_for<sen, ito> && std::output_iterator<ito, digit> && std::input_iterator<iti> 
    digit minus (sen z, ito &i, iti &a, iti &b) {
        using two_digits = typename encoding::twice<digit>::type;
        
        two_digits remainder = 0;
        
        while (i != z) {

            auto step = encoding::combine<digit> (1, *a) - *b - remainder;

            *i = encoding::lesser_half<typename encoding::twice<digit>::type> (step);

            remainder = encoding::greater_half<typename encoding::twice<digit>::type> (step) == 0 ? 1 : 0;

            i++;
            a++;
            b++;
        }
        
        return remainder;
    }
    
    // bit shift operations are defined in terms of big-endian numbers. 
    // but really they are operations to powers of 2 ignoring sign, 
    // regardless of the way that a number is actually represented. 
    // we can only do this with bytes and not bigger numbers because of 
    // little-endian encoding. 
    template <typename sen, typename ita, typename itb>
    requires std::input_iterator<itb> && std::output_iterator<ita, byte> && std::sentinel_for<sen, itb>
    void shift_left (ita &i, sen z, itb b, byte amount, byte fill) {
        using two_digits = typename encoding::twice<byte>::type;
        
        while (b != z) {
            auto bp = b;
            bp++;
            two_digits result = encoding::combine<byte> (*b, bp != z ? *bp : fill) << amount;
            *i = encoding::greater_half (result);
            i++;
            b = bp;
        }
    }
    
    // you have to use reverse iterators for this function. 
    template <typename sen, typename ita, typename itb>
    requires std::input_iterator<itb> && std::output_iterator<ita, byte> && std::sentinel_for<sen, itb>
    void shift_right (ita &i, sen z, itb b, byte amount, byte fill) {
        using two_digits = typename encoding::twice<byte>::type;
        
        while (b != z) {
            auto bp = b;
            bp++;
            two_digits x = encoding::combine<byte> (bp != z ? *bp : fill, *b);
            two_digits result = encoding::combine<byte> (bp != z ? *bp : fill, *b) >> amount;
            *i = encoding::lesser_half (result);
            i++;
            b = bp;
        }
    }
    
    template <typename it, typename sen>
    void bit_shift_left (it i, sen z, uint32 x, bool fill) {
        auto bytes = x / 8;
        auto bits = x % 8;
        byte filler = fill ? ~0 : 0;
        
        size_t size = z - i;
        if (bytes <= size) arithmetic::shift_left (i, z, i + bytes, bits, filler);
        
        while (i != z) {
            *i = filler;
            i++;
        }
    }
    
    template <typename it, typename sen>
    void bit_shift_right (it i, sen z, uint32 x, bool fill) {
        auto bytes = x / 8;
        auto bits = x % 8;
        byte filler = fill ? ~0 : 0;
        
        size_t size = z - i;
        if (bytes <= size) arithmetic::shift_right (i, z, i + bytes, bits, filler);
        
        while (i != z) {
            *i = filler;
            i++;
        }
    }
}

namespace data::encoding {
    
    template <endian::order o, typename digit> struct words;
    
    template <typename digit> struct words<endian::little, digit> {
        slice<digit> Data;
        
        digit &operator [] (int i);
        const digit &operator [] (int i) const;
        
        using iterator = slice<digit>::iterator;
        using const_iterator = slice<digit>::const_iterator;
        using reverse_iterator = slice<digit>::reverse_iterator;
        using const_reverse_iterator = slice<digit>::const_reverse_iterator;
        
        iterator begin ();
        iterator end ();
        
        const_iterator begin () const;
        const_iterator end () const;
        
        reverse_iterator rbegin ();
        reverse_iterator rend ();
        
        const_reverse_iterator rbegin () const;
        const_reverse_iterator rend () const;
        
        words<endian::big, digit> reverse () const;
        
        size_t size () const {
            return end () - begin ();
        }
        
        void bit_shift_left (uint32 x, bool fill = false) {
            arithmetic::bit_shift_left (
                std::reverse_iterator {(byte*) Data.data () + Data.size () * sizeof (digit)},
                std::reverse_iterator {(byte*) Data.data ()}, x, fill);
        }
        
        void bit_shift_right (uint32 x, bool fill = false) {
            auto it = (byte*) Data.data ();
            arithmetic::bit_shift_right (it, it + Data.size () * sizeof (digit), x, fill);
        }
        
    };
    
    template <typename digit> struct words<endian::big, digit> {
        slice<digit> Data;
        
        digit &operator [] (int i);
        const digit &operator [] (int i) const;
        
        using iterator = slice<digit>::reverse_iterator;
        using const_iterator = slice<digit>::const_reverse_iterator;
        using reverse_iterator = slice<digit>::iterator;
        using const_reverse_iterator = slice<digit>::const_iterator;
        
        iterator begin ();
        iterator end ();
        
        const_iterator begin () const;
        const_iterator end () const;
        
        reverse_iterator rbegin ();
        reverse_iterator rend ();
        
        const_reverse_iterator rbegin () const;
        const_reverse_iterator rend () const;
        
        words<endian::little, digit> reverse () const;
        
        size_t size () const {
            return end () - begin ();
        }
        
        void bit_shift_left (uint32 x, bool fill = false) {
            auto it = (byte*) Data.data ();
            arithmetic::bit_shift_left (it, it + Data.size () * sizeof (digit), x, fill);
        }
        
        void bit_shift_right (uint32 x, bool fill = false) {
            arithmetic::bit_shift_right (
                std::reverse_iterator {(byte*) Data.data () + Data.size () * sizeof (digit)},
                std::reverse_iterator {(byte*) Data.data ()}, x, fill);
        }
        
    };
    
    template <typename digit> 
    digit inline &words<endian::little, digit>::operator [] (int i) {
        return Data[i];
    }
    
    template <typename digit> 
    const digit inline &words<endian::little, digit>::operator [] (int i) const {
        return Data[i];
    }
    
    template <typename digit>
    digit inline &words<endian::big, digit>::operator [] (int i) {
        return Data[-i - 1];
    }
    
    template <typename digit> 
    const digit inline &words<endian::big, digit>::operator [] (int i) const {
        return Data[-i - 1];
    }
    
    template <typename digit> 
    words<endian::little, digit>::iterator inline words<endian::little, digit>::begin () {
        return Data.begin ();
    }
    
    template <typename digit> 
    words<endian::little, digit>::iterator inline words<endian::little, digit>::end () {
        return Data.end ();
    }
    
    template <typename digit> 
    words<endian::big, digit>::iterator inline words<endian::big, digit>::begin () {
        return Data.rbegin ();
    }
    
    template <typename digit> 
    words<endian::big, digit>::iterator inline words<endian::big, digit>::end () {
        return Data.rend ();
    }
    
    template <typename digit> 
    words<endian::little, digit>::const_iterator inline words<endian::little, digit>::begin () const {
        return Data.begin ();
    }
    
    template <typename digit> 
    words<endian::little, digit>::const_iterator inline words<endian::little, digit>::end () const {
        return Data.end ();
    }
    
    template <typename digit> 
    words<endian::big, digit>::const_iterator inline words<endian::big, digit>::begin () const {
        return Data.rbegin ();
    }
    
    template <typename digit> 
    words<endian::big, digit>::const_iterator inline words<endian::big, digit>::end () const {
        return Data.rend ();
    }
    
    template <typename digit> 
    words<endian::little, digit>::reverse_iterator inline words<endian::little, digit>::rbegin () {
        return Data.rbegin ();
    }
    
    template <typename digit> 
    words<endian::little, digit>::reverse_iterator inline words<endian::little, digit>::rend () {
        return Data.rend ();
    }
    
    template <typename digit> 
    words<endian::big, digit>::reverse_iterator inline words<endian::big, digit>::rbegin () {
        return Data.begin ();
    }
    
    template <typename digit> 
    words<endian::big, digit>::reverse_iterator inline words<endian::big, digit>::rend () {
        return Data.end ();
    }
    
    template <typename digit> 
    words<endian::little, digit>::const_reverse_iterator inline words<endian::little, digit>::rbegin () const {
        return Data.rbegin ();
    }
    
    template <typename digit> 
    words<endian::little, digit>::const_reverse_iterator inline words<endian::little, digit>::rend () const {
        return Data.rend ();
    }
    
    template <typename digit> 
    words<endian::big, digit>::const_reverse_iterator inline words<endian::big, digit>::rbegin () const {
        return Data.begin ();
    }
    
    template <typename digit> 
    words<endian::big, digit>::const_reverse_iterator inline words<endian::big, digit>::rend () const {
        return Data.end ();
    }
    
    template <typename digit> 
    words<endian::big, digit> inline words<endian::little, digit>::reverse () const {
        return {Data};
    }
    
    template <typename digit> 
    words<endian::little, digit> inline words<endian::big, digit>::reverse () const {
        return {Data};
    }
    
}

#endif
