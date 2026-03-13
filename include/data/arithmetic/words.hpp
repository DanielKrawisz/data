// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ARITHMETIC_WORDS
#define DATA_ARITHMETIC_WORDS

#include <iterator>
#include <limits>
#include <type_traits>
#include <sstream>
#include <data/io/exception.hpp>
#include <data/slice.hpp>
#include <data/arithmetic/arithmetic.hpp>
#include <data/tools/index_iterator.hpp>

namespace data::arithmetic {
    
    // Words is for iterating over digits that have an endian order.
    // Using Words, you will always iterate from least to most
    // significant digits. Reverse iteration will go from most to least.
    template <endian::order o, typename digit> struct Words;
    
    template <typename digit> struct Words<endian::little, digit> {
        slice<digit> Data;

        constexpr Words (slice<digit> d) : Data {d} {}
        
        constexpr digit &operator [] (int i);
        constexpr const digit &operator [] (int i) const;
        
        using iterator = slice<digit>::iterator;
        using const_iterator = slice<digit>::const_iterator;
        using reverse_iterator = slice<digit>::reverse_iterator;
        using const_reverse_iterator = slice<digit>::const_reverse_iterator;
        
        constexpr iterator begin ();
        constexpr iterator end ();
        
        constexpr const_iterator begin () const;
        constexpr const_iterator end () const;
        
        constexpr reverse_iterator rbegin ();
        constexpr reverse_iterator rend ();
        
        constexpr const_reverse_iterator rbegin () const;
        constexpr const_reverse_iterator rend () const;
        
        constexpr Words<endian::big, digit> reverse () const;
        
        constexpr size_t size () const {
            return Data.size ();
        }
        
        constexpr void bit_shift_left (uint32 x, bool fill = false);
        
        constexpr void bit_shift_right (uint32 x, bool fill = false);
        
    };
    
    template <typename digit> struct Words<endian::big, digit> {
        slice<digit> Data;

        constexpr Words (slice<digit> d) : Data {d} {}
        
        constexpr digit &operator [] (int i);
        constexpr const digit &operator [] (int i) const;
        
        using iterator = slice<digit>::reverse_iterator;
        using const_iterator = slice<digit>::const_reverse_iterator;
        using reverse_iterator = slice<digit>::iterator;
        using const_reverse_iterator = slice<digit>::const_iterator;
        
        constexpr iterator begin ();
        constexpr iterator end ();
        
        constexpr const_iterator begin () const;
        constexpr const_iterator end () const;
        
        constexpr reverse_iterator rbegin ();
        constexpr reverse_iterator rend ();
        
        constexpr const_reverse_iterator rbegin () const;
        constexpr const_reverse_iterator rend () const;
        
        constexpr Words<endian::little, digit> reverse () const;
        
        constexpr size_t size () const {
            return Data.size ();
        }
        
        constexpr void bit_shift_left (uint32 x, bool fill = false);
        
        constexpr void bit_shift_right (uint32 x, bool fill = false);
        
    };

    // must check that the input has at least size 1 to use.
    template <endian::order r, typename digit> constexpr void inline flip_sign_bit (Words<r, digit> x) {
        if (x[-1] & get_sign_bit<digit>::value) x[-1] &= ~get_sign_bit<digit>::value;
        else x[-1] |= get_sign_bit<digit>::value;
    }

    template <endian::order r, typename digit> constexpr void negate_ones (Words<r, digit> x) {
        bit_negate<digit> (x.end (), x.begin (), x.begin ());
        auto o = x.begin ();
        auto i = x.begin ();
        //plus (x.end (), o, 1, i);
        add_with_carry<digit> (x.end (), o, i, 1);
    }

    template <endian::order r, typename digit> constexpr void inline negate_twos (Words<r, digit> x) {
        if (x.size () == 0) return;
        flip_sign_bit (x);
    }

    template <endian::order r, typename digit>
    constexpr void set_max_unsigned (Words<r, digit> a) {
        for (digit &x : a) x = max_unsigned<digit>;
    }

    template <endian::order r, typename digit>
    constexpr void set_max_signed_twos (Words<r, digit> a) {
        auto i = a.rbegin ();
        if (i == a.rend ()) return;
        *i = max_signed_twos<digit>;
        while (true) {
            i++;
            if (i == a.rend ()) return;
            *i = max_unsigned<digit>;
        }
    }

