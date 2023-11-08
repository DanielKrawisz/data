// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_WORDS
#define DATA_ENCODING_WORDS

#include <iterator>
#include <limits>
#include <type_traits>
#include <data/slice.hpp>
#include <data/math/number/bytes/arithmetic.hpp>
#include <data/tools/index_iterator.hpp>
#include <iostream>
#include <sstream>

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
        
        void bit_shift_left (uint32 x, bool fill = false);
        
        void bit_shift_right (uint32 x, bool fill = false);
        
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
        
        void bit_shift_left (uint32 x, bool fill = false);
        
        void bit_shift_right (uint32 x, bool fill = false);
        
    };
}

namespace data::math::number::arithmetic {

    // must check that the input has at least size 1 to use.
    template <endian::order r, typename digit> void inline flip_sign_bit (encoding::words<r, digit> x) {
        if (x[-1] & get_sign_bit<digit>::value) x[-1] &= ~get_sign_bit<digit>::value;
        else x[-1] |= get_sign_bit<digit>::value;
    }

    template <endian::order r, typename digit> void negate_ones (encoding::words<r, digit> x) {
        bit_negate<digit> (x.end (), x.begin (), x.begin ());
        auto o = x.begin ();
        auto i = x.begin ();
        plus (x.end (), o, 1, i);
    }

    template <endian::order r, typename digit> void inline negate_twos (encoding::words<r, digit> x) {
        if (x.size () == 0) return;
        flip_sign_bit (x);
    }

    template <typename digit> struct get_limit;

    template<> struct get_limit<byte> {
        static const byte max_unsigned = 0xff;
        static const byte max_signed = 0x7f;
    };

    template<> struct get_limit<uint16> {
        static const uint16 max_unsigned = 0xffff;
        static const uint16 max_signed = 0x7fff;
    };

    template<> struct get_limit<uint32> {
        static const uint32 max_unsigned = 0xffffffff;
        static const uint32 max_signed = 0x7ffffff;
    };

    template<> struct get_limit<uint64> {
        static const uint64 max_unsigned = 0xffffffffffffffff;
        static const uint64 max_signed = 0x7fffffffffffffff;
    };

    template <typename digit> const digit max_unsigned = get_limit<digit>::max_unsigned;
    template <typename digit> const digit max_signed_ones = get_limit<digit>::max_signed;
    template <typename digit> const digit min_unsigned_ones = get_sign_bit<digit>::value;

    template <endian::order r, typename digit>
    void set_max_unsigned (encoding::words<r, digit> a) {
        for (digit &x : a) x = max_unsigned<digit>;
    }

    template <endian::order r, typename digit>
    void set_max_signed_ones (encoding::words<r, digit> a) {
        auto i = a.rbegin ();
        if (i == a.rend ()) return;
        *i = max_signed_ones<digit>;
        while (true) {
            i++;
            if (i == a.rend ()) return;
            *i = max_unsigned<digit>;
        }
    }

    template <endian::order r, typename digit>
    void set_min_signed_ones (encoding::words<r, digit> a) {
        auto i = a.rbegin ();
        if (i == a.rend ()) return;
        *i = min_unsigned_ones<digit>;
        while (true) {
            i++;
            if (i == a.rend ()) return;
            *i = 0;
        }
    }

    template <endian::order r, typename digit>
    void plus (encoding::words<r, digit> &o, const encoding::words<r, digit> &a, const encoding::words<r, digit> &b) {
        if (a.size () < b.size ()) return plus (o, b, a);
        if (o.size () <= a.size ()) throw exception {"need a bigger space to add numbers"};

        auto oit = o.begin ();
        auto ait = a.begin ();
        auto bit = b.begin ();

        auto end_step_1 = oit + b.size ();
        auto end_step_2 = oit + a.size ();

        digit remainder_1 = plus<digit> (end_step_1, oit, ait, bit);
        digit remainder_2 = plus (end_step_2, oit, remainder_1, ait);
        auto oiti = oit;
        plus (o.end (), oit, remainder_2, oiti);

    }

