// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_MODULAR
#define DATA_MATH_NUMBER_MODULAR

namespace data::math::number {
    template <typename X, typename mod>
    struct modular {
        X Value;
        
        modular() : Value{} {}
        modular(X x) : Value{x} {}
        
        bool valid() const {
            return Value < *mod{};
        }
        
        bool operator==(const modular& m) const {
            return Value == m.Value;
        }
        
        bool operator!=(const modular& m) const {
            return Value != m.Value;
        }
        
        bool operator<(const modular& m) const {
            return Value < m.Value;
        }
        
        bool operator>(const modular& m) const {
            return Value > m.Value;
        }
        
        bool operator<=(const modular& m) const {
            return Value <= m.Value;
        }
        
        bool operator>=(const modular& m) const {
            return Value >= m.Value;
        }
        
        modular operator+(const modular& m) const {
            return (Value + m.Value) % *mod{};
        }
        
        modular operator*(const modular& m) const {
            return (Value * m.Value) % *mod{};
        }
        
        modular operator-(const modular& m) const {
            if (Value < m.Value) *mod{} - ((m.Value - Value) % *mod{});
            return Value - m.Value;
        }
        
    };
    
}

#endif

