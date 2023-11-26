// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_COMPLEMENTS
#define DATA_MATH_NUMBER_COMPLEMENTS

#include <data/cross.hpp>
#include <data/math/number/complement.hpp>

// these functions will not trim the input or result.

namespace data::math::number::arithmetic {

    // provide a zero of a given size.
    template <endian::order r, std::integral word> bytestring<word> zero (size_t new_size);

    template <endian::order r, complement c, std::integral word> bool is_zero (view<word>);

    template <endian::order r, complement c, std::integral word> bool sign_bit (view<word>);

    template <endian::order r, complement c, std::integral word> size_t minimal_size (view<word>);

    template <endian::order r, complement c, std::integral word> bool is_minimal (view<word>);

    template <endian::order r, complement c, std::integral word>
    bytestring<word> &extend (bytestring<word> &, size_t new_size);

    template <endian::order r, complement c, std::integral word>
    bytestring<word> extend (view<word>, size_t new_size);

    template <endian::order r, complement c, std::integral word>
    bytestring<word> &trim (bytestring<word> &);

    template <endian::order r, complement c, std::integral word>
    bytestring<word> trim (view<word>);

    template <endian::order r, complement c, std::integral word>
    bytestring<word> &bit_and (bytestring<word> &, view<word>);

    template <endian::order r, complement c, std::integral word>
    bytestring<word> &bit_or (bytestring<word> &, view<word>);

    template <endian::order r, complement c, std::integral word>
    bytestring<word> &bit_xor (bytestring<word> &, view<word>);

    template <endian::order r, complement c, std::integral word>
    bytestring<word> bit_and (view<word>, view<word>);

    template <endian::order r, complement c, std::integral word>
    bytestring<word> bit_or (view<word>, view<word>);

    template <endian::order r, complement c, std::integral word>
    bytestring<word> bit_xor (view<word>, view<word>);

}

namespace data::math::number::arithmetic::nones {

    template <endian::order r, std::integral word> std::weak_ordering compare (view<word>, view<word>);

