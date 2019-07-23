// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BOUNDED
#define DATA_MATH_NUMBER_BOUNDED

#include <data/words.hpp>
#include <data/math/group.hpp>
#include <data/math/sign.hpp>
#include <data/math/number/division.hpp>

namespace data {
        
    namespace math::number::bounded {
    
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
            
            array(const array& a) : parent{static_cast<const parent&>(a)} {}
            array(const parent& a) : parent{a} {}
            array(array&& a) : parent{a} {}
            array(parent&& a) : parent{a} {}
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
            using bit32 = uint32;
            using bit64 = uint64;
            using words_type = data::words<size, bit32, bit64>;
            
            using ray = array<size, number, bit32, bit64>;
            using ray::operator-=;
            using ray::operator+=;
            using ray::operator*=;
            using ray::operator<<=;
            using ray::operator>>=;
            
            number() : ray{} {}
            number(bit32 x) : ray{x} {}
            number(const number& n) : ray{static_cast<const ray&>(n)} {}
            number(number&& n) : ray{static_cast<ray&&>(n)} {}
            number(const std::array<byte, size*4>& a) : ray{a} {};
            number(std::array<byte, size*4>&& a) : ray{a} {};
            number(vector<byte>& v) : ray{v} {};
            
            number& operator=(const number&) const;
            
            // power
            number operator^(const number&) const;
            number& operator^=(const number&);
            
            bool operator<(const number&) const;
            bool operator<=(const number&) const;
        };
        
        template <uint32_t size>
        struct number<size, true> : public array<size, number<size, true>, int32, int64> {
            using bit32 = int32;
            using bit64 = int64;
            using words_type = data::words<size, bit32, bit64>;
            
            using ray = array<size, number<size, true>, bit32, bit64>;
            using ray::operator-=;
            using ray::operator+=;
            using ray::operator*=;
            using ray::operator<<=;
            using ray::operator>>=;
            
            number() : ray{} {}
            number(bit32 x) : ray{x} {}
            number(const number& n) : ray{static_cast<const ray&>(n)} {}
            number(number&& n) : ray{static_cast<ray&&>(n)} {}
            number(const typename ray::parent& n) : ray{n} {}
            number(typename ray::parent&& n) : ray{n} {}
            number(vector<byte>& v) : ray{v} {};
            
            number& operator=(const number&) const;
            
            // power
            number operator^(const number<size, false>&) const;
            number& operator^=(const number<size, false>&);
            
            bool operator<(const number& d) const;
            bool operator<=(const number& d) const;
        };
    
        template <uint32_t size, bool is_signed>
        typename number<size, is_signed>::words_type words(number<size, is_signed>& n) {
            return {n};
        }
        
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
            return d <= bounded{*this};
        }
        
        template <uint32_t size, typename bounded, typename bit32, typename bit64>
        inline bool array<size, bounded, bit32, bit64>::operator>=(const bounded& d) const {
            return d < bounded{*this};
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
