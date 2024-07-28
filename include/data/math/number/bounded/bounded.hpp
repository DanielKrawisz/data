// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BOUNDED_BOUNDED
#define DATA_MATH_NUMBER_BOUNDED_BOUNDED

#include <type_traits>
#include <data/math/group.hpp>
#include <data/math/abs.hpp>
#include <data/math/sign.hpp>
#include <data/math/division.hpp>
#include <data/math/number/bytes/Z.hpp>
#include <data/math/number/gmp/Z.hpp>

namespace data::math::number {
    
    // satisfies range<byte>
    template <bool u, endian::order, size_t size> requires (size >= 8) struct bounded;
    
    template <bool u, endian::order r, size_t x>
    bounded<u, r, x> operator ~ (const bounded<u, r, x> &);
    
    template <bool u, endian::order r, size_t x>
    bounded<u, r, x> operator ^ (const bounded<u, r, x> &, const bounded<u, r, x> &);
    
    template <bool u, endian::order r, size_t x>
    bounded<u, r, x> operator & (const bounded<u, r, x> &, const bounded<u, r, x> &);
    
    template <bool u, endian::order r, size_t x>
    bounded<u, r, x> operator | (const bounded<u, r, x> &, const bounded<u, r, x> &);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> &operator &= (bounded<u, r, size> &, const bounded<u, r, size> &);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> &operator |= (bounded<u, r, size> &, const bounded<u, r, size> &);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> &operator ^= (bounded<u, r, size> &, const bounded<u, r, size> &);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> operator << (const bounded<u, r, size> &, int);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> operator >> (const bounded<u, r, size> &, int);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> &operator <<= (bounded<u, r, size> &, int);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> &operator >>= (bounded<u, r, size> &, int);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> operator ++ (bounded<u, r, size> &, int);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> &operator ++ (bounded<u, r, size> &);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> operator -- (bounded<u, r, size> &, int);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> &operator -- (bounded<u, r, size> &);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> operator + (const bounded<u, r, size> &, const bounded<u, r, size> &);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> &operator += (bounded<u, r, size> &, const bounded<u, r, size> &);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> operator - (const bounded<u, r, size> &, const bounded<u, r, size> &);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> &operator -= (bounded<u, r, size> &, const bounded<u, r, size> &);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> operator * (const bounded<u, r, size> &, const bounded<u, r, size> &);
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> &operator *= (bounded<u, r, size> &, const bounded<u, r, size> &);
}

namespace data::math { 
    
    template <endian::order r, size_t x> using uint = number::bounded<false, r, x>;
    template <endian::order r, size_t x> using sint = number::bounded<true, r, x>;
    
    template <size_t size>
    using uint_little = typename number::bounded<false, endian::little, size>;
    
    template <size_t size>
    using uint_big = typename number::bounded<false, endian::big, size>;
    
    template <size_t size>
    using int_little = typename number::bounded<true, endian::little, size>;
    
    template <size_t size>
    using int_big = typename number::bounded<true, endian::big, size>;

    // plus and times are associative and commutative
    // for both bounded and bytes. 
    template <bool is_signed, endian::order r, size_t size> 
    struct commutative<plus<number::bounded<is_signed, r, size>>, 
        number::bounded<is_signed, r, size>> {};
    
    template <bool is_signed, endian::order r, size_t size> 
    struct associative<plus<number::bounded<is_signed, r, size>>, 
        number::bounded<is_signed, r, size>> {};
    
    template <bool is_signed, endian::order r, size_t size> 
    struct commutative<times<number::bounded<is_signed, r, size>>, 
        number::bounded<is_signed, r, size>> {};
    
    template <bool is_signed, endian::order r, size_t size> 
    struct associative<times<number::bounded<is_signed, r, size>>, 
        number::bounded<is_signed, r, size>> {};

    template <bool is_signed, endian::order r, size_t size>
    struct times<number::bounded<is_signed, r, size>> {
        number::bounded<is_signed, r, size> operator ()
        (const number::bounded<is_signed, r, size> &a, const number::bounded<is_signed, r, size> &b);

        nonzero<number::bounded<is_signed, r, size>> operator ()
        (const nonzero<number::bounded<is_signed, r, size>> &a, const nonzero<number::bounded<is_signed, r, size>> &b);
    };
    
    template <bool u, endian::order r, size_t x> struct first<number::bounded<u, r, x>> {
        number::bounded<u, r, x> operator () ();
    };
    
    template <bool u, endian::order r, size_t x> 
    struct inverse<plus<number::bounded<u, r, x>>, number::bounded<u, r, x>> {
        number::bounded<u, r, x> operator () (const number::bounded<u, r, x> &a, const number::bounded<u, r, x> &b);
    };
    
    template <endian::order r, size_t x> struct abs<uint<r, x>> {
        uint<r, x> operator () (const uint<r, x> &);
    };
    
    template <endian::order r, size_t x> struct abs<sint<r, x>> {
        uint<r, x> operator () (const sint<r, x> &);
    };
    
    template <endian::order r, size_t x> struct quadrance<uint<r, x>> {
        uint<r, x> operator () (const uint<r, x> &);
    };
    
    template <endian::order r, size_t x> struct quadrance<sint<r, x>> {
        uint<r, x> operator () (const sint<r, x> &);
    };

    template <endian::order r, size_t x> struct sign<uint<r, x>> {
        signature operator () (const uint<r, x> &);
    };

    template <endian::order r, size_t x> struct sign<sint<r, x>> {
        signature operator () (const sint<r, x> &);
    };
    
