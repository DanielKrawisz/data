// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES
#define DATA_MATH_NUMBER_BYTES

#include <data/encoding/integer.hpp>
#include <data/math/number/bytes/Z.hpp>
#include <data/math/number/integer.hpp>

namespace data::math::number {
    
    template <endian::order r> N_bytes<r> inline N_bytes<r>::read (string_view x) {
        if (!encoding::natural::valid (x)) throw std::invalid_argument {string {"invalid number string"} + string {x}};
        if (encoding::hexidecimal::valid (x)) return *encoding::natural::read<r> (x);
        if (encoding::decimal::valid (x)) return N_bytes<r> (N::read (x));
        throw std::invalid_argument {string {"invalid number string"} + string {x}};
    }
    
    template <endian::order r> Z_bytes<r, complement::ones> inline Z_bytes<r, complement::ones>::read (string_view x) {
        if (!encoding::integer::valid (x)) throw std::invalid_argument {string {"invalid number string"} + string {x}};
        if (encoding::hexidecimal::valid (x)) return *encoding::integer::read<r, complement::ones> (x);
        return Z_bytes<r, complement::ones> (Z::read (x));
    }
    
    template <endian::order r> Z_bytes<r, complement::twos> inline Z_bytes<r, complement::twos>::read (string_view x) {
        if (!encoding::integer::valid (x)) throw std::invalid_argument {string {"invalid number string"} + string {x}};
        if (encoding::hexidecimal::valid (x)) return *encoding::integer::read<r, complement::twos> (x);
        return Z_bytes<r, complement::twos> (Z::read (x));
    }
    
    template <endian::order r> 
    std::ostream inline &operator << (std::ostream &o, const N_bytes<r>& n) {
        if (o.flags () & std::ios::hex) return encoding::hexidecimal::write (o, n);
        else if (o.flags () & std::ios::dec) return encoding::decimal::write (o, N (n));
        return o;
    }
    
    template <endian::order r, complement c> 
    std::ostream inline &operator << (std::ostream &o, const Z_bytes<r, c>& n) {
        if (o.flags () & std::ios::hex) return encoding::hexidecimal::write (o, n);
        if (o.flags () & std::ios::dec) return encoding::signed_decimal::write (o, Z (n));
        return o;
    }
    
    template <endian::order r> inline N_bytes<r>::operator double () const {
        return double (N (*this));
    }
    
    template <endian::order r> inline Z_bytes<r, complement::ones>::operator double () const {
        return double (Z (*this));
    }
    
    template <endian::order r> inline Z_bytes<r, complement::twos>::operator double () const {
        return double (Z (*this));
    }
    
