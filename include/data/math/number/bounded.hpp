// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BOUNDED
#define DATA_MATH_NUMBER_BOUNDED

#include <type_traits>
#include <iterator>
#include <data/encoding/halves.hpp>
#include <data/math/arithmetic.hpp>
#include <data/math/commutative.hpp>
#include <data/math/associative.hpp>
#include <data/math/division.hpp>
#include <data/cross.hpp>

namespace data::math::number {
    
    // satisfies range<byte>
    template <bool is_signed, endian::order, size_t size> class bounded;
    
    // satisfies range<byte>
    template <bool is_signed, endian::order> class bytes;
    
    template <endian::order r, size_t size>
    bounded<true, r, size> operator-(const bounded<true, r, size>&);
    
    template <endian::order r>
    bytes<true, r> operator-(const bytes<true, r>&);
    
}

namespace data::math { 
    template <endian::order r, size_t size>
    using uint = typename number::bounded<false, r, size>;
    
    template <endian::order r, size_t size>
    using sint = typename number::bounded<true, r, size>;
    
    template <endian::order r>
    using N_bytes = typename number::bytes<false, r>;
    
    template <endian::order r>
    using Z_bytes = typename number::bytes<true, r>;
    
    template <size_t size>
    using uint_little = typename number::bounded<false, endian::little, size>;
    
    template <size_t size>
    using uint_big = typename number::bounded<false, endian::big, size>;
    
    template <size_t size>
    using sint_little = typename number::bounded<true, endian::little, size>;
    
    template <size_t size>
    using sint_big = typename number::bounded<true, endian::big, size>;
    
    using N_bytes_little = typename number::bytes<false, endian::little>;
    
    using Z_bytes_little = typename number::bytes<true, endian::little>;
    
    using N_bytes_big = typename number::bytes<false, endian::big>;
    
    using Z_bytes_big = typename number::bytes<true, endian::big>;

    // plus and times are associative and commutative
    // for both bounded and bytes. 
    template <bool is_signed, endian::order r, size_t size> 
    struct commutative<plus<number::bounded<is_signed, r, size>>, 
        number::bounded<is_signed, r, size>> {};
    
    template <bool is_signed, endian::order r, size_t size> 
    struct associative<plus<number::bounded<is_signed, r, size>>, 
        number::bounded<is_signed, r, size>> {};
    
    template <bool is_signed, endian::order r, size_t size> 
    struct commutative<times<number::bounded<is_signed, r, size>>, 
        number::bounded<is_signed, r, size>> {};
    
    template <bool is_signed, endian::order r, size_t size> 
    struct associative<times<number::bounded<is_signed, r, size>>, 
        number::bounded<is_signed, r, size>> {};
        
    template <bool is_signed, endian::order r> 
    struct commutative<plus<number::bytes<is_signed, r>>, 
        number::bytes<is_signed, r>> {};
    
    template <bool is_signed, endian::order r> 
    struct associative<plus<number::bytes<is_signed, r>>, 
        number::bytes<is_signed, r>> {};
    
    template <bool is_signed, endian::order r> 
    struct commutative<times<number::bytes<is_signed, r>>, 
        number::bytes<is_signed, r>> {};
    
    template <bool is_signed, endian::order r> 
    struct associative<times<number::bytes<is_signed, r>>, 
        number::bytes<is_signed, r>> {};
    /*
    
    // identity operations for plus and times. 
    template <bool is_signed, endian::order r, size_t size> 
    struct identity<plus<number::bounded<is_signed, r, size>>, number::bounded<is_signed, r, size>> {
        number::bounded<is_signed, r, size> operator()() {
            return {0};
        }
    };
    
    template <bool is_signed, endian::order r, size_t size>
    struct identity<times<number::bounded<is_signed, r, size>>, number::bounded<is_signed, r, size>> {
        number::bounded<is_signed, r, size> operator()() {
            return {1};
        }
    };
    
    template <bool is_signed, endian::order r> 
    struct identity<plus<number::bytes<is_signed, r>>, number::bytes<is_signed, r>> {
        number::bytes<is_signed, r> operator()() {
            return {0};
        }
    };
    
    template <bool is_signed, endian::order r>
    struct identity<times<number::bytes<is_signed, r>>, number::bytes<is_signed, r>> {
        number::bytes<is_signed, r> operator()() {
            return {1};
        }
    };
    
    // inverse addition. 
    template <bool is_signed, endian::order r, size_t size>
    struct inverse<
        number::bounded<is_signed, r, size>, 
        plus<number::bounded<is_signed, r, size>>, 
        number::bounded<is_signed, r, size>> {
        static number::bounded<is_signed, r, size> invert(
            const number::bounded<is_signed, r, size>& a, 
            const plus<number::bounded<is_signed, r, size>>&, 
            const number::bounded<is_signed, r, size>& b) {
            return a - b;
        }
    };
    
    template <bool is_signed, endian::order r>
    struct inverse<
        number::bytes<is_signed, r>, 
        plus<number::bytes<is_signed, r>>, 
        number::bytes<is_signed, r>> {
        static number::bytes<is_signed, r> invert(
            const number::bytes<is_signed, r>& a, 
            const plus<number::bytes<is_signed, r>>&, 
            const number::bytes<is_signed, r>& b) {
            return a - b;
        }
    };*/
    