    template <bool u, endian::order r, size_t x> struct is_zero<number::bounded<u, r, x>> {
        bool operator () (const number::bounded<u, r, x> &);
    };
    
    template <endian::order r, size_t x> struct is_negative<uint<r, x>> {
        bool operator () (const uint<r, x> &);
    };

    template <endian::order r, size_t x> struct is_negative<sint<r, x>> {
        bool operator () (const sint<r, x> &);
    };
    
    template <endian::order r, size_t x> struct is_positive<uint<r, x>> {
        bool operator () (const uint<r, x> &);
    };

    template <endian::order r, size_t x> struct is_positive<sint<r, x>> {
        bool operator () (const sint<r, x> &);
    };

    template <endian::order r, size_t x> struct divide<uint<r, x>, uint<r, x>> {
        division<uint<r, x>, uint<r, x>> operator () (const uint<r, x> &, const nonzero<uint<r, x>> &);
    };

    template <endian::order r, size_t x> struct divide<sint<r, x>, sint<r, x>> {
        division<sint<r, x>, uint<r, x>> operator () (const sint<r, x> &, const nonzero<sint<r, x>> &);
    };

    template <endian::order r, size_t x> struct divide<sint<r, x>, uint<r, x>> {
        division<sint<r, x>, uint<r, x>> operator () (const sint<r, x> &, const nonzero<uint<r, x>> &);
    };

}

namespace data::encoding::decimal {

    struct string;

    template <bool u, endian::order r, size_t x>
    string write (const math::uint<r, x> &);

    template <bool u, endian::order r, size_t x>
    std::ostream inline &write (std::ostream &o, const math::uint<r, x> &n) {
        return o << write (n);
    }

}

namespace data::encoding::signed_decimal {

    struct string;

    template <bool u, endian::order r, size_t x>
    string write (const math::sint<r, x> &);

    template <bool u, endian::order r, size_t x>
    std::ostream &write (std::ostream &o, const math::sint<r, x> &z) {
        return o << write (z);
    }

}

namespace data::encoding::hexidecimal {

    template <endian::order r, size_t x>
    std::ostream &write (std::ostream &, const oriented<r, byte, x> &, hex_case q = hex_case::lower);

    template <hex_case zz> struct string;

    template <hex_case zz, endian::order r, size_t x>
    string<zz> write (const oriented<r, byte, x> &);

}


namespace data::math::number {

    template <endian::order r, size_t x> struct increment<uint<r, x>> {
        nonzero<uint<r, x>> operator () (const uint<r, x> &);
    };

    template <endian::order r, size_t x> struct decrement<uint<r, x>> {
        uint<r, x> operator () (const nonzero<uint<r, x>> &);
        uint<r, x> operator () (const uint<r, x> &);
    };

    template <endian::order r, size_t x> struct increment<sint<r, x>> {
        sint<r, x> operator () (const sint<r, x> &);
    };

    template <endian::order r, size_t x> struct decrement<sint<r, x>> {
        sint<r, x> operator () (const sint<r, x> &);
    };
    
    template <data::endian::order r, size_t x>
    std::ostream &operator << (std::ostream &s, const uint<r, x> &n);

    template <data::endian::order r, size_t x>
    std::ostream &operator << (std::ostream &s, const sint<r, x> &n);
    
    template <bool x, endian::order r, size_t n, bool y, endian::order o, size_t z>
    bool operator == (const bounded<x, r, n> &, const bounded<y, o, z> &);
    
    template <endian::order r, size_t size>
    std::strong_ordering operator <=> (const sint<r, size> &, const sint<r, size> &);
    
    template <endian::order r, size_t size>
    std::strong_ordering operator <=> (const uint<r, size> &, const uint<r, size> &);
    
    template <bool x, endian::order r, size_t n, bool y, endian::order o, size_t z>
    std::strong_ordering operator <=> (const bounded<x, r, n> &, const bounded<y, o, z> &);
    
    template <bool x, endian::order r, size_t n, bool y, endian::order o, size_t z>
    bool operator == (const bounded<x, r, n> &, const endian::arithmetic<y, o, z> &);
    
    template <bool x, endian::order r, size_t n, bool y, endian::order o, size_t z>
    std::strong_ordering operator <=> (const bounded<x, r, n> &, const endian::arithmetic<y, o, z> &);
    
    template <endian::order r, size_t size, endian::order o>
    bool operator == (const sint<r, size> &, const Z_bytes<o, complement::ones> &);
    
    template <endian::order r, size_t size, endian::order o>
    std::weak_ordering operator <=> (const sint<r, size> &, const Z_bytes<o, complement::ones> &);
    
    template <endian::order r, size_t size, endian::order o>
    bool operator == (const uint<r, size> &, const N_bytes<o> &);
    
    template <endian::order r, size_t size, endian::order o>
    std::weak_ordering operator <=> (const uint<r, size> &, const N_bytes<o> &);
    
    template <endian::order r, size_t size>
    bool operator == (const sint<r, size> &, const Z &);
    
    template <endian::order r, size_t size>
    std::weak_ordering operator <=> (const sint<r, size> &, const Z &);
    
    template <endian::order r, size_t size>
    bool operator == (const uint<r, size> &, const math::N &);
    
    template <endian::order r, size_t size>
    std::weak_ordering operator <=> (const uint<r, size> &, const math::N &);

    template <endian::order r, size_t x> std::weak_ordering inline operator <=> (const uint<r, x> &a, int64 b);

