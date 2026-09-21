// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES
#define DATA_MATH_NUMBER_BYTES

#ifdef BIGNUM_GMP
#include <data/math/number/gmp/Z.hpp>
#else
#include <data/math/number/NTL/Z.hpp>
#endif

#include <data/encoding/digits.hpp>

// TODO it should be possible to get rid of this whole file and
// put it in bytes/Z.hpp, or alternately to get rid of Z.hpp
// and put it here.

namespace data::math::number {
    
    template <endian r, std::unsigned_integral word>
    N_bytes<r, word> inline N_bytes<r, word>::read (string_view x) {
        if (!encoding::natural::valid (x))
            throw exception {} << "invalid number string A " << x;

        if (encoding::hexidecimal::valid (x)) {
            if (auto m = encoding::hexidecimal::read<r, word> (x); bool (m)) {
                return N_bytes<r, word> {slice<const word> (*m)};
            } else throw exception {} << "invalid hex string size " << x.size () << "; " << x;
        }

        // TODO N_bytes should not depend on N.
        if (encoding::decimal::valid (x)) return N_bytes<r, word> (N::read (x));
        throw exception {} << "invalid number string B " << x;
    }

    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline Z_bytes<r, negativity::twos, word>::read (string_view x) {

        if (!encoding::integer::valid (x))
            throw exception {} << "invalid number string C \"" << x << "\"";

        if (encoding::hexidecimal::valid (x))
            return *encoding::integer::read<r, negativity::twos, word> (x);

        return convert<Z_bytes<r, negativity::twos, word>> (Z::read (x));
    }

    template <endian r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> inline Z_bytes<r, negativity::BC, word>::read (string_view x) {
        if (!encoding::integer::valid (x)) throw exception {} << "invalid number string \"" << x << "\"";
        if (encoding::hexidecimal::valid (x)) return *encoding::integer::read<r, negativity::BC, word> (x);
        return Z::read (x).operator Z_bytes<r, negativity::BC, word> ();
    }

    template <endian r, std::unsigned_integral word>
    std::ostream inline &operator << (std::ostream &o, const N_bytes<r, word> &n) {
        if (o.flags () & std::ios::hex) return encoding::hexidecimal::write (o, n);
        else if (o.flags () & std::ios::dec) return encoding::decimal::write (o, N (n));
        return o;
    }

    template <endian r, negativity c, std::unsigned_integral word>
    std::ostream inline &operator << (std::ostream &o, const Z_bytes<r, c, word> &n) {
        if (o.flags () & std::ios::hex) return encoding::hexidecimal::write (o, n);
        if (o.flags () & std::ios::dec) return encoding::signed_decimal::write (o, Z (n));
        return o;
    }

    template <endian r, std::unsigned_integral word>
    std::istream &operator >> (std::istream &i, N_bytes<r, word> &n) {
        encoding::natural::string x;
        i >> x;
        try {
            n = N_bytes<r, word>::read (x);
        } catch (...) {
            i.setstate (std::ios::failbit);
        }
        return i;
    }
    
    template <endian r, negativity c, std::unsigned_integral word>
    std::istream &operator >> (std::istream &i, Z_bytes<r, c, word> &n) {
        encoding::integer::string x;
        i >> x;
        try {
            n = Z_bytes<r, c, word>::read (x);
        } catch (...) {
            i.setstate (std::ios::failbit);
        }
        return i;
    }
    
    template <endian r, std::unsigned_integral word> inline
    N_bytes<r, word>::operator double () const {
        return double (N (*this));
    }
    
    template <endian r, std::unsigned_integral word> inline
    Z_bytes<r, negativity::twos, word>::operator double () const {
        return double (Z (*this));
    }
    
    template <endian r, std::unsigned_integral word> inline
    Z_bytes<r, negativity::BC, word>::operator double () const {
        return double (Z (*this));
    }
    
}

namespace data::encoding::decimal {
    
    template <endian::order r, std::unsigned_integral word>
    maybe<N_bytes<r, word>> inline read (string_view s) {
        if (!valid (s)) return {};
        return {read_base<N_bytes<r, word>> (s, 10, digit)};
    }
    
