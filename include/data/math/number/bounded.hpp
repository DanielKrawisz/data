// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BOUNDED
#define DATA_MATH_NUMBER_BOUNDED

#include <data/types.hpp>
#include <data/math/group.hpp>
#include <data/math/sign.hpp>
#include <data/math/number/endian.hpp>
#include <data/tools/index_iterator.hpp>
#include <data/math/number/division.hpp>

namespace data {
    namespace math::number::bounded {
    
        template <uint32_t size, typename bit32, typename bit64>
        struct words {
            using word = data::number::endian::big<bit64>;
            using index = uint32;
            
            std::array<byte, size * 4>& Words;
            
            static const index last = size - 1;
            
            static bit32 lesser(word w);
            static bit32 greater(word w);
            
            bit32& operator[](index i) {
                if (i >= size) throw 0;
                return *(uint32*)(Words.data() + 4 * i);
            }
            
            static word extend(uint32);
            
            static bool overflow(word w) {
                return greater(w) != 0;
            }
            
            words(std::array<byte, 4 * size>& b) : Words{b} {}
            
            static void bit_negate(words);
            static void bit_or(const words, const words, words);
            static void bit_and(const words, const words, words);
            
            static void bit_shift_left(const words, uint32, words);
            static void bit_shift_right(const words, uint32, words);
            
            static void bit_shift_left(const words in, int32 bits, words w) {
                if (bits < 0) return bit_shift_right(in, -bits, w);
                return bit_shift_left(uint32(in), bits, w);
            }
            
            static void bit_shift_right(const words in, int32 bits, words w) {
                if (bits < 0) return bit_shift_left(in, -bits, w);
                return bit_shift_right(uint32(in), bits, w);
            }
            
            static void minus(const words, const words, words);
            static void plus(const words, const words, words);
            static void times(const words, const words, words);
            
            using iterator = index_iterator<words&, bit32&>;

            iterator begin();
            iterator end();
            const iterator begin() const;
            const iterator end() const;
    
        };
    
        template <uint32_t size, typename bounded, typename bit32, typename bit64>
        struct array : public std::array<byte, size * 4> {
            using parent = std::array<byte, size * 4>;
            using word = data::number::endian::big<bit64>;
            using w = words<size, bit32, bit64>;
            using index = uint32;
            
            static const index last = size - 1;
            
            array() : parent() {}
            array(bit32 x) : array() {
                w{*this}[last] = x;
            }
            
            array(const std::array<byte, 4 * size>& b);
            array(vector<byte>& v);
            
            static array zero() {
                return array{};
            }
            
            bounded& operator=(const bounded&);

            bool operator==(const bounded& d) const;
            bool operator!=(const bounded& d) const;
            
            bounded operator~() const;
            bounded operator|(const bounded&) const;
            bounded operator^(const bounded&) const;
            
            bounded& operator|=(const bounded&);
            bounded& operator^=(const bounded&);
                
            bounded operator<<(uint32) const;
            bounded operator>>(uint32) const;
                
            bounded operator<<(int32) const;
            bounded operator>>(int32) const;
                
            bounded& operator<<=(uint32);
            bounded& operator>>=(uint32);
                
            bounded& operator<<=(int32);
            bounded& operator>>=(int32);
            
            bool operator>(const bounded& d) const;
            bool operator>=(const bounded& d) const;
            
            math::sign sign() const {
                if (bounded{*this} < 0) return math::negative;
                if (bounded{*this} > 0) return math::positive;
                return math::zero;
            }
                
            bounded operator-() const;
            bounded operator-(const bounded&) const;
            bounded operator+(const bounded&) const;
            bounded operator*(const bounded&) const;
                
            bounded& operator+=(const bounded&);
            bounded& operator-=(const bounded&);
            bounded& operator*=(const bounded&);
            
            math::number::division<bounded> divide(const bounded&) const;
            bounded operator/(const bounded&) const;
            bounded operator%(const bounded&) const;
            
            constexpr static math::group::abelian<bounded> is_group{}; 
        };
        
        template <uint32_t size, bool is_signed> struct number;
        