    template <endian::order r, size_t x> std::weak_ordering inline operator <=> (const sint<r, x> &a, int64 b);
    
    template <endian::order r, size_t size>
    sint<r, size> operator | (const sint<r, size> &, const uint<r, size> &);

    template <endian::order r, size_t size>
    uint<r, size> operator | (const uint<r, size> &, uint64);
    
    template <endian::order r, size_t size>
    sint<r, size> operator & (const sint<r, size> &, const uint<r, size> &);

    template <endian::order r, size_t size>
    uint<r, size> operator & (const uint<r, size> &, uint64);
    
    template <endian::order r, size_t size>
    sint<r, size> operator ^ (const sint<r, size> &, const uint<r, size> &);
    
    template <endian::order r, size_t size>
    sint<r, size> operator + (const sint<r, size> &, const uint<r, size> &);
    
    template <endian::order r, size_t size>
    sint<r, size> operator - (const sint<r, size> &, const uint<r, size> &);
    
    template <endian::order r, size_t size>
    sint<r, size> operator * (const sint<r, size> &, const uint<r, size> &);
    
    template <endian::order r, size_t size>
    sint<r, size> operator | (const uint<r, size> &, const sint<r, size> &);
    
    template <endian::order r, size_t size>
    sint<r, size> operator & (const uint<r, size> &, const sint<r, size> &);
    
    template <endian::order r, size_t size>
    sint<r, size> operator ^ (const uint<r, size> &, const sint<r, size> &);
    
    template <endian::order r, size_t size>
    sint<r, size> operator + (const uint<r, size> &, const sint<r, size> &);
    
    template <endian::order r, size_t size>
    sint<r, size> operator - (const uint<r, size> &, const sint<r, size> &);
    
    template <endian::order r, size_t size>
    sint<r, size> operator * (const uint<r, size> &, const sint<r, size> &);
    
    template <bool u, endian::order r, size_t x>
    sint<r, x> operator - (const bounded<u, r, x> &);
    
    template <endian::order r, size_t x>
    uint<r, x> operator + (const uint<r, x> &, uint64);
    
    template <endian::order r, size_t x>
    uint<r, x> operator - (const uint<r, x> &, uint64);
    
    template <endian::order r, size_t x>
    uint<r, x> operator * (const uint<r, x> &, uint64);
    
    template <endian::order r, size_t x>
    sint<r, x> operator + (const sint<r, x> &, int64);
    
    template <endian::order r, size_t x>
    sint<r, x> operator - (const sint<r, x> &, int64);
    
    template <endian::order r, size_t x>
    sint<r, x> operator * (const sint<r, x> &, int64);
    
    template <bool u, endian::order r, size_t x> bounded<u, r, x> operator / (const bounded<u, r, x> &, const bounded<u, r, x> &);
    
    template <endian::order r, size_t x> uint<r, x> operator / (const uint<r, x> &, uint64);
    template <endian::order r, size_t x> sint<r, x> operator / (const sint<r, x> &, int64);
    
    template <endian::order r, size_t x> uint<r, x> operator % (const uint<r, x> &, const uint<r, x> &);
    template <endian::order r, size_t x> uint<r, x> operator % (const sint<r, x> &, const uint<r, x> &);
    
    template <endian::order r, size_t x> uint64 operator % (const uint<r, x> &, uint64);
    template <endian::order r, size_t x> uint64 operator % (const sint<r, x> &, uint64);
    
    template <endian::order r, size_t x> uint<r, x> &operator += (uint<r, x> &, uint64);
    template <endian::order r, size_t x> sint<r, x> &operator += (sint<r, x> &, int64);
    
    template <endian::order r, size_t x> uint<r, x> &operator -= (uint<r, x> &, uint64);
    template <endian::order r, size_t x> sint<r, x> &operator -= (sint<r, x> &, int64);
    
    template <endian::order r, size_t x> uint<r, x> &operator *= (uint<r, x> &, uint64);
    template <endian::order r, size_t x> sint<r, x> &operator *= (sint<r, x> &, int64);
    
    template <endian::order r, size_t x> uint<r, x> &operator /= (uint<r, x> &, uint64);
    template <endian::order r, size_t x> sint<r, x> &operator /= (sint<r, x> &, int64);

    template <endian::order r, size_t size>
    struct bounded<false, r, size> : oriented<r, byte, size> {
        
        bounded () : oriented<r, byte, size> () {
            this->fill (0x00);
        }
        
        bounded (const uint64 x);
        
        bounded (const byte_array<size> &);
        
        explicit bounded (slice<byte, size> x);
        
        // The string can be a hex string or a representation of a number. 
        explicit bounded (const string &s) : bounded {read (s)} {}
        static bounded read (string_view);
        
        math::division<bounded> divide (const bounded&) const;
        
        static bounded max ();
        static bounded min ();
        static N_bytes<r> modulus ();
        
        operator N_bytes<r> () const;
        
        explicit operator math::N () const {
            return math::N (N_bytes<r> (*this));
        }
        
        explicit operator double () const;
        explicit operator bounded<true, r, size> () const;

        explicit bounded (const math::N &n): bounded {N_bytes<r> (n)} {}
        explicit bounded (const N_bytes<r>& n) : bounded {} {
            if (n.size () <= size) std::copy (n.words ().begin (), n.words ().end (), this->words ().begin ());
            else if (N_bytes<r> (n) <= N_bytes<r> {max ()}) std::copy (n.words ().begin (), n.words ().begin () + size, this->begin ());
            else throw std::invalid_argument {"N_bytes too big"};
        }

