// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BOUNDED
#define DATA_MATH_NUMBER_BOUNDED

#include <type_traits>
#include <iterator>
#include <data/math/number/bounded/bounded.hpp>
#include <data/math/number/gmp/N.hpp>
#include <data/math/number/integer.hpp>

namespace data::math::number {
    
    template <bool u, endian::order r, size_t x> bounded<u, r, x> inline operator / (const bounded<u, r, x> &a, const bounded<u, r, x> &b) {
        return divide<bounded<u, r, x>> (a, b).Quotient;
    }
    
    template <endian::order r, size_t x> uint<r, x> inline operator / (const uint<r, x> &a, uint64 b) {
        return a / uint<r, x> (b);
    }
    
    template <endian::order r, size_t x> sint<r, x> inline operator / (const sint<r, x> &a, int64 b) {
        return a / sint<r, x> (b);
    }
    
    template <endian::order r, size_t x> uint<r, x> inline operator % (const uint<r, x> &a, const uint<r, x> &b) {
        return divide<uint<r, x>> (a, x).Remainder;
    }
    
    template <endian::order r, size_t x> uint<r, x> inline operator % (const sint<r, x> &a, const uint<r, x> &b) {
        return divide<sint<r, x>> (a, sint<r, x> (x)).Remainder;
    }
    
    template <endian::order r, size_t x> uint64 inline operator % (const uint<r, x> &a, uint64 b) {
        return uint64 (a % uint<r, x> (x));
    }
    
    template <endian::order r, size_t x> uint64 inline operator % (const sint<r, x> &a, uint64 b) {
        return uint64 (a % uint<r, x> (x));
    }
    
    template <endian::order r, size_t size> inline uint<r, size>::operator double () const {
        return double (N (N_bytes<r> (*this)));
    }
    
    template <endian::order r, size_t size> inline sint<r, size>::operator double () const {
        return double (Z (Z_bytes<r, complement::ones> (*this)));
    }
}

namespace data::encoding::hexidecimal {

    template <endian::order r, size_t x>
    std::ostream &write (std::ostream &o, const oriented<r, byte, x> &z, hex_case q) {
        o << "0x";
        return encoding::hex::write (o, z.words ().reverse (), q);
    }

    template <hex_case cx, endian::order r, size_t x>
    string<cx> write (const oriented<r, byte, x> &z) {
        std::stringstream ss;
        write (ss, z, cx);
        return string<cx> {ss.str ()};
    }

}

namespace data::math::number {

    template struct bounded<false, endian::big, 16>;
    template struct bounded<true, endian::big, 16>;
    template struct bounded<false, endian::little, 16>;
    template struct bounded<true, endian::little, 16>;

    template struct bounded<false, endian::big, 20>;
    template struct bounded<true, endian::big, 20>;
    template struct bounded<false, endian::little, 20>;
    template struct bounded<true, endian::little, 20>;

    template struct bounded<false,endian::big,  28>;
    template struct bounded<true, endian::big, 28>;
    template struct bounded<false, endian::little, 28>;
    template struct bounded<true, endian::little, 28>;

    template struct bounded<false, endian::big, 32>;
    template struct bounded<true, endian::big, 32>;
    template struct bounded<false, endian::little, 32>;
    template struct bounded<true, endian::little, 32>;

    template struct bounded<false, endian::big, 40>;
    template struct bounded<true, endian::big, 40>;
    template struct bounded<false, endian::little, 40>;
    template struct bounded<true, endian::little, 40>;

    template struct bounded<false, endian::big, 56>;
    template struct bounded<true, endian::big, 56>;
    template struct bounded<false, endian::little, 56>;
    template struct bounded<true, endian::little, 56>;

    template struct bounded<false, endian::big, 64>;
    template struct bounded<true, endian::big, 64>;
    template struct bounded<false, endian::little, 64>;
    template struct bounded<true, endian::little, 64>;

}
#endif
