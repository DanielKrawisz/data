// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES
#define DATA_MATH_NUMBER_BYTES

#include <data/encoding/integer.hpp>
#include <data/math/number/bytes/Z.hpp>
#include <data/math/number/division.hpp>

#include <data/encoding/digits.hpp>

namespace data::math::number {
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline N_bytes<r, word>::read (string_view x) {
        if (!encoding::natural::valid (x))
            throw exception {} << "invalid number string " << x;

        if (encoding::hexidecimal::valid (x)) {
            if (auto m = encoding::hexidecimal::read<r, word> (x); bool (m)) {
                return N_bytes<r, word> {slice<const word> (*m)};
            } else throw exception {} << "invalid hex string size " << x.size () << "; " << x;
        }

        if (encoding::decimal::valid (x)) return N_bytes<r, word> (math::N {x});
        throw exception {} << "invalid number string" << x;
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::twos, word> inline Z_bytes<r, negativity::twos, word>::read (string_view x) {
        if (!encoding::integer::valid (x)) throw std::invalid_argument {string {"invalid number string"} + string {x}};
        if (encoding::hexidecimal::valid (x)) return *encoding::integer::read<r, negativity::twos, word> (x);
        return Z_bytes<r, negativity::twos, word> (Z {x});
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, negativity::BC, word> inline Z_bytes<r, negativity::BC, word>::read (string_view x) {
        if (!encoding::integer::valid (x)) throw std::invalid_argument {string {"invalid number string"} + string {x}};
        if (encoding::hexidecimal::valid (x)) return *encoding::integer::read<r, negativity::BC, word> (x);
        return Z_bytes<r, negativity::BC, word> (Z {x});
    }
    
    template <endian::order r, std::unsigned_integral word>
    std::ostream inline &operator << (std::ostream &o, const N_bytes<r, word> &n) {
        if (o.flags () & std::ios::hex) return encoding::hexidecimal::write (o, n);
        else if (o.flags () & std::ios::dec) return encoding::decimal::write (o, math::N (n));
        return o;
    }
    
    template <endian::order r, negativity c, std::unsigned_integral word>
    std::ostream inline &operator << (std::ostream &o, const Z_bytes<r, c, word> &n) {
        if (o.flags () & std::ios::hex) return encoding::hexidecimal::write (o, n);
        if (o.flags () & std::ios::dec) return encoding::signed_decimal::write (o, Z (n));
        return o;
    }

    template <endian::order r, std::unsigned_integral word>
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
    
    template <endian::order r, negativity c, std::unsigned_integral word>
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
    
    template <endian::order r, std::unsigned_integral word> inline
    N_bytes<r, word>::operator double () const {
        return double (math::N (*this));
    }
    
    template <endian::order r, std::unsigned_integral word> inline
    Z_bytes<r, negativity::twos, word>::operator double () const {
        return double (Z (*this));
    }
    
    template <endian::order r, std::unsigned_integral word> inline
    Z_bytes<r, negativity::BC, word>::operator double () const {
        return double (Z (*this));
    }
    
}

namespace data::encoding::decimal {
    
    template <endian::order r, std::unsigned_integral word>
    maybe<math::N_bytes<r, word>> inline read (string_view s) {
        if (!valid (s)) return {};
        return {read_base<math::N_bytes<r, word>> (s, 10, digit)};
    }
    
    template <endian::order r, std::unsigned_integral word>
    string inline write (const math::N_bytes<r, word> &n) {
        std::string x = write_base (math::number::GMP::N (n), characters ());
        return x == "" ? string {"0"} : string {x};
    }
}

namespace data::encoding::signed_decimal {
    
    template <endian::order r, math::negativity n, std::unsigned_integral word>
    maybe<math::number::Z_bytes<r, n, word>> read (string_view s) {
        if (!valid (s)) return {};
        bool negative = s[0] == '-';
        string_view positive = negative ? s.substr (1) : s;
        auto z = math::number::Z_bytes<r, n, word> ((math::number::Z_bytes<r, n, word>) (*decimal::read<r, word> (positive)));
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
    
    namespace {
        template <endian::order r, negativity c> struct get_bytes_type {
            using value = math::number::Z_bytes<r, c, byte>;
        };
        
        template <endian::order r> struct get_bytes_type<r, negativity::nones> {
            using value = math::number::N_bytes<r, byte>;
        };
        
        template <endian::order r, math::negativity c>
        using bytes_type = get_bytes_type<r, c>::value;
        
        using nat = math::number::GMP::N;
        
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
        string<zz> shift (const string<zz> &x, int i) {
            auto o = read<endian::big, byte> (x);
            math::number::N_bytes<endian::big, byte> n;
            n.resize (o->size ());
            std::copy (o->begin (), o->end (), n.begin ());
            n = n << i;
            return write<zz> (math::number::extend (n, n.size () + 1));
        }
        
        template <hex::letter_case zz> 
        integer<negativity::twos, zz> inline bit_shift (const integer<negativity::twos, zz> &x, int i) {
            auto o = read<endian::big, byte> (x);
            math::number::Z_bytes<endian::big, negativity::twos, byte> n;
            n.resize (o->size ());
            std::copy (o->begin (), o->end (), n.begin ());
            return write<zz> (n << i);
        }
        
        // the out string will always be the size of the sum of the two inputs, which won't necessarily be equal size. 
        template <hex::letter_case zz>
        void times (string<zz> &out, const string<zz> &a, const string<zz> &b) {
            auto characters = hex::characters (zz);
            
            int a_max = a.size () - 3;
            int b_max = b.size () - 3;
            
            math::Z remainder = 0;
            int io_max = out.size () - 2;
            
            for (int io = 0; io < io_max; io++) {
                math::Z total = remainder;
                int ia_min = std::max (0, io - b_max);
                int ib_min = std::max (0, io - a_max);
                int ia_max = io - ib_min;
                int ib_max = io - ia_min;
                for (int ia = ia_min; ia <= ia_max; ia++) {
                    int ib = ib_max + ia_min - ia; 
                    math::Z next (int (digit (a[a.size () - 1 - ia])) * int (digit (b[b.size () - 1 - ib])));
                    total += next;
                }
                
                out[out.size () - 1 - io] = characters[total % 16];
                remainder = total >> 4;
            }
        }

        template <math::negativity c, hex::letter_case zz> struct divide {
            // if c is twos, nn must be either twos or nones.
            // if c is BC, nn must be BC.
            template <math::negativity nn>
            division<integer<c, zz>, decltype (abs (std::declval<integer<c, zz>> ()))>
            operator () (const integer<c, zz> &n, const math::nonzero<integer<nn, zz>> &x) const {
                if (x.Value == 0) throw math::division_by_zero {};

                using abs_type = decltype (abs (std::declval<integer<c, zz>> ()));
                // We need this optimization because we use division to convert from hex strings to N.
                if (x.Value == 16) return division<integer<c, zz>, abs_type> {n >> 4, n & integer<c, zz> {4}};
                
                else return math::number::integer_divide (n, x.Value);
            }
        };
        
        template <hex::letter_case zz> struct divide<negativity::nones, zz> {
            division<integer<negativity::nones, zz>> operator ()
                (const integer<negativity::nones, zz> &n, const math::nonzero<integer<negativity::nones, zz>> &x) const {
                if (x.Value == 0) throw math::division_by_zero {} ;
                // We need this optimization because we use division to convert from hex strings to N.
                if (x.Value == 16) return division<integer<negativity::nones, zz>>{
                    n >> 4, n & integer<negativity::nones, zz> {4}};
                
                return math::number::natural_divide (n, x.Value);
            }
        };

        template <negativity n, hex::letter_case zz> struct read_dec_integer {
            integer<n, zz> operator () (const data::string &x) {
                if (decimal::valid (x)) {
                    integer<negativity::nones, zz> z {write<zz> (*decimal::read<endian::little, byte> (x))};
                    return math::number::trim (integer<n, zz> {math::number::extend (z, z.size () + 2)});
                }

                if (signed_decimal::valid (x)) {
                    integer<negativity::nones, zz> z {write<zz> (*decimal::read<endian::little, byte> (x.substr (1)))};
                    return math::number::trim (-integer<n, zz> {math::number::extend (z, z.size () + 2)});
                }

                throw exception {} << "invalid number string: " << x;
            }
        };

        template <hex::letter_case zz> struct read_dec_integer<negativity::nones, zz> {
            integer<negativity::nones, zz> operator () (const data::string &x) {
                auto np = decimal::read<endian::little, byte> (x);
                if (!np) throw exception {} << "invalid number string: " << x;
                return integer<negativity::nones, zz> {write<zz> (*np)};
            }
        };
        
    }
    
    template <negativity c, hex::letter_case zz>
    inline integer<c, zz>::operator double () const {
        return double (bytes_type<endian::little, c>::read (*this));
    }

    template <negativity n, hex::letter_case zz>
    integer<n, zz> integer<n, zz>::read (const std::string &x) {
        if (hexidecimal::valid (x)) return integer<n, zz> {x};
        return read_dec_integer<n, zz> {} (x);
    }
    
}

namespace data::encoding::integer {
    
    template <endian::order r, math::negativity c, std::unsigned_integral word>
    std::ostream inline &write (std::ostream &o, const math::number::Z_bytes<r, c, word> &z) {
        if (sign (z) == math::negative) o << "-";
        return decimal::write (o, abs (z));
    }
    
    template <endian::order r, math::negativity c, std::unsigned_integral word>
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

namespace data::math {

    template <hex_case zz>
    division<hex::uint<zz>, unsigned int> inline
    divide<hex::uint<zz>, int>::operator () (const hex::uint<zz> &d, const nonzero<int> &z) {
        if (z.Value == 16) {
            if (d < 16) return {hex::uint<zz> {"0x"}, static_cast<unsigned int> (uint64 (d))};
            return {hex::uint<zz> {std::string {"0x0"} + d.substr (2, d.size () - 3)}, static_cast<unsigned int> (uint64 (d & 0x0f))};
        }

        auto [quotient, remainder] = divide<Z, Z> {} (Z (d), nonzero<Z> {z.Value});
        return division<hex::uint<zz>, unsigned int> {hex::uint<zz> {data::abs (quotient)}, static_cast<unsigned int> (uint64 (remainder))};
    }

    template <hex_case zz>
    division<hex::int2<zz>, unsigned int> inline
    divide<hex::int2<zz>, int>::operator () (const hex::int2<zz> &d, const nonzero<int> &z) {
        if (z.Value == 16) {
            if (d < 0) {
                auto [quotient, remainder] = division<hex::int2<zz>, int> {} (-d, z);
                return {-quotient - 1, 16 - remainder};
            }

            if (d < 16) return {hex::int2<zz> {"0x"}, static_cast<unsigned int> (int64 (d))};
            return {hex::int2<zz> {std::string {"0x0"} + d.substr (2, d.size () - 3)}, static_cast<unsigned int> (int64 (d & 0x0f))};
        }

        auto [quotient, remainder] = divide<Z, Z> {} (Z (d), nonzero<Z> {z.Value});
        return division<hex::int2<zz>, unsigned int> {hex::int2<zz> {quotient}, static_cast<unsigned int> (uint64 (remainder))};
    }

    template <hex_case zz>
    division<hex::intBC<zz>, int> inline
    divide<hex::intBC<zz>, int>::operator () (const hex::intBC<zz> &d, const nonzero<int> &z) {
        if (z.Value == 16) {
            if (d < 0) {
                auto [quotient, remainder] = division<hex::intBC<zz>, int> {} (-d, z);
                return {-quotient - 1, 16 - remainder};
            }

            if (d < 16) return {hex::intBC<zz> {"0x"}, static_cast<unsigned int> (int64 (d))};
            return {hex::intBC<zz> {std::string {"0x0"} + d.substr (2, d.size () - 3)}, static_cast<int> (int64 (d & 0x0f))};
        }

        auto [quotient, remainder] = divide<Z, Z> {} (Z (d), nonzero<Z> {z.Value});
        return division<hex::intBC<zz>, unsigned int> {hex::intBC<zz> {quotient}, static_cast<unsigned int> (uint64 (remainder))};
    }

    template <endian::order r, std::unsigned_integral word>
    division<N_bytes<r, word>, N_bytes<r, word>> inline
    divide<N_bytes<r, word>, N_bytes<r, word>>::operator ()
        (const N_bytes<r, word> &a, const nonzero<N_bytes<r, word>> &b) {
        return number::natural_divide (a, b.Value);
    }

    template <endian::order r, std::unsigned_integral word>
    division<Z_bytes<r, word>, N_bytes<r, word>> inline
    divide<Z_bytes<r, word>, N_bytes<r, word>>::operator ()
        (const Z_bytes<r, word> &a, const nonzero<N_bytes<r, word>> &b) {
        return number::integer_divide (a, b.Value);
    }

    template <endian::order r, std::unsigned_integral word>
    division<Z_bytes<r, word>, N_bytes<r, word>> inline
    divide<Z_bytes<r, word>, Z_bytes<r, word>>::operator ()
        (const Z_bytes<r, word> &a, const nonzero<Z_bytes<r, word>> &b) {
        return number::integer_divide (a, b.Value);
    }

    template <endian::order r, std::unsigned_integral word>
    division<Z_bytes_BC<r, word>, Z_bytes_BC<r, word>> inline
    divide<Z_bytes_BC<r, word>, Z_bytes_BC<r, word>>::operator ()
        (const Z_bytes_BC<r, word> &a, const nonzero<Z_bytes_BC<r, word>> &b) {
        return number::integer_divide (a, b.Value);
    }
}

#endif