        explicit operator uint64 () const;
        
    private:
        bounded (const sint<r, size> &);
        
    };
    
    template <endian::order r, size_t size>
    struct bounded<true, r, size> : oriented<r, byte, size> {
        
        bounded () : oriented<r, byte, size> {} {
            this->fill (0);
        }
        
        bounded (const int64 x);
        
        bounded (const byte_array<size> &x) : oriented<r, byte, size> {x} {}
        
        bounded (const bounded<false, r, size> &);
        
        explicit bounded (string_view s) : bounded {read (s)} {}
        static bounded read (string_view);

        math::division<bounded> divide (const bounded &) const;
        
        operator Z_bytes<r, complement::ones> () const;
        
        explicit operator Z () const {
            return Z (Z_bytes<r, complement::ones> (*this));
        }
        
        explicit bounded (slice<byte, size>);
        
        explicit operator double () const;
        
        static bounded max ();
        static bounded min ();

        explicit operator int64 () const;
        
        explicit bounded (const Z_bytes<r, complement::ones> &z) {
            if (z.size () <= size) {
                std::copy (z.words ().begin (), z.words ().end (), this->words ().begin ());
                char leading = data::is_negative (z) ? 0xff : 0x00;
                for (int i = z.size (); i < size; i++) this->words ()[i] = leading;
            } else if (z <= Z_bytes<r, complement::ones> {max ()} && z >= Z_bytes<r, complement::ones> {min ()})
                std::copy (z.words ().begin (), z.words ().begin () + size, this->begin ());
            else throw std::invalid_argument {"Z_bytes too big"};
        }
        
    };
    
    template <bool u, endian::order r, size_t x>
    bounded<u, r, x> inline operator ~ (const bounded<u, r, x> &n) {
        auto z = n;
        z.bit_negate ();
        return z;
    }
    
    template <bool x, endian::order r, size_t n, bool y, endian::order o, size_t z>
    bool inline operator == (const bounded<x, r, n> &a, const bounded<y, o, z> &b) {
        return (a <=> b) == 0;
    }
    
    template <bool x, endian::order r, size_t n, bool y, endian::order o, size_t z>
    bool inline operator == (const bounded<x, r, n> &a, const endian::arithmetic<y, o, z> &b) {
        return (a <=> b) == 0;
    }
    
    template <endian::order r, size_t size, endian::order o>
    bool inline operator == (const sint<r, size> &a, const Z_bytes<o, complement::ones> &b) {
        return (a <=> b) == 0;
    }
    
    template <endian::order r, size_t size, endian::order o>
    bool inline operator == (const uint<r, size> &a, const N_bytes<o> &b) {
        return (a <=> b) == 0;
    }
    
    template <endian::order r, size_t size>
    bool inline operator == (const sint<r, size> &a, const Z &b) {
        return (a <=> b) == 0;
    }
    
    template <endian::order r, size_t size>
    bool inline operator == (const uint<r, size> &a, const math::N &b) {
        return (a <=> b) == 0;
    }
    
    template <endian::order r, size_t size, endian::order o>
    std::weak_ordering inline operator <=> (const sint<r, size> &a, const Z_bytes<o, complement::ones> &b) {
        return Z_bytes<r, complement::ones> (a) <=> b;
    }
    
    template <endian::order r, size_t size, endian::order o>
    std::weak_ordering inline operator <=> (const uint<r, size> &a, const N_bytes<o> &b) {
        return Z_bytes<r, complement::ones> (a) <=> b;
    }
    
    template <endian::order r, size_t size>
    std::weak_ordering inline operator <=> (const sint<r, size> &a, const Z &b) {
        return Z (a) <=> b;
    }
    
    template <endian::order r, size_t size>
    std::weak_ordering inline operator <=> (const uint<r, size> &a, const math::N &b) {
        return math::N (a) <=> b;
    }
    
    template <bool x, endian::order r, size_t n, bool y, endian::order o, size_t z>
    std::weak_ordering inline operator <=> (const bounded<x, r, n> &a, const endian::arithmetic<y, o, z> &b) {
        return a <=> bounded<y, o, z> (b);
    }

    template <data::endian::order r, size_t size>
    std::ostream inline &operator << (std::ostream &o, const uint<r, size>& n) {
        if (o.flags () & std::ios::hex) return encoding::hexidecimal::write (o, n);
        else if (o.flags () & std::ios::dec) return encoding::decimal::write (o, math::N (n));
        return o;
    }

    template <data::endian::order r, size_t size>
    std::ostream inline &operator << (std::ostream &o, const sint<r, size>& n) {
        if (o.flags () & std::ios::hex) return encoding::hexidecimal::write (o, n);
        if (o.flags () & std::ios::dec) return encoding::signed_decimal::write (o, Z (n));
        return o;
    }
    
    template <endian::order r, size_t x> uint<r, x> inline operator * (const uint<r, x> &a, uint64 b) {
        return a * uint<r, x> (b);
    }
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> inline operator ++ (bounded<u, r, size> &x, int) {
        bounded n = x;
        ++x;
        return n;
    }
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> inline operator -- (bounded<u, r, size> &x, int) {
        bounded n = x;
        ++x;
        return n;
    }
    
    template <endian::order r, size_t size>
    sint<r, size> inline operator | (const sint<r, size> &a, const uint<r, size> &b) {
        sint<r, size> x;
        std::copy (b.begin (), b.end (), x.begin ());
        return a | x;
    }
    