    template <bool is_signed, endian::order r, size_t size> 
    struct divide<number::bounded<is_signed, r, size>, uint64> {
        division<number::bounded<is_signed, r, size>, uint64> operator()(const number::bounded<is_signed, r, size>&, uint64) const;
    };
    
    template <bool is_signed, endian::order r>  
    struct divide<number::bytes<is_signed, r>, uint64> {
        division<number::bytes<is_signed, r>, uint64> operator()(const number::bytes<is_signed, r>&, uint64) const;
    };
    
    template <bool is_signed, endian::order r, size_t size> 
    struct divide<number::bounded<is_signed, r, size>, number::bounded<is_signed, r, size>> {
        division<number::bounded<is_signed, r, size>> operator()(
            const number::bounded<is_signed, r, size>&, 
            const number::bounded<is_signed, r, size>&) const;
    };
    
    template <bool is_signed, endian::order r>  
    struct divide<number::bytes<is_signed, r>, number::bytes<is_signed, r>> {
        division<number::bytes<is_signed, r>> operator()(
            const number::bytes<is_signed, r>&, 
            const number::bytes<is_signed, r>&) const;
    };
    
}

namespace data::encoding::hexidecimal {
    template <bool is_signed, endian::order r> string write(const math::number::bytes<is_signed, r>&);
    template <bool is_signed, endian::order r, size_t size> string write(const math::number::bounded<is_signed, r, size>&);
}

namespace data::encoding::integer {
    template <endian::order r> string write(const math::number::bytes<true, r>&);
    template <endian::order r, size_t size> string write(const math::number::bounded<true, r, size>&);
}

namespace data::encoding::natural {
    template <endian::order r> string write(const math::number::bytes<false, r>&);
    template <endian::order r, size_t size> string write(const math::number::bounded<false, r, size>&);
}

namespace data::math::number::meta {
    
    template <bool is_signed, endian::order> struct get_fundamental_arithmatic;
    
    template <bool is_signed, endian::order r> 
    using fundamental_arithmatic = typename get_fundamental_arithmatic<is_signed, r>::type;
    
    template <> struct get_fundamental_arithmatic<false, endian::order::little> {
        using type = uint32_little;
    };
    
    template <> struct get_fundamental_arithmatic<false, endian::order::big> {
        using type = uint32_big;
    };
    
    template <> struct get_fundamental_arithmatic<true, endian::order::little> {
        using type = int32_little;
    };
    
    template <> struct get_fundamental_arithmatic<true, endian::order::big> {
        using type = int32_big;
    };
    
    template <typename> struct get_unordered;
    
    template <typename X> 
    using unordered = typename get_unordered<X>::type;
    
    template <endian::order o> struct get_unordered<endian::arithmetic<o, false, 4>> {
        using type = uint32;
    };
    
    template <endian::order o> struct get_unordered<endian::arithmetic<o, true, 4>> {
        using type = int32;
    };
    
    template <endian::order r, size_t ... sizes> struct oriented {
        oriented operator<<(int) const;
        oriented operator>>(int) const;
    };

}

namespace data::math::number {
    template <bool is_signed, endian::order r> 
    class bytes : public data::bytes {
        using bit32 = meta::fundamental_arithmatic<is_signed, r>;
        using bit64 = twice<meta::unordered<bit32>>;
        
    public:
        bytes();
        bytes(bit64);
        
        explicit bytes(string_view);
        explicit bytes(bytes_view);
        
        static bytes read(bytes_view);
        static bytes read(string_view);
        
        bool operator==(const bytes&) const;
        bool operator!=(const bytes&) const;
        bool operator>=(const bytes&) const;
        bool operator<=(const bytes&) const;
        bool operator>(const bytes&) const;
        bool operator<(const bytes&) const;
        
        bytes& operator++();
        bytes& operator--();
        
        bytes operator++(int);
        bytes operator--(int);
        
        bytes operator~() const;
        
        bytes& operator+=(const bytes&);
        bytes& operator-=(const bytes&);
        bytes& operator*=(const bytes&);
        bytes& operator%=(const bytes&);
        bytes& operator/=(const bytes&);
        
