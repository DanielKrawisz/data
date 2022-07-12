// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CROSS
#define DATA_CROSS

#include <data/sequence.hpp>
#include <data/slice.hpp>
#include <data/encoding/endian/endian.hpp>
#include <data/valid.hpp>
#include <data/math/arithmetic.hpp>
#include <data/stream.hpp>

namespace data {
    
    // The cartesian product. 
    // it is the same as a vector with some slight improvements. 
    template <typename X> struct cross : std::vector<X> {
        cross();
        cross(size_t size);
        cross(size_t size, X fill);
        
        cross(std::initializer_list<X> x);
        
        template<sequence list>
        explicit cross(list l);
        
        bool valid() const {
            for (const X& x : *this) if (!data::valid(x)) return false;
            return true;
        }
        
        X& operator[](int i) {
            size_t size = this->size();
            if (size == 0) throw std::out_of_range{"cross size 0"};
            if (i < 0 || i >= size) return this->operator[]((i + size) % size);
            return std::vector<X>::operator[](i);
        }
        
        const X& operator[](int i) const {
            size_t size = this->size();
            if (size == 0) throw std::out_of_range{"cross size 0"};
            if (i < 0 || i >= size) return this->operator[]((i + size) % size);
            return std::vector<X>::operator[](i);
        }
        
        explicit operator slice<X>();
        
        slice<X> range(int);
        slice<X> range(int, int);
        
        slice<X> range(data::range r);
        
        explicit cross(std::vector<X>&& v) : std::vector<X>{v} {}
        
    protected:
        void fill(const X& x) {
            for (X& z : *this) z = x;
        }
        
    };
    
    template <typename X>
    std::ostream& operator<<(std::ostream& o, const cross<X>& s);
    
    template <typename X, size_t... > struct array;
    