    template <endian::order r, size_t size>
    sint<r, size> inline operator & (const sint<r, size> &a, const uint<r, size> &b) {
        sint<r, size> x;
        std::copy (b.begin (), b.end (), x.begin ());
        return a & x;        
    }
    
    template <endian::order r, size_t size>
    sint<r, size> inline operator ^ (const sint<r, size> &a, const uint<r, size> &b) {
        sint<r, size> x;
        std::copy (b.begin (), b.end (), x.begin ());
        return a ^ x;
    }
    
    template <endian::order r, size_t size>
    sint<r, size> inline operator + (const sint<r, size> &a, const uint<r, size> &b) {
        sint<r, size> x;
        std::copy (b.begin (), b.end (), x.begin ());
        return a + x;
    }
    
    template <endian::order r, size_t size>
    sint<r, size> inline operator - (const sint<r, size> &a, const uint<r, size> &b) {
        sint<r, size> x;
        std::copy (b.begin (), b.end (), x.begin ());
        return a - x;
    }
    
    template <endian::order r, size_t size>
    sint<r, size> inline operator * (const sint<r, size> &a, const uint<r, size> &b) {
        sint<r, size> x;
        std::copy (b.begin (), b.end (), x.begin ());
        return a * x;
    }

}

namespace data::math {
    
    template <size_t size, endian::order o>
    struct abs<number::bounded<size, o, false>> {
        number::bounded<size, o, false> operator()(const number::bounded<size, o, false>& i) {
            return i;
        }
    };
    
}

namespace data {
    
    namespace encoding::hexidecimal {

        template <bool is_signed, endian::order r, size_t size>
        std::string inline write (const math::number::bounded<is_signed, r, size> &n) {
            std::stringstream ss;
            ss << std::hex << n;
            return ss.str ();
        }

    }

    namespace encoding::decimal {

        template <bool is_signed, endian::order r, size_t size>
        std::string inline write (const math::number::bounded<is_signed, r, size> &n) {
            std::stringstream ss;
            ss << std::dec << n;
            return ss.str ();
        }

    }

}

namespace data::math::number {

    template <endian::order r, size_t x> nonzero<uint<r, x>> inline increment<uint<r, x>>::operator () (const uint<r, x> &n) {
        if (n == uint<r, x>::max ()) throw exception {} << "cannot increment";
        return nonzero<uint<r, x>> {n + 1};
    }

    template <endian::order r, size_t x> uint<r, x> inline decrement<uint<r, x>>::operator () (const nonzero<uint<r, x>> &n) {
        auto z = n.Value;
        return --z;
    }

    template <endian::order r, size_t x> uint<r, x> inline decrement<uint<r, x>>::operator () (const uint<r, x> &n) {
        if (data::is_zero (n)) return n;
        auto z = n;
        return --z;
    }

    template <endian::order r, size_t x> sint<r, x> inline increment<sint<r, x>>::operator () (const sint<r, x> &n) {
        if (n == sint<r, x>::max ()) throw exception {} << "cannot increment";
        auto z = n;
        return ++z;
    }

    template <endian::order r, size_t x> sint<r, x> inline decrement<sint<r, x>>::operator () (const sint<r, x> &n) {
        if (n == sint<r, x>::min ()) throw exception {} << "cannot decrement";
        auto z = n;
        return --z;
    }
    
    template <endian::order r, size_t size>
    std::strong_ordering operator <=> (const sint<r, size> &a, const sint<r, size> &b) {
        bool na = data::is_negative (a);
        bool nb = data::is_negative (b);
        if (na == nb) return arithmetic::compare<complement::nones> (a.words (), b.words ());
        return na ? std::strong_ordering::less : std::strong_ordering::greater;
    }
    
    template <endian::order r, size_t size>
    std::strong_ordering inline operator <=> (const uint<r, size> &a, const uint<r, size> &b) {
        return arithmetic::compare<complement::nones> (a.words (), b.words ());
    }
    
    template <bool x, endian::order r, size_t n, bool y, endian::order o, size_t z>
    std::strong_ordering inline operator <=> (const bounded<x, r, n> &a, const bounded<y, o, z> &b);

    template <endian::order r, size_t size>
    uint<r, size> inline operator | (const uint<r, size> &a, uint64 b) {
        return a | uint<r, size> (b);
    }

    template <endian::order r, size_t size>
    uint<r, size> inline operator & (const uint<r, size> &a, uint64 b) {
        return a & uint<r, size> (b);
    }
    
    template <endian::order r, size_t size>
    sint<r, size> inline operator | (const uint<r, size> &a, const sint<r, size> &b) {
        return sint<r, size> (a) | b;
    }
    
    template <endian::order r, size_t size>
    sint<r, size> inline operator & (const uint<r, size> &a, const sint<r, size> &b) {
        return sint<r, size> (a) & b;
    }
    
    template <endian::order r, size_t size>
    sint<r, size> inline operator ^ (const uint<r, size> &a, const sint<r, size> &b) {
        return sint<r, size> (a) ^ b;
    }
    
    template <endian::order r, size_t size>
    sint<r, size> inline operator + (const uint<r, size> &a, const sint<r, size> &b) {
        return sint<r, size> (a) + b;
    }
    
    template <endian::order r, size_t size>
    sint<r, size> inline operator - (const uint<r, size> &a, const sint<r, size> &b) {
        return sint<r, size> (a) - b;
    }
    
    template <endian::order r, size_t size>
    sint<r, size> inline operator * (const uint<r, size> &a, const sint<r, size> &b) {
        return sint<r, size> (a) * b;
    }
    