    // we should already be able to expect that a > b, so the result will not go from positive to negative.
    template <endian::order r, typename digit>
    void minus (encoding::words<r, digit> &o, const encoding::words<r, digit> &a, const encoding::words<r, digit> &b) {

        auto oit = o.begin ();
        auto ait = a.begin ();
        auto bit = b.begin ();

        auto end_step_1 = oit + std::min (a.size (), b.size ());
        auto end_step_2 = oit + a.size ();

        digit remainder = minus<digit> (end_step_1, oit, ait, bit);
        minus<digit> (end_step_2, oit, remainder, ait);

    }

    template <endian::order r, typename digit>
    void times (encoding::words<r, digit> &o, const encoding::words<r, digit> &a, const encoding::words<r, digit> &b) {

        // if the size of b is zero, then the answer is zero and we can skip to the end.
        if (a.size () == 0 || b.size () == 0) return;

        // we ensure that a is at least as big as b.
        if (a.size () < b.size ()) return times (o, b, a);

        using two_digits = typename encoding::twice<digit>::type;

        auto io = o.begin ();
        auto ia = a.begin ();
        auto ib = b.begin ();

        // We leave iterators at the beginning of a and b.
        // We know that neither of these is at the end and
        // we will use that information.
        auto ba = ia;
        auto bb = ib;

        // first we deal with the case where we have not exhausted b or a.
        do {

            // we start two new iterators, one at the start of a
            // and the other at the current position of b.
            auto ax = ba;
            auto bx = ib;

            while (true) {
                two_digits result = two_digits (*ax) * two_digits (*bx);

                auto pio = io;
                auto pia = io;
                plus<digit> (o.end (), pio, encoding::lesser_half (result), pia);

                auto mio = io + 1;
                auto mia = io + 1;
                plus<digit> (o.end (), mio, encoding::greater_half (result), mia);

                if (ax == ia) break;
                ax++;
                bx--;
            }

            io++;
            ia++;
            ib++;

        } while (ib != b.end ());

        // b has been exhausted but not a.
        while (ia != a.end ()) {

            // we know we can do this because the size of a
            // is greater than zero. Therefore ia will be
            // ahead of ba.
            ba++;

            auto ax = ba;
            auto bx = ib - 1;

            while (true) {
                two_digits result = two_digits (*ax) * two_digits (*bx);

                auto pio = io;
                auto pia = io;
                plus<digit> (o.end (), pio, encoding::lesser_half (result), pia);

                auto mio = io + 1;
                auto mia = io + 1;
                plus<digit> (o.end (), mio, encoding::greater_half (result), mia);

                // when this happens, bx will be back at the beginning of b.
                if (ax == ia) break;
                ax++;
                bx--;
            }

            io++;
            ia++;

        }

        while (io != o.end ()) {

            ba++;

            if (ba == a.end ()) break;

            auto ax = ba;
            auto bx = ib - 1;

            while (ax != a.end ()) {
                two_digits result = two_digits (*ax) * two_digits (*bx);

                auto pio = io;
                auto pia = io;
                plus<digit> (o.end (), pio, encoding::lesser_half (result), pia);

                auto mio = io + 1;
                auto mia = io + 1;
                plus<digit> (o.end (), mio, encoding::greater_half (result), mia);

                ax++;
                bx--;
            }

            io++;
        }

    }
}

namespace data::encoding {

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

    template <typename digit>
    void words<endian::little, digit>::bit_shift_left (uint32 x, bool fill) {
        math::number::arithmetic::bit_shift_left (
            std::reverse_iterator {(byte*) Data.data () + Data.size () * sizeof (digit)},
            std::reverse_iterator {(byte*) Data.data ()}, x, fill);
    }

    template <typename digit>
    void words<endian::little, digit>::bit_shift_right (uint32 x, bool fill) {
        auto it = (byte*) Data.data ();
        math::number::arithmetic::bit_shift_right (it, it + Data.size () * sizeof (digit), x, fill);
    }
    
    template <typename digit>
    void words<endian::big, digit>::bit_shift_left (uint32 x, bool fill) {
        auto it = (byte*) Data.data ();
        math::number::arithmetic::bit_shift_left (it, it + Data.size () * sizeof (digit), x, fill);
    }

    template <typename digit>
    void words<endian::big, digit>::bit_shift_right (uint32 x, bool fill) {
        math::number::arithmetic::bit_shift_right (
            std::reverse_iterator {(byte*) Data.data () + Data.size () * sizeof (digit)},
            std::reverse_iterator {(byte*) Data.data ()}, x, fill);
    }
}

#endif