    template <endian::order r, std::unsigned_integral word>
    string inline write (const math::N_bytes<r, word> &n) {
        std::string x = write_base (N (n), characters ());
        return x == "" ? string {"0"} : string {x};
    }
}

namespace data::encoding::signed_decimal {
    
    template <endian::order r, negativity n, std::unsigned_integral word>
    maybe<math::number::Z_bytes<r, n, word>> read (string_view s) {
        if (!valid (s)) return {};
        bool negative = s[0] == '-';
        string_view positive = negative ? s.substr (1) : s;
        auto z = math::number::Z_bytes<r, n, word> (*decimal::read<r, word> (positive));
        if (negative) z = -z;
        return {z};
    }
    
}

namespace data::encoding::natural {
    
    template <endian::order r, std::unsigned_integral word>
    maybe<math::N_bytes<r, word>> inline read (string_view s) {
        if (!valid (s)) return {};

        if (hexidecimal::valid (s)) {
            auto p = hexidecimal::read<r, word> (s);
            if (!p) return {};
            return math::number::N_bytes<r, word> (math::number::N_bytes<r, word>::read (view<word> (*p)));
        }
        
        return decimal::read<r, word> (s);
    }
    
}

namespace data::encoding::hexidecimal {

    template <negativity n, hex::letter_case cx, std::integral I>
    string<cx> write (I x) {
        if constexpr (n == negativity::nones)
            return write<cx> (math::number::N_bytes<endian::big> {x});
        else return write<cx> (math::number::Z_bytes<endian::big, n> {x});
    }

    template <negativity neg, hex::letter_case zz>
    integer<neg, zz> inline bit_shift (const integer<neg, zz> &x, int i) {
        return write<zz> (data::bit_shift (
            std::conditional_t<neg == negativity::nones,
                math::number::N_bytes<endian::big, byte>,
                math::number::Z_bytes<endian::big, neg, byte>> (x), i));
    }

    template <hex_case zz, endian::order r, std::unsigned_integral word>
    integer<negativity::nones, zz> write (const math::number::N_bytes<r, word> &z) {
        std::stringstream ss;
        write (ss, static_cast<const oriented<r, word> &> (z), zz);
        return integer<negativity::nones, zz> {ss.str ()};
    }

    template <hex_case zz, endian::order r, negativity n, std::unsigned_integral word>
    integer<n, zz> write (const math::number::Z_bytes<r, n, word> &z) {
        std::stringstream ss;
        write (ss, static_cast<const oriented<r, word> &> (z), zz);
        return integer<n, zz> {ss.str ()};
    }

    template <negativity n, hex::letter_case cx>
    template <endian::order e> inline complemented_string<n, cx>::operator math::number::Z_bytes<e, n, byte> () const {
        auto result = math::number::Z_bytes<e, n, byte>::zero (this->size () / 2 - 1);
        hex::decode (this->end (), this->begin () + 2, result.words ().rbegin ());
        return result;
    }

    template <hex::letter_case cx>
    template <endian::order e> inline complemented_string<negativity::nones, cx>::operator math::number::N_bytes<e, byte> () const {
        auto result = math::number::N_bytes<e, byte>::zero (this->size () / 2 - 1);
        hex::decode (this->end (), this->begin () + 2, result.words ().rbegin ());
        return result;
    }
    
    namespace {
        template <endian::order r, negativity c> struct get_bytes_type {
            using value = math::number::Z_bytes<r, c, byte>;
        };
        
        template <endian::order r> struct get_bytes_type<r, negativity::nones> {
            using value = math::number::N_bytes<r, byte>;
        };
        
        template <endian::order r, negativity c>
        using bytes_type = get_bytes_type<r, c>::value;
        
        using nat = math::number::N;
        
        template <hex::letter_case zz>
        inline nat read_num (const integer<negativity::nones, zz> &n) {
            return read_base<nat> (n.substr (2), 16, &digit);
        } 
        
