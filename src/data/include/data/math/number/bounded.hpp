// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BOUNDED
#define DATA_MATH_NUMBER_BOUNDED

#include <type_traits>
#include <iterator>
#include <data/math/number/bounded/bounded.hpp>
#include <data/math/number/gmp/Z.hpp>
#include <data/math/number/integer.hpp>

namespace data::math::number {

    template <bool u, endian::order r, size_t x, std::unsigned_integral word>
    bounded<u, r, x, word> inline operator / (const bounded<u, r, x, word> &a, const bounded<u, r, x, word> &b) {
        if (b == 0) throw division_by_zero {};
        return divide<bounded<u, r, x, word>> {} (a, nonzero<bounded<u, r, x, word>> {b}).Quotient;
    }
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> inline operator / (const uint<r, x, word> &a, uint64 b) {
        return a / uint<r, x, word> (b);
    }
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    sint<r, x, word> inline operator / (const sint<r, x, word> &a, int64 b) {
        return a / sint<r, x, word> (b);
    }

    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> inline operator % (const uint<r, x, word> &a, const uint<r, x, word> &b) {
        if (b == 0) throw division_by_zero {};
        return data::divide<uint<r, x, word>> (a, nonzero<uint<r, x, word>> {x}).Remainder;
    }
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    uint<r, x, word> inline operator % (const sint<r, x, word> &a, const uint<r, x, word> &b) {
        if (b == 0) throw division_by_zero {};
        return data::divide<sint<r, x, word>> (a, nonzero<sint<r, x, word>> {sint<r, x, word> (x)}).Remainder;
    }
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    uint64 inline operator % (const uint<r, x, word> &a, uint64 b) {
        return uint64 (a % uint<r, x, word> (x));
    }
    
    template <endian::order r, size_t x, std::unsigned_integral word>
    uint64 inline operator % (const sint<r, x, word> &a, uint64 b) {
        return uint64 (a % uint<r, x, word> (x));
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    inline uint<r, size, word>::operator double () const {
        return double (math::N (N_bytes<r, word> (*this)));
    }
    
    template <endian::order r, size_t size, std::unsigned_integral word>
    inline sint<r, size, word>::operator double () const {
        return double (Z (Z_bytes<r, complement::ones, word> (*this)));
    }
}

namespace data::math {
    template <endian::order r, size_t x, std::unsigned_integral word>
    division<uint<r, x, word>, uint<r, x, word>> inline divide<uint<r, x, word>, uint<r, x, word>>::operator ()
    (const uint<r, x, word> &v, const nonzero<uint<r, x, word>> &z) {
        auto d = divide<N_bytes<r, word>, N_bytes<r, word>> {} (v, nonzero<N_bytes<r, word>> {z.Value});
        return {uint<r, x, word> {d.Quotient}, uint<r, x, word> {d.Remainder}};
    }

    template <endian::order r, size_t x, std::unsigned_integral word>
    division<sint<r, x, word>, uint<r, x, word>> inline divide<sint<r, x, word>, sint<r, x, word>>::operator ()
    (const sint<r, x, word> &v, const nonzero<sint<r, x, word>> &z) {
        auto d = divide<Z_bytes<r, word>, Z_bytes<r, word>> {} (v, nonzero<Z_bytes<r, word>> {z.Value});
        return {sint<r, x, word> {d.Quotient}, uint<r, x, word> {d.Remainder}};
    }

    template <endian::order r, size_t x, std::unsigned_integral word>
    division<sint<r, x, word>, uint<r, x, word>> inline divide<sint<r, x, word>, uint<r, x, word>>::operator ()
    (const sint<r, x, word> &v, const nonzero<uint<r, x, word>> &z) {
        auto d = divide<Z_bytes<r, word>, N_bytes<r, word>> {} (v, nonzero<N_bytes<r, word>> {z.Value});
        return {sint<r, x, word> {d.Quotient}, uint<r, x, word> {d.Remainder}};
    }
}

namespace data::encoding::hexidecimal {

    template <endian::order r, std::unsigned_integral word, size_t x>
    std::ostream &write (std::ostream &o, const oriented<r, word, x> &z, hex_case q) {
        o << "0x";
        return encoding::hex::write (o, z.words ().reverse (), q);
    }

    template <hex_case cx, endian::order r, std::unsigned_integral word, size_t x>
    string<cx> write (const oriented<r, word, x> &z) {
        std::stringstream ss;
        write (ss, z, cx);
        return string<cx> {ss.str ()};
    }

}

namespace data::math::number {

    template struct bounded<false, endian::big, 16, byte>;
    template struct bounded<true, endian::big, 16, byte>;
    template struct bounded<false, endian::little, 16, byte>;
    template struct bounded<true, endian::little, 16, byte>;

    template struct bounded<false, endian::big, 20, byte>;
    template struct bounded<true, endian::big, 20, byte>;
    template struct bounded<false, endian::little, 20, byte>;
    template struct bounded<true, endian::little, 20, byte>;

    template struct bounded<false,endian::big, 28, byte>;
    template struct bounded<true, endian::big, 28, byte>;
    template struct bounded<false, endian::little, 28, byte>;
    template struct bounded<true, endian::little, 28, byte>;

    template struct bounded<false, endian::big, 32, byte>;
    template struct bounded<true, endian::big, 32, byte>;
    template struct bounded<false, endian::little, 32, byte>;
    template struct bounded<true, endian::little, 32, byte>;

    template struct bounded<false, endian::big, 40, byte>;
    template struct bounded<true, endian::big, 40, byte>;
    template struct bounded<false, endian::little, 40, byte>;
    template struct bounded<true, endian::little, 40, byte>;

    template struct bounded<false, endian::big, 56, byte>;
    template struct bounded<true, endian::big, 56, byte>;
    template struct bounded<false, endian::little, 56, byte>;
    template struct bounded<true, endian::little, 56, byte>;

    template struct bounded<false, endian::big, 64, byte>;
    template struct bounded<true, endian::big, 64, byte>;
    template struct bounded<false, endian::little, 64, byte>;
    template struct bounded<true, endian::little, 64, byte>;

}

#endif
