// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES
#define DATA_MATH_NUMBER_BYTES

#include <data/encoding/integer.hpp>
#include <data/math/number/bytes/Z.hpp>
#include <data/math/number/integer.hpp>

namespace data::math::number {
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline N_bytes<r, word>::read (string_view x) {
        if (!encoding::natural::valid (x)) throw std::invalid_argument {string {"invalid number string "} + string {x}};

        if (encoding::hexidecimal::valid (x)) {
            if (auto m = encoding::hexidecimal::read<r, word> (x); bool (m)) return N_bytes<r, word> {view<word> (*m)};
            else throw std::invalid_argument {string {"invalid hex string size "} + std::to_string (x.size ()) + "; " + string {x}};
        }

        if (encoding::decimal::valid (x)) return N_bytes<r, word> (math::N {x});
        throw std::invalid_argument {string {"invalid number string"} + string {x}};
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, complement::ones, word> inline Z_bytes<r, complement::ones, word>::read (string_view x) {
        if (!encoding::integer::valid (x)) throw std::invalid_argument {string {"invalid number string"} + string {x}};
        if (encoding::hexidecimal::valid (x)) return *encoding::integer::read<r, complement::ones, word> (x);
        return Z_bytes<r, complement::ones, word> (Z {x});
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, complement::twos, word> inline Z_bytes<r, complement::twos, word>::read (string_view x) {
        if (!encoding::integer::valid (x)) throw std::invalid_argument {string {"invalid number string"} + string {x}};
        if (encoding::hexidecimal::valid (x)) return *encoding::integer::read<r, complement::twos, word> (x);
        return Z_bytes<r, complement::twos, word> (Z {x});
    }
    
    template <endian::order r, std::unsigned_integral word>
    std::ostream inline &operator << (std::ostream &o, const N_bytes<r, word> &n) {
        if (o.flags () & std::ios::hex) return encoding::hexidecimal::write (o, n);
        else if (o.flags () & std::ios::dec) return encoding::decimal::write (o, math::N (n));
        return o;
    }
    
    template <endian::order r, complement c, std::unsigned_integral word>
    std::ostream inline &operator << (std::ostream &o, const Z_bytes<r, c, word> &n) {
        if (o.flags () & std::ios::hex) return encoding::hexidecimal::write (o, n);
        if (o.flags () & std::ios::dec) return encoding::signed_decimal::write (o, Z (n));
        return o;
    }
    
    template <endian::order r, std::unsigned_integral word> inline
    N_bytes<r, word>::operator double () const {
        return double (math::N (*this));
    }
    
    template <endian::order r, std::unsigned_integral word> inline
    Z_bytes<r, complement::ones, word>::operator double () const {
        return double (Z (*this));
    }
    
    template <endian::order r, std::unsigned_integral word> inline
    Z_bytes<r, complement::twos, word>::operator double () const {
        return double (Z (*this));
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline operator / (const N_bytes<r, word> &x, const N_bytes<r, word> &j) {
        if (j == 0) throw division_by_zero {};
        return divide<N_bytes<r, word>> {} (x, nonzero<N_bytes<r, word>> {j}).Quotient;
    }
    
    template <endian::order r, complement c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator / (const Z_bytes<r, c, word> &x, const Z_bytes<r, c, word> &j) {
        if (j == 0) throw division_by_zero {};
        return divide<Z_bytes<r, c, word>> {} (x, nonzero<Z_bytes<r, c, word>> {j}).Quotient;
    }
    
    template <endian::order r, complement c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator / (const Z_bytes<r, c, word> &x, const N_bytes<r, word> &j) {
        if (j == 0) throw division_by_zero {};
        return divide<Z_bytes<r, c, word>> {} (x, nonzero<Z_bytes<r, c, word>> {Z_bytes<r, c, word> {j}}).Quotient;
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline operator / (const N_bytes<r, word> &x, uint64 j) {
        if (j == 0) throw division_by_zero {};
        return divide<N_bytes<r, word>> {} (x, nonzero<N_bytes<r, word>> {N_bytes<r, word> {j}}).Quotient;
    }
    
    template <endian::order r, complement c, std::unsigned_integral word>
    Z_bytes<r, c, word> inline operator / (const Z_bytes<r, c, word> &x, int64 j) {
        if (j == 0) throw division_by_zero {};
        return divide<Z_bytes<r, c, word>> {} (x, nonzero<Z_bytes<r, c, word>> {Z_bytes<r, c, word> {j}}).Quotient;
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline operator % (const N_bytes<r, word> &x, const N_bytes<r, word> &j) {
        if (j == 0) throw division_by_zero {};
        return divide<N_bytes<r, word>> {} (x, nonzero<N_bytes<r, word>> {j}).Remainder;
    }
    
    template <endian::order r, std::unsigned_integral word>
    N_bytes<r, word> inline operator % (const Z_bytes<r, complement::ones, word> &x, const N_bytes<r, word> &j) {
        if (j == 0) throw division_by_zero {};
        return divide<Z_bytes<r, complement::ones, word>, N_bytes<r, word>> {}
            (x, nonzero<Z_bytes<r, complement::ones, word>> {Z_bytes<r, complement::ones, word> {j}}).Remainder;
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, complement::twos, word> inline operator %
        (const Z_bytes<r, complement::twos, word> &x, const Z_bytes<r, complement::twos, word> &j) {
        if (j == 0) throw division_by_zero {};
        return divide<Z_bytes<r, complement::twos, word>> {} (x, nonzero<Z_bytes<r, complement::twos, word>> {j}).Remainder;
    }
    
    template <endian::order r, std::unsigned_integral word>
    uint64 inline operator % (const N_bytes<r, word> &x, uint64 j) {
        if (j == 0) throw division_by_zero {};
        return uint64 (divide<N_bytes<r, word>> {} (x, nonzero<N_bytes<r, word>> {N_bytes<r, word> {j}}).Remainder);
    }

    template <endian::order r, complement c, std::unsigned_integral word>
    uint64 inline operator % (const Z_bytes<r, c, word> &x, uint64 j) {
        if (j == 0) throw division_by_zero {};
        return uint64 (divide<Z_bytes<r, c, word>> {} (x, nonzero<Z_bytes<r, c, word>> {Z_bytes<r, c, word> {j}}).Remainder);
    }
    
}

#include <data/encoding/digits.hpp>
#include <data/math/number/integer.hpp>

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
    
    template <endian::order r, math::number::complement n, std::unsigned_integral word>
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
        template <endian::order r, complement c> struct get_bytes_type {
            using value = math::number::Z_bytes<r, c, byte>;
        };
        
        template <endian::order r> struct get_bytes_type<r, complement::nones> {
            using value = math::number::N_bytes<r, byte>;
        };
        
        template <endian::order r, math::number::complement c>
        using bytes_type = get_bytes_type<r, c>::value;
        
        using nat = math::number::GMP::N;
        
        template <hex::letter_case zz>
        inline nat read_num (const integer<complement::nones, zz> &n) {
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
        integer<complement::ones, zz> inline bit_shift (const integer<complement::ones, zz> &x, int i) {
            auto o = read<endian::big, byte> (x);
            math::number::Z_bytes<endian::big, complement::ones, byte> n;
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
        
        template <math::number::complement c, hex::letter_case zz> struct divide {
            math::division<integer<c, zz>> operator () (const integer<c, zz> &n, const integer<c, zz> &x) const {
                if (x == 0) throw math::division_by_zero {};
                // it is important to have this optimization. 
                // I can't say why or I'll be embarrassed. 
                if (x == 16) return math::division<integer<c, zz>> {*this >> 4, *this & integer<c, zz> {4}};
                
                else return math::number::integer_divide (*this, x);
            }
        };
        
        template <hex::letter_case zz> struct divide<complement::nones, zz> {
            math::division<integer<complement::nones, zz>> operator ()
                (const integer<complement::nones, zz> &n, const integer<complement::nones, zz> &x) const {
                if (x == 0) throw math::division_by_zero {} ;
                // it is important to have this optimization. 
                // I can't say why or I'll be embarrassed. 
                if (x == 16) return math::division<integer<complement::nones, zz>>{
                    *this >> 4, *this & integer<complement::nones, zz> {4}};
                
                return math::number::natural_divide (*this, x);
            }
        };

        template <complement n, hex::letter_case zz> struct read_dec_integer {
            integer<n, zz> operator () (const data::string &x) {
                if (decimal::valid (x)) {
                    integer<complement::nones, zz> z {write<zz> (*decimal::read<endian::little, byte> (x))};
                    return math::number::trim (integer<n, zz> {math::number::extend (z, z.size () + 2)});
                }

                if (signed_decimal::valid (x)) {
                    integer<complement::nones, zz> z {write<zz> (*decimal::read<endian::little, byte> (x.substr (1)))};
                    return math::number::trim (-integer<n, zz> {math::number::extend (z, z.size () + 2)});
                }

                throw exception {} << "invalid number string: " << x;
            }
        };

        template <hex::letter_case zz> struct read_dec_integer<complement::nones, zz> {
            integer<complement::nones, zz> operator () (const data::string &x) {
                auto np = decimal::read<endian::little, byte> (x);
                if (!np) throw exception {} << "invalid number string: " << x;
                return integer<complement::nones, zz> {write<zz> (*np)};
            }
        };
        
    }
    
    template <complement c, hex::letter_case zz>
    math::division<integer<c, zz>> integer<c, zz>::divide (const integer<c, zz> &x) const {
        if (x == 0) throw math::division_by_zero {};
        // it is important to have this optimization. 
        // I can't say why or I'll be embarrassed. 
        if (x == 16) return math::division<integer<c, zz>> {*this >> 4, *this & integer<c, zz> {4}};
        
        if constexpr (c == complement::nones) return math::number::natural_divide (*this, x);
        else return math::number::integer_divide<integer<c, zz>, integer<c, zz>> (*this, x);
    }
    
    template <complement c, hex::letter_case zz> 
    inline integer<c, zz>::operator double () const {
        return double (bytes_type<endian::little, c>::read (*this));
    }

    template <complement n, hex::letter_case zz>
    integer<n, zz> integer<n, zz>::read (const std::string &x) {
        if (hexidecimal::valid (x)) return integer<n, zz> {x};
        return read_dec_integer<n, zz> {} (x);
    }
    
}

namespace data::encoding::integer {
    
    template <endian::order r, math::number::complement c, std::unsigned_integral word>
    std::ostream inline &write (std::ostream &o, const math::number::Z_bytes<r, c, word> &z) {
        if (sign (z) == math::negative) o << "-";
        return decimal::write (o, abs (z));
    }
    
    template <endian::order r, math::number::complement c, std::unsigned_integral word>
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

#endif
