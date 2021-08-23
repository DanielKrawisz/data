// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CROSS
#define DATA_CROSS

#include <data/iterable.hpp>
#include <data/slice.hpp>
#include <data/encoding/endian/endian.hpp>
#include <data/valid.hpp>
#include <data/math/arithmetic.hpp>

namespace data {
    
    // The cartesian product. 
    // it is the same as a vector with some slight improvements. 
    template <typename X> struct cross : std::vector<X> {
        cross();
        cross(size_t size);
        cross(size_t size, X fill);
        
        cross(std::initializer_list<X> x);
        
        template<typename list, typename constraint = interface::sequence<list>>
        explicit cross(list l);
        
        bool valid() const {
            for (const X& x : *this) if (!data::valid(x)) return false;
            return true;
        }
        
        X& operator[](int i) {
            return std::vector<X>::operator[](i < 0 ? std::vector<X>::size() + i : i);
        }
        
        const X& operator[](int i) const {
            return std::vector<X>::operator[](i < 0 ? std::vector<X>::size() + i : i);
        }
        
        explicit operator slice<X>();
        
        slice<X> range(int);
        slice<X> range(int, int);
        
        slice<X> range(data::range r);
        
    protected:
        void fill(const X& x) {
            for (X& z : *this) z = x;
        }
    };
    
    template <typename X>
    std::ostream& operator<<(std::ostream& o, const cross<X>& s) {
        auto b = s.begin();
        while (true) {
            if (b == s.end()) return o << "]";
            else if (b == s.begin()) o << "[";
            else o << ", ";
            o << *b;
            b++;
        }
    }
    
    template <typename X, size_t size> struct array : public cross<X> {
        array() : cross<X>(size) {}
        
        static array filled(const X& x) {
            array n{};
            for (X& z : n) z = x;
            return n;
        }
        
    protected:
        template <typename iterator>
        array(iterator it) : array() {
            for (X& x : *this) {
                x = *it; 
                it++;
            }
        }
    };
    
    struct bytes : cross<byte> {
        using cross<byte>::cross;
        
        operator bytes_view() const {
            return bytes_view(cross<byte>::data(), cross<byte>::size());
        }
        
        bytes(bytes_view x) : cross<byte>(x.size()) {
            std::copy(x.begin(), x.end(), cross<byte>::begin());
        }
        
        static bytes from_hex(string_view s);
        static bytes from_string(string_view s) {
            bytes b;
            b.resize(s.size());
            for (int i = 0; i < s.size(); i++) b[i] = static_cast<byte>(s[i]);
            return b;
        }
        
        bytes operator~() const {
            bytes n(*this);
            n.bit_negate();
            return n;
        }
        
        operator slice<byte>() {
            return {this->data(), this->size()};
        }
        
    protected:
        void bit_negate() {
            math::arithmetic::bit_negate(this->end(), this->begin(), this->begin());
        }
    };
    
    template <size_t size> struct byte_array : public array<byte, size> {
        byte_array() : array<byte, size>{} {}
        byte_array(bytes_view v) : array<byte, size>{} {
            if (v.size() == size) std::copy(v.begin(), v.end(), this->begin());
        }
        
        operator bytes_view() const;
        
        byte_array operator~() const {
            byte_array n;
            math::arithmetic::bit_negate(n.end(), n.begin(), this->begin());
            return n;
        }
        
        byte_array operator<<(int32) const;
        byte_array operator>>(int32) const;
        
        byte_array operator|(const slice<byte, size> a) const {
            byte_array n(*this);
            n.bit_or(a);
            return n;
        }
        
        byte_array operator&(const slice<byte, size> a) const {
            byte_array n(*this);
            n.bit_and(a);
            return n;
        }
        
        byte_array operator^(const slice<byte, size> a) const {
            byte_array n(*this);
            n.bit_xor(a);
            return n;
        }
        
        operator slice<byte, size>();
        
        static byte_array fill(byte b) {
            byte_array n{};
            for (byte& z : n) z = b;
            return n;
        }
        
    protected:
        void bit_negate() {
            math::arithmetic::bit_negate(this->end(), this->begin(), this->begin());
        }
        
        void bit_and(const slice<byte, size> a) {
            math::arithmetic::bit_and(this->end(), this->begin(), this->begin(), a.begin());
        }
        
        void bit_or(const slice<byte, size> a) {
            math::arithmetic::bit_or(this->end(), this->begin(), this->begin(), a.begin());
        }
        
        void bit_xor(const slice<byte, size> a) {
            math::arithmetic::bit_xor(this->end(), this->begin(), this->begin(), a.begin());
        }
        
    };
    
    std::ostream &operator<<(std::ostream &o, const bytes &s);
    
    template <size_t size> 
    std::ostream &operator<<(std::ostream &o, const byte_array<size> &s);
    
    template <typename X>
    inline cross<X>::cross() : std::vector<X>{} {}
    
    template <typename X>
    inline cross<X>::cross(size_t size) : std::vector<X>(size) {}
    
    template <typename X>
    inline cross<X>::cross(size_t size, X fill) : std::vector<X>(size) {
        for (auto it = std::vector<X>::begin(); it < std::vector<X>::end(); it++) *it = fill;
    }
    
    template <typename X>
    inline cross<X>::cross(std::initializer_list<X> x) : std::vector<X>{x} {}
    
    template <typename X>
    template<typename list, typename constraint>
    cross<X>::cross(list l) : cross{} {
        std::vector<X>::resize(data::size(l));
        auto b = std::vector<X>::begin();
        while (!l.empty()) {
            *b = l.first();
            l = l.rest();
        }
    }
    
    template <typename X>
    inline cross<X>::operator slice<X>() {
        return slice<X>(static_cast<std::vector<X>&>(*this));
    }
        
    template <typename X>
    inline slice<X> cross<X>::range(int e) {
        return operator slice<X>().range(e);
    }
    
    template <typename X>
    inline slice<X> cross<X>::range(int b, int e) {
        return operator slice<X>().range(e);
    }
        
    template <typename X>
    inline slice<X> cross<X>::range(data::range r) {
        return operator slice<X>().range(r);
    }
    
    template <size_t size>
    inline byte_array<size>::operator slice<byte, size>() {
        return {array<byte, size>::data()};
    }
    
    template <size_t size>
    inline byte_array<size>::operator bytes_view() const {
        return {array<byte, size>::data(), size};
    }
    
}

#endif