        bytes operator+(const bytes&) const;
        bytes operator-(const bytes&) const;
        bytes operator*(const bytes&) const;
        bytes operator%(const bytes&) const;
        bytes operator/(const bytes&) const;
        
        bytes& operator<<=(int);
        bytes& operator>>=(int);
        
        bytes trim() const;
        
    };
    
    template <size_t x> struct next_multiple_of_4 {
        static const size_t Remainder = x % 4;
        static const size_t Value = Remainder == 0 ? x : x + 4 - Remainder;
    };
    
    template <bool is_signed, endian::order r, size_t x> 
    class bounded : public byte_array<x> {
        using bit32 = meta::fundamental_arithmatic<is_signed, r>;
        using bit64 = twice<meta::unordered<bit32>>;
        
    public:
        bounded();
        bounded(bit64);
        explicit bounded(const byte_array<x>&);
        explicit bounded(string_view);
        
        bool operator==(const bounded&) const;
        bool operator!=(const bounded&) const;
        bool operator>=(const bounded&) const;
        bool operator<=(const bounded&) const;
        bool operator>(const bounded&) const;
        bool operator<(const bounded&) const;
        
        bounded& operator++();
        bounded& operator--();
        
        bounded operator++(int);
        bounded operator--(int);
        
        bounded operator~() const;
        
        bounded operator+(const bounded&) const;
        bounded operator-(const bounded&) const;
        bounded operator*(const bounded&) const;
        bounded operator%(const bounded&) const;
        bounded operator/(const bounded&) const;
        
        bounded& operator+=(const bounded&);
        bounded& operator-=(const bounded&);
        bounded& operator*=(const bounded&);
        bounded& operator%=(const bounded&);
        bounded& operator/=(const bounded&);
        bounded& operator^=(const bounded&);
        bounded& operator|=(const bounded&);
        
        bounded& operator<<=(int);
        bounded& operator>>=(int);
        
        bounded operator<<(int);
        bounded operator>>(int);
        
        static bounded min();
        static bounded max();
        
        static N_bytes<r> modulus();
        
    };
    
}

namespace data::math::number::meta {
    
    template <typename T, endian::order r, size_t ... sizes>
    void mask(oriented<T, r, sizes...>&, uint32 size);
    
    template <typename T, endian::order r, size_t ... sizes>
    void bit_negate(oriented<T, r, sizes...>&);
    
    template <typename T, endian::order r, size_t ... sizes>
    void bit_add(oriented<T, r, sizes...>&, const oriented<T, r, sizes...>&, const oriented<T, r, sizes...>&);
    
    template <typename T, endian::order r, size_t ... sizes>
    void bit_or(oriented<T, r, sizes...>&, const oriented<T, r, sizes...>&, const oriented<T, r, sizes...>&);
    
    template <typename T, endian::order r, size_t ... sizes>
    void add(oriented<T, r, sizes...>&, const oriented<T, r, sizes...>&, const oriented<T, r, sizes...>&);
    
    template <typename T, endian::order r, size_t ... sizes>
    void subtract(oriented<T, r, sizes...>&, const oriented<T, r, sizes...>&, const oriented<T, r, sizes...>&);
    
    template <typename T, endian::order r, size_t ... sizes>
    void multiply(oriented<T, r, sizes...>&, const oriented<T, r, sizes...>&, const oriented<T, r, sizes...>&);
    
}

namespace data::math::number {
    template <bool is_signed, endian::order r, size_t x> 
    bounded<is_signed, r, x>::bounded(string_view s) {
        throw 0;
    }
    
    template <bool is_signed, endian::order r> 
    bytes<is_signed, r>::bytes(string_view s) {
        throw 0;
    }
    
    // define pre-increment/decrement in terms of adding and subtracting 1. 
    template <bool is_signed, endian::order r>
    inline bytes<is_signed, r>& bytes<is_signed, r>::operator++() {
        return operator+=(1);
    }
    
    template <bool is_signed, endian::order r, size_t x>
    inline bounded<is_signed, r, x>& bounded<is_signed, r, x>::operator++() {
        return operator+=(1);
    }
    
    template <bool is_signed, endian::order r>
    inline bytes<is_signed, r>& bytes<is_signed, r>::operator--() {
        return operator-=(1);
    }
    
    template <bool is_signed, endian::order r, size_t x>
    inline bounded<is_signed, r, x>& bounded<is_signed, r, x>::operator--() {
        return operator-=(1);
    }
    
    // post increment 
    template <bool is_signed, endian::order r>
    inline bytes<is_signed, r> bytes<is_signed, r>::operator++(int) {
        auto n = *this;
        operator++();
        return n;
    }
    