        template <typename N>
        std::string write_hexidecimal (const N &n) {
            static std::string Characters = hex::characters_lower ();
            std::string p = write_base<N> (n, Characters);
            if ((p.size () % 2) == 1) return std::string {"0x0"} + p;
            return std::string {"0x"} + p;
        }
        
        template <hex::letter_case zz> 
        integer<negativity::twos, zz> inline bit_shift (const integer<negativity::twos, zz> &x, int i) {
            auto o = read<endian::big, byte> (x);
            math::number::Z_bytes<endian::big, negativity::twos, byte> n;
            n.resize (o->size ());
            std::copy (o->begin (), o->end (), n.begin ());
            return write<zz> (n << i);
        }

        template <negativity c, hex::letter_case zz> struct divide {
            // if c is twos, nn must be either twos or nones.
            // if c is BC, nn must be BC.
            template <negativity nn>
            division<integer<c, zz>, decltype (abs (std::declval<integer<c, zz>> ()))>
            operator () (const integer<c, zz> &n, const math::nonzero<integer<nn, zz>> &x) const {
                if (x.Value == 0) throw math::division_by_zero {};

                using abs_type = decltype (abs (std::declval<integer<c, zz>> ()));
                // We need this optimization because we use division to convert from hex strings to N.
                if (x.Value == 16) return division<integer<c, zz>, abs_type> {n >> 4, n & integer<c, zz> {4}};
                
                else return math::number::integer_divmod (n, x.Value);
            }
        };
        
        template <hex::letter_case zz> struct divide<negativity::nones, zz> {
            division<integer<negativity::nones, zz>> operator ()
                (const integer<negativity::nones, zz> &n, const math::nonzero<integer<negativity::nones, zz>> &x) const {
                if (x.Value == 0) throw math::division_by_zero {} ;
                // We need this optimization because we use division to convert from hex strings to N.
                if (x.Value == 16) return division<integer<negativity::nones, zz>>{
                    n >> 4, n & integer<negativity::nones, zz> {4}};
                
                return math::number::natural_divmod (n, x.Value);
            }
        };

        template <negativity n, hex::letter_case zz> struct read_dec_integer {
            integer<n, zz> operator () (string_view x) {
                if (decimal::valid (x)) {
                    integer<negativity::nones, zz> z {write<zz> (*decimal::read<endian::little, byte> (x))};
                    return math::number::trim (integer<n, zz> {math::number::extend (z, z.size () + 2)});
                }

                if (signed_decimal::valid (x)) {
                    integer<negativity::nones, zz> z {write<zz> (*decimal::read<endian::little, byte> (x.substr (1)))};
                    return math::number::trim (-integer<n, zz> {math::number::extend (z, z.size () + 2)});
                }

                throw exception {} << "invalid number string: D " << x;
            }
        };

        template <hex::letter_case zz> struct read_dec_integer<negativity::nones, zz> {
            integer<negativity::nones, zz> operator () (string_view x) {
                auto np = decimal::read<endian::little, byte> (x);
                if (!np) throw exception {} << "invalid number string: E " << x;
                return integer<negativity::nones, zz> {write<zz> (*np)};
            }
        };
        
    }
    
    template <negativity c, hex::letter_case zz>
    inline integer<c, zz>::operator double () const {
        return double (bytes_type<endian::little, c>::read (*this));
    }

    template <negativity n, hex::letter_case zz>
    integer<n, zz> integer<n, zz>::read (string_view x) {
        if (hexidecimal::valid (x)) return integer<n, zz> {x};
        return read_dec_integer<n, zz> {} (x);
    }

    template <hex::letter_case cx>
    integer<negativity::nones, cx> inline operator % (const integer<negativity::nones, cx> &n, const integer<negativity::nones, cx> &x) {
        return write<cx> (N {n} % N {x});
    }

    template <hex::letter_case cx>
    integer<negativity::nones, cx> inline operator % (const integer<negativity::twos, cx> &n, const integer<negativity::nones, cx> &x) {
        return write<cx> (Z {n} % N {x});
    }
    
}