        template <uint32_t size>
        struct number<size, false> : public array<size, number<size, false>, uint32, uint64> {
            using ray = array<size, number<size, false>, uint32, uint64>;
            using ray::operator-=;
            using ray::operator+=;
            using ray::operator*=;
            using ray::operator<<=;
            using ray::operator>>=;
            
            number<size, false>& operator=(const number<size, false>&) const;
            
            // power
            number<size, false> operator^(const number<size, false>&) const;
            number<size, false>& operator^=(const number<size, false>&);
            
            bool operator<(const number<size, false>& d) const;
            bool operator<=(const number<size, false>& d) const;
        };
        
        template <uint32_t size>
        struct number<size, true> : public array<size, number<size, true>, int32, int64> {
            using ray = array<size, number<size, true>, int32, int64>;
            using ray::operator-=;
            using ray::operator+=;
            using ray::operator*=;
            using ray::operator<<=;
            using ray::operator>>=;
            
            number<size, true>& operator=(const number<size, true>&) const;
            
            // power
            number<size, true> operator^(const number<size, false>&) const;
            number<size, true>& operator^=(const number<size, false>&);
            
            bool operator<(const number<size, true>& d) const;
            bool operator<=(const number<size, true>& d) const;
        };
        
    }

    using uint160 = math::number::bounded::number<5, false>;
    using uint256 = math::number::bounded::number<8, false>;
    using uint512 = math::number::bounded::number<16, false>;
    
    using int160 = math::number::bounded::number<5, true>;
    using int256 = math::number::bounded::number<8, true>;
    using int512 = math::number::bounded::number<16, true>;

    namespace math::number::bounded {
        
        template <uint32_t size, typename bounded, typename bit32, typename bit64>
        inline bool array<size, bounded, bit32, bit64>::operator>(const bounded& d) const {
            return d <= *this;
        }
        
        template <uint32_t size, typename bounded, typename bit32, typename bit64>
        inline bool array<size, bounded, bit32, bit64>::operator>=(const bounded& d) const {
            return d < *this;
        }
        
        template <uint32_t size, typename bounded, typename bit32, typename bit64>
        inline bool array<size, bounded, bit32, bit64>::operator==(const bounded& d) const {
            return static_cast<const parent&>(*this) == static_cast<const parent&>(d);
        }
        
        template <uint32_t size, typename bounded, typename bit32, typename bit64>
        inline bool array<size, bounded, bit32, bit64>::operator!=(const bounded& d) const {
            return static_cast<const parent&>(*this) != static_cast<const parent&>(d);
        }
        
        template <uint32 size, typename bounded, typename bit32, typename bit64> 
        inline bounded array<size, bounded, bit32, bit64>::operator-() const {
            return bounded{0} - *this;
        }
        
        
        template <uint32 size, typename bit32, typename bit64> 
        inline void words<size, bit32, bit64>::bit_negate(words w) {
            for (bit32& u : w) u = ~u;
        }
        
        template <uint32 size, typename bit32, typename bit64> 
        inline void words<size, bit32, bit64>::bit_and(
            const words a,
            const words b, 
            words result) {
            for (uint32 i = 0; i < size; i++) result[i] = a[i]^b[i];
        }
        
        template <uint32 size, typename bit32, typename bit64> 
        inline void words<size, bit32, bit64>::bit_or(
            const words a,
            const words b,
            words result) {
            for (uint32 i = 0; i < size; i++) result[i] = a[i]|b[i];
        }
        
        template <uint32 size, typename bounded, typename bit32, typename bit64> 
        inline bounded array<size, bounded, bit32, bit64>::operator~() const {
            bounded n = *this;
            w{n}.bit_negate();
            return n;
        }
        
        template <uint32_t size, typename bounded, typename bit32, typename bit64>
        inline bounded& array<size, bounded, bit32, bit64>::operator|=(const bounded& n) {
            w::bit_and(*this, n, *this);
            return *this;
        }
        
        template <uint32_t size, typename bounded, typename bit32, typename bit64>
        inline bounded& array<size, bounded, bit32, bit64>::operator^=(const bounded& n) {
            w::bit_and(*this, n, *this);
            return *this;
        }
        