    template <bool is_signed, endian::order r, size_t x>
    inline bounded<is_signed, r, x> bounded<is_signed, r, x>::operator++(int) {
        auto n = *this;
        operator++();
        return n;
    }
    
    template <bool is_signed, endian::order r>
    inline bytes<is_signed, r> bytes<is_signed, r>::operator--(int) {
        auto n = *this;
        operator--();
        return n;
    }
    
    template <bool is_signed, endian::order r, size_t x>
    inline bounded<is_signed, r, x> bounded<is_signed, r, x>::operator--(int) {
        auto n = *this;
        operator--();
        return n;
    }
    
    template <bool is_signed, endian::order r>
    inline bytes<is_signed, r> bytes<is_signed, r>::operator~() const {
        bytes x = *this;
        bit_negate(x.Words);
        mask(x.Words, x.size());
        return x;
    }
    
    template <bool is_signed, endian::order r, size_t size>
    inline bounded<is_signed, r, size> bounded<is_signed, r, size>::operator~() const {
        bounded x = *this;
        bit_negate(x.Words);
        mask(x.Words, x.size());
        return x;
    }
    
    template <bool is_signed, endian::order r, size_t size>
    inline bounded<is_signed, r, size>& bounded<is_signed, r, size>::operator^=(const bounded& b) {
        bit_add(Words, Words, b.Words);
        mask(Words, size());
        return *this;
    }
    
    template <bool is_signed, endian::order r, size_t size>
    inline bounded<is_signed, r, size>& bounded<is_signed, r, size>::operator|=(const bounded& b) {
        bit_add(Words, Words, b.Words);
        mask(Words, size());
        return *this;
    }
    
    template <bool is_signed, endian::order r>
    inline bytes<is_signed, r> bytes<is_signed, r>::operator+(const bytes& b) const {
        bytes x(size() + 1);
        add(x.Words, Words, b.Words);
        mask(x.Words, x.size());
        return x.trim();
    }
    
    template <bool is_signed, endian::order r>
    inline bytes<is_signed, r>& bytes<is_signed, r>::operator+=(const bytes& b) {
        *this = *this + b;
        return *this;
    }
    
    template <bool is_signed, endian::order r, size_t size>
    inline bounded<is_signed, r, size> bounded<is_signed, r, size>::operator+(const bounded& b) const {
        bounded x = *this;
        x += b;
        return x;
    }
    
    template <bool is_signed, endian::order r, size_t size>
    inline bounded<is_signed, r, size>& bounded<is_signed, r, size>::operator+=(const bounded& b) {
        add(Words, Words, b.Words);
        mask(Words, size());
        return *this;
    }
    
    template <bool is_signed, endian::order r>
    inline bytes<is_signed, r> bytes<is_signed, r>::operator-(const bytes& b) const {
        bytes x(size() + 1);
        subtract(x.Words, Words, b.Words);
        mask(x.Words, x.size());
        return x.trim();
    }
    
    template <bool is_signed, endian::order r>
    inline bytes<is_signed, r>& bytes<is_signed, r>::operator-=(const bytes& b) {
        *this = *this - b;
        return *this;
    }
    
    template <bool is_signed, endian::order r, size_t size>
    inline bounded<is_signed, r, size> bounded<is_signed, r, size>::operator-(const bounded& b) const {
        bounded x = *this;
        x -= b;
        return x;
    }
    
    template <bool is_signed, endian::order r, size_t size>
    inline bounded<is_signed, r, size>& bounded<is_signed, r, size>::operator-=(const bounded& b) {
        subtract(Words, Words, b.Words);
        mask(Words, size());
        return *this;
    }
    
    template <bool is_signed, endian::order r>
    inline bytes<is_signed, r> bytes<is_signed, r>::operator*(const bytes& b) const {
        bytes x(size() * b.size());
        subtract(x.Words, Words, b.Words);
        mask(x.Words, x.size());
        return x.trim();
    }
    
    template <bool is_signed, endian::order r>
    inline bytes<is_signed, r>& bytes<is_signed, r>::operator*=(const bytes& b) {
        *this = *this * b;
        return this;
    }
    
    template <bool is_signed, endian::order r, size_t size>
    inline bounded<is_signed, r, size> bounded<is_signed, r, size>::operator*(const bounded& b) const {
        bounded x = *this;
        x *= b;
        return x;
    }
    
    template <bool is_signed, endian::order r, size_t size>
    bounded<is_signed, r, size>& bounded<is_signed, r, size>::operator*=(const bounded& b) {
        multiply(Words, Words, b.Words);
        mask(Words, size());
        return *this;
    }
}

#endif