namespace data::encoding::integer {
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    std::ostream inline &write (std::ostream &o, const math::number::Z_bytes<r, c, word> &z) {
        if (sign (z) == negative) o << "-";
        return decimal::write (o, abs (z));
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    string inline write (const math::number::Z_bytes<r, c, word> &z) {
        std::stringstream ss;
        write (ss, z);
        return ss.str ();
    }
    
    template <endian::order r, std::unsigned_integral word>
    std::ostream inline &write (std::ostream &o, const math::number::N_bytes<r, word> &z) {
        return decimal::write (o, z);
    }
    
    template <endian::order r, std::unsigned_integral word>
    string inline write (const math::number::N_bytes<r, word> &z) {
        std::stringstream ss;
        write (ss, z);
        return ss.str ();
    }
    
}
namespace data::encoding::base58 {

    template <std::integral I> inline string::string (I x): string {encode (N {x})} {}

}

namespace data::math::def {

    template <hex_case zz>
    division<hex::uint<zz>, unsigned int>
    divmod<hex::uint<zz>, int>::operator () (const hex::uint<zz> &d, const nonzero<int> &z) {
        if (z.Value == 16) {
            if (d < 16) return {hex::uint<zz> {"0x"}, static_cast<unsigned int> (uint64 (d))};
            return {hex::uint<zz> {string::write ("0x0", d.substr (2, d.size () - 3))}, static_cast<unsigned int> (uint64 (d & 0x0fu))};
        }

        auto [quotient, remainder] = divmod<Z, Z> {} (Z (d), nonzero<Z> {z.Value});

        return division<hex::uint<zz>, unsigned int> {hex::uint<zz> {data::abs (quotient)}, static_cast<unsigned int> (uint64 (remainder))};
    }

    template <hex_case zz>
    division<hex::int2<zz>, unsigned int>
    divmod<hex::int2<zz>, int>::operator () (const hex::int2<zz> &d, const nonzero<int> &z) {
        if (z.Value == 16) {
            if (d < 0) {
                auto [quotient, remainder] = division<hex::int2<zz>, int> {} (-d, z);
                return {-quotient - 1, 16 - remainder};
            }

            if (d < 16) return {hex::int2<zz> {"0x"}, static_cast<unsigned int> (int64 (d))};
            return {hex::int2<zz> {std::string {"0x0"} + d.substr (2, d.size () - 3)}, static_cast<unsigned int> (int64 (d & 0x0f))};
        }

        auto [quotient, remainder] = divmod<Z, Z> {} (Z (d), nonzero<Z> {z.Value});
        return division<hex::int2<zz>, unsigned int> {hex::int2<zz> {quotient}, static_cast<unsigned int> (uint64 (remainder))};
    }

    template <hex_case zz>
    division<hex::intBC<zz>, int>
    divmod<hex::intBC<zz>, int>::operator () (const hex::intBC<zz> &d, const nonzero<int> &z) {
        if (z.Value == 16) {
            if (d < 0) {
                auto [quotient, remainder] = division<hex::intBC<zz>, int> {} (-d, z);
                return {-quotient - 1, 16 - remainder};
            }

            if (d < 16) return {hex::intBC<zz> {"0x"}, static_cast<unsigned int> (int64 (d))};
            return {hex::intBC<zz> {std::string {"0x0"} + d.substr (2, d.size () - 3)}, static_cast<int> (int64 (d & 0x0f))};
        }

        auto [quotient, remainder] = divmod<Z, Z> {} (Z (d), nonzero<Z> {z.Value});
        return division<hex::intBC<zz>, unsigned int> {hex::intBC<zz> {quotient}, static_cast<unsigned int> (uint64 (remainder))};
    }

    template <endian r, std::unsigned_integral word>
    division<N_bytes<r, word>, N_bytes<r, word>> inline
    divmod<N_bytes<r, word>, N_bytes<r, word>>::operator ()
        (const N_bytes<r, word> &a, const nonzero<N_bytes<r, word>> &b) {
        return number::natural_divmod (a, b.Value);
    }