    template <endian::order r, size_t x> sint<r, x> inline operator + (const sint<r, x> &a, int64 b) {
        return a + sint<r, x> (b);
    }
    
    template <endian::order r, size_t x> sint<r, x> inline operator - (const sint<r, x> &a, int64 b) {
        return a - sint<r, x> (b);
    }
    
    template <endian::order r, size_t x> sint<r, x> inline operator * (const sint<r, x> &a, int64 b) {
        return a * sint<r, x> (b);
    }
    
    template <endian::order r, size_t x> uint<r, x> inline &operator += (uint<r, x> &a, uint64 b) {
        return a += uint<r, x> (b);
    }
    
    template <endian::order r, size_t x> sint<r, x> inline &operator += (sint<r, x> &a, int64 b) {
        return a += sint<r, x> (b);
    }
    
    template <endian::order r, size_t x> uint<r, x> inline &operator -= (uint<r, x> &a, uint64 b) {
        return a -= uint<r, x> (b);
    }
    
    template <endian::order r, size_t x> sint<r, x> inline &operator -= (sint<r, x> &a, int64 b) {
        return a -= sint<r, x> (b);
    }
    
    template <endian::order r, size_t x> uint<r, x> inline &operator *= (uint<r, x> &a, uint64 b) {
        return a *= uint<r, x> (b);
    }
    
    template <endian::order r, size_t x> sint<r, x> inline &operator *= (sint<r, x> &a, int64 b) {
        return a *= sint<r, x> (b);
    }
    
    template <endian::order r, size_t x> uint<r, x> inline &operator /= (uint<r, x> &a, uint64 b) {
        return a = a / b;
    }
    
    template <endian::order r, size_t x> sint<r, x> inline &operator /= (sint<r, x> &a, int64 b) {
        return a = a / b;
    }
    
    template <bool is_signed, endian::order r, size_t size>
    bounded<is_signed, r, size> inline operator << (const bounded<is_signed, r, size> &z, int i) {
        auto n = z;
        return n <<= i;
    }
    
    template <bool is_signed, endian::order r, size_t size>
    bounded<is_signed, r, size> inline operator >> (const bounded<is_signed, r, size> &z, int i) {
        auto n = z;
        return n >>= i;
    }
    
    template <endian::order r, size_t size>
    inline bounded<true, r, size> operator - (const bounded<true, r, size>& a) {
        auto x = ~a;
        return ++x;
    }
    
    template <endian::order r, size_t size>
    bool inline operator == (const uint<r, size> &a, uint64 b) {
        return a == uint<r, size> {b};
    }
    
    template <endian::order r, size_t size>
    bool inline operator == (const sint<r, size> &a, int64 b) {
        return a == sint<r, size> {b};
    }
    
}

namespace data::math {

    template <endian::order r, size_t x> math::signature inline sign<math::uint<r, x>>::operator () (const math::uint<r, x> &z) {
        return math::number::arithmetic::nones::sign (z.words ());
    }

    template <endian::order r, size_t x> math::signature inline sign<math::sint<r, x>>::operator () (const math::sint<r, x> &z) {
        return math::number::arithmetic::ones::sign (z.words ());
    }
    
    template <endian::order r, size_t x> bool inline is_positive<uint<r, x>>::operator () (const uint<r, x> &n) {
        return !data::is_zero (n);
    }
    
    template <endian::order r, size_t x> bool inline is_positive<sint<r, x>>::operator () (const sint<r, x> &n) {
        return !data::is_negative (n) && !data::is_zero (n);
    }
    
    template <endian::order r, size_t x> bool inline is_negative<uint<r, x>>::operator () (const uint<r, x> &n) {
        return false;
    }

    template <endian::order r, size_t x> bool inline is_negative<sint<r, x>>::operator () (const sint<r, x> &z) {
        return number::arithmetic::sign_bit (z.words ());
    }

    template <bool u, endian::order r, size_t x> bool inline is_zero<number::bounded<u, r, x>>::operator () (const number::bounded<u, r, x> &z) {
        return number::arithmetic::is_zero (z.words ());
    }
    
    template <bool u, endian::order r, size_t x> 
    number::bounded<u, r, x> inline first<number::bounded<u, r, x>>::operator () () {
        return number::bounded<u, r, x> {};
    }
    
    template <endian::order r, size_t x> inline uint<r, x> abs<uint<r, x>>::operator () (const uint<r, x> &z) {
        return z;
    }
    
    template <endian::order r, size_t x> inline uint<r, x> abs<sint<r, x>>::operator () (const sint<r, x> &z) {
        uint<r, x> n {};
        std::copy (z.begin (), z.end (), n.begin ());
        if (data::is_negative (z)) number::arithmetic::negate_ones (n.words ());
        return n;
    }
    
    template <endian::order r, size_t x> inline uint<r, x> quadrance<uint<r, x>>::operator () (const uint<r, x> &z) {
        return z * z;
    }
    
    template <endian::order r, size_t x> inline uint<r, x> quadrance<sint<r, x>>::operator () (const sint<r, x> &z) {
        return data::abs (z * z);
    }
    
    template <bool u, endian::order r, size_t x> 
    number::bounded<u, r, x> inline inverse<plus<number::bounded<u, r, x>>, number::bounded<u, r, x>>::operator () (
        const number::bounded<u, r, x> &a, 
        const number::bounded<u, r, x> &b) {
        return b - a;
    }
}

namespace data::math::number {
    
