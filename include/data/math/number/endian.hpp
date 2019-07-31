// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENDIAN
#define DATA_ENDIAN

#include <boost/endian/buffers.hpp>

namespace data {
    
    namespace math::number {
        template <typename X, boost::endian::order> struct ordered;
    }
    
    namespace endian {
        using order = boost::endian::order;
        template <typename X> math::number::ordered<X, endian::order::big> big(X x);
        template <typename X> math::number::ordered<X, endian::order::little> little(X x);
        
        template <typename half, typename whole>
        struct split {
            split() = delete;
        };
        
        template <> struct split<uint32, uint64> {
            constexpr static bool is_signed = false;
            static uint32 greater(uint64);
            static uint32 lesser(uint64);
        };
        
        template <> struct split<int32, int64> {
            constexpr static bool is_signed = true;
            static int32 greater(uint64);
            static int32 lesser(uint64);
        };
        
        template <> struct split<uint16, uint32> {
            constexpr static bool is_signed = false;
            static uint16 greater(uint32);
            static uint16 lesser(uint32);
        };
        
        template <> struct split<int16, int32> {
            constexpr static bool is_signed = true;
            static int16 greater(uint32);
            static int16 lesser(uint32);
        };
        
        template <typename X, boost::endian::order> struct native {
            static X from(const X);
            static X to(const X);
        };
    }
    
    namespace math::number {
    
        template <typename X, endian::order o> struct ordered {
            X Value;
            
            ordered<X, o>() : Value{0} {}
            ordered<X, o>(X x) : Value{x} {}
            
            ordered<X, o>& operator=(ordered<X, o>& x) {
                Value = x.Value; 
                return *this;
            }
            
            bool operator==(const ordered<X, o>& x) const {
                return Value == x.Value;
            }
            
            bool operator!=(const ordered<X, o>& x) const {
                return Value != x.Value;
            }
            
            bool operator<(const ordered<X, o>&) const;
            bool operator>(const ordered<X, o>&) const;
            bool operator<=(const ordered<X, o>&) const;
            bool operator>=(const ordered<X, o>&) const;
            
            ordered<X, o> operator~() const {
                return ~Value;
            }
            
            ordered<X, o> operator|(const ordered<X, o>& x) const {
                return Value | x.Value;
            }
            
            ordered<X, o> operator^(const ordered<X, o>& x) const {
                return Value ^ x.Value;
            }
            
            ordered<X, o> operator+(const ordered<X, o>&) const;
            ordered<X, o> operator-(const ordered<X, o>&) const;
            ordered<X, o> operator*(const ordered<X, o>&) const;
            ordered<X, o> operator/(const ordered<X, o>&) const;
            ordered<X, o> operator%(const ordered<X, o>&) const;
            
            ordered<X, o> operator<<(uint32 n) const {
                return Value << n;
            }
            
            ordered<X, o> operator>>(uint32 n) const {
                return Value >> n;
            }
            
            ordered<X, o>& operator|=(const ordered<X, o>& x) {
                return operator=(operator|(x));
            }
            
            ordered<X, o>& operator^=(const ordered<X, o>& x) {
                return operator=(operator^(x));
            }
            
            ordered<X, o>& operator+=(const ordered<X, o>& x) {
                return operator=(operator+(x));
            }
            ordered<X, o>& operator-=(const ordered<X, o>& x) {
                return operator=(operator-(x));
            }
            ordered<X, o>& operator*=(const ordered<X, o>& x) {
                return operator=(operator*(x));
            }
            ordered<X, o>& operator/=(const ordered<X, o>& x) {
                return operator=(operator/(x));
            }
            ordered<X, o>& operator%=(const ordered<X, o>& x) {
                return operator=(operator%(x));
            }
            
            ordered<X, o>& operator<<=(uint32 x) {
                return operator=(operator<<(x));
            }
            ordered<X, o>& operator>>=(uint32 x) {
                return operator=(operator>>(x));
            }
        };
        
        template <typename half, typename whole, boost::endian::order o>
        ordered<half, o> lesser_half(ordered<whole, o> w);
        
        template <typename half, typename whole, boost::endian::order o>
        ordered<half, o> greater_half(ordered<whole, o> w);
        
        template <typename X, endian::order o>
        inline bool ordered<X, o>::operator<(const ordered<X, o>& n) const {
            return endian::native<X, o>::to(Value) < endian::native<X, o>::to(n.Value);
        }
        
        template <typename X, endian::order o>
        inline bool ordered<X, o>::operator>(const ordered<X, o>& n) const {
            return endian::native<X, o>::to(Value) > endian::native<X, o>::to(n.Value);
        }
        
        template <typename X, endian::order o>
        inline bool ordered<X, o>::operator<=(const ordered<X, o>& n) const {
            return endian::native<X, o>::to(Value) <= endian::native<X, o>::to(n.Value);
        }
        
        template <typename X, endian::order o>
        inline bool ordered<X, o>::operator>=(const ordered<X, o>& n) const {
            return endian::native<X, o>::to(Value) >= endian::native<X, o>::to(n.Value);
        }
        
        template <typename X, endian::order o>
        inline ordered<X, o> ordered<X, o>::operator+(const ordered<X, o>& n) const {
            return endian::native<X, o>::from(
                endian::native<X, o>::to(Value) + endian::native<X, o>::to(n.Value));
        }
        
        template <typename X, endian::order o>
        inline ordered<X, o> ordered<X, o>::operator-(const ordered<X, o>& n) const {
            return endian::native<X, o>::from(
                endian::native<X, o>::to(Value) - endian::native<X, o>::to(n.Value));
        }
        
        template <typename X, endian::order o>
        inline ordered<X, o> ordered<X, o>::operator*(const ordered<X, o>& n) const {
            return endian::native<X, o>::from(
                endian::native<X, o>::to(Value) * endian::native<X, o>::to(n.Value));
        }
        
        template <typename X, endian::order o>
        inline ordered<X, o> ordered<X, o>::operator/(const ordered<X, o>& n) const {
            return endian::native<X, o>::from(
                endian::native<X, o>::to(Value) / endian::native<X, o>::to(n.Value));
        }
        
        template <typename X, endian::order o>
        inline ordered<X, o> ordered<X, o>::operator%(const ordered<X, o>& n) const {
            return endian::native<X, o>::from(
                endian::native<X, o>::to(Value) % endian::native<X, o>::to(n.Value));
        }
    }
    
    namespace endian {
        template <typename X>
        inline math::number::ordered<X, endian::order::big> big(X x) {
            return math::number::ordered<X, endian::order::big>{boost::endian::native_to_big(x)};
        }
        
        template <typename X>
        inline math::number::ordered<X, endian::order::little> little(X x) {
            return math::number::ordered<X, endian::order::little>{boost::endian::native_to_big(x)};
        }
    }

}

#endif