    template <endian::order r, typename digit>
    constexpr void set_min_signed_twos (Words<r, digit> a) {
        auto i = a.rbegin ();
        if (i == a.rend ()) return;
        *i = min_signed_twos<digit>;
        while (true) {
            i++;
            if (i == a.rend ()) return;
            *i = 0;
        }
    }

    template <endian::order r, typename digit>
    constexpr digit plus (Words<r, digit> &o, const Words<r, digit> &a, const Words<r, digit> &b) {
        if (a.size () < b.size ()) return plus (o, b, a);
        if (o.size () < a.size ()) throw exception {"need a bigger space to add numbers"};

        auto oit = o.begin ();
        auto ait = a.begin ();
        auto bit = b.begin ();

        auto end_step_1 = oit + b.size ();
        auto end_step_2 = oit + a.size ();

        //digit remainder_1 = plus<digit> (end_step_1, oit, ait, bit);
        bool carry_1 = add_with_carry<digit> (end_step_1, oit, ait, bit);
        //digit remainder_2 = plus (end_step_2, oit, remainder_1, ait);
        bool carry_2 = add_with_carry<digit> (end_step_2, oit, ait, carry_1 ? 1 : 0);
        auto oiti = oit;
        //return plus (o.end (), oit, remainder_2, oiti);
        return add_with_carry<digit> (o.end (), oit, oiti, carry_2 ? 1 : 0) ? 1 : 0;

    }

    // we should already be able to expect that a > b, so the result will not go from positive to negative.
    template <endian::order r, typename digit>
    constexpr digit minus (Words<r, digit> &o, const Words<r, digit> &a, const Words<r, digit> &b) {

        auto oit = o.begin ();
        auto ait = a.begin ();
        auto bit = b.begin ();

        auto end_step_1 = oit + std::min (a.size (), b.size ());
        auto end_step_2 = oit + a.size ();

        //digit remainder = minus<digit> (end_step_1, oit, ait, bit);
        //return minus<digit> (end_step_2, oit, remainder, ait);

        bool carry = subtract_with_carry<digit> (end_step_1, oit, ait, bit);
        return subtract_with_carry<digit> (end_step_2, oit, ait, carry ? 1 : 0) ? 1 : 0;

    }

    template <endian::order r, typename digit>
    constexpr void times (Words<r, digit> &o, const Words<r, digit> &a, const Words<r, digit> &b) {

        // if the size of b is zero, then the answer is zero.
        if (a.size () == 0 || b.size () == 0) {
            for (auto io = o.begin (); io != o.end (); io++) *io = 0;
            return;
        }

        // ensure that a is at least as big as b.
        if (a.size () < b.size ()) return times (o, b, a);
        if (a.size () > o.size ()) throw exception {} << "multiplication output must be at least as big as inputs";

        auto io = o.begin ();
        auto ia = a.begin ();
        auto ib = b.begin ();

        // We leave iterators at the beginning of a and b.
        auto ba = ia;
        auto bb = ib;

        // first we deal with the case where we have not exhausted b or a.
        do {

            // we start two new iterators, one at the start of a
            // and the other at the current position of b.
            auto ax = ba;
            auto bx = ib;

            while (true) {
                auto [lesser, greater] = multiply_with_carry (*ax, *bx);

                auto pio = io;
                auto pia = io;
                add_with_carry<digit> (o.end (), pio, pia, lesser);

                // we know that we will not go off the end because we are
                // not yet at the end of b, which we know is smaller than a.
                auto mio = io + 1;
                auto mia = io + 1;
                add_with_carry<digit> (o.end (), mio, mia, greater);

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
                auto [lesser, greater] = multiply_with_carry (*ax, *bx);

                auto pio = io;
                auto pia = io;
                add_with_carry<digit> (o.end (), pio, pia, lesser);

                auto mio = io + 1;
                auto mia = io + 1;
                add_with_carry <digit> (o.end (), mio, mia, greater);

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
                auto [lesser, greater] = multiply_with_carry (*ax, *bx);

                auto pio = io;
                auto pia = io;
                add_with_carry<digit> (o.end (), pio, pia, lesser);

                auto mio = io + 1;
                auto mia = io + 1;
                add_with_carry <digit> (o.end (), mio, mia, greater);

                ax++;
                bx--;
            }

            io++;
        }

    }

    template <typename digit> 
    constexpr digit inline &Words<endian::little, digit>::operator [] (int i) {
        return Data[i];
    }
    
    template <typename digit> 
    constexpr const digit inline &Words<endian::little, digit>::operator [] (int i) const {
        return Data[i];
    }
    