    template <endian::order r, size_t size>
    uint<r, size> uint<r, size>::read (string_view x) {
        if (encoding::decimal::valid (x) || encoding::hexidecimal::valid (x) && x.size () == 2 * size + 2)
            return bounded {N_bytes<r>::read (x)};
        
        if (encoding::hex::valid (x) && x.size () == 2 * size) {
            bounded z;
            boost::algorithm::unhex (x.begin (), x.end (), z.begin ());
            return z;
        }
        
        throw std::invalid_argument {std::string {"invalid natural string "} + std::string {x}};
    }
    
    template <endian::order r, size_t size>
    sint<r, size> sint<r, size>::read (string_view x) {
        if (encoding::signed_decimal::valid (x) ||
            encoding::hexidecimal::valid (x) && x.size () == 2 * size + 2)
                return bounded {Z_bytes<r, complement::ones>::read (x)};
        
        if (encoding::hex::valid (x) && x.size () == 2 * size) {
            bounded z;
            boost::algorithm::unhex (x.begin (), x.end (), z.begin ());
            return z;
        }
        
        throw std::invalid_argument {"invalid integer string"};
    }
    
    template <endian::order o, size_t size>
    uint<o, size> uint<o, size>::min () {
        bounded b {};
        for (int i = 0; i <= size; i++) b[i] = 0x00;
        return b;
    }
 
    template <bool u, endian::order r, size_t x>
    bounded<u, r, x> inline operator ^ (const bounded<u, r, x> &a, const bounded<u, r, x> &b) {
        bounded<u, r, x> z {};
        arithmetic::bit_xor<byte> (z.end (), z.begin (), a.begin (), b.begin ());
        return z;
    }
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> inline operator & (const bounded<u, r, size> &a, const bounded<u, r, size> &b) {
        bounded<u, r, size> x;
        arithmetic::bit_and<byte>
            (x.words ().end (), x.words ().begin (), a.words ().begin (), b.words ().begin ());
        return x;
    }
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> inline operator | (const bounded<u, r, size> &a, const bounded<u, r, size> &b) {
        bounded<u, r, size> x;
        arithmetic::bit_or<byte>
            (x.words ().end (), x.words ().begin (), a.words ().begin (), b.words ().begin ());
        return x;
    }
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> inline &operator &= (bounded<u, r, size> &a, const bounded<u, r, size> &b) {
        arithmetic::bit_and<byte> (a.words ().end (), a.words ().begin (),
            const_cast<const bounded<u, r, size>&> (a).words ().begin (), b.words ().begin ());
        return a;
    }
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> inline &operator |= (bounded<u, r, size> &a, const bounded<u, r, size> &b) {
        arithmetic::bit_or<byte> (a.words ().end (), a.words().begin(),
            const_cast<const bounded<u, r, size>&> (a).words ().begin(), b.words().begin());
        return a;
    }
    
    template <bool u, endian::order r, size_t size>
    bounded<u, r, size> inline &operator ^= (bounded<u, r, size> &a, const bounded<u, r, size> &b) {
        arithmetic::bit_xor<byte> (a.words ().end (), a.words ().begin (),
            const_cast<const bounded<u, r, size>&> (a).words ().begin (), b.words ().begin ());
        return a;
    }
    
    template <endian::order r, size_t x> std::weak_ordering inline operator <=> (const uint<r, x> &a, int64 b) {
        if (b < 0) return std::weak_ordering::greater;
        return arithmetic::compare<complement::nones> (a.words (), endian::arithmetic<true, r, 8> {b}.words ());
    }
    
    template <endian::order r, size_t x> std::weak_ordering inline operator <=> (const sint<r, x> &a, int64 b) {
        return arithmetic::ones::compare<r, byte> (a, bytes_view (endian::arithmetic<true, r, 8> (b)));
    }
    
    template <endian::order r, size_t size>
    uint<r, size>::bounded (const uint64 x) : bounded {} {
        endian::arithmetic<false, endian::little, 8> n {x};
        std::copy (n.begin (), n.end (), this->words ().begin ());
    }
    
    template <endian::order r, size_t size>
    sint<r, size>::bounded (const int64 x) : oriented<r, byte, size>
        {x < 0 ?
            bytes_array<byte, size>::filled (0xff) :
            bytes_array<byte, size>::filled (0x00)} {
        endian::arithmetic<true, endian::little, 8> n {x};
        std::copy (n.begin (), n.end (), this->words ().begin ());
    }
    
    template <endian::order r, size_t size>
    uint<r, size>::bounded (const byte_array<size>& b) : oriented<r, byte, size> {b} {}
    
    template <endian::order r, size_t size>
    uint<r, size>::bounded (slice<byte, size> x) {
        std::copy (x.begin (), x.end (), this->begin ());
    }
    
    template <endian::order r, size_t size> bounded<false, r, size>::operator N_bytes<r> () const {
        N_bytes<r> n {};
        n.resize (size);
        std::copy (this->begin (), this->end (), n.begin ());
        return n;
    }
    
    template <endian::order r, size_t size> bounded<true, r, size>::operator Z_bytes<r, complement::ones> () const {
        Z_bytes<r, complement::ones> z {};
        z.resize (size);
        std::copy (this->begin (), this->end (), z.begin ());
        return z;
    }
    
    template <endian::order r, size_t size>
    uint<r, size> inline operator + (const uint<r, size>& a, uint64 b) {
        return a + bounded<false, r, size> {b};
    }
    
