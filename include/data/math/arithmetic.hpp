// Copyright (c) 2020-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ARITHMETIC
#define DATA_MATH_ARITHMETIC

#include <data/math/sign.hpp> 
#include <data/encoding/words.hpp>

namespace data::math {
    
    template <typename X> struct first;
    
    template <std::integral X> struct first<X> {
        X operator () () {
            return X {0};
        }
    };
    
    template <typename F, typename x> struct identity;
    template <typename F, typename x> struct inverse;
    
    template <typename X> struct plus {
        X operator () (const X &a, const X &b) {
            return a + b;
        }
    };
    
    template <typename X> struct identity<plus<X>, X> {
        X operator () () {
            return X {0};
        }
    };
    
    template <typename X> struct times {
        X operator () (const X &a, const X &b) {
            return a * b;
        }
    };
    
    template <typename X> struct identity<times<X>, X> {
        X operator () () {
            return X {1};
        }
    };
    
    template <> struct inverse<plus<int8_t>, int8_t> {
        int8_t operator () (const int8_t &a, const int8_t &b) {
            return b - a;
        }
    };
    
    template <std::integral X> struct inverse<plus<X>, X> {
        X operator () (X a, X b) {
            return b - a;
        }
    };

}

namespace data {
    template <std::integral X> X inline increment (X x) {
        return x + 1;
    }
    
    template <std::signed_integral X> X inline decrement (X x) {
        return x - 1;
    }
    
    template <std::unsigned_integral X> X inline decrement (X x) {
        return x == 0 ? 0 : x - 1;
    }
}

namespace data::math::arithmetic {
    template<size_t size, size_t mod8 = size % 8, size_t mod4 = size % 4, size_t mod2 = size % 2> 
    struct max_word_size;
    
    template<size_t size> struct max_word_size<size, 0, 0, 0> {
        using type = uint64;
    };
    
    template<size_t size> struct max_word_size<size, 4, 0, 0> {
        using type = uint32;
    };
    
    template<size_t size, size_t mod8> struct max_word_size<size, mod8, 2, 0> {
        using type = uint16;
    };
    
    template<size_t size, size_t mod8, size_t mod4> struct max_word_size<size, mod8, mod4, 0> {
        using type = byte;
    };
    
    template <typename digit> struct get_sign_bit;
    
    template<> struct get_sign_bit<byte> {
        static const byte value = 0x80;
    };
    
    template<> struct get_sign_bit<uint16> {
        static const uint16 value = 0x8000;
    };
    
    template<> struct get_sign_bit<uint32> {
        static const uint32 value = 0x80000000;
    };
    
    template<> struct get_sign_bit<uint64> {
        static const uint64 value = 0x8000000000000000;
    };
    
    template <endian::order r, typename digit> bool inline sign_bit (const encoding::words<r, digit> x) {
        return (x.size () != 0) && (x[-1] & get_sign_bit<digit>::value) != 0;
    }
    
    template <endian::order r, typename digit> void inline flip_sign_bit (encoding::words<r, digit> x) {
        if (x[-1] & get_sign_bit<digit>::value) x[-1] &= ~get_sign_bit<digit>::value;
        else x[-1] |= get_sign_bit<digit>::value;
    }
    
    template <endian::order r, typename digit> bool inline ones_is_zero (const encoding::words<r, digit> x) {
        for (const digit d : x) if (d != 0) return false;
        return true;
    }
    
    template <endian::order r, typename digit> bool twos_is_zero (const encoding::words<r, digit> x) {
        auto i = x.rbegin ();
        auto e = x.rend ();
        if (i == e) return true;
        if (*i != 0 && *i != get_sign_bit<digit>::value) return false;
        while (true) {
            i++;
            if (i == e) return true;
            if (*i != 0) return false;
        }
    }
    
    template <endian::order r, typename digit> math::sign N_sign (const encoding::words<r, digit> x) {
        for (auto i = x.rbegin (); i != x.rend (); i++) if (*i != 0) return math::positive;
        return math::zero;
    } 
    
    template <endian::order r, typename digit> math::sign Z_sign_ones (const encoding::words<r, digit> x) {
        auto i = x.rbegin ();
        if (i == x.rend ()) return math::zero;
        if (*i & get_sign_bit<digit>::value) return math::negative;
        while (i != x.rend ()) {
            if (*i != 0) return math::positive;
            i++;
        }
        return math::zero;
    } 
    
    template <endian::order r, typename digit> math::sign Z_sign_twos (const encoding::words<r, digit> x) {
        auto i = x.rbegin ();
        if (i == x.rend ()) return math::zero;
        math::sign nonzero = (*i & sign_bit<digit> ()) ? math::negative : math::positive;
        if (*i & ~sign_bit<digit> ()) return nonzero;
        while (true) {
            i++; 
            if (i != x.rend ()) return math::zero;
            if (*i != 0) return nonzero;
        }
    } 
    
    template <endian::order r, typename digit> bool inline N_is_minimal (const encoding::words<r, digit> x) {
        return x.size () == 0 || x[-1] != 0;
    }
    
    template <endian::order r, typename digit> bool ones_is_minimal (const encoding::words<r, digit> x) {
        // minimal zero. 
        if (x.size () == 0) return true;
        // numbers of one byte. 
        if (x.size () == 1) return x[1] != 0;
        // numbers without an initial 00 or ff. 
        digit biggest = x[-1];
        digit max_value = std::numeric_limits<digit>::max ();
        if (biggest != max_value && biggest != 0) return true;
        // numbers that would be interpreted with the wrong sign if they were shortened. 
        digit next_biggest = x[-2];
        return (biggest == 0 && (next_biggest & get_sign_bit<digit>::value)) ||
            (biggest == max_value && !(next_biggest & get_sign_bit<digit>::value));
    }
    
