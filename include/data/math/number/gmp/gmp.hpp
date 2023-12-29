// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP
#define DATA_MATH_NUMBER_GMP

#include <data/math/number/gmp/N.hpp>
#include <data/math/number/gmp/sqrt.hpp>

namespace data::math {

    division<N, N> inline divide<N, N>::operator () (const N &v, const N &z) {
        return number::natural::divide (v, z);
    }

    division<Z, N> inline divide<Z, Z>::operator () (const Z &v, const Z &z) {
        return number::integer::divide (v, z);
    }

    division<Z, N> inline divide<Z, N>::operator () (const Z &v, const N &z) {
        return number::integer::divide (v, Z (z));
    }

    division<dec_uint, dec_uint> inline divide<dec_uint, dec_uint>::operator () (const dec_uint &v, const dec_uint &z) {
        auto d = divide<N> {} (N (v), N (z));
        return {encoding::decimal::write (d.Quotient), encoding::decimal::write (d.Remainder)};
    }

    division<dec_int, dec_uint> inline divide<dec_int, dec_uint>::operator () (const dec_int &v, const dec_uint &z) {
        auto d = divide<Z, N> {} (Z (v), N (z));
        return {encoding::signed_decimal::write (d.Quotient), encoding::decimal::write (d.Remainder)};
    }

    division<dec_int, dec_uint> inline divide<dec_int, dec_int>::operator () (const dec_int &v, const dec_int &z) {
        auto d = divide<Z, Z> {} (Z (v), Z (z));
        return {encoding::signed_decimal::write (d.Quotient), encoding::decimal::write (d.Remainder)};
    }

    template <hex_case zz>
    division<hex::uint<zz>, hex::uint<zz>> divide<hex::uint<zz>, hex::uint<zz>>::operator () (const hex::uint<zz> &v, const hex::uint<zz> &z) {
        auto d = divide<N> {} (N (v), N (z));
        return {encoding::hexidecimal::write<zz> (d.Quotient), encoding::hexidecimal::write<zz> (d.Remainder)};
    }

    template <hex_case zz>
    division<hex::int1<zz>, hex::uint<zz>> divide<hex::int1<zz>, hex::int1<zz>>::operator () (const hex::int1<zz> &v, const hex::int1<zz> &z) {
        auto d = divide<Z, N> {} (Z (v), N (z));
        return {encoding::hexidecimal::write<number::complement::ones, zz> (d.Quotient), encoding::hexidecimal::write<zz> (d.Remainder)};
    }

    template <hex_case zz>
    division<hex::int1<zz>, hex::uint<zz>> divide<hex::int1<zz>, hex::uint<zz>>::operator () (const hex::int1<zz> &v, const hex::uint<zz> &z) {
        auto d = divide<Z, N> {} (Z (v), N (z));
        return {encoding::hexidecimal::write<number::complement::ones, zz> (d.Quotient), encoding::hexidecimal::write<zz> (d.Remainder)};
    }

    template <hex_case zz>
    division<hex::int2<zz>, hex::int2<zz>> divide<hex::int2<zz>, hex::int2<zz>>::operator () (const hex::int2<zz> &v, const hex::int2<zz> &z) {
        auto d = divide<Z, Z> {} (Z (v), Z (z));
        return {encoding::hexidecimal::write<number::complement::twos, zz> (d.Quotient),
            encoding::hexidecimal::write<number::complement::twos, zz> (d.Remainder)};
    }
}

namespace data::encoding::decimal {
    inline string::operator math::N () const {
        return math::N::read (*this);
    }
}

namespace data::encoding::signed_decimal {
    inline string::operator math::Z () const {
        return math::Z::read (*this);
    }
}

namespace data::encoding::hexidecimal {

    template <complement c, hex::letter_case cx> inline complemented_string<c, cx>::operator math::Z () const {
        return math::Z (math::Z_bytes_twos<endian::big>::read (*this));
    }

    template <hex::letter_case cx> inline complemented_string<complement::nones, cx>::operator math::N () const {
        return math::N::read (*this);
    }
}

#endif 
