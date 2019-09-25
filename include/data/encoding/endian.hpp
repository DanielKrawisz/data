// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENDIAN
#define DATA_ENDIAN

#include <boost/endian/buffers.hpp>
#include <data/slice.hpp>

namespace data::endian {
    // A number that has a set endian ordering. 
    template <typename X, boost::endian::order> struct ordered;
    
    using order = boost::endian::order;
    constexpr order big = boost::endian::order::big;
    constexpr order little = boost::endian::order::little;
    
    // Can only be constructed if type half is half of type whole. 
    // Used to collect overflow bits when doing big number arithmetic.
    template <typename half, typename whole, order o>
    struct halves {
        halves() = delete;
    };
    
    template <order o> struct halves<uint32, uint64, o> {
        constexpr static bool is_signed = false;
        static uint32 greater(uint64);
        static uint32 lesser(uint64);
        static uint64 combine(uint32, uint32);
    };
    
    template <order o> struct halves<int32, int64, o> {
        constexpr static bool is_signed = true;
        static int32 greater(uint64);
        static int32 lesser(uint64);
        static int64 combine(int32, int32);
    };
    
    template <order o> struct halves<uint16, uint32, o> {
        constexpr static bool is_signed = false;
        static uint16 greater(uint32);
        static uint16 lesser(uint32);
        static uint32 combine(uint16, uint16);
    };
    
    template <order o> struct halves<int16, int32, o> {
        constexpr static bool is_signed = true;
        static int16 greater(uint32);
        static int16 lesser(uint32);
        static int32 combine(int16, int16);
    };
    
    template <order o> struct halves<byte, uint16, o> {
        constexpr static bool is_signed = false;
        static byte greater(uint32);
        static byte lesser(uint32);
        static uint16 combine(uint16, uint16);
    };
    
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
        
        ordered<X, o>() : Value{0} {}
        ordered<X, o>(X x) : Value{x} {}
        
        constexpr static order opposite = o == little ? big : little;
        
        ordered<X, o>& operator=(X x) {
            Value = native<X, o>::from(x.Value); 
            return *this;
        }
        
        ordered<X, o>& operator=(ordered<X, o>& x) {
            Value = x.Value; 
            return *this;
        }
        
        ordered<X, o>& operator=(ordered<X, opposite>& x) {
            Value = boost::endian::endian_reverse(x.Value); 
            return *this;
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
    
    template <> struct ordered<slice<byte>, big> {
        slice<byte> Data;
        
        ordered(slice<byte> d) : Data{d} {}
        
        size_t size() const {
            return Data.size();
        }
        
        byte& operator[](uint32 index) {
            return Data[index];
        }
        
        const byte& operator[](uint32 index) const {
            return Data[index];
        }

        ordered range(int32 begin, int32 end) const {
            return Data.range(begin, end);
        }

        ordered range(int32 begin) const {
            return Data.range(begin);
        }
    };
    
    template <> struct ordered<slice<byte>, little> {
        slice<byte> Data;
        
        ordered(slice<byte> d) : Data{d} {}
        
        size_t size() const {
            return Data.size();
        }
        
        byte& operator[](uint32 index) {
            return Data[size() - index - 1];
        }
        
        const byte& operator[](uint32 index) const {
            return Data[size() - index - 1];
        }

        ordered range(int32 begin, int32 end) const {
            size_t s = size();
            return Data.range((s - end - 1) % s, (s - begin - 1) % s);
        }

        ordered range(int32 begin) const {
            size_t s = size();
            return Data.range(0, (s - begin - 1) % s);
        }
    };
    
    template <size_t Size, endian::order o> struct ordered<slice<byte, Size>, o> {
        slice<byte, Size> Data;
        
        ordered(slice<byte, Size> d) : Data{d} {}
        
        size_t size() const {
            return Size;
        }
        
        byte& operator[](uint32 index) {
            return ordered<slice<byte>, o>{slice<byte>{Data}}[index];
        }
        
        const byte& operator[](uint32 index) const {
            return ordered<slice<byte>, o>{slice<byte>{Data}}[index];
        }

        ordered range(int32 b, int32 e) const {
            return ordered<slice<byte>, o>{slice<byte>{Data}}.range(b, e);
        }

        ordered range(int32 b) const {
            return ordered<slice<byte>, o>{slice<byte>{Data}}.range(b);
        }
        
        template <size_t b, size_t e>
        ordered<slice<byte, meta::unsigned_minus<e, b>::result>, o> range() const {
            return slice<byte>{Data}.range<(Size - e - 1) % Size, (Size - b - 1) % Size>();
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