    template <typename digit>
    constexpr digit inline &Words<endian::big, digit>::operator [] (int i) {
        return Data[-i - 1];
    }
    
    template <typename digit> 
    constexpr const digit inline &Words<endian::big, digit>::operator [] (int i) const {
        return Data[-i - 1];
    }
    
    template <typename digit> 
    constexpr Words<endian::little, digit>::iterator inline Words<endian::little, digit>::begin () {
        return Data.begin ();
    }
    
    template <typename digit> 
    constexpr Words<endian::little, digit>::iterator inline Words<endian::little, digit>::end () {
        return Data.end ();
    }
    
    template <typename digit> 
    constexpr Words<endian::big, digit>::iterator inline Words<endian::big, digit>::begin () {
        return Data.rbegin ();
    }
    
    template <typename digit> 
    constexpr Words<endian::big, digit>::iterator inline Words<endian::big, digit>::end () {
        return Data.rend ();
    }
    
    template <typename digit> 
    constexpr Words<endian::little, digit>::const_iterator inline Words<endian::little, digit>::begin () const {
        return Data.begin ();
    }
    
    template <typename digit> 
    constexpr Words<endian::little, digit>::const_iterator inline Words<endian::little, digit>::end () const {
        return Data.end ();
    }
    
    template <typename digit> 
    constexpr Words<endian::big, digit>::const_iterator inline Words<endian::big, digit>::begin () const {
        return Data.rbegin ();
    }
    
    template <typename digit> 
    constexpr Words<endian::big, digit>::const_iterator inline Words<endian::big, digit>::end () const {
        return Data.rend ();
    }
    
    template <typename digit> 
    constexpr Words<endian::little, digit>::reverse_iterator inline Words<endian::little, digit>::rbegin () {
        return Data.rbegin ();
    }
    
    template <typename digit> 
    constexpr Words<endian::little, digit>::reverse_iterator inline Words<endian::little, digit>::rend () {
        return Data.rend ();
    }
    
    template <typename digit> 
    constexpr Words<endian::big, digit>::reverse_iterator inline Words<endian::big, digit>::rbegin () {
        return Data.begin ();
    }
    
    template <typename digit> 
    constexpr Words<endian::big, digit>::reverse_iterator inline Words<endian::big, digit>::rend () {
        return Data.end ();
    }
    
    template <typename digit> 
    constexpr Words<endian::little, digit>::const_reverse_iterator inline Words<endian::little, digit>::rbegin () const {
        return Data.rbegin ();
    }
    
    template <typename digit> 
    constexpr Words<endian::little, digit>::const_reverse_iterator inline Words<endian::little, digit>::rend () const {
        return Data.rend ();
    }
    
    template <typename digit> 
    constexpr Words<endian::big, digit>::const_reverse_iterator inline Words<endian::big, digit>::rbegin () const {
        return Data.begin ();
    }
    
    template <typename digit> 
    constexpr Words<endian::big, digit>::const_reverse_iterator inline Words<endian::big, digit>::rend () const {
        return Data.end ();
    }
    
    template <typename digit> 
    constexpr Words<endian::big, digit> inline Words<endian::little, digit>::reverse () const {
        return {Data};
    }
    
    template <typename digit> 
    constexpr Words<endian::little, digit> inline Words<endian::big, digit>::reverse () const {
        return {Data};
    }

    template <typename digit>
    constexpr void Words<endian::little, digit>::bit_shift_left (uint32 x, bool fill) {
        arithmetic::bit_shift_left<digit> (
            std::reverse_iterator {Data.begin () + Data.size ()},
            std::reverse_iterator {Data.begin ()}, x, fill);
    }

    template <typename digit>
    constexpr void Words<endian::little, digit>::bit_shift_right (uint32 x, bool fill) {
        auto it = Data.begin ();
        arithmetic::bit_shift_right<digit> (it, it + Data.size (), x, fill);
    }
    
    template <typename digit>
    constexpr void Words<endian::big, digit>::bit_shift_left (uint32 x, bool fill) {
        auto it = Data.begin ();
        arithmetic::bit_shift_left<digit> (it, it + Data.size (), x, fill);
    }

    template <typename digit>
    constexpr void Words<endian::big, digit>::bit_shift_right (uint32 x, bool fill) {
        arithmetic::bit_shift_right<digit> (
            std::reverse_iterator {Data.begin () + Data.size ()},
            std::reverse_iterator {Data.begin ()}, x, fill);
    }
}

#endif
