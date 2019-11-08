// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_N
#define DATA_MATH_NUMBER_GMP_N

#include <data/math/number/natural.hpp>
#include <data/math/number/gmp/Z.hpp>
#include <data/encoding/endian.hpp>
#include <data/math/number/abs.hpp>
#include <limits>

namespace data::math::number::gmp {
    
    struct Q;
    
    struct N {
        Z Value;
        
        N() : Value{} {}
        
        N(gmp_uint n) : Value{} {
            mpz_init_set_ui(Value.MPZ, n);
        }
        
        explicit N(string_view s);
        
        N& operator=(const N& n) {
            Value = n.Value;
            return *this;
        }
        
        bool valid() const {
            return Value.valid() && Value >= 0;
        }
        
        bool operator==(uint64 n) const {
            if (!valid()) return false;
            return Value == n;
        }
        
        bool operator==(const N& n) const {
            if (!valid() && !n.valid()) return true;
            if (!valid() || !n.valid()) return false;
            return Value == n.Value;
        }
        
        bool operator==(const Z& z) const {
            if (z < 0) return false;
            return Value == z;
        }
        
        bool operator!=(uint64 n) const {
            return !operator==(n);
        }
        
        bool operator!=(const N& n) const {
            return !operator==(n);
        }
        
        bool operator!=(const Z& z) const {
            return !operator==(z);
        }
        
        bool operator<(uint64 n) const {
            return __gmp_binary_less::eval(Value.MPZ, (unsigned long int)(n));
        }
        
        bool operator<(const N& n) const {
            return Value < n.Value;
        }
        
        bool operator<(const Z& z) const {
            if (z < 0) return false;
            return Value < z;
        }
        
        bool operator>(uint64 n) const {
            return __gmp_binary_greater::eval(Value.MPZ, (unsigned long int)(n));
        }
        
        bool operator>(const N& n) const {
            return Value > n.Value;
        }
        
        bool operator>(const Z& z) const {
            if (z < 0) return true;
            return Value > z;
        }
        
        bool operator<=(uint64 n) const {
            return Value <= n;
        }
        
        bool operator<=(const N& n) const {
            return Value <= n.Value;
        }
        
        bool operator<=(const Z& z) const {
            if (z < 0) return false;
            return Value <= z;
        }
        
        bool operator>=(uint64 n) const {
            return Value >= n;
        }
        
        bool operator>=(const N& n) const {
            return Value >= n.Value;
        }
        
        bool operator>=(const Z& z) const {
            if (z < 0) return true;
            return Value == z;
        }
        
        explicit operator uint64() {
            if (operator>(std::numeric_limits<uint64>::max())) throw std::logic_error{"too big"};
            return mpz_get_ui(Value.MPZ);
        } 
        
        N& operator++() {
            ++Value;
            return *this;
        }
        
        N& operator--() {
            if (Value != 0) --Value; 
            return *this;
        }
        
        N operator++(int) {
            Z z = *this;
            ++(*this);
            return N{z};
        }
        
        N operator--(int) {
            Z z = *this;
            ++(*this);
            return N{z};
        }
        
        N operator+(uint64 n) const {
            return N{Value + n};
        }
        
        N operator+(const N& n) const {
            return N{Value + n.Value};
        }
        
        N operator-(uint64 n) const {
            if (Value < n) return 0;
            return N{Value - n};
        }
        
        N operator-(const N& n) const {
            if (Value < n) return N{0};
            return N{Value - n.Value};
        }
        
        N& operator+=(uint64 n) {
            Value += n;
            return *this;
        }
        
        N& operator+=(const N& n) {
            Value += n;
            return *this;
        }
        
        N operator*(uint64 n) const {
            return N{Value * n};
        }
        
        N operator*(const N& n) const {
            return N{Value * n.Value};
        }
        
        N& operator*=(uint64 n) {
            Value *= n;
            return *this;
        }
        
        N& operator*=(const N& n) {
            Value *= n.Value;
            return *this;
        }
        
        N operator^(uint32 n) const {
            return N{Value ^ n};
        }
        