    template <endian::order r, std::integral word> bytestring<word> &increment (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> increment (view<word>);

    template <endian::order r, std::integral word> bytestring<word> &decrement (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> decrement (view<word>);

    template <endian::order r, std::integral word> bytestring<word> &times_2 (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> times_2 (view<word>);

    template <endian::order r, std::integral word> bytestring<word> &div_by_2 (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> div_by_2 (view<word>);

    template <endian::order r, std::integral word> bytestring<word> &plus (bytestring<word> &, view<word>);
    template <endian::order r, std::integral word> bytestring<word> plus (view<word>, view<word>);

    // assume first input is greater than second.
    template <endian::order r, std::integral word> bytestring<word> &minus (bytestring<word> &, view<word>);
    template <endian::order r, std::integral word> bytestring<word> minus (view<word>, view<word>);

    template <endian::order r, std::integral word> bytestring<word> &times (bytestring<word> &, view<word>);
    template <endian::order r, std::integral word> bytestring<word> times (view<word>, view<word>);

}

namespace data::math::number::arithmetic::ones {

    template <endian::order r, std::integral word> std::weak_ordering compare (view<word>, view<word>);

    template <endian::order r, std::integral word> bytestring<word> &negate (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> negate (view<word>);

    template <endian::order r, std::integral word> bytestring<word> &increment (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> increment (view<word>);

    template <endian::order r, std::integral word> bytestring<word> &decrement (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> decrement (view<word>);

    template <endian::order r, std::integral word> bytestring<word> &times_2 (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> times_2 (view<word>);

    template <endian::order r, std::integral word> bytestring<word> &div_by_2 (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> div_by_2 (view<word>);

    template <endian::order r, std::integral word> bytestring<word> &negate (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> negate (view<word>);

    template <endian::order r, std::integral word> bytestring<word> &abs (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> abs (view<word>);

    template <endian::order r, std::integral word> bytestring<word> &plus (bytestring<word> &, view<word>);
    template <endian::order r, std::integral word> bytestring<word> plus (view<word>, view<word>);

    template <endian::order r, std::integral word> bytestring<word> &times (bytestring<word> &, view<word>);
    template <endian::order r, std::integral word> bytestring<word> times (view<word>, view<word>);

}

namespace data::math::number::arithmetic::twos {

    template <endian::order r, std::integral word> std::weak_ordering compare (view<word>, view<word>);

    template <endian::order r, std::integral word> bytestring<word> &negate (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> negate (view<word>);

    template <endian::order r, std::integral word> bytestring<word> zero (size_t size, bool negative = false);

    template <endian::order r, std::integral word> bytestring<word> &increment (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> increment (view<word>);

    template <endian::order r, std::integral word> bytestring<word> &decrement (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> decrement (view<word>);

    template <endian::order r, std::integral word> bytestring<word> &times_2 (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> times_2 (view<word>);

    template <endian::order r, std::integral word> bytestring<word> &div_by_2 (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> div_by_2 (view<word>);

    // operate directly on the sign bit, so change negative zero to zero and vice versa.
    template <endian::order r, std::integral word> bytestring<word> &negate (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> negate (view<word>);

    template <endian::order r, std::integral word> bytestring<word> &abs (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> abs (view<word>);

    template <endian::order r, std::integral word> bytestring<word> &plus (bytestring<word> &, view<word>);
    template <endian::order r, std::integral word> bytestring<word> plus (view<word>, view<word>);

    template <endian::order r, std::integral word> bytestring<word> &times (bytestring<word> &, view<word>);
    template <endian::order r, std::integral word> bytestring<word> times (view<word>, view<word>);

}

namespace data::math::number::arithmetic {

    template <endian::order r, complement c, std::integral word>
    bytestring<word> inline extend (view<word> x, size_t new_size) {
        bytestring<word> y {x};
        return extend<r, c> (y, new_size);
    }

    template <endian::order r, complement c, std::integral word>
    bytestring<word> inline trim (view<word> x) {
        bytestring<word> y {x};
        return trim<r, c> (y);
    }

    template <endian::order r, complement c, std::integral word>
    bytestring<word> inline &bit_and (bytestring<word> &a, view<word> b) {
        if (a.size () == b.size ()) {
            for (int i = 0; i < a.size (); i++) a[i] &= b[i];
            return a;
        }
        return a = view<word> (a) & b;
    }

    template <endian::order r, complement c, std::integral word>
    bytestring<word> inline &bit_or (bytestring<word> &a, view<word> b) {
        if (a.size () == b.size ()) {
            for (int i = 0; i < a.size (); i++) a[i] |= b[i];
            return a;
        }

        return b = view<word> (b) | b;
    }

}

namespace data::math::number::arithmetic::nones {

    template <endian::order r, std::integral word>
    bytestring<word> inline increment (view<word> x) {
        bytestring<word> y {x};
        return increment<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline decrement (view<word> x) {
        bytestring<word> y {x};
        return decrement<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline times_2 (view<word> x) {
        bytestring<word> y {x};
        return times_2<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline div_by_2 (view<word> x) {
        bytestring<word> y {x};
        return div_by_2<r> (y);
    }

    template <endian::order r, std::integral word> bytestring<word> inline &plus (bytestring<word> &x, view<word>y) {
        return x = plus<r> (view<word> (x), y);
    }

    template <endian::order r, std::integral word> bytestring<word> inline &minus (bytestring<word> &x, view<word>y) {
        return x = minus<r> (view<word> (x), y);
    }

    template <endian::order r, std::integral word> bytestring<word> inline &times (bytestring<word> &x, view<word>y) {
        return x = times<r> (view<word> (x), y);
    }

}

namespace data::math::number::arithmetic::ones {

    template <endian::order r, std::integral word>
    bytestring<word> inline negate (view<word> x) {
        bytestring<word> y {x};
        return negate<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline increment (view<word> x) {
        bytestring<word> y {x};
        return increment<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline decrement (view<word> x) {
        bytestring<word> y {x};
        return decrement<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline times_2 (view<word> x) {
        bytestring<word> y {x};
        return times_2<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline div_by_2 (view<word> x) {
        bytestring<word> y {x};
        return div_by_2<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline abs (view<word> x) {
        bytestring<word> y {x};
        return abs<r> (y);
    }

    template <endian::order r, std::integral word> bytestring<word> inline &plus (bytestring<word> &x, view<word>y) {
        return x = plus<r> (view<word> (x), y);
    }

    template <endian::order r, std::integral word> bytestring<word> inline &times (bytestring<word> &x, view<word>y) {
        return x = times<r> (view<word> (x), y);
    }

}

namespace data::math::number::arithmetic::twos {

    template <endian::order r, std::integral word>
    bytestring<word> inline negate (view<word> x) {
        bytestring<word> y {x};
        return negate<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline increment (view<word> x) {
        bytestring<word> y {x};
        return increment<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline decrement (view<word> x) {
        bytestring<word> y {x};
        return decrement<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline times_2 (view<word> x) {
        bytestring<word> y {x};
        return times_2<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline div_by_2 (view<word> x) {
        bytestring<word> y {x};
        return div_by_2<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline abs (view<word> x) {
        bytestring<word> y {x};
        return abs<r> (y);
    }

    template <endian::order r, std::integral word> bytestring<word> inline &plus (bytestring<word> &x, view<word>y) {
        return x = plus<r> (view<word> (x), y);
    }

    template <endian::order r, std::integral word> bytestring<word> inline &times (bytestring<word> &x, view<word>y) {
        return x = times<r> (view<word> (x), y);
    }

}

namespace data::math::number::arithmetic {

    template <endian::order r, std::integral word>
    auto inline words (bytestring<word> &z) {
        return encoding::words<r, word> {slice<word> (z)};
    }

    template <endian::order r, std::integral word>
    auto inline words (view<word> z) {
        return encoding::words<r, word> {slice<word> {const_cast<word *> (z.data ()), z.size ()}};
    }

    template <endian::order r, complement c, std::integral word> size_t inline minimal_size (view<word> x) {
        return minimal_size<c> (words<r> (x));
    }

    template <endian::order r, complement c, std::integral word> bool inline is_minimal (view<word> x) {
        return minimal_size<r, c> (x) == x.size ();
    }

    template <endian::order r, std::integral word> bytestring<word> inline zero (size_t size) {
        return bytestring<word> (size, 0);
    }

    template <endian::order r, complement c, std::integral word>
    bool inline is_zero (view<word> x) {
        if constexpr (c == complement::twos) {
            return number::arithmetic::twos::is_zero (words<r> (x));
        } else {
            return number::arithmetic::is_zero (words<r> (x));
        }
    }

    template <endian::order r, complement c, std::integral word> bool inline sign_bit (view<word> x) {
        if constexpr (c == complement::nones) return false;
        else return number::arithmetic::sign_bit (words<r> (x));
    }

    template <endian::order r, complement c, std::integral word>
    bytestring<word> &trim (bytestring<word> &x) {

        auto w = words<r> (x);
        size_t min_size = minimal_size<c> (w);
        if (min_size == size (w)) return x;
        auto n = zero<r, word> (min_size);
        std::copy (w.begin (), w.begin () + min_size, words<r> (n).begin ());

        if constexpr (c == complement::twos)
            if (min_size != 0) words<r> (n)[-1] |= (w[-1] & get_sign_bit<word>::value);

        return x = n;
    }

    template <endian::order r, complement c, std::integral word>
    bytestring<word> &extend (bytestring<word> &x, size_t new_size) {
        auto w = words<r> (x);

        if (new_size < size (w)) {
            size_t min_size = minimal_size<c> (w);
            if (new_size < min_size) throw std::invalid_argument {"cannot extend smaller than minimal size"};
            return extend<r, c> (trim<r, c> (x), new_size);
        }

        if (new_size == size (w)) return x;

        bytestring<word> z = zero<r, word> (new_size);

        if constexpr (c == complement::nones) {
            std::copy (w.rbegin (), w.rend (), words<r> (z).rbegin () + new_size - size (w));
        } else if constexpr (c == complement::ones) {
            word extend_digit = ones::is_negative (w) ? std::numeric_limits<word>::max () : 0;

            auto i = words<r> (z).rbegin ();
            for (int n = 0; n < new_size - size (w); n++) {
                *i = extend_digit;
                i++;
            }

            std::copy (w.rbegin (), w.rend (), i);
        } else if constexpr (c == complement::twos) {
            if (size (w) == 0) return x = z;

            auto v = words<r> (z);
            std::copy (w.begin (), w.end () - 1, v.begin ());
            word last = w[-1];

            v[-1] = last & get_sign_bit<word>::value ? get_sign_bit<word>::value : 0;
            v[size (w) - 1] = last & ~get_sign_bit<word>::value;
        }

        return x = z;
    }

    template <endian::order r, complement c, std::integral word>
    bytestring<word> bit_and (view<word> a, view<word> b) {
        if (a.size () < b.size ()) return bit_and<r, c> (b, a);
        auto bt = extend<r, c> (b, a.size ());
        auto x = N_bytes<r>::zero (a.size ());
        bit_and<byte> (x.end (), x.begin (), a.begin (),
            view<word> (bt).begin ());
        return x;
    }

    template <endian::order r, complement c, std::integral word>
    bytestring<word> bit_or (view<word> a, view<word> b) {
        if (a.size () < b.size ()) return bit_or<r, c> (b, a);
        auto bt = extend<r, c> (b, a.size ());
        auto x = N_bytes<r>::zero (a.size ());
        bit_or<byte> (x.end (), x.begin (), a.begin (),
            view<word> (bt).begin ());
        return x;
    }
}

namespace data::math::number::arithmetic::nones {

    // must be the same size.
    template <endian::order r, std::integral word> std::weak_ordering inline compare (view<word> a, view<word> b) {
        return arithmetic::compare<complement::nones> (words<r> (a), words<r> (b));
    }

    template <endian::order r, std::integral word>
    bytestring<word> &increment (bytestring<word> &x) {
        auto w = words<r> (x);

        auto o = w.begin ();
        auto i = w.begin ();
        auto remainder = arithmetic::plus<byte> (w.end (), o, 1, i);

        if (remainder != 0) {
            x = extend<r, complement::nones> (x, size (w) + 1);
            words<r> (x)[-1] = remainder;
        }

        return x;
    }

    template <endian::order r, std::integral word>
    bytestring<word> &decrement (bytestring<word> &x) {
        auto w = words<r> (x);
        auto xx = w.begin ();
        auto xy = w.begin ();
        if (!is_zero (w)) arithmetic::minus<byte> (w.end (), xx, 1, xy);
        return x;
    }

    template <endian::order r, std::integral word> bytestring<word> plus (view<word> a, view<word> b) {
        bytestring<word> n (std::max (a.size (), b.size ()), 0);
        auto wn = words<r> (n);
        word remainder = arithmetic::plus<r, byte> (wn, words<r> (a), words<r> (b));
        if (remainder > 0) {
            extend<r, complement::nones, word> (n, n.size () + 1);
            *words<r> (n).rbegin () = remainder;
        }
        return n;
    }

    // assume a > b;
    template <endian::order r, std::integral word> bytestring<word> minus (view<word> a, view<word> b) {
        bytestring<word> n (a.size (), 0);
        auto wn = words<r> (n);
        arithmetic::minus<r, byte> (wn, words<r> (a), words<r> (b));
        return n;
    }
}

namespace data::math::number::arithmetic::ones {
    template <endian::order r, std::integral word> bytestring<word> inline &negate (bytestring<word> &x) {
        return increment<r, word> (x.bit_negate ());
    }

    template <endian::order r, std::integral word> std::weak_ordering compare (view<word> a, view<word> b) {

        auto wa = words<r> (a);
        auto wb = words<r> (b);

        bool na = is_negative (wa);
        bool nb = is_negative (wb);

        if (na && nb) {
            auto ya = negate<r> (b);
            auto yb = negate<r> (a);
            return arithmetic::compare<complement::nones> (words<r> (ya), words<r> (yb));
        }

        if (!na && !nb) return arithmetic::compare<complement::nones> (wa, wb);
        return na ? std::weak_ordering::less : std::weak_ordering::greater;
    }

    // we extend the number once with every increment, which will get very
    // inefficient if you don't trim regularly. It should be possible to
    // extend only when we need to, but we don't right now.
    template <endian::order r, std::integral word>
    bytestring<word> &increment (bytestring<word> &x) {

        extend<r, complement::ones> (x, size (words<r> (x)) + 1);

        auto w = words<r> (x);
        auto oit = w.begin ();
        auto iit = w.begin ();

        // a remainder is provided here that we throw out
        // but it could be used to detect an overflow.
        arithmetic::plus<byte> (w.end (), oit, 1, iit);
        return x;
    }

    template <endian::order r, std::integral word>
    bytestring<word> &decrement (bytestring<word> &x) {
        auto w = words<r> (x);
        if (is_zero (w)) return x = bytestring<word> (1, std::numeric_limits<word>::max ());

        extend<r, complement::ones> (x, size (w) + 1);

        w = words<r> (x);
        auto xx = w.begin ();
        auto xy = w.begin ();

        // a remainder is provided here that we throw out
        // but it could be used to detect an overflow.
        arithmetic::minus<byte> (w.end (), xx, 1, xy);
        return x;
    }
}

namespace data::math::number::arithmetic::twos {

    template <endian::order r, std::integral word> bytestring<word> zero (size_t size, bool negative) {
        if (negative && size == 0) throw exception {"cannot make negative zero of size zero."};
        bytestring<word> z (size, 0);
        if (negative) words<r> (z)[-1] = get_sign_bit<word>::value;
        return z;
    }

    template <endian::order r, std::integral word> std::weak_ordering compare (view<word> a, view<word> b) {

        auto wa = words<r> (a);
        auto wb = words<r> (b);

        math::sign na = sign (wa);
        math::sign nb = sign (wb);

        if (na == math::zero) switch (nb) {
            case positive: return std::weak_ordering::less;
            case negative: return std::weak_ordering::greater;
            default: return std::weak_ordering::equivalent;
        }

        if (nb == math::zero) return na == math::positive ? std::weak_ordering::greater : std::weak_ordering::less;

        if (na == math::positive && nb == math::positive)
            return arithmetic::compare<complement::nones> (wa, wb);

        if (na == math::negative && nb == math::negative) {
            auto ya = negate<r> (b);
            auto yb = negate<r> (a);
            return arithmetic::compare<complement::nones> (words<r> (ya), words<r> (yb));
        }

        return na == negative ? std::weak_ordering::less : std::weak_ordering::greater;
    }

    template <endian::order r, std::integral word>
    bytestring<word> &increment (bytestring<word> &x) {
        if (is_zero (words<r> (x))) x = zero<r, word> (1);

        auto w = words<r> (x);
        if (sign_bit (w))
            return negate<r> (decrement<r> (negate<r> (x)));

        auto oit = w.begin ();
        auto iit = w.begin ();

        auto remainder = arithmetic::plus<byte> (w.end (), oit, 1, iit);
        if (remainder != 0) {
            auto n = zero<r, word> (size (w) + 1);
            auto v = words<r> (n);
            std::copy (w.begin (), w.end (), v.begin ());
            v[-1] = remainder;
            x = n;
        } else if (sign_bit (w)) {
            auto n = zero<r, word> (size (w) + 1);
            std::copy (w.begin (), w.end (), words<r> (n).begin ());
            x = n;
        }

        return x;
    }

    template <endian::order r, std::integral word>
    bytestring<word> &decrement (bytestring<word> &x) {
        auto w = words<r> (x);
        if (is_zero (w)) return negate<r> (increment<r> (x = zero<r, word> (0)));

        if (arithmetic::sign_bit (w))
            return negate<r> (increment<r> (negate<r> (x)));

        auto xx = w.begin ();
        auto xy = w.begin ();
        arithmetic::minus<byte> (w.end (), xx, 1, xy);
        return x;
    }

    template <endian::order r, std::integral word>
    bytestring<word> &negate (bytestring<word> &x) {
        auto w = words<r> (x);
        if (size (w) == 0) return x = zero<r, word> (1, true);
        flip_sign_bit (w);
        return x;
    }

    template <endian::order r, std::integral word>
    bytestring<word> &abs (bytestring<word> &x) {
        auto w = words<r> (x);
        if (sign_bit (w)) return negate<r> (x);
        return x;
    }

    template <endian::order r, std::integral word> bytestring<word> plus (view<word> a, view<word> b) {

        auto wa = words<r> (a);
        auto wb = words<r> (b);

        bool an = is_negative (wa);
        {
            bool bn = is_negative (wb);
            if (!an && !bn) {
                bytestring<word> c (std::max (a.size (), b.size ()), 9);
                auto wc = words<r> (c);
                word remainder = arithmetic::plus<r, byte> (wc, wa, wb);
                if (remainder > 0 || sign_bit (wc)) {
                    extend<r, complement::nones, word> (c, c.size () + 1);
                    *words<r> (c).rbegin () = remainder;
                }
                return c;
            }

            if (an && bn) return negate<r, byte> (plus<r, byte> (negate<r, byte> (a),  negate<r, byte> (b)));
        }

        if (an) {
            bytestring<word> na = negate<r, byte> (a);
            return compare<r, word> (na, b) > 0 ? negate<r, byte> (nones::minus<r, word> (na, b)): nones::minus<r, word> (b, na);
        } else {
            bytestring<word> nb = negate<r, byte> (b);
            return compare<r, word> (a, nb) > 0 ? nones::minus<r, word> (a, nb): negate<r, byte> (nones::minus<r, word> (nb, a));
        }
    }

    template <endian::order r, std::integral word> bytestring<word> times (view<word> a, view<word> b) {

        auto as = abs<r, byte> (a);
        auto bs = abs<r, byte> (b);
        bytestring<word> cs (as.size () + bs.size () + 1, 0);

        auto wa = words<r> (as);
        auto wb = words<r> (bs);
        auto wc = words<r> (cs);

        arithmetic::times (wc, wa, wb);

        return is_negative (words<r> (a)) == is_negative (words<r> (b)) ? cs : negate<r, byte> (cs);
    }
}

#endif