    template <endian r, std::unsigned_integral word>
    division<Z_bytes<r, word>, N_bytes<r, word>> inline
    divmod<Z_bytes<r, word>, N_bytes<r, word>>::operator ()
        (const Z_bytes<r, word> &a, const nonzero<N_bytes<r, word>> &b) {
        return number::integer_natural_divmod (a, b.Value);
    }

    template <endian r, std::unsigned_integral word>
    division<Z_bytes<r, word>, N_bytes<r, word>> inline
    divmod<Z_bytes<r, word>, Z_bytes<r, word>>::operator ()
        (const Z_bytes<r, word> &a, const nonzero<Z_bytes<r, word>> &b) {
        return number::integer_divmod<number::EUCLIDIAN_ALWAYS_POSITIVE> (a, b.Value);
    }

    template <endian r, std::unsigned_integral word>
    division<Z_bytes_BC<r, word>, Z_bytes_BC<r, word>> inline
    divmod<Z_bytes_BC<r, word>, Z_bytes_BC<r, word>>::operator ()
        (const Z_bytes_BC<r, word> &a, const nonzero<Z_bytes_BC<r, word>> &b) {
        return number::integer_divmod<number::TRUNCATE_TOWARD_ZERO> (a, b.Value);
    }

    division<dec_uint, dec_uint> inline divmod<dec_uint, dec_uint>::operator () (const dec_uint &v, const nonzero<dec_uint> &z) {
        auto d = divmod<N> {} (N (v), nonzero<N> {N (z.Value)});
        return {
            encoding::decimal::write (d.Quotient),
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
            encoding::hexidecimal::write<zz> (d.Quotient),
            encoding::hexidecimal::write<zz> (d.Remainder)};
    }

    template <hex_case zz>
    division<hex::int2<zz>, hex::uint<zz>> inline
    divmod<hex::int2<zz>, hex::uint<zz>>::operator ()
    (const hex::int2<zz> &v, const nonzero<hex::uint<zz>> &z) {
        auto d = divmod<Z, N> {} (Z::read (v), nonzero<N> {N {Z::read (z.Value)}});
        return {
            encoding::hexidecimal::write<zz> (d.Quotient),
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

    template <endian r, negativity c, std::unsigned_integral word>
    number::Z_bytes<r, c, word> inline convert<number::Z_bytes<r, c, word>, Z>::operator () (const Z &z) const {
        return z.operator number::Z_bytes<r, c, word> ();
    }

    template <negativity c, hex_case zz>
    hex::integer<c, zz> inline times<hex::integer<c, zz>>::operator ()
    (const hex::integer<c, zz> &a, const hex::integer<c, zz> &b) {
        if constexpr (c == negativity::nones)
            return encoding::hexidecimal::write<zz> (N (a) * N (b));
        else return encoding::hexidecimal::write<zz> (Z (a) * Z (b));
    }

    template <negativity c, hex_case zz>
    nonzero<hex::integer<c, zz>> inline times<hex::integer<c, zz>>::operator ()
    (const nonzero<hex::integer<c, zz>> &a, const nonzero<hex::integer<c, zz>> &b) {
        if constexpr (c == negativity::nones)
            return nonzero {encoding::hexidecimal::write<zz> (N (a.Value) * N (b.Value))};
        else return nonzero {encoding::hexidecimal::write<zz> (Z (a.Value) * Z (b.Value))};
    }

    dec_int inline times<dec_int, dec_int>::operator () (const dec_int &a, const dec_int &b) {
        return encoding::signed_decimal::write (Z (a) * Z (b));
    }

    nonzero<dec_int> inline times<dec_int, dec_int>::operator () (const nonzero<dec_int> &a, const nonzero<dec_int> &b) {
        return nonzero {encoding::signed_decimal::write (Z (a.Value) * Z (b.Value))};
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

    template <negativity neg, hex_case zz>
    dec_int inline convert<dec_int, hex::integer<neg, zz>>::operator () (const hex::integer<neg, zz> &x) const {
        return encoding::signed_decimal::write ((number::Z_bytes<endian::little, negativity::twos, byte> (x)));
    }
}

#endif