    template <endian::order r> N_bytes<r> inline operator / (const N_bytes<r> &x, const N_bytes<r> &j) {
        return natural::divide<N_bytes<r>> (x, j).Quotient;
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> inline operator / (const Z_bytes<r, c> &x, const Z_bytes<r, c> &j) {
        return integer::divide<Z_bytes<r, c>> (x, j).Quotient;
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> inline operator / (const Z_bytes<r, c> &x, const N_bytes<r> &j) {
        return integer::divide<Z_bytes<r, c>> (x, Z_bytes<r, c> {j}).Quotient;
    }
    
    template <endian::order r> N_bytes<r> inline operator / (const N_bytes<r> &x, uint64 j) {
        return integer::divide<N_bytes<r>> (x, N_bytes<r> {j}).Quotient;
    }
    
    template <endian::order r, complement c> Z_bytes<r, c> inline operator / (const Z_bytes<r, c> &x, int64 j) {
        return integer::divide<Z_bytes<r, c>> (x, Z_bytes<r, c> {j}).Quotient;
    }
    
    template <endian::order r> N_bytes<r> inline operator % (const N_bytes<r> &x, const N_bytes<r> &j) {
        return natural::divide<N_bytes<r>> (x, j).Remainder;
    }
    
    template <endian::order r> N_bytes<r> inline operator % (const Z_bytes<r, complement::ones> &x, const N_bytes<r> &j) {
        return integer::divide<Z_bytes<r, complement::ones>, N_bytes<r>> (x, Z_bytes<r, complement::ones> {j}).Remainder;
    }
    
    template <endian::order r> Z_bytes<r, complement::twos> inline operator %
        (const Z_bytes<r, complement::twos> &x, const Z_bytes<r, complement::twos> &j) {
        return integer::divide<Z_bytes<r, complement::twos>> (x, j).Remainder;
    }
    
    template <endian::order r> uint64 inline operator % (const N_bytes<r> &x, uint64 j) {
        return uint64(natural::divide<N_bytes<r>> (x, N_bytes<r> {j}).Remainder);
    }
    
    template <endian::order r, complement c> uint64 inline operator % (const Z_bytes<r, c> &x, uint64 j) {
        return uint64(integer::divide<Z_bytes<r, c>> (x, Z_bytes<r, c> {j}).Remainder);
    }
    
}

#include <data/encoding/digits.hpp>
#include <data/math/number/integer.hpp>

namespace data::encoding::decimal {
    
    template <endian::order r> 
    ptr<math::number::N_bytes<r>> read (string_view s) {
        if (!valid (s)) return nullptr;
        
        return std::make_shared<math::number::N_bytes<r>> (read_base<math::number::N_bytes<r>> (s, 10, digit));
    }
    
    template <endian::order r> 
    string inline write (const math::number::N_bytes<r> &n) {
        return string {write_base (math::number::GMP::N (n), characters ())};
    }
}

namespace data::encoding::signed_decimal {
    
    template <endian::order r, math::number::complement n> 
    ptr<math::number::Z_bytes<r, n>> read (string_view s) {
        if (!valid (s)) return nullptr;
        bool negative = s[0] == '-';
        string_view positive = negative ? s.substr (1) : s;
        auto z = std::make_shared<math::number::Z_bytes<r, n>> ((math::number::Z_bytes<r, n>) (*decimal::read<r> (positive)));
        if (negative) *z = -*z;
        return z;
    }
    
}

namespace data::encoding::natural {
    
    template <endian::order r> 
    ptr<math::number::N_bytes<r>> inline read (string_view s) {
        if (!valid (s)) return nullptr;
        if (hexidecimal::valid (s)) {
            auto p = hexidecimal::read<r> (s);
            if (p == nullptr) return nullptr;
            return std::make_shared<math::number::N_bytes<r>> (math::number::N_bytes<r>::read (bytes_view (*p)));
        }
        
        return decimal::read<r> (s);
    }
    
}

namespace data::encoding::hexidecimal {
    
    namespace {
        template <endian::order r, complement c> struct get_bytes_type {
            using value = math::number::Z_bytes<r, c>;
        };
        
        template <endian::order r> struct get_bytes_type<r, complement::nones> {
            using value = math::number::N_bytes<r>;
        };
        
        template <endian::order r, math::number::complement c> using bytes_type = get_bytes_type<r, c>::value;
        
        using nat = math::number::GMP::N;
        
        template <hex::letter_case zz>
        inline nat read_num (const integer<complement::nones, zz>& n) {
            return read_base<nat> (n.substr (2), 16, &digit);
        } 
        
        template <typename N>
        std::string write_hexidecimal (const N& n) {
            static std::string Characters = hex::characters_lower ();
            std::string p = write_base<N> (n, Characters);
            if ((p.size () % 2) == 1) return std::string {"0x0"} + p;
            return std::string {"0x"} + p;
        }
        
        template <hex::letter_case zz> 
        string<zz> shift (const string<zz> &x, int i) {
            auto o = read<endian::big> (x);
            math::number::N_bytes<endian::big> n;
            n.resize (o->size ());
            std::copy (o->begin (), o->end (), n.begin ());
            n = n << i;
            return write<zz> (math::number::extend (n, n.size () + 1));
        }
        
        template <hex::letter_case zz> 
        integer<complement::ones, zz> inline bit_shift (const integer<complement::ones, zz> &x, int i) {
            auto o = read<endian::big> (x);
            math::number::Z_bytes<endian::big, complement::ones> n;
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
                
                else return math::number::integer::divide (*this, x);
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
                
                return math::number::natural::divide (*this, x);
            }
        };
        
    }
    
    template <complement c, hex::letter_case zz>
    math::division<integer<c, zz>> integer<c, zz>::divide (const integer<c, zz> &x) const {
        if (x == 0) throw math::division_by_zero {};
        // it is important to have this optimization. 
        // I can't say why or I'll be embarrassed. 
        if (x == 16) return math::division<integer<c, zz>> {*this >> 4, *this & integer<c, zz> {4}};
        
        if constexpr (c == complement::nones) return math::number::natural::divide (*this, x);
        else return math::number::integer::divide (*this, x);
    }
    
    template <complement c, hex::letter_case zz> 
    inline integer<c, zz>::operator double () const {
        return double (bytes_type<endian::little, c>::read (*this));
    }
    
}

namespace data::encoding::integer {
    
    template <endian::order r, math::number::complement c> 
    std::ostream inline &write (std::ostream &o, const math::number::Z_bytes<r, c>& z) {
        if (sign (z) == math::negative) o << "-";
        return decimal::write (o, abs (z));
    }
    
    template <endian::order r, math::number::complement c> 
    string inline write (const math::number::Z_bytes<r, c> &z) {
        std::stringstream ss;
        write (ss, z);
        return ss.str ();
    }
    
    template <endian::order r> 
    std::ostream inline &write (std::ostream &o, const math::number::N_bytes<r>& z) {
        return decimal::write (o, z);
    }
    
    template <endian::order r> 
    string inline write (const math::number::N_bytes<r> &z) {
        std::stringstream ss;
        write (ss, z);
        return ss.str ();
    }
    
}

#endif
