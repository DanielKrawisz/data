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

        if (encoding::decimal::valid (x)) return N_bytes<r, word> (N {x});
        throw exception {} << "invalid number string" << x;
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::twos, word> inline Z_bytes<r, neg::twos, word>::read (string_view x) {
        if (!encoding::integer::valid (x)) throw exception {} << "invalid number string \"" << x << "\"";
        if (encoding::hexidecimal::valid (x)) return *encoding::integer::read<r, neg::twos, word> (x);
        return Z_bytes<r, neg::twos, word> (Z {x});
    }
    
    template <endian::order r, std::unsigned_integral word>
    Z_bytes<r, neg::BC, word> inline Z_bytes<r, neg::BC, word>::read (string_view x) {
        if (!encoding::integer::valid (x)) throw exception {} << "invalid number string \"" << x << "\"";
        if (encoding::hexidecimal::valid (x)) return *encoding::integer::read<r, neg::BC, word> (x);
        return Z_bytes<r, neg::BC, word> (Z {x});
    }
    
    template <endian::order r, std::unsigned_integral word>
    std::ostream inline &operator << (std::ostream &o, const N_bytes<r, word> &n) {
        if (o.flags () & std::ios::hex) return encoding::hexidecimal::write (o, n);
        else if (o.flags () & std::ios::dec) return encoding::decimal::write (o, N (n));
        return o;
    }
    
    template <endian::order r, neg c, std::unsigned_integral word>
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
    
    template <endian::order r, neg c, std::unsigned_integral word>
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
        return double (N (*this));
    }
    
    template <endian::order r, std::unsigned_integral word> inline
    Z_bytes<r, neg::twos, word>::operator double () const {
        return double (Z (*this));
    }
    
    template <endian::order r, std::unsigned_integral word> inline
    Z_bytes<r, neg::BC, word>::operator double () const {
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
    
    template <endian::order r, neg n, std::unsigned_integral word>
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

    template <hex::letter_case zz>
    string<zz> shift (const string<zz> &x, int i) {
        auto o = read<endian::big, byte> (x);
        math::number::N_bytes<endian::big, byte> n;
        n.resize (o->size ());
        std::copy (o->begin (), o->end (), n.begin ());
        n = n << i;
        return write<zz> (math::number::extend (n, n.size () + 1));
    }
    
    namespace {
        template <endian::order r, neg c> struct get_bytes_type {
            using value = math::number::Z_bytes<r, c, byte>;
        };
        
        template <endian::order r> struct get_bytes_type<r, neg::nones> {
            using value = math::number::N_bytes<r, byte>;
        };
        
        template <endian::order r, neg c>
        using bytes_type = get_bytes_type<r, c>::value;
        
        using nat = math::number::GMP::N;
        
        template <hex::letter_case zz>
        inline nat read_num (const integer<neg::nones, zz> &n) {
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
        integer<neg::twos, zz> inline bit_shift (const integer<neg::twos, zz> &x, int i) {
            auto o = read<endian::big, byte> (x);
            math::number::Z_bytes<endian::big, neg::twos, byte> n;
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
            
            Z remainder = 0;
            int io_max = out.size () - 2;
            
            for (int io = 0; io < io_max; io++) {
                Z total = remainder;
                int ia_min = std::max (0, io - b_max);
                int ib_min = std::max (0, io - a_max);
                int ia_max = io - ib_min;
                int ib_max = io - ia_min;
                for (int ia = ia_min; ia <= ia_max; ia++) {
                    int ib = ib_max + ia_min - ia; 
                    Z next (int (digit (a[a.size () - 1 - ia])) * int (digit (b[b.size () - 1 - ib])));
                    total += next;
                }
                
                out[out.size () - 1 - io] = characters[total % 16];
                remainder = total >> 4;
            }
        }

        template <neg c, hex::letter_case zz> struct divide {
            // if c is twos, nn must be either twos or nones.
            // if c is BC, nn must be BC.
            template <neg nn>
            division<integer<c, zz>, decltype (abs (std::declval<integer<c, zz>> ()))>
            operator () (const integer<c, zz> &n, const math::nonzero<integer<nn, zz>> &x) const {
                if (x.Value == 0) throw math::division_by_zero {};

                using abs_type = decltype (abs (std::declval<integer<c, zz>> ()));
                // We need this optimization because we use division to convert from hex strings to N.
                if (x.Value == 16) return division<integer<c, zz>, abs_type> {n >> 4, n & integer<c, zz> {4}};
                
                else return math::number::integer_divmod (n, x.Value);
            }
        };
        
        template <hex::letter_case zz> struct divide<neg::nones, zz> {
            division<integer<neg::nones, zz>> operator ()
                (const integer<neg::nones, zz> &n, const math::nonzero<integer<neg::nones, zz>> &x) const {
                if (x.Value == 0) throw math::division_by_zero {} ;
                // We need this optimization because we use division to convert from hex strings to N.
                if (x.Value == 16) return division<integer<neg::nones, zz>>{
                    n >> 4, n & integer<neg::nones, zz> {4}};
                
                return math::number::natural_divmod (n, x.Value);
            }
        };

        template <neg n, hex::letter_case zz> struct read_dec_integer {
            integer<n, zz> operator () (string_view x) {
                if (decimal::valid (x)) {
                    integer<neg::nones, zz> z {write<zz> (*decimal::read<endian::little, byte> (x))};
                    return math::number::trim (integer<n, zz> {math::number::extend (z, z.size () + 2)});
                }

                if (signed_decimal::valid (x)) {
                    integer<neg::nones, zz> z {write<zz> (*decimal::read<endian::little, byte> (x.substr (1)))};
                    return math::number::trim (-integer<n, zz> {math::number::extend (z, z.size () + 2)});
                }

                throw exception {} << "invalid number string: " << x;
            }
        };

        template <hex::letter_case zz> struct read_dec_integer<neg::nones, zz> {
            integer<neg::nones, zz> operator () (string_view x) {
                auto np = decimal::read<endian::little, byte> (x);
                if (!np) throw exception {} << "invalid number string: " << x;
                return integer<neg::nones, zz> {write<zz> (*np)};
            }
        };
        
    }
    
    template <neg c, hex::letter_case zz>
    inline integer<c, zz>::operator double () const {
        return double (bytes_type<endian::little, c>::read (*this));
    }

    template <neg n, hex::letter_case zz>
    integer<n, zz> integer<n, zz>::read (string_view x) {
        if (hexidecimal::valid (x)) return integer<n, zz> {x};
        return read_dec_integer<n, zz> {} (x);
    }
    
}

namespace data::encoding::integer {
    
    template <endian::order r, neg c, std::unsigned_integral word>
    std::ostream inline &write (std::ostream &o, const math::number::Z_bytes<r, c, word> &z) {
        if (sign (z) == negative) o << "-";
        return decimal::write (o, abs (z));
    }
    
    template <endian::order r, neg c, std::unsigned_integral word>
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

namespace data::math::def {

    template <hex_case zz>
    division<hex::uint<zz>, unsigned int> inline
    divmod<hex::uint<zz>, int>::operator () (const hex::uint<zz> &d, const nonzero<int> &z) {
        if (z.Value == 16) {
            if (d < 16) return {hex::uint<zz> {"0x"}, static_cast<unsigned int> (uint64 (d))};
            return {hex::uint<zz> {string::write ("0x0", d.substr (2, d.size () - 3))}, static_cast<unsigned int> (uint64 (d & 0x0fu))};
        }

        auto [quotient, remainder] = divmod<Z, Z> {} (Z (d), nonzero<Z> {z.Value});
        return division<hex::uint<zz>, unsigned int> {hex::uint<zz> {data::abs (quotient)}, static_cast<unsigned int> (uint64 (remainder))};
    }

    template <hex_case zz>
    division<hex::int2<zz>, unsigned int> inline
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
    division<hex::intBC<zz>, int> inline
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

    template <endian::order r, std::unsigned_integral word>
    division<N_bytes<r, word>, N_bytes<r, word>> inline
    divmod<N_bytes<r, word>, N_bytes<r, word>>::operator ()
        (const N_bytes<r, word> &a, const nonzero<N_bytes<r, word>> &b) {
        return math::number::natural_divmod (a, b.Value);
    }

    template <endian::order r, std::unsigned_integral word>
    division<Z_bytes<r, word>, N_bytes<r, word>> inline
    divmod<Z_bytes<r, word>, N_bytes<r, word>>::operator ()
        (const Z_bytes<r, word> &a, const nonzero<N_bytes<r, word>> &b) {
        return math::number::integer_natural_divmod (a, b.Value);
    }

    template <endian::order r, std::unsigned_integral word>
    division<Z_bytes<r, word>, N_bytes<r, word>> inline
    divmod<Z_bytes<r, word>, Z_bytes<r, word>>::operator ()
        (const Z_bytes<r, word> &a, const nonzero<Z_bytes<r, word>> &b) {
        return math::number::integer_divmod (a, b.Value);
    }

    template <endian::order r, std::unsigned_integral word>
    division<Z_bytes_BC<r, word>, Z_bytes_BC<r, word>> inline
    divmod<Z_bytes_BC<r, word>, Z_bytes_BC<r, word>>::operator ()
        (const Z_bytes_BC<r, word> &a, const nonzero<Z_bytes_BC<r, word>> &b) {
        return math::number::integer_divmod (a, b.Value);
    }
}

#endif
