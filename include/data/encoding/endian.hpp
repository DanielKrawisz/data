// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENDIAN
#define DATA_ENDIAN

#include <boost/endian/buffers.hpp>

namespace data::endian {
    // A number that has a set endian ordering. 
    template <typename X, boost::endian::order> struct ordered;
    
    using order = boost::endian::order;
    constexpr order big = boost::endian::order::big;
    constexpr order little = boost::endian::order::little;
    
    // convert native to and from the given endian ordering. 
    template <typename X, order> struct native;
    
    template <typename X> struct native<X, big> {
        static X from(const X x) {
            return boost::endian::native_to_big(x);
        }
        
        static X to(const X x) {
            return boost::endian::big_to_native(x);
        }
    };
    
    template <typename X> struct native<X, little> {
        static X from(const X x) {
            return boost::endian::native_to_little(x);
        }
        
        static X to(const X x) {
            return boost::endian::little_to_native(x);
        }
    };
    
    template <typename X, order o> struct ordered {
        X Value;
        ordered<X, o>(X x) : Value{native<X, o>::from(x)} {}
        
        ordered<X, o>() : Value{0} {}
        
        static ordered as(X x) {
            ordered oo{};
            oo.Value = x;
            return oo;
        }
        
        constexpr static order opposite = o == little ? big : little;
        
        ordered<X, o>& operator=(const typename std::remove_reference<X>::type x) {
            Value = native<typename std::remove_reference<X>::type, o>::from(x); 
            return *this;
        }
        
        ordered<X, o>& operator=(const ordered<X, o>& x) {
            Value = x.Value; 
            return *this;
        }
        
        ordered<X, o>& operator=(const ordered<X, opposite>& x) {
            Value = boost::endian::endian_reverse(x.Value); 
            return *this;
        }
        
        operator X() const {
            return native<X, o>::to(Value);
        }
        
        bool operator==(const ordered<X, o>& x) const {
            return Value == x.Value;
        }
        
        bool operator==(const ordered<X, opposite>& x) const {
            return Value == boost::endian::endian_reverse(x.Value);
        }
        
        bool operator==(const X& x) const {
            return Value == native<X, o>::from(x);
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
    
    template <typename half, typename whole, order o>
    ordered<half, o> lesser_half(ordered<whole, o> w);
    
    template <typename half, typename whole, order o>
    ordered<half, o> greater_half(ordered<whole, o> w);
    
    template <typename X, order o>
    inline bool ordered<X, o>::operator<(const ordered<X, o>& n) const {
        return native<X, o>::to(Value) < native<X, o>::to(n.Value);
    }
    
    template <typename X, order o>
    inline bool ordered<X, o>::operator>(const ordered<X, o>& n) const {
        return native<X, o>::to(Value) > native<X, o>::to(n.Value);
    }
    
    template <typename X, order o>
    inline bool ordered<X, o>::operator<=(const ordered<X, o>& n) const {
        return native<X, o>::to(Value) <= native<X, o>::to(n.Value);
    }
    
    template <typename X, order o>
    inline bool ordered<X, o>::operator>=(const ordered<X, o>& n) const {
        return native<X, o>::to(Value) >= native<X, o>::to(n.Value);
    }
    
    template <typename X, order o>
    inline ordered<X, o> ordered<X, o>::operator+(const ordered<X, o>& n) const {
        return native<X, o>::from(
            native<X, o>::to(Value) + native<X, o>::to(n.Value));
    }
    
    template <typename X, order o>
    inline ordered<X, o> ordered<X, o>::operator-(const ordered<X, o>& n) const {
        return native<X, o>::from(
            native<X, o>::to(Value) - native<X, o>::to(n.Value));
    }
    
    template <typename X, order o>
    inline ordered<X, o> ordered<X, o>::operator*(const ordered<X, o>& n) const {
        return native<X, o>::from(
            native<X, o>::to(Value) * native<X, o>::to(n.Value));
    }
    
    template <typename X, order o>
    inline ordered<X, o> ordered<X, o>::operator/(const ordered<X, o>& n) const {
        return native<X, o>::from(
            native<X, o>::to(Value) / native<X, o>::to(n.Value));
    }
    
    template <typename X, order o>
    inline ordered<X, o> ordered<X, o>::operator%(const ordered<X, o>& n) const {
        return native<X, o>::from(
            native<X, o>::to(Value) % native<X, o>::to(n.Value));
    }
    
}

#endif