        template <uint32 size, typename bounded, typename bit32, typename bit64> 
        inline bounded array<size, bounded, bit32, bit64>::operator+(const bounded& n) const {
            bounded result;
            w::plus(*this, n, result);
            return result;
        }
        
        template <uint32 size, typename bounded, typename bit32, typename bit64> 
        inline bounded array<size, bounded, bit32, bit64>::operator<<(uint32 bits) const {
            bounded result;
            w::bit_shift_left(*this, bits, result);
            return result;
        }
    
        template <uint32_t size, typename bounded, typename bit32, typename bit64>
        inline bounded array<size, bounded, bit32, bit64>::operator>>(uint32 bits) const {
            bounded result;
            w::bit_shift_right(*this, bits, result);
            return result;
        }
        
        template <uint32 size, typename bounded, typename bit32, typename bit64> 
        inline bounded array<size, bounded, bit32, bit64>::operator<<(int32 bits) const {
            bounded result;
            w::bit_shift_left(*this, bits, result);
            return result;
        }
    
        template <uint32_t size, typename bounded, typename bit32, typename bit64>
        inline bounded array<size, bounded, bit32, bit64>::operator>>(int32 bits) const {
            bounded result;
            w::bit_shift_right(*this, bits, result);
            return result;
        }
        
        template <uint32_t size, typename bounded, typename bit32, typename bit64>
        inline bounded& array<size, bounded, bit32, bit64>::operator<<=(uint32 bits) {
            w::bit_shift_left(*this, bits, *this);
            return *this;
        }
        
        template <uint32_t size, typename bounded, typename bit32, typename bit64>
        inline bounded& array<size, bounded, bit32, bit64>::operator>>=(uint32 bits) {
            w::bit_shift_right(*this, bits, *this);
            return *this;
        }
        
        template <uint32_t size, typename bounded, typename bit32, typename bit64>
        inline bounded& array<size, bounded, bit32, bit64>::operator<<=(int32 bits) {
            w::bit_shift_left(*this, bits, *this);
            return *this;
        }
        
        template <uint32_t size, typename bounded, typename bit32, typename bit64>
        inline bounded& array<size, bounded, bit32, bit64>::operator>>=(int32 bits) {
            w::bit_shift_right(*this, bits, *this);
            return *this;
        }
        
        template <uint32 size, typename bounded, typename bit32, typename bit64> 
        inline bounded array<size, bounded, bit32, bit64>::operator-(const bounded& n) const {
            bounded result;
            w::minus(*this, n, result);
            return result;
        }
    
        template <uint32_t size, typename bounded, typename bit32, typename bit64>
        inline bounded array<size, bounded, bit32, bit64>::operator*(const bounded& n) const {
            bounded result;
            array::times(*this, n, result);
            return result;
        }
        
        template <uint32_t size, typename bounded, typename bit32, typename bit64>
        inline bounded& array<size, bounded, bit32, bit64>::operator+=(const bounded& n) {
            return array::plus(*this, n,*this);
        }
        
        template <uint32_t size, typename bounded, typename bit32, typename bit64>
        inline bounded& array<size, bounded, bit32, bit64>::operator-=(const bounded& n) {
            return array::plus(*this, n,*this);
        }
        
        template <uint32_t size, typename bounded, typename bit32, typename bit64>
        inline bounded& array<size, bounded, bit32, bit64>::operator*=(const bounded& n) {
            return operator=(operator*(n));
        }
        
        template <uint32_t size, typename bounded, typename bit32, typename bit64>
        inline math::number::division<bounded> 
        array<size, bounded, bit32, bit64>::divide(const bounded& n) const {
            return math::number::division<number<size, true>>::divide(*this, n);
        }
        
        template <uint32_t size, typename bounded, typename bit32, typename bit64>
        inline bounded array<size, bounded, bit32, bit64>::operator/(const bounded& n) const {
            return divide(n).Quotient;
        }
        
        template <uint32_t size, typename bounded, typename bit32, typename bit64>
        inline bounded array<size, bounded, bit32, bit64>::operator%(const bounded& n) const {
            return divide(n).Remainder;
        }
        
    }

}
#endif
