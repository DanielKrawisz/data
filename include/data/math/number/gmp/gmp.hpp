// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_GMP
#define DATA_MATH_NUMBER_GMP_GMP

#include <data/math/number/gmp/aks.hpp>
#include <data/math/number/gmp/sqrt.hpp>
#include <data/encoding/digits.hpp>
#include <data/math/number/integer.hpp>
#include <data/math/power.hpp>

namespace data::math::number {
    
    bool inline is_zero(const N &x) {
        return data::sign(x) == zero;
    }
    
    bool inline is_zero(const Z &x) {
        return data::sign(x) == zero;
    }
    
    bool inline is_negative(const N&) {
        return false;
    }
    
    bool inline is_negative(const Z &z) {
        return data::sign(z) == negative;
    }
    
    bool inline is_positive(const N &x) {
        return !is_zero(x);
    }
    
    bool inline is_positive(const Z &z) {
        return data::sign(z) == positive;
    }
    
}

namespace data::math::number::GMP {
    /*
    N inline operator^(const N &x, const N &j) {
        return power<N, N>{}(x, j);
    }
    
    Z inline operator^(const Z &x, const N &j) {
        return power<Z, N>{}(x, j);
    }
    
    N inline operator^(const N &x, uint64 j) {
        return power<N, uint64>{}(x, j);
    }
    
    Z inline operator^(const Z &x, uint64 j) {
        return power<Z, int64>{}(x, j);
    }*/
    
    N inline operator/(const N &x, const N &j) {
        return divide(x, j).Quotient;
    }
    
    Z inline operator/(const Z &x, const Z &j) {
        return divide(x, j).Quotient;
    }
    
    Z inline operator/(const Z &x, const N &j) {
        return divide(x, Z{j}).Quotient;
    }
    
    N inline operator/(const N &x, uint64 j) {
        return divide(x, N{j}).Quotient;
    }
    
    Z inline operator/(const Z &x, int64 j) {
        return divide(x, Z{j}).Quotient;
    }
    
    N inline operator%(const N &x, const N &j) {
        return divide(x, j).Remainder;
    }
    
    N inline operator%(const Z &x, const N &j) {
        return divide(x, Z{j}).Remainder;
    }
    
    uint64 inline operator%(const N &x, uint64 j) {
        return uint64(divide(x, N{j}).Remainder);
    }
    
    uint64 inline operator%(const Z &x, uint64 j) {
        return uint64(divide(x, Z{N{j}}).Remainder);
    }
    
}

namespace data::encoding::decimal {
    
    string inline write(const math::N &n) {
        return string{write_base<math::N>(n, characters())};
    }
    
    std::ostream inline &write(std::ostream &o, const math::N &n) {
        return o << write(n);
    }
    
}

namespace data::encoding::hexidecimal {
    
    std::ostream inline &write(std::ostream &o, const math::Z &n) {
        return write(o, math::number::Z_bytes<endian::big, math::number::ones>(n));
    }
    
    std::ostream inline &write(std::ostream& o, const math::N& n) {
        return write(o, math::number::N_bytes<endian::big>(n));
    }
    
    std::string inline write(const math::Z &n) {
        std::stringstream ss;
        write(ss, n);
        return ss.str();
    }
    
    std::string inline write(const math::N &n) {
        std::stringstream ss;
        write(ss, n);
        return ss.str();
    }
    
}

namespace data::encoding::natural {
    
    std::ostream inline &write(std::ostream& o, const math::N &n) {
        return decimal::write(o, n);
    }
    
    std::string inline write(const math::N &n) {
        std::stringstream ss;
        write(ss, n);
        return ss.str();
    }
    
}

namespace data::encoding::integer {
    
    std::ostream inline &write(std::ostream &o, const math::Z &n) {
        if (math::number::is_negative(n)) o << "-";
        return decimal::write(o, data::abs(n));
    }
    
    std::string inline write(const math::Z &n) {
        std::stringstream ss;
        write(ss, n);
        return ss.str();
    }
    
}

namespace data::math::number::GMP {
    
    std::ostream inline &operator<<(std::ostream& o, const Z& n) {
        if (o.flags() & std::ios::hex) return encoding::hexidecimal::write(o, n);
        if (o.flags() & std::ios::dec) return encoding::integer::write(o, n);
        return o << &n.MPZ;
    }
    
    std::ostream inline &operator<<(std::ostream& o, const N& n) {
        if (o.flags() & std::ios::hex) return encoding::hexidecimal::write(o, n);
        if (o.flags() & std::ios::dec) return encoding::decimal::write(o, n);
        return o << &n.Value.MPZ;
    }
    
}

#endif