    template <typename X, size_t size> struct array<X, size> : public cross<X> {
        array() : cross<X>(size) {}
        
        array(std::initializer_list<X> x) : cross<X>{x} {
            if (x.size() != size) throw std::invalid_argument{"invalid size"};
        }
        
        static array filled(const X& x) {
            array n{};
            n.fill(x);
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
    
    template <typename X, size_t size, size_t... sizes> struct array<X, size, sizes...> : public cross<array<X, sizes...>> {
        array() : cross<array<X, sizes...>>(size) {}
    };
    
    struct bytes;
    
    std::ostream &operator<<(std::ostream &o, const bytes &s);
    
    template <std::unsigned_integral word>
    struct bytestring : public cross<word> {
        using cross<word>::cross;
        bytestring(view<word> v) : cross<word>(v.size()) {
            std::copy(v.begin(), v.end(), this->begin());
        }
        
        operator view<word>() const {
            return bytes_view(cross<word>::data(), cross<word>::size());
        }
        
        operator slice<word>() {
            return {this->data(), this->size()};
        }
        
        explicit operator bytes() const;
        
        void bit_negate() {
            math::arithmetic::bit_negate<word>(this->end(), this->begin(), this->begin());
        }
        
        void bit_shift_left(uint32 x, bool fill = false);
        void bit_shift_right(uint32 x, bool fill = false);
    };
    
    template <std::unsigned_integral word>
    bytestring<word> operator~(const bytestring<word> &b);
    
    template <std::unsigned_integral word>
    bytestring<word> operator<<(const bytestring<word> &b, int32 i);
    
    template <std::unsigned_integral word>    
    bytestring<word> operator>>(const bytestring<word> &b, int32 i);
    
    struct bytes : bytestring<byte> {
        using bytestring<byte>::bytestring;
        bytes(bytestring<byte>&& b) : bytestring<byte>{b} {}
        
        static bytes from_hex(string_view s);
        static bytes from_string(string_view s) {
            bytes b;
            b.resize(s.size());
            for (int i = 0; i < s.size(); i++) b[i] = static_cast<byte>(s[i]);
            return b;
        }
        
        template <typename X, typename... P>
        static bytes write(size_t size, X x, P... p) {
            bytes b(size);
            bytes_writer w(b.begin(), b.end());
            write(w, x, p...);
            return b;
        }
        
    private:
        template <typename X>
        static writer<byte> &write(writer<byte> &w, X x) {
            return w << x;
        }
        
        template <typename X, typename... P>
        static writer<byte> &write(writer<byte> &w, X x, P... p) {
           return write(write(w, x), p...);
        }
    };
    
    bytes operator~(const bytes &b);
    
    bytes operator<<(const bytes &b, int i);
    
    bytes operator>>(const bytes &b, int i);
    
    template <std::unsigned_integral word, size_t...> struct bytes_array;
    
    template <std::unsigned_integral word, size_t size> struct bytes_array<word, size> : public bytestring<word> {
        bytes_array() : bytestring<word>(size) {}
        
        static bytes_array filled(const word& x) {
            bytes_array n{};
            n.fill(x);
            return n;
        }
        
        operator view<word>() const;
        operator slice<word, size>();
        
        bytes_array operator|(const slice<word, size> a) const {
            bytes_array n(*this);
            n.bit_or(a);
            return n;
        }
        
        bytes_array operator&(const slice<word, size> a) const {
            bytes_array n(*this);
            n.bit_and(a);
            return n;
        }
        
        bytes_array operator^(const slice<word, size> a) const {
            bytes_array n(*this);
            n.bit_xor(a);
            return n;
        }
        
        static bytes_array fill(byte b) {
            bytes_array n{};
            for (byte& z : n) z = b;
            return n;
        }
        
        explicit operator bytes() const;
        
    protected:
        void bit_and(const slice<word, size> a) {
            math::arithmetic::bit_and<word>(this->end(), this->begin(), const_cast<const word*>(this->data()), a.begin());
        }
        
        void bit_or(const slice<word, size> a) {
            math::arithmetic::bit_or<word>(this->end(), this->begin(), const_cast<const word*>(this->data()), a.begin());
        }
        
        void bit_xor(const slice<word, size> a) {
            math::arithmetic::bit_xor<word>(this->end(), this->begin(), const_cast<const word*>(this->data()), a.begin());
        }
        
    };
    
    template <std::unsigned_integral word, size_t size> 
    bytes_array<word, size> operator~(const bytes_array<word, size> &b);
    
    template <std::unsigned_integral word, size_t size> 
    bytes_array<word, size> operator<<(const bytes_array<word, size> &b, int32 i);
    
    template <std::unsigned_integral word, size_t size> 
    bytes_array<word, size> operator>>(const bytes_array<word, size> &b, int32 i);
    
    template <std::unsigned_integral word>
    std::ostream inline &operator<<(std::ostream &o, const bytestring<word> &s);
    
    template <std::unsigned_integral word, size_t size> 
    std::ostream inline &operator<<(std::ostream &o, const bytes_array<word, size> &s);
    
    template <size_t size> using byte_array = bytes_array<byte, size>;
    
    template <std::unsigned_integral word>
    writer<word> inline &operator<<(writer<word> &w, const bytestring<word> &x) {
        w.write(x.data(), x.size());
        return w;
    }
    
    template <std::unsigned_integral word, size_t size> 
    writer<word> inline &operator<<(writer<word> &w, const bytes_array<word, size> &x) {
        w.write(x.data(), size);
        return w;
    }
    
    template <std::unsigned_integral word>
    reader<word> inline &operator>>(reader<byte> &r, bytestring<word> &x) {
        r.read(x.data(), x.size());
        return r;
    }
    
    template <std::unsigned_integral word, size_t size> 
    reader<word> inline &operator>>(reader<word> &r, bytes_array<word, size> &x) {
        r.read(x.data(), size);
        return r;
    }
    
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
    
    template <std::unsigned_integral word>
    bytestring<word> operator~(const bytestring<word> &b) {
        bytestring n(b);
        n.bit_negate();
        return n;
    }
    
    template <std::unsigned_integral word>
    bytestring<word> operator<<(const bytestring<word> &b, int32 i) {
        bytestring n(b);
        if (i < 0) n.bit_shift_right(-i);
        else n.bit_shift_left(i);
        return n;
    }
    
    template <std::unsigned_integral word>    
    bytestring<word> operator>>(const bytestring<word> &b, int32 i) {
        bytestring n(b);
        if (i < 0) n.bit_shift_left(-i);
        else n.bit_shift_right(i);
        return n;
    }
    
    template <std::unsigned_integral word>
    std::ostream inline &operator<<(std::ostream &o, const bytestring<word> &s) {
        return o << bytes(s);
    }
    
    template <std::unsigned_integral word, size_t size> 
    std::ostream inline &operator<<(std::ostream &o, const bytes_array<word, size> &s) {
        return o << bytes(s);
    }
    
    template <std::unsigned_integral word>
    void bytestring<word>::bit_shift_left(uint32 x, bool fill) {
        auto it = (byte*)this->data();
        math::arithmetic::bit_shift_left(it, it + this->size() * sizeof(word), x, fill);
    }
    
    template <std::unsigned_integral word>
    void bytestring<word>::bit_shift_right(uint32 x, bool fill) {
        math::arithmetic::bit_shift_right(
            std::reverse_iterator{(byte*)this->data() + this->size() * sizeof(word)}, 
            std::reverse_iterator{(byte*)this->data()}, x, fill);
    }
    
    template <std::unsigned_integral word, size_t size> 
    bytes_array<word, size> operator~(const bytes_array<word, size> &b) {
        bytes_array<word, size> n;
        math::arithmetic::bit_negate<word>(n.end(), n.begin(), b.begin());
        return n;
    }
    
    template <std::unsigned_integral word, size_t size> 
    bytes_array<word, size> operator<<(const bytes_array<word, size> &b, int32 i) {
        bytes_array<word, size> n(b);
        if (i < 0) n.bit_shift_right(-i);
        else n.bit_shift_left(i);
        return n;
    }
    
    template <std::unsigned_integral word, size_t size> 
    bytes_array<word, size> operator>>(const bytes_array<word, size> &b, int32 i) {
        bytes_array<word, size> n(b);
        if (i < 0) n.bit_shift_left(-i);
        else n.bit_shift_right(i);
        return n;
    }
    
    bytes inline operator~(const bytes &b) {
        return ~static_cast<bytestring<byte>>(b);
    }
    
    bytes inline operator<<(const bytes &b, int i) {
        return static_cast<bytestring<byte>>(b) << i;
    }
    
    bytes inline operator>>(const bytes &b, int i) {
        return static_cast<bytestring<byte>>(b) >> i;
    }
    
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
    template<sequence list>
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
    
    template <std::unsigned_integral word, size_t size>
    inline bytes_array<word, size>::operator slice<word, size>() {
        return {this->data()};
    }
    
    template <std::unsigned_integral word, size_t size>
    inline bytes_array<word, size>::operator view<word>() const {
        return {this->data(), size};
    }
    
    template <std::unsigned_integral word>
    bytestring<word>::operator bytes() const {
        size_t x = sizeof(word) * this->size();
        bytes b(x);
        const byte *w = (const byte*)(this->data());
        std::copy(w, w + x, b.begin());
        return b;
    }
    
    template <std::unsigned_integral word, size_t size> 
    bytes_array<word, size>::operator bytes() const {
        size_t x = sizeof(word) * size;
        bytes b(x);
        const byte *w = (const byte*)(this->data());
        std::copy(w, w + x, b.begin());
        return b;
    }
    
}

#endif


