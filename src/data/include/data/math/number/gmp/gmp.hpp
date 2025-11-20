// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP
#define DATA_MATH_NUMBER_GMP

// TODO this can all be moved into Z
#include <data/math/number/gmp/Z.hpp>

namespace data::math::def {

    division<dec_uint, dec_uint> inline divmod<dec_uint, dec_uint>::operator () (const dec_uint &v, const nonzero<dec_uint> &z) {
        auto d = divmod<N> {} (N (v), nonzero<N> {N (z.Value)});
        return {
            encoding::decimal::write (d.Quotient), 
            encoding::decimal::write (d.Remainder)};
    }

    division<dec_int, dec_uint> inline divmod<dec_int, dec_uint>::operator () (const dec_int &v, const nonzero<dec_uint> &z) {
        auto d = divmod<Z, N> {} (Z (v), nonzero<N> {N (z.Value)});
        return {
            encoding::signed_decimal::write (d.Quotient), 
            encoding::decimal::write (d.Remainder)};
    }

    division<dec_int, dec_uint> inline divmod<dec_int, dec_int>::operator () (const dec_int &v, const nonzero<dec_int> &z) {
        auto d = divmod<Z, Z> {} (Z {v}, nonzero<Z> {Z {z.Value}});
        return {
            encoding::signed_decimal::write (d.Quotient), 
            encoding::decimal::write (d.Remainder)};
    }

    template <hex_case zz>
    division<hex::uint<zz>, hex::uint<zz>> inline
    divmod<hex::uint<zz>, hex::uint<zz>>::operator ()
        (const hex::uint<zz> &v, const nonzero<hex::uint<zz>> &z) {
        auto d = divmod<N> {} (N (v), nonzero<N> {N (z.Value)});
        return {
            encoding::hexidecimal::write<zz> (d.Quotient), 
            encoding::hexidecimal::write<zz> (d.Remainder)};
    }

    template <hex_case zz>
    division<hex::int2<zz>, hex::uint<zz>> inline
    divmod<hex::int2<zz>, hex::int2<zz>>::operator ()
        (const hex::int2<zz> &v, const nonzero<hex::int2<zz>> &z) {
        auto d = divmod<Z> {} (Z::read (v), nonzero<Z> {Z::read (z.Value)});
        return {
            encoding::hexidecimal::write<neg::twos, zz> (d.Quotient),
            encoding::hexidecimal::write<zz> (d.Remainder)};
    }

    template <hex_case zz>
    division<hex::int2<zz>, hex::uint<zz>> inline
    divmod<hex::int2<zz>, hex::uint<zz>>::operator ()
        (const hex::int2<zz> &v, const nonzero<hex::uint<zz>> &z) {
        auto d = divmod<Z, N> {} (Z::read (v), nonzero<N> {N {Z::read (z.Value)}});
        return {
            encoding::hexidecimal::write<neg::twos, zz> (d.Quotient),
            encoding::hexidecimal::write<zz> (d.Remainder)};
    }

    template <hex_case zz>
    division<hex::intBC<zz>, hex::intBC<zz>> inline
    divmod<hex::intBC<zz>, hex::intBC<zz>>::operator ()
        (const hex::intBC<zz> &v, const nonzero<hex::intBC<zz>> &z) {
        auto d = data::divmod (Z_bytes_BC<endian::big> (v), nonzero {Z_bytes_BC<endian::big> (z.Value)});
        return {
            encoding::hexidecimal::write<zz> (d.Quotient),
            encoding::hexidecimal::write<zz> (d.Remainder)};
    }
}

namespace data::encoding::decimal {

    inline string::string (const N n) {
        *this = decimal::write (n);
    }
}

namespace data::encoding::signed_decimal {

    inline string::string (const Z n) {
        *this = signed_decimal::write (n);
    }
}

namespace data::encoding::hexidecimal {

    template <neg c, hex::letter_case cx> inline complemented_string<c, cx>::operator Z () const {
        if constexpr (c == neg::twos) return Z {string_view (*this)};
        else return Z (math::Z_bytes_BC<endian::big, byte>::read (*this));
    }

    template <hex::letter_case zz>
    inline complemented_string<neg::nones, zz>::complemented_string (const N n) {
        *this = write<zz> (n);
    }

    template <neg c, hex::letter_case zz>
    inline complemented_string<c, zz>::complemented_string (const Z n) {
        *this = write<c, zz> (n);
    }
}

#endif 