        N& operator^=(uint32 n) {
            Value ^= n;
            return *this;
        };
        
        math::division<N> divide(const N& n) const {
            auto div = Value.divide(n.Value);
            return math::division<N>{N{div.Quotient}, N{div.Remainder}};
        }
        
        bool operator|(const N& n) const {
            return divide(n).Remainder == 0;
        }
        
        N operator/(const N& n) const {
            return divide(n).Quotient;
        }
        
        N operator%(const N& n) const {
            return divide(n).Remainder;
        }
        
        N& operator/=(const N& n) {
            N q = operator/(n);
            return operator=(q);
        }
        
        N& operator%=(const N& n) {
            N r = operator%(n);
            return operator=(r);
        }
        
        N operator<<(int64 x) const {
            return N{Value << x};
        }
        
        N operator>>(int64 x) const {
            return N{Value >> x};
        }
        
        N& operator<<=(int64 x) {
            __gmp_binary_lshift::eval(&Value.MPZ[0], &Value.MPZ[0], x);
            return *this;
        }
        
        N& operator>>=(int64 x) {
            __gmp_binary_lshift::eval(&Value.MPZ[0], &Value.MPZ[0], x);
            return *this;
        }
        
        static N as(const Z& z) {
            if (z < 0) return N{0};
            return N{z};
        }
        
        explicit N(bytes_view, endian::order);
        
        bytes write(endian::order) const;
        
        constexpr static math::number::natural::interface<N> is_natural{};
        
        template <typename indexed, size_t size, endian::order o> 
        explicit N(const bounded<indexed, size, o, false>& b) : Value{b} {}
        
    private:
        explicit N(const Z& z) : Value{z} {}
        
        friend struct Z;
    };
    
    inline bool Z::operator==(const N& n) const {
        return operator==(n.Value);
    }
    
    inline bool Z::operator!=(const N& n) const {
        return operator!=(n.Value);
    }
    
    inline bool Z::operator<(const N& n) const {
        return operator<(n.Value);
    }
    
    inline bool Z::operator>(const N& n) const {
        return operator>(n.Value);
    }
    
    inline bool Z::operator<=(const N& n) const {
        return operator<=(n.Value);
    }
    
    inline bool Z::operator>=(const N& n) const {
        return operator>=(n.Value);
    }
    
    inline Z Z::operator+(const N& n) const {
        return operator+(n.Value);
    }
    
    inline Z& Z::operator+=(const N& n) {
        return operator+=(n.Value);
    }
    
    inline Z Z::operator-(const N& n) const {
        return operator-(n.Value);
    }
    
    inline Z& Z::operator-=(const N& n) {
        return operator-=(n.Value);
    }
    
    inline Z Z::operator*(const N& n) const {
        return operator*(n.Value);
    }
    
    inline Z& Z::operator*=(const N& n) {
        return operator*=(n.Value);
    }
    
    inline division<Z> Z::divide(const N& n) const {
        return divide(n.Value);
    }
    
    inline N Z::abs() const {
        N n;
        __gmp_abs_function::eval(n.Value.MPZ, MPZ);
        return n;
    }
    
    inline N square(Z &z) {
        return N::as(z * z);
    }
    
    inline N abs(N n) {
        return n;
    }
    
    inline N square(const N& n) {
        return n * n;
    }
    
    inline N norm(const N& n) {
        return n;
    }
    
    inline N norm(const Z& z) {
        return z.abs();
    }
    
    inline N abs(const Z& z) {
        return z.abs();
    }
    
    inline N abs(const N& n) {
        return n;
    }
    
}

namespace data::math::number {
    template <> 
    struct abs<gmp::N, gmp::Z> {
        gmp::N operator()(const gmp::Z& i) {
            gmp::abs(i);
        }
    };
}

namespace data::encoding::hexidecimal { 
    
    string write(const math::number::gmp::N& n);
    
}

namespace data::encoding::decimal {
    
    string write(const math::number::gmp::N& n);
    
}

std::ostream& operator<<(std::ostream& o, const data::math::number::gmp::N& n);

#endif
