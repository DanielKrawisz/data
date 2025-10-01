// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ARITHMETIC_COMPLEMENTARY
#define DATA_ARITHMETIC_COMPLEMENTARY

#include <data/bytes.hpp>
#include <data/arithmetic/negativity.hpp>
#include <data/encoding/endian.hpp>

// these functions will not trim the input or result.

namespace data::arithmetic {

    // provide a zero of a given size.
    template <endian::order r, std::integral word> bytestring<word> zero (size_t new_size);

    template <endian::order r, negativity c, std::integral word> bool is_zero (slice<const word>);

    template <endian::order r, negativity c, std::integral word> bool sign_bit (slice<const word>);

    template <endian::order r, negativity c, std::integral word> size_t minimal_size (slice<const word>);

    template <endian::order r, negativity c, std::integral word> bool is_minimal (slice<const word>);

    template <endian::order r, negativity c, std::integral word>
    bytestring<word> &extend (bytestring<word> &, size_t new_size);

    template <endian::order r, negativity c, std::integral word>
    bytestring<word> extend (slice<const word>, size_t new_size);

    template <endian::order r, negativity c, std::integral word>
    bytestring<word> &trim (bytestring<word> &);

    template <endian::order r, negativity c, std::integral word>
    bytestring<word> trim (slice<const word>);

    template <endian::order r, negativity c, std::integral word>
    bytestring<word> &bit_and (bytestring<word> &, slice<const word>);

    template <endian::order r, negativity c, std::integral word>
    bytestring<word> &bit_or (bytestring<word> &, slice<const word>);

    template <endian::order r, negativity c, std::integral word>
    bytestring<word> &bit_xor (bytestring<word> &, slice<const word>);

    template <endian::order r, negativity c, std::integral word>
    bytestring<word> bit_and (slice<const word>, slice<const word>);

    template <endian::order r, negativity c, std::integral word>
    bytestring<word> bit_or (slice<const word>, slice<const word>);

    template <endian::order r, negativity c, std::integral word>
    bytestring<word> bit_xor (slice<const word>, slice<const word>);

}

namespace data::arithmetic::nones {

    template <endian::order r, std::integral word> std::strong_ordering compare (slice<const word>, slice<const word>);

