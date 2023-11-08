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
    template <endian::order r, std::integral word, complement c> bytestring<word> zero (size_t new_size);

}

namespace data::math::number::arithmetic::nones {

    template <endian::order r, std::integral word> bytestring<word> &trim (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> trim (const bytestring<word> &);

    template <endian::order r, std::integral word> bytestring<word> &extend (bytestring<word> &, size_t new_size);
    template <endian::order r, std::integral word> bytestring<word> extend (const bytestring<word> &, size_t new_size);

    template <endian::order r, std::integral word> bytestring<word> &increment (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> increment (const bytestring<word> &);

    template <endian::order r, std::integral word> bytestring<word> &decrement (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> decrement (const bytestring<word> &);

    template <endian::order r, std::integral word> bytestring<word> &times_2 (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> times_2 (const bytestring<word> &);

    template <endian::order r, std::integral word> bytestring<word> &div_by_2 (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> div_by_2 (const bytestring<word> &);

    template <endian::order r, std::integral word> bytestring<word> &plus (bytestring<word> &, const bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> plus (const bytestring<word> &, const bytestring<word> &);

    template <endian::order r, std::integral word> bytestring<word> &minus (bytestring<word> &, const bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> minus (const bytestring<word> &, const bytestring<word> &);

    template <endian::order r, std::integral word> bytestring<word> &times (bytestring<word> &, const bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> times (const bytestring<word> &, const bytestring<word> &);

}

namespace data::math::number::arithmetic::ones {

    template <endian::order r, std::integral word> bytestring<word> &trim (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> trim (const bytestring<word> &);

    template <endian::order r, std::integral word> bytestring<word> &extend (bytestring<word> &, size_t new_size);
    template <endian::order r, std::integral word> bytestring<word> extend (const bytestring<word> &, size_t new_size);

    template <endian::order r, std::integral word> bytestring<word> &increment (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> increment (const bytestring<word> &);

    template <endian::order r, std::integral word> bytestring<word> &decrement (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> decrement (const bytestring<word> &);

    template <endian::order r, std::integral word> bytestring<word> &times_2 (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> times_2 (const bytestring<word> &);

    template <endian::order r, std::integral word> bytestring<word> &div_by_2 (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> div_by_2 (const bytestring<word> &);

    template <endian::order r, std::integral word> bytestring<word> &negate (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> negate (const bytestring<word> &);

    template <endian::order r, std::integral word> bytestring<word> &abs (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> abs (const bytestring<word> &);

    template <endian::order r, std::integral word> bytestring<word> &plus (bytestring<word> &, const bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> plus (const bytestring<word> &, const bytestring<word> &);

    template <endian::order r, std::integral word> bytestring<word> &minus (bytestring<word> &, const bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> minus (const bytestring<word> &, const bytestring<word> &);

    template <endian::order r, std::integral word> bytestring<word> &times (bytestring<word> &, const bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> times (const bytestring<word> &, const bytestring<word> &);

}

namespace data::math::number::arithmetic::twos {

    template <endian::order r, std::integral word> bytestring<word> zero (size_t size, bool negative = false);

    template <endian::order r, std::integral word> bytestring<word> &trim (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> trim (const bytestring<word> &);

    template <endian::order r, std::integral word> bytestring<word> &extend (bytestring<word> &, size_t new_size);
    template <endian::order r, std::integral word> bytestring<word> extend (const bytestring<word> &, size_t new_size);

    template <endian::order r, std::integral word> bytestring<word> &increment (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> increment (const bytestring<word> &);

    template <endian::order r, std::integral word> bytestring<word> &decrement (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> decrement (const bytestring<word> &);

    template <endian::order r, std::integral word> bytestring<word> &times_2 (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> times_2 (const bytestring<word> &);

    template <endian::order r, std::integral word> bytestring<word> &div_by_2 (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> div_by_2 (const bytestring<word> &);

    // operate directly on the sign bit, so change negative zero to zero and vice versa.
    template <endian::order r, std::integral word> bytestring<word> &negate (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> negate (const bytestring<word> &);

    template <endian::order r, std::integral word> bytestring<word> &abs (bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> abs (const bytestring<word> &);

    template <endian::order r, std::integral word> bytestring<word> &plus (bytestring<word> &, const bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> plus (const bytestring<word> &, const bytestring<word> &);

    template <endian::order r, std::integral word> bytestring<word> &minus (bytestring<word> &, const bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> minus (const bytestring<word> &, const bytestring<word> &);

    template <endian::order r, std::integral word> bytestring<word> &times (bytestring<word> &, const bytestring<word> &);
    template <endian::order r, std::integral word> bytestring<word> times (const bytestring<word> &, const bytestring<word> &);

}

namespace data::math::number::arithmetic::nones {

    template <endian::order r, std::integral word>
    bytestring<word> inline trim (const bytestring<word> &x) {
        auto y = x;
        return trim<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline extend (const bytestring<word> &x, size_t new_size) {
        auto y = x;
        return extend<r> (y, new_size);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline increment (const bytestring<word> &x) {
        auto y = x;
        return increment<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline decrement (const bytestring<word> &x) {
        auto y = x;
        return decrement<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline times_2 (const bytestring<word> &x) {
        auto y = x;
        return times_2<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline div_by_2 (const bytestring<word> &x) {
        auto y = x;
        return div_by_2<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline plus (const bytestring<word> &x, const bytestring<word> &z) {
        auto y = x;
        return plus<r> (y, z);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline minus (const bytestring<word> &x, const bytestring<word> &z) {
        auto y = x;
        return minus<r> (y, z);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline times (const bytestring<word> &x, const bytestring<word> &z) {
        auto y = x;
        return times<r> (y, z);
    }

}

namespace data::math::number::arithmetic::ones {

    template <endian::order r, std::integral word>
    bytestring<word> inline trim (const bytestring<word> &x) {
        auto y = x;
        return trim<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline extend (const bytestring<word> &x, size_t new_size) {
        auto y = x;
        return extend<r> (y, new_size);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline increment (const bytestring<word> &x) {
        auto y = x;
        return increment<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline decrement (const bytestring<word> &x) {
        auto y = x;
        return decrement<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline times_2 (const bytestring<word> &x) {
        auto y = x;
        return times_2<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline div_by_2 (const bytestring<word> &x) {
        auto y = x;
        return div_by_2<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline negate (const bytestring<word> &x) {
        auto y = x;
        return negate<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline abs (const bytestring<word> &x) {
        auto y = x;
        return abs<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline plus (const bytestring<word> &x, const bytestring<word> &z) {
        auto y = x;
        return plus<r> (y, z);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline minus (const bytestring<word> &x, const bytestring<word> &z) {
        auto y = x;
        return minus<r> (y, z);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline times (const bytestring<word> &x, const bytestring<word> &z) {
        auto y = x;
        return times<r> (y, z);
    }

}

namespace data::math::number::arithmetic::twos {

    template <endian::order r, std::integral word>
    bytestring<word> inline trim (const bytestring<word> &x) {
        auto y = x;
        return trim<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline extend (const bytestring<word> &x, size_t new_size) {
        auto y = x;
        return extend<r> (y, new_size);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline increment (const bytestring<word> &x) {
        auto y = x;
        return increment<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline decrement (const bytestring<word> &x) {
        auto y = x;
        return decrement<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline times_2 (const bytestring<word> &x) {
        auto y = x;
        return times_2<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline div_by_2 (const bytestring<word> &x) {
        auto y = x;
        return div_by_2<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline negate (const bytestring<word> &x) {
        auto y = x;
        return negate<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline abs (const bytestring<word> &x) {
        auto y = x;
        return abs<r> (y);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline plus (const bytestring<word> &x, const bytestring<word> &z) {
        auto y = x;
        return plus<r> (y, z);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline minus (const bytestring<word> &x, const bytestring<word> &z) {
        auto y = x;
        return minus<r> (y, z);
    }

    template <endian::order r, std::integral word>
    bytestring<word> inline times (const bytestring<word> &x, const bytestring<word> &z) {
        auto y = x;
        return times<r> (y, z);
    }

}

namespace data::math::number::arithmetic {

    template <endian::order r, std::integral word>
    auto words (bytestring<word> &z) {
        return encoding::words<r, word> {slice<word> (z)};
    }

    template <endian::order r, std::integral word> bytestring<word> inline zero (size_t size) {
        return bytestring<word> (size, 0);
    }

}

namespace data::math::number::arithmetic::twos {

    template <endian::order r, std::integral word> bytestring<word> zero (size_t size, bool negative) {
        if (negative && size == 0) throw exception {"cannot make negative zero of size zero."};
        bytestring<word> z (size, 0);
        if (negative) words<r> (z)[-1] = get_sign_bit<word>::value;
        return z;
    }

}

namespace data::math::number::arithmetic::nones {

    template <endian::order r, std::integral word>
    bytestring<word> &trim (bytestring<word> &x) {
        auto w = words<r> (x);
        size_t min_size = minimal_size (w);
        if (min_size == size (w)) return x;
        auto n = zero<r, word> (min_size);
        std::copy (w.begin (), w.begin () + min_size, words<r> (n).begin ());
        return x = n;
    }
}

namespace data::math::number::arithmetic::ones {

    template <endian::order r, std::integral word>
    bytestring<word> &trim (bytestring<word> &x) {
        auto w = words<r> (x);
        size_t min_size = minimal_size (w);
        if (min_size == size (w)) return x;
        auto n = zero<r, word> (min_size);
        std::copy (w.begin (), w.begin () + min_size, words<r> (n).begin ());
        return x = n;
    }
}

namespace data::math::number::arithmetic::twos {

    template <endian::order r, std::integral word>
    bytestring<word> &trim (bytestring<word> &x) {
        auto w = words<r> (x);
        size_t min_size = minimal_size (w);
        if (min_size == size (w)) return x;
        auto n = zero<r, word> (min_size);
        std::copy (w.begin (), w.begin () + min_size, words<r> (n).begin ());
        if (min_size != 0) words<r> (n)[-1] |= (w[-1] & get_sign_bit<word>::value);
        return x = n;
    }
}

namespace data::math::number::arithmetic::nones {

    template <endian::order r, std::integral word>
    bytestring<word> &extend (bytestring<word> &x, size_t new_size) {
        auto w = words<r> (x);

        if (new_size < size (w)) {
            size_t min_size = minimal_size (w);
            if (new_size < min_size) throw std::invalid_argument {"cannot extend smaller than minimal size"};
            return extend<r> (trim<r> (x), new_size);
        }

        if (new_size == size (w)) return x;

        bytestring<word> z = zero<r, word> (new_size);
        std::copy (w.rbegin (), w.rend (), words<r> (z).rbegin () + new_size - size (w));
        return x = z;
    }
}

namespace data::math::number::arithmetic::ones {

    template <endian::order r, std::integral word>
    bytestring<word> &extend (bytestring<word> &x, size_t new_size) {
        auto w = words<r> (x);

        if (new_size < size (w)) {
            size_t min_size = minimal_size (w);
            if (new_size < min_size) throw std::invalid_argument {"cannot extend smaller than minimal size"};
            return extend<r> (trim<r> (x), new_size);
        }

        if (new_size == size (w)) return x;

        word extend_digit = is_negative (w) ? std::numeric_limits<word>::max () : 0;

        bytestring<word> z = zero<r, word> (new_size);

        auto i = words<r> (z).rbegin ();
        for (int n = 0; n < new_size - size (w); n++) {
            *i = extend_digit;
            i++;
        }

        std::copy (w.rbegin (), w.rend (), i);
        return x = z;
    }
}

namespace data::math::number::arithmetic::twos {

    template <endian::order r, std::integral word>
    bytestring<word> &extend (bytestring<word> &x, size_t new_size) {
        auto w = words<r> (x);

        if (new_size < size (w)) {
            size_t min_size = minimal_size (w);
            if (new_size < min_size) throw std::invalid_argument {"cannot extend smaller than minimal size"};
            return extend<r> (trim<r> (x), new_size);
        }

        if (new_size == size (w)) return x;

        bytestring<word> z = zero<r, word> (new_size);

        if (size (w) == 0) return x = z;

        auto v = words<r> (z);
        std::copy (w.begin (), w.end () - 1, v.begin ());
        word last = w[-1];

        v[-1] = last & get_sign_bit<word>::value ? get_sign_bit<word>::value : 0;
        v[size (w) - 1] = last & ~get_sign_bit<word>::value;
        return x = z;
    }
}

namespace data::math::number::arithmetic::nones {

    template <endian::order r, std::integral word>
    bytestring<word> &increment (bytestring<word> &x) {
        auto w = words<r> (x);

        auto o = w.begin ();
        auto i = w.begin ();
        auto remainder = arithmetic::plus<byte> (w.end (), o, 1, i);

        if (remainder != 0) {
            x = extend<r> (x, size (w) + 1);
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
}

namespace data::math::number::arithmetic::ones {

    // we extend the number once with every increment, which will get very
    // inefficient if you don't trim regularly. It should be possible to
    // extend only when we need to, but we don't right now.
    template <endian::order r, std::integral word>
    bytestring<word> &increment (bytestring<word> &x) {

        extend<r> (x, size (words<r> (x)) + 1);

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

        extend<r> (x, size (w) + 1);

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
}

#endif
