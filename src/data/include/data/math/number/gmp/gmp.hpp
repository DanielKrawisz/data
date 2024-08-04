// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP
#define DATA_MATH_NUMBER_GMP

#include <data/math/number/gmp/Z.hpp>
#include <data/math/number/gmp/sqrt.hpp>

namespace data::math {

    division<dec_uint, dec_uint> inline divide<dec_uint, dec_uint>::operator () (const dec_uint &v, const nonzero<dec_uint> &z) {
        auto d = divide<N> {} (N (v), nonzero<N> {N (z.Value)});
        return {encoding::decimal::write (d.Quotient), encoding::decimal::write (d.Remainder)};
    }

    division<dec_int, dec_uint> inline divide<dec_int, dec_uint>::operator () (const dec_int &v, const nonzero<dec_uint> &z) {
        auto d = divide<Z, N> {} (Z (v), nonzero<N> {N (z.Value)});
        return {encoding::signed_decimal::write (d.Quotient), encoding::decimal::write (d.Remainder)};
    }

    division<dec_int, dec_uint> inline divide<dec_int, dec_int>::operator () (const dec_int &v, const nonzero<dec_int> &z) {
        auto d = divide<Z, Z> {} (Z {v}, nonzero<Z> {Z {z.Value}});
        return {encoding::signed_decimal::write (d.Quotient), encoding::decimal::write (d.Remainder)};
    }

    template <hex_case zz> division<hex::uint<zz>, hex::uint<zz>> divide<hex::uint<zz>, hex::uint<zz>>::operator ()
    (const hex::uint<zz> &v, const nonzero<hex::uint<zz>> &z) {
        auto d = divide<N> {} (N (v), nonzero<N> {N (z.Value)});
        return {encoding::hexidecimal::write<zz> (d.Quotient), encoding::hexidecimal::write<zz> (d.Remainder)};
    }

    template <hex_case zz>
    division<hex::int1<zz>, hex::uint<zz>>
    divide<hex::int1<zz>, hex::int1<zz>>::operator () (const hex::int1<zz> &v, const nonzero<hex::int1<zz>> &z) {
        auto d = divide<Z> {} (Z (v), nonzero<Z> {Z (z.Value)});
        return {encoding::hexidecimal::write<number::complement::ones, zz> (d.Quotient), encoding::hexidecimal::write<zz> (d.Remainder)};
    }

    template <hex_case zz>
    division<hex::int1<zz>, hex::uint<zz>>
    divide<hex::int1<zz>, hex::uint<zz>>::operator () (const hex::int1<zz> &v, const nonzero<hex::uint<zz>> &z) {
        auto d = divide<Z, N> {} (Z (v), nonzero<N> {N (z.Value)});
        return {encoding::hexidecimal::write<number::complement::ones, zz> (d.Quotient), encoding::hexidecimal::write<zz> (d.Remainder)};
    }

    template <hex_case zz> requires number::integer<hex::int2<zz>>
    division<hex::int2<zz>, hex::int2<zz>> divide<hex::int2<zz>, hex::int2<zz>>::operator ()
    (const hex::int2<zz> &v, const nonzero<hex::int2<zz>> &z) {
        auto d = divide<Z, Z> {} (Z (v), nonzero<Z> {Z (z.Value)});
        return {encoding::hexidecimal::write<number::complement::twos, zz> (d.Quotient),
            encoding::hexidecimal::write<number::complement::twos, zz> (d.Remainder)};
    }
}

namespace data::encoding::decimal {

    inline string::string (const math::N &n) {
        *this = write (n);
    }
}

namespace data::encoding::signed_decimal {

    inline string::string (const math::Z &n) {
        *this = write (n);
    }
}

namespace data::encoding::hexidecimal {

    template <complement c, hex::letter_case cx> inline complemented_string<c, cx>::operator math::Z () const {
        if constexpr (c == complement::ones) return math::Z {string_view (*this)};
        else return math::Z (math::Z_bytes_twos<endian::big, byte>::read (*this));
    }

    template <hex::letter_case zz>
    inline complemented_string<complement::nones, zz>::complemented_string (const math::N &n) {
        *this = write<zz> (n);
    }

    template <complement c, hex::letter_case zz>
    inline complemented_string<c, zz>::complemented_string (const math::Z &n) {
        *this = write<c, zz> (n);
    }
}

#endif 