    template <endian::order r, typename digit> bool inline twos_is_minimal (const encoding::words<r, digit> x) {
        // minimal zero. 
        return (x.size () == 0) ||
            // numbers without an initial 00 or 80. 
            (x[-1] != 0 && x[-1] != get_sign_bit<digit>::value) ||
            // numbers that would be interpreted as having the wrong sign if they were shortened. 
            (x.size () > 1 && (x[-2] & get_sign_bit<digit>::value));
    }
    
    template <endian::order r, typename digit> size_t inline N_minimal_size (const encoding::words<r, digit> x) {
        int size = x.size ();
        for (auto i = x.rbegin (); i != x.rend (); i++) {
            if (*i != 0x00) break;
            else size--;
        }
        return size;
    }
    
    template <endian::order r, typename digit> size_t ones_minimal_size (const encoding::words<r, digit> x) {
        if (x.size () == 0) return 0;
        digit d = *x.rbegin ();
        if (d != 0xff && d != 0x00) return x.size ();
        int repeated_bytes = 0;
        auto i = x.rbegin () + 1;
        while (true) {
            // if we reach the end, then the number is either 0 or -1.
            if (i == x.rend ()) return d == 0x00 ? 0 : 1;
            if (*i == d) repeated_bytes ++;
            else return x.size () - repeated_bytes - (*i < 0x80 && d == 0x00 || *i >= 0x80 && d == 0xff ? 1 : 0);
            i++;
        }
    }
    
    template <endian::order r, typename digit> size_t inline twos_minimal_size (const encoding::words<r, digit> x) {
        if (x.size () == 0) return 0;
        // numbers that don't begin with 00 or 80 are minimal. 
        auto i = x.rbegin ();
        digit d = *i;
        if (d != 0x80 && d != 0x00) return x.size ();
        
        // count the number of zero bytes after the first. 
        int zero_bytes = 0; 
        
        while (true) {
            i++;
            // if we reach the end then this number is zero. 
            if (i == x.rend ()) return 0;
            if (*i == 0x00) zero_bytes ++;
            // if the first non-zero digit does not have the
            // sign bit set then we can remove an extra digit. 
            else return x.size () - zero_bytes - (*i < 8 ? 1 : 0);
        }
    }
    
    template <endian::order r, typename digit> void negate_ones (encoding::words<r, digit> x) {
        data::arithmetic::bit_negate<digit> (x.end (), x.begin (), x.begin ());
        auto o = x.begin ();
        auto i = x.begin ();
        data::arithmetic::plus (x.end (), o, 1, i);
    }
    
    template <endian::order r, typename digit> void inline negate_twos (encoding::words<r, digit> x) {
        if (x.size () == 0) return;
        flip_sign_bit (x);
    }
    
    template <endian::order r, typename digit> std::weak_ordering N_compare
        (const encoding::words<r, digit> a, const encoding::words<r, digit> b) {
        if (a.size () < b.size ()) return 0 <=> N_compare (b, a);
        size_t size_difference = a.size () - b.size ();
        
        auto ai = a.rbegin ();
        for (int i = 0; i < size_difference; i++) {
            if (0 != *ai) return std::weak_ordering::greater;
            ai++;
        }
        
        return data::arithmetic::compare (a.rend (), ai, b.rbegin ());
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

        digit remainder_1 = data::arithmetic::plus<digit> (end_step_1, oit, ait, bit);
        digit remainder_2 = data::arithmetic::plus (end_step_2, oit, remainder_1, ait);
        auto oiti = oit;
        data::arithmetic::plus (o.end (), oit, remainder_2, oiti);

    }

    // we should already be able to expect that a > b, so the result will not go from positive to negative.
    template <endian::order r, typename digit>
    void minus (encoding::words<r, digit> &o, const encoding::words<r, digit> &a, const encoding::words<r, digit> &b) {

        auto oit = o.begin ();
        auto ait = a.begin ();
        auto bit = b.begin ();

        auto end_step_1 = oit + std::min (a.size (), b.size ());
        auto end_step_2 = oit + a.size ();

        digit remainder = data::arithmetic::minus<digit> (end_step_1, oit, ait, bit);
        data::arithmetic::minus<digit> (end_step_2, oit, remainder, ait);

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
                data::arithmetic::plus<digit> (o.end (), pio, encoding::lesser_half (result), pia);

                auto mio = io + 1;
                auto mia = io + 1;
                data::arithmetic::plus<digit> (o.end (), mio, encoding::greater_half (result), mia);

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
                data::arithmetic::plus<digit> (o.end (), pio, encoding::lesser_half (result), pia);

                auto mio = io + 1;
                auto mia = io + 1;
                data::arithmetic::plus<digit> (o.end (), mio, encoding::greater_half (result), mia);

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
                data::arithmetic::plus<digit> (o.end (), pio, encoding::lesser_half (result), pia);

                auto mio = io + 1;
                auto mia = io + 1;
                data::arithmetic::plus<digit> (o.end (), mio, encoding::greater_half (result), mia);

                ax++;
                bx--;
            }

            io++;
        }

    }
}

#endif