    template <endian::order r, std::integral word> bytestring<word> &increment (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> increment (slice<const word>);

    template <endian::order r, std::integral word> bytestring<word> &decrement (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> decrement (slice<const word>);

    template <endian::order r, std::integral word> bytestring<word> &times_2 (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> times_2 (slice<const word>);

    template <endian::order r, std::integral word> bytestring<word> &div_by_2 (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> div_by_2 (slice<const word>);

    template <endian::order r, std::integral word> bytestring<word> &plus (bytestring<word> &, slice<const word>);
    template <endian::order r, std::integral word> bytestring<word> plus (slice<const word>, slice<const word>);

    // assume first input is greater than second.
    template <endian::order r, std::integral word> bytestring<word> &minus (bytestring<word> &, slice<const word>);
    template <endian::order r, std::integral word> bytestring<word> minus (slice<const word>, slice<const word>);

    template <endian::order r, std::integral word> bytestring<word> &times (bytestring<word> &, slice<const word>);
    template <endian::order r, std::integral word> bytestring<word> times (slice<const word>, slice<const word>);

}

namespace data::arithmetic::twos {

    template <endian::order r, std::integral word> std::strong_ordering compare (slice<const word>, slice<const word>);

    template <endian::order r, std::integral word> bytestring<word> &negate (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> negate (slice<const word>);

    template <endian::order r, std::integral word> bytestring<word> &increment (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> increment (slice<const word>);

    template <endian::order r, std::integral word> bytestring<word> &decrement (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> decrement (slice<const word>);

    template <endian::order r, std::integral word> bytestring<word> &times_2 (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> times_2 (slice<const word>);

    template <endian::order r, std::integral word> bytestring<word> &div_by_2 (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> div_by_2 (slice<const word>);

    template <endian::order r, std::integral word> bytestring<word> &negate (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> negate (slice<const word>);

    template <endian::order r, std::integral word> bytestring<word> &abs (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> abs (slice<const word>);

    template <endian::order r, std::integral word> bytestring<word> &plus (bytestring<word> &, slice<const word>);
    template <endian::order r, std::integral word> bytestring<word> plus (slice<const word>, slice<const word>);

    template <endian::order r, std::integral word> bytestring<word> &times (bytestring<word> &, slice<const word>);
    template <endian::order r, std::integral word> bytestring<word> times (slice<const word>, slice<const word>);

}

namespace data::arithmetic::BC {

    template <endian::order r, std::integral word> std::weak_ordering compare (slice<const word>, slice<const word>);

    template <endian::order r, std::integral word> bytestring<word> &negate (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> negate (slice<const word>);

    template <endian::order r, std::integral word> bytestring<word> zero (size_t size, bool negative = false);

    template <endian::order r, std::integral word> bytestring<word> &increment (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> increment (slice<const word>);

    template <endian::order r, std::integral word> bytestring<word> &decrement (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> decrement (slice<const word>);

    template <endian::order r, std::integral word> bytestring<word> &times_2 (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> times_2 (slice<const word>);

    template <endian::order r, std::integral word> bytestring<word> &div_by_2 (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> div_by_2 (slice<const word>);

    // operate directly on the sign bit, so change negative zero to zero and vice versa.
    template <endian::order r, std::integral word> bytestring<word> &negate (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> negate (slice<const word>);

    template <endian::order r, std::integral word> bytestring<word> &abs (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> abs (slice<const word>);

    template <endian::order r, std::integral word> bytestring<word> &plus (bytestring<word> &, slice<const word>);
    template <endian::order r, std::integral word> bytestring<word> plus (slice<const word>, slice<const word>);

    template <endian::order r, std::integral word> bytestring<word> &times (bytestring<word> &, slice<const word>);
    template <endian::order r, std::integral word> bytestring<word> times (slice<const word>, slice<const word>);

}

namespace data::arithmetic {

    template <endian::order r, negativity c, std::integral word>
    bytestring<word> inline extend (slice<const word> x, size_t new_size) {
        bytestring<word> y {x};
        return extend<r, c> (y, new_size);
    }

    template <endian::order r, negativity c, std::integral word>
    bytestring<word> inline trim (slice<const word> x) {
        bytestring<word> y {x};
        return trim<r, c> (y);
    }

    template <endian::order r, negativity c, std::integral word>
    bytestring<word> inline &bit_and (bytestring<word> &a, slice<const word> b) {
        if (a.size () == b.size ()) {
            for (int i = 0; i < a.size (); i++) a[i] &= b[i];
            return a;
        }

        if (a.size () > b.size ()) return a = extend (a, b.size ()) &= b;
        else return a &= extend (b, a.size ());
    }

    template <endian::order r, negativity c, std::integral word>
    bytestring<word> inline &bit_or (bytestring<word> &a, slice<const word> b) {
        if (a.size () == b.size ()) {
            for (int i = 0; i < a.size (); i++) a[i] |= b[i];
            return a;
        }

        if (a.size () > b.size ()) return a = extend (a, b.size ()) |= b;
        else return a |= extend (b, a.size ());
    }

    template <endian::order r, negativity c, std::integral word>
    bytestring<word> inline &bit_xor (bytestring<word> &a, slice<const word> b) {
        if (a.size () == b.size ()) {
            for (int i = 0; i < a.size (); i++) a[i] ^= b[i];
            return a;
        }

        if (a.size () > b.size ()) return a = extend (a, b.size ()) ^ b;
        else return a ^= extend (b, a.size ());
    }

}

namespace data::arithmetic::nones {

    template <endian::order r, std::integral word>
    bytestring<word> inline increment (slice<const word> x) {
        bytestring<word> y {x};
        return increment<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline decrement (slice<const word> x) {
        bytestring<word> y {x};
        return decrement<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline times_2 (slice<const word> x) {
        bytestring<word> y {x};
        return times_2<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline div_by_2 (slice<const word> x) {
        bytestring<word> y {x};
        return div_by_2<r> (y);
    }

    template <endian::order r, std::integral word> bytestring<word> inline &plus (bytestring<word> &x, slice<const word>y) {
        return x = plus<r> (slice<const word> (x), y);
    }

    template <endian::order r, std::integral word> bytestring<word> inline &minus (bytestring<word> &x, slice<const word>y) {
        return x = minus<r> (slice<const word> (x), y);
    }

    template <endian::order r, std::integral word> bytestring<word> inline &times (bytestring<word> &x, slice<const word>y) {
        return x = times<r> (slice<const word> (x), y);
    }

}

namespace data::arithmetic::twos {

    template <endian::order r, std::integral word>
    bytestring<word> inline negate (slice<const word> x) {
        bytestring<word> y {x};
        return negate<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline increment (slice<const word> x) {
        bytestring<word> y {x};
        return increment<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline decrement (slice<const word> x) {
        bytestring<word> y {x};
        return decrement<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline times_2 (slice<const word> x) {
        bytestring<word> y {x};
        return times_2<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline div_by_2 (slice<const word> x) {
        bytestring<word> y {x};
        return div_by_2<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline abs (slice<const word> x) {
        bytestring<word> y {x};
        return abs<r> (y);
    }

    template <endian::order r, std::integral word> bytestring<word> inline &plus (bytestring<word> &x, slice<const word>y) {
        return x = plus<r> (slice<const word> (x), y);
    }

    template <endian::order r, std::integral word> bytestring<word> inline &times (bytestring<word> &x, slice<const word>y) {
        return x = times<r> (slice<const word> (x), y);
    }

}

namespace data::arithmetic::BC {

    template <endian::order r, std::integral word>
    bytestring<word> inline negate (slice<const word> x) {
        bytestring<word> y {x};
        return negate<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline increment (slice<const word> x) {
        bytestring<word> y {x};
        return increment<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline decrement (slice<const word> x) {
        bytestring<word> y {x};
        return decrement<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline times_2 (slice<const word> x) {
        bytestring<word> y {x};
        return times_2<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline div_by_2 (slice<const word> x) {
        bytestring<word> y {x};
        return div_by_2<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline abs (slice<const word> x) {
        bytestring<word> y {x};
        return abs<r> (y);
    }

    template <endian::order r, std::integral word> bytestring<word> inline &plus (bytestring<word> &x, slice<const word>y) {
        return x = plus<r> (slice<const word> (x), y);
    }

    template <endian::order r, std::integral word> bytestring<word> inline &times (bytestring<word> &x, slice<const word>y) {
        return x = times<r> (slice<const word> (x), y);
    }

}

namespace data::arithmetic {

    template <endian::order r, std::integral word>
    auto inline words (bytestring<word> &z) {
        return Words<r, word> {slice<word> (z)};
    }

    template <endian::order r, std::integral word>
    auto inline words (slice<const word> z) {
        return Words<r, word> {slice<word> {const_cast<word *> (z.data ()), z.size ()}};
    }

    template <endian::order r, negativity c, std::integral word> size_t inline minimal_size (slice<const word> x) {
        return minimal_size<c> (words<r> (x));
    }

    template <endian::order r, negativity c, std::integral word> bool inline is_minimal (slice<const word> x) {
        return minimal_size<r, c> (x) == x.size ();
    }

    template <endian::order r, std::integral word> bytestring<word> inline zero (size_t size) {
        return bytestring<word> (size, 0);
    }

    template <endian::order r, negativity c, std::integral word>
    bool inline is_zero (slice<const word> x) {
        if constexpr (c == negativity::BC) {
            return arithmetic::BC::is_zero (words<r> (x));
        } else {
            return arithmetic::is_zero (words<r> (x));
        }
    }

    template <endian::order r, negativity c, std::integral word> bool inline sign_bit (slice<const word> x) {
        if constexpr (c == negativity::nones) return false;
        else return arithmetic::sign_bit (words<r> (x));
    }

    template <endian::order r, negativity c, std::integral word>
    bytestring<word> &trim (bytestring<word> &x) {
        auto w = words<r> (x);
        size_t min_size = minimal_size<c> (w);
        if (min_size == size (w)) return x;
        auto n = zero<r, word> (min_size);
        std::copy (w.begin (), w.begin () + min_size, words<r> (n).begin ());

        if constexpr (c == negativity::BC)
            if (min_size != 0) words<r> (n)[-1] |= (w[-1] & get_sign_bit<word>::value);

        return x = n;
    }

    template <endian::order r, negativity c, std::integral word>
    bytestring<word> &extend (bytestring<word> &x, size_t new_size) {
        auto w = words<r> (x);

        if (new_size < size (w)) {
            size_t min_size = minimal_size<c> (w);
            if (new_size < min_size) throw exception {} << "cannot extend smaller than minimal size";
            return extend<r, c> (trim<r, c> (x), new_size);
        }

        if (new_size == size (w)) return x;

        bytestring<word> z = zero<r, word> (new_size);

        if constexpr (c == negativity::nones) {
            std::copy (w.rbegin (), w.rend (), words<r> (z).rbegin () + new_size - size (w));
        } else if constexpr (c == negativity::twos) {
            word extend_digit = twos::is_negative (w) ? std::numeric_limits<word>::max () : 0;

            auto i = words<r> (z).rbegin ();
            for (int n = 0; n < new_size - size (w); n++) {
                *i = extend_digit;
                i++;
            }

            std::copy (w.rbegin (), w.rend (), i);
        } else if constexpr (c == negativity::BC) {
            if (size (w) == 0) return x = z;

            auto v = words<r> (z);
            std::copy (w.begin (), w.end () - 1, v.begin ());
            word last = w[-1];

            v[-1] = last & get_sign_bit<word>::value ? get_sign_bit<word>::value : 0;
            v[size (w) - 1] = last & ~get_sign_bit<word>::value;
        }

        return x = z;
    }

    template <endian::order r, negativity c, std::integral word>
    bytestring<word> bit_and (slice<const word> a, slice<const word> b) {
        if (a.size () < b.size ()) return bit_and<r, c> (b, a);
        auto bt = extend<r, c> (b, a.size ());
        auto x = zero<r, word> (a.size ());
        bit_and<word> (x.end (), x.begin (), a.begin (),
            slice<const word> (bt).begin ());
        return x;
    }

    template <endian::order r, negativity c, std::integral word>
    bytestring<word> bit_or (slice<const word> a, slice<const word> b) {
        if (a.size () < b.size ()) return bit_or<r, c> (b, a);
        auto bt = extend<r, c> (b, a.size ());
        auto x = zero<r, word> (a.size ());
        bit_or<word> (x.end (), x.begin (), a.begin (),
            slice<const word> (bt).begin ());
        return x;
    }
}

namespace data::arithmetic::nones {

    // must be the same size.
    template <endian::order r, std::integral word> std::strong_ordering inline compare (slice<const word> a, slice<const word> b) {
        return arithmetic::compare<negativity::nones> (words<r> (a), words<r> (b));
    }

    template <endian::order r, std::integral word>
    bytestring<word> &increment (bytestring<word> &x) {
        auto w = words<r> (x);

        auto o = w.begin ();
        auto i = w.begin ();
        //auto remainder = arithmetic::plus<word> (w.end (), o, 1, i);
        bool remainder = arithmetic::add_with_carry<word> (w.end (), o, i, 1);
        if (remainder) {
            x = extend<r, negativity::nones> (x, size (w) + 1);
            words<r> (x)[-1] = remainder;
        }

        return x;
    }

    template <endian::order r, std::integral word>
    bytestring<word> &decrement (bytestring<word> &x) {
        auto w = words<r> (x);
        auto xx = w.begin ();
        auto xy = w.begin ();
        //if (!is_zero (w)) arithmetic::minus<word> (w.end (), xx, 1, xy);
        if (!is_zero (w)) arithmetic::subtract_with_carry<word> (w.end (), xx, xy, 1);
        return x;
    }

    template <endian::order r, std::integral word> bytestring<word> plus (slice<const word> a, slice<const word> b) {
        bytestring<word> n (std::max (a.size (), b.size ()), 0);
        auto wn = words<r> (n);
        word remainder = arithmetic::plus<r, word> (wn, words<r> (a), words<r> (b));
        if (remainder > 0) {
            extend<r, negativity::nones, word> (n, n.size () + 1);
            *words<r> (n).rbegin () = remainder;
        }

        return n;
    }

    // assume a > b;
    template <endian::order r, std::integral word> bytestring<word> minus (slice<const word> a, slice<const word> b) {
        bytestring<word> n (a.size (), 0);
        auto wn = words<r> (n);
        arithmetic::minus<r, word> (wn, words<r> (a), words<r> (b));
        return n;
    }
}

namespace data::arithmetic::twos {
    template <endian::order r, std::integral word> bytestring<word> inline &negate (bytestring<word> &x) {
        return increment<r, word> (x.bit_negate ());
    }

    template <endian::order r, std::integral word> std::strong_ordering compare (slice<const word> a, slice<const word> b) {

        auto wa = words<r> (a);
        auto wb = words<r> (b);

        bool na = is_negative (wa);
        bool nb = is_negative (wb);

        // TODO it should be possible to compare negative numbers
        // without negating them.
        if (na && nb) {
            auto ya = negate<r> (b);
            auto yb = negate<r> (a);
            return arithmetic::compare<negativity::nones> (words<r> (ya), words<r> (yb));
        }

        if (!na && !nb) return arithmetic::compare<negativity::nones> (wa, wb);
        return na ? std::strong_ordering::less : std::strong_ordering::greater;
    }

    // we extend the number once with every increment, which will get very
    // inefficient if you don't trim regularly. It should be possible to
    // extend only when we need to, but we don't right now.
    template <endian::order r, std::integral word>
    bytestring<word> &increment (bytestring<word> &x) {

        auto w = words<r> (x);
        if (is_negative_one (w)) return x = zero<r, word> (0);

        // always extend by one in case we would increment something that would produce a positive number.
        extend<r, negativity::twos> (x, size (w) + 1);
        w = words<r> (x);

        auto oit = w.begin ();
        auto iit = w.begin ();

        arithmetic::add_with_carry<word> (w.end (), oit, iit, 1);
        return x;
    }

    template <endian::order r, std::integral word>
    bytestring<word> &decrement (bytestring<word> &x) {

        auto w = words<r> (x);
        if (is_zero (w)) return x = bytestring<word> (1, std::numeric_limits<word>::max ());

        extend<r, negativity::twos> (x, size (w) + 1);

        w = words<r> (x);
        auto xx = w.begin ();
        auto xy = w.begin ();

        // a remainder is provided here that we throw out
        // but it could be used to detect an overflow.
        arithmetic::subtract_with_carry<word> (w.end (), xx, xy, 1);
        return x;
    }
}

namespace data::arithmetic::BC {

    template <endian::order r, std::integral word> bytestring<word> zero (size_t size, bool negative) {
        if (negative && size == 0) throw exception {"cannot make negative zero of size zero."};
        bytestring<word> z (size, 0);
        if (negative) words<r> (z)[-1] = get_sign_bit<word>::value;
        return z;
    }

    template <endian::order r, std::integral word> std::weak_ordering compare (slice<const word> a, slice<const word> b) {

        auto wa = words<r> (a);
        auto wb = words<r> (b);

        math::signature na = sign (wa);
        math::signature nb = sign (wb);

        if (na == math::zero) switch (nb) {
            case math::positive: return std::weak_ordering::less;
            case math::negative: return std::weak_ordering::greater;
            default: return std::weak_ordering::equivalent;
        }

        if (nb == math::zero) return na == math::positive ? std::weak_ordering::greater : std::weak_ordering::less;

        if (na == math::positive && nb == math::positive)
            return arithmetic::compare<negativity::nones> (wa, wb);

        if (na == math::negative && nb == math::negative) {
            auto ya = negate<r> (b);
            auto yb = negate<r> (a);
            return arithmetic::compare<negativity::nones> (words<r> (ya), words<r> (yb));
        }

        return na == math::negative ? std::weak_ordering::less : std::weak_ordering::greater;
    }

    template <endian::order r, std::integral word>
    bytestring<word> &increment (bytestring<word> &x) {
        auto w = words<r> (x);

        // zero can be empty, so we handle this case separately.
        if (BC::is_zero (w)) {
            x = zero<r, word> (1);
            words<r> (x)[1] = 1;
            return x;
        }

        if (sign_bit (w)) return negate<r> (decrement<r> (negate<r> (x)));

        auto oit = w.begin ();
        auto iit = w.begin ();

        //auto remainder = arithmetic::plus<word> (w.end (), oit, 1, iit);
        auto remainder = arithmetic::add_with_carry<word> (w.end (), oit, iit, 1);
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
        if (BC::is_zero (w)) return negate<r> (increment<r> (x = zero<r, word> (0)));

        if (arithmetic::sign_bit (w))
            return negate<r> (increment<r> (negate<r> (x)));

        auto xx = w.begin ();
        auto xy = w.begin ();
        arithmetic::subtract_with_carry<word> (w.end (), xx, xy, 1);
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

    template <endian::order r, std::integral word> bytestring<word> plus (slice<const word> a, slice<const word> b) {

        auto wa = words<r> (a);
        auto wb = words<r> (b);

        bool an = is_negative (wa);
        {
            bool bn = is_negative (wb);
            if (!an && !bn) {
                bytestring<word> c (std::max (a.size (), b.size ()), 9);
                auto wc = words<r> (c);
                word remainder = arithmetic::plus<r, word> (wc, wa, wb);
                if (remainder > 0 || sign_bit (wc)) {
                    extend<r, negativity::nones, word> (c, c.size () + 1);
                    *words<r> (c).rbegin () = remainder;
                }
                return c;
            }

            if (an && bn) return negate<r, word> (plus<r, word> (negate<r, word> (a),  negate<r, word> (b)));
        }

        if (an) {
            bytestring<word> na = negate<r, word> (a);
            return compare<r, word> (na, b) > 0 ? negate<r, word> (nones::minus<r, word> (na, b)): nones::minus<r, word> (b, na);
        } else {
            bytestring<word> nb = negate<r, word> (b);
            return compare<r, word> (a, nb) > 0 ? nones::minus<r, word> (a, nb): negate<r, word> (nones::minus<r, word> (nb, a));
        }
    }

    template <endian::order r, std::integral word> bytestring<word> times (slice<const word> a, slice<const word> b) {

        auto as = abs<r, word> (a);
        auto bs = abs<r, word> (b);
        bytestring<word> cs (as.size () + bs.size () + 1, 0);

        auto wa = words<r> (as);
        auto wb = words<r> (bs);
        auto wc = words<r> (cs);

        arithmetic::times (wc, wa, wb);

        return is_negative (words<r> (a)) == is_negative (words<r> (b)) ? cs : negate<r, word> (cs);
    }
}

#endif
