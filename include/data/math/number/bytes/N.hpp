// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BYTES_N
#define DATA_MATH_NUMBER_BYTES_N

#include <data/math/number/natural.hpp>
#include <data/encoding/endian.hpp>
#include <data/math/number/abs.hpp>
#include <limits>

namespace data::math::number {
    
    template <endian::order o> struct Z_bytes;
    
    template <endian::order o>
    struct N_bytes {
        bytes Value;
        
        N_bytes();
        
        N_bytes(uint64 n);
        
        explicit N_bytes(string_view s);
        
        N_bytes& operator=(const N_bytes& n);
        
        bool operator==(uint64 n) const;
        
        bool operator==(const N_bytes& n) const;
        
        bool operator==(const Z_bytes<o>& z) const ;
        
        bool operator!=(uint64 n) const;
        
        bool operator!=(const N_bytes& n) const;
        
        bool operator!=(const Z_bytes<o>& z) const;
        
        bool operator<(uint64 n) const;
        
        bool operator<(const N_bytes& n) const;
        
        bool operator<(const Z_bytes<o>& z) const;
        
        bool operator>(uint64 n) const;
        
        bool operator>(const N_bytes& n) const;
        
        bool operator>(const Z_bytes<o>& z) const;
        
        bool operator<=(uint64 n) const;
        
        bool operator<=(const N_bytes& n) const;
        
        bool operator<=(const Z_bytes<o>& z) const;
        
        bool operator>=(uint64 n) const;
        
        bool operator>=(const N_bytes& n) const;
        
        bool operator>=(const Z_bytes<o>& z) const;
        
        explicit operator uint64();
        
        N_bytes& operator++();
        
        N_bytes& operator--();
        
        N_bytes operator++(int);
        
        N_bytes operator--(int);
        
        N_bytes operator+(uint64 n) const;
        
        N_bytes operator+(const N_bytes& n) const;
        
        N_bytes operator-(uint64 n) const;
        
        N_bytes operator-(const N_bytes& n) const;
        
        N_bytes& operator+=(uint64 n);
        
        N_bytes& operator+=(const N_bytes& n);
        
        N_bytes operator*(uint64 n) const;
        
        N_bytes operator*(const N_bytes& n) const;
        
        N_bytes& operator*=(uint64 n);
        
        N_bytes& operator*=(const N& n);
        
        N_bytes operator^(uint32 n) const;
        
        N_bytes& operator^=(uint32 n);
        
        math::division<N_bytes> divide(const N_bytes& n) const;
        
        bool operator|(const N_bytes& n) const {
            return divide(n).Remainder == 0;
        }
        
        N_bytes operator/(const N_bytes& n) const {
            return divide(n).Quotient;
        }
        
        N_bytes operator%(const N_bytes& n) const {
            return divide(n).Remainder;
        }
        
        N_bytes& operator/=(const N_bytes& n) {
            N_bytes q = operator/(n);
            return operator=(q);
        }
        
        N_bytes& operator%=(const N_bytes& n) {
            N_bytes r = operator%(n);
            return operator=(r);
        }
        
        N_bytes operator<<(int64 x) const {
            return N_bytes{Value << x};
        }
        
        N_bytes operator>>(int64 x) const {
            return N_bytes{Value >> x};
        }
        
        N_bytes& operator<<=(int64 x);
        
        N_bytes& operator>>=(int64 x);
        
        explicit N_bytes(bytes_view, endian::order);
        
        bytes write(endian::order) const;
        
        constexpr static math::number::natural::interface<N_bytes> is_natural{};
        
    };
    
    template <endian::order o> 
    struct abs<N_bytes<o>, Z_bytes<o>> {
        N_bytes<o> operator()(const Z_bytes<o>& i);
    };
}

namespace data::encoding::hexidecimal { 
    
    template <endian::order o>
    string write(const math::number::N_bytes<o>& n);
    
}

namespace data::encoding::decimal {
    
    template <endian::order o>
    string write(const math::number::Z_bytes<o>& n);
    
}

template <data::endian::order o>
std::ostream& operator<<(std::ostream& s, const data::math::number::N_bytes<o>& n);

#endif