    template <bool u, endian::order r, size_t x>
    sint<r, x> inline operator - (const bounded<u, r, x> &n) {
        sint<r, x> z;
        arithmetic::bit_negate<byte> (z.words ().end (), z.words ().begin (), n.words ().begin ());
        return ++z;
    }
    
    template <bool u, endian::order r, size_t x> bounded<u, r, x> operator +
        (const bounded<u, r, x> &a, const bounded<u, r, x> &b) {
        bounded<u, r, x> z {};
        auto i = z.words ().begin ();
        auto j = a.words ().begin ();
        auto k = b.words ().begin ();
        arithmetic::plus<byte> (z.words ().end (), i, j, k);
        return z;
    }
    
    template <bool u, endian::order r, size_t x>
    bounded<u, r, x> operator - (const bounded<u, r, x> &a, const bounded<u, r, x> &b) {
        bounded<u, r, x> z {};
        auto i = z.words ().begin ();
        auto j = a.words ().begin ();
        auto k = b.words ().begin ();
        arithmetic::minus<byte> (z.words ().end (), i, j, k);
        return z;
    }

    template <bool u, endian::order r, size_t x>
    bounded<u, r, x> &operator += (bounded<u, r, x> &a, const bounded<u, r, x> &b) {
        auto awb = a.words ().begin ();
        auto cawb = const_cast<const bounded<u, r, x> &> (a).words ().begin ();
        auto bwb = b.words ().begin ();
        arithmetic::plus<byte> (a.words ().end (), awb, cawb, bwb);
        return a;
    }

    template <bool u, endian::order r, size_t x>
    bounded<u, r, x> &operator -= (bounded<u, r, x> &a, const bounded<u, r, x> &b) {
        auto awb = a.words ().begin ();
        auto cawb = const_cast<const bounded<u, r, x> &> (a).words ().begin ();
        auto bwb = b.words ().begin ();
        arithmetic::minus<byte> (a.words ().end (), awb, cawb, bwb);
        return a;
    }
    
    template <bool u, endian::order r, size_t x>
    bounded<u, r, x> inline operator * (const bounded<u, r, x> &a, const bounded<u, r, x> &b) {
        bounded<u, r, x> z {};
        auto w = z.words ();
        arithmetic::times (w, a.words (), b.words ());
        return z;
    }
    
    template <endian::order r, size_t size>
    uint<r, size> inline operator - (const uint<r, size>& a, uint64 b) {
        return a - bounded<false, r, size> {b};
    }
    
    template <bool u, endian::order r, size_t x> bounded<u, r, x> inline &operator *=
        (bounded<u, r, x> &a, const bounded<u, r, x> &b) {
        auto w = a.words ();
        arithmetic::times (w, const_cast<const bounded<u, r, x> &> (a).words (), b.words ());
        return a;
    }
    
    template <bool u, endian::order r, size_t x> bounded<u, r, x> inline &operator ++ (bounded<u, r, x> &n) {
        auto o = n.words ().begin ();
        auto i = n.words ().begin ();
        arithmetic::plus<byte> (n.words ().end (), o, 1, i);
        return n;
    }   
    
    template <bool u, endian::order r, size_t x> bounded<u, r, x> inline &operator -- (bounded<u, r, x> &n) {
        auto xx = n.words ().begin ();
        auto xy = n.words ().begin ();
        arithmetic::minus<byte> (n.words ().end (), xx, 1, xy);
        return n;
    }
    
    template <endian::order r, size_t size>
    inline sint<r, size>::bounded (const uint<r, size> &x) {
        std::copy (x.begin (), x.end (), this->begin ());
    }
    
    template <endian::order r, size_t size> uint<r, size> inline uint<r, size>::max () {
        uint<r, size> n;
        arithmetic::set_max_unsigned (n.words ());
        return n;
    }
        
    template <endian::order r, size_t size> N_bytes<r> inline uint<r, size>::modulus () {
        return N_bytes<r> (max ()) + 1;
    }
    
    template <endian::order r, size_t size> sint<r, size> inline sint<r, size>::max () {
        uint<r, size> n;
        arithmetic::set_max_signed_ones (n.words ());
        return n;
    }
    
    template <endian::order r, size_t size> sint<r, size> inline sint<r, size>::min () {
        uint<r, size> n;
        arithmetic::set_min_signed_ones (n.words ());
        return n;
    }
    
    namespace {
        template <endian::order r, size_t size>
        void shift_right (byte_array<size> &n, uint32 i, byte fill) {
            if (r == endian::big) arithmetic::bit_shift_right (n.rbegin (), n.rend (), i, fill);
            else arithmetic::bit_shift_right (n.begin (), n.end (), i, fill);
        }
        
        template <endian::order r, size_t size>
        void shift_left (byte_array<size> &n, uint32 i, byte fill) {
            if (r == endian::big) arithmetic::bit_shift_left (n.begin (), n.end (), i, fill);
            else arithmetic::bit_shift_left (n.rbegin (), n.rend (), i, fill);
        }
    }
    
    template <bool is_signed, endian::order r, size_t size>
    bounded<is_signed, r, size> inline &operator <<= (bounded<is_signed, r, size> &n, int i) {
        (i < 0 ? shift_right<r, size> : shift_left<r, size>) (n, i, data::is_negative (n));
        return n; 
    }
    
    template <bool is_signed, endian::order r, size_t size>
    bounded<is_signed, r, size> inline &operator >>= (bounded<is_signed, r, size> &n, int i) {
        (i < 0 ? shift_left<r, size> : shift_right<r, size>) (n, i, data::is_negative (n));
        return n; 
    }
    
}

#endif
