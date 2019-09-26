// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BOUNDED
#define DATA_MATH_NUMBER_BOUNDED

#include <data/encoding/words.hpp>
#include <data/math/group.hpp>
#include <data/math/sign.hpp>
#include <data/math/ordered.hpp>
#include <data/math/number/division.hpp>
#include <data/container.hpp>
#include <type_traits>

namespace data {
        
    namespace math::number::bounded {
    
        // A type for treating sequences of bytes as numbers. 
        template <
            typename indexed, // inner type. Some kind of array 
            size_t n,         // size (number of bytes)
            endian::order o>    // endianness. 
        struct ordered : public container::array<indexed, byte, n> {
            using container::array<indexed, byte, n>::array;
            using container::array<indexed, byte, n>::Array;
            
            using order = typename endian::order;
            
            constexpr static order endian = o;
            constexpr static order opposite_endian = endian == order::little ? order::big : order::little;
            
            ordered(ordered<indexed, n, opposite_endian> reversed) {
                std::reverse_copy(reversed.begin(), reversed.end(), Array.begin());
            } 
            
        };
    
        // A type for treating sequences of bytes as numbers. 
        template <
            typename bounded, // type derived from array. 
            typename indexed, // inner type. Some kind of array 
            size_t n,         // size (number of bytes)
            typename bit32,   // either int32 or uint32 
            typename bit64,   // either int64 or uint64
            endian::order o>    // endianness. 
        struct array : public ordered<indexed, n, o> {
            using ordered<indexed, n, o>::ordered;
            using ordered<indexed, n, o>::Array;
            using ordered<indexed, n, o>::endian;
            using ordered<indexed, n, o>::opposite_endian;
            
            using word = endian::ordered<bit64, endian>;
            using number_type = ::data::encoding::number<n, bit32, bit64, endian>;
            using words_type = typename number_type::words;
            
            words_type words() {
                return words_type{Array};
            }
            
            const words_type words() const {
                return words_type{Array};
            }
            
            array(bit32 x) : array() {
                words()[0].Value = endian::native<bit32, o>::to(x);
            }
            
            array(endian::ordered<bit32, endian> x) : array() {
                words()[0].Value = x.Value;
            }
            
            array(endian::ordered<bit32, opposite_endian> x) : array() {
                words()[0].Value = boost::endian::endian_reverse(x.Value);
            }

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
                if (bounded{Array} < 0) return math::negative;
                if (bounded{Array} > 0) return math::positive;
                return math::zero;
            }
                
            bounded operator-() const;
            bounded operator-(const bounded&) const;
            bounded operator+(const bounded&) const;
            bounded operator*(const bounded&) const;
                
            bounded& operator+=(const bounded&);
            bounded& operator-=(const bounded&);
            bounded& operator*=(const bounded&);
            
            bounded operator-(const bit32&) const;
            bounded operator+(const bit32&) const;
            bounded operator*(const bit32&) const;
                
            bounded& operator+=(const bit32&);
            bounded& operator-=(const bit32&);
            bounded& operator*=(const bit32&);
            
            math::number::division<bounded> divide(const bounded&) const;
            bounded operator/(const bounded&) const;
            bounded operator%(const bounded&) const;
            
            constexpr static math::group::abelian<bounded> is_group{}; 
        };
        
        template <typename indexed, size_t size, endian::order o, bool is_signed> struct number;
        
        template <typename indexed, size_t size, endian::order o>
        struct number<indexed, size, o, false> : 
            public array<number<indexed, size, o, false>, indexed, size, uint32, uint64, o> {
            using bit32 = uint32;
            using bit64 = uint64;
            
            using ray = array<number, indexed, size, bit32, bit64, o>;
            using ray::ray;
            using ray::operator-=;
            using ray::operator+=;
            using ray::operator*=;
            using ray::operator<<=;
            using ray::operator>>=;
            using typename ray::words_type;
            
            number(number<indexed, size, ray::opposite_endian, false> n) : 
                number{static_cast<ordered<indexed, size, ray::opposite_endian>>(n)} {}
            
            // power
            number operator^(const number&) const;
            number& operator^=(const number&);
            
            bool operator<(const number&) const;
            bool operator<=(const number&) const;
        };
        
        template <typename indexed, size_t size, endian::order o>
        struct number<indexed, size, o, true> : public array<number<indexed, size, o, true>, indexed, size, int32, int64, o> {
            using bit32 = int32;
            using bit64 = int64;
            
            using ray = array<number, indexed, size, bit32, bit64, o>;
            using ray::ray;
            using ray::operator-=;
            using ray::operator+=;
            using ray::operator*=;
            using ray::operator<<=;
            using ray::operator>>=;
            using typename ray::words_type;
            
            number(number<indexed, size, ray::opposite_endian, true> n) : 
                number{static_cast<ordered<indexed, size, ray::opposite_endian>>(n)} {}
            
            // power
            number operator^(const number<indexed, size, o, false>&) const;
            number& operator^=(const number<indexed, size, o, false>&);
            
            bool operator<(const number& d) const;
            bool operator<=(const number& d) const;
        };
        
    }
    
    template <size_t size> using uint = 
        math::number::bounded::number<std::array<byte, size>, size, endian::order::big, false>;
    template <size_t size> using integer = 
        math::number::bounded::number<std::array<byte, size>, size, endian::order::big, true>;
    
    using uint160 = uint<20>;
    using uint256 = uint<32>;
    using uint512 = uint<64>;
    
    using int160 = integer<20>;
    using int256 = integer<32>;
    using int512 = integer<64>;

    namespace math::number::bounded {
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o>
        inline bool array<bounded, indexed, size, bit32, bit64, o>::operator>(const bounded& d) const {
            return d <= bounded{Array};
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o>
        inline bool array<bounded, indexed, size, bit32, bit64, o>::operator>=(const bounded& d) const {
            return d < bounded{Array};
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o>
        inline bool array<bounded, indexed, size, bit32, bit64, o>::operator==(const bounded& d) const {
            return Array == d.Array;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o>
        inline bool array<bounded, indexed, size, bit32, bit64, o>::operator!=(const bounded& d) const {
            return Array != d.Array;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o> 
        inline bounded array<bounded, indexed, size, bit32, bit64, o>::operator-() const {
            return bounded{0} - *this;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o> 
        inline bounded array<bounded, indexed, size, bit32, bit64, o>::operator~() const {
            bounded n = *this;
            words_type{n}.bit_negate();
            return n;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o>
        inline bounded& array<bounded, indexed, size, bit32, bit64, o>::operator|=(const bounded& n) {
            words_type::bit_and(*this, n, *this);
            return *this;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o>
        inline bounded& array<bounded, indexed, size, bit32, bit64, o>::operator^=(const bounded& n) {
            words_type::bit_and(*this, n, *this);
            return *this;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o> 
        inline bounded array<bounded, indexed, size, bit32, bit64, o>::operator<<(uint32 bits) const {
            bounded result;
            words_type::bit_shift_left(*this, bits, result);
            return result;
        }
    
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o>
        inline bounded array<bounded, indexed, size, bit32, bit64, o>::operator>>(uint32 bits) const {
            bounded result;
            words_type::bit_shift_right(*this, bits, result);
            return result;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o> 
        inline bounded array<bounded, indexed, size, bit32, bit64, o>::operator<<(int32 bits) const {
            bounded result;
            words_type::bit_shift_left(words(), bits, result.words());
            return result;
        }
    
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o>
        inline bounded array<bounded, indexed, size, bit32, bit64, o>::operator>>(int32 bits) const {
            bounded result;
            words_type::bit_shift_right(*this, bits, result);
            return result;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o>
        inline bounded& array<bounded, indexed, size, bit32, bit64, o>::operator<<=(uint32 bits) {
            words_type::bit_shift_left(*this, bits, *this);
            return *this;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o>
        inline bounded& array<bounded, indexed, size, bit32, bit64, o>::operator>>=(uint32 bits) {
            words_type::bit_shift_right(*this, bits, *this);
            return *this;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o>
        inline bounded& array<bounded, indexed, size, bit32, bit64, o>::operator<<=(int32 bits) {
            words_type::bit_shift_left(*this, bits, *this);
            return *this;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o>
        inline bounded& array<bounded, indexed, size, bit32, bit64, o>::operator>>=(int32 bits) {
            words_type::bit_shift_right(*this, bits, *this);
            return *this;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o> 
        inline bounded array<bounded, indexed, size, bit32, bit64, o>::operator-(const bounded& n) const {
            bounded result;
            words_type::minus(*this, n, result);
            return result;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o> 
        inline bounded array<bounded, indexed, size, bit32, bit64, o>::operator+(const bounded& n) const {
            bounded result;
            words_type::plus(*this, n, result);
            return result;
        }
    
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o>
        inline bounded array<bounded, indexed, size, bit32, bit64, o>::operator*(const bounded& n) const {
            bounded result;
            array::times(*this, n, result);
            return result;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o>
        inline bounded& array<bounded, indexed, size, bit32, bit64, o>::operator+=(const bounded& n) {
            return array::plus(*this, n,*this);
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o>
        inline bounded& array<bounded, indexed, size, bit32, bit64, o>::operator-=(const bounded& n) {
            return array::plus(*this, n,*this);
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o>
        inline bounded& array<bounded, indexed, size, bit32, bit64, o>::operator*=(const bounded& n) {
            return operator=(operator*(n));
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o> 
        inline bounded array<bounded, indexed, size, bit32, bit64, o>::operator-(const bit32& n) const {
            bounded result;
            words_type::minus(*this, n, result);
            return result;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o> 
        inline bounded array<bounded, indexed, size, bit32, bit64, o>::operator+(const bit32& n) const {
            bounded result;
            words_type::plus(*this, n, result);
            return result;
        }
    
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o>
        inline bounded array<bounded, indexed, size, bit32, bit64, o>::operator*(const bit32& n) const {
            bounded result;
            array::times(*this, n, result);
            return result;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o>
        inline bounded& array<bounded, indexed, size, bit32, bit64, o>::operator+=(const bit32& n) {
            return array::plus(*this, n,*this);
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o>
        inline bounded& array<bounded, indexed, size, bit32, bit64, o>::operator-=(const bit32& n) {
            return array::plus(*this, n,*this);
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o>
        inline bounded& array<bounded, indexed, size, bit32, bit64, o>::operator*=(const bit32& n) {
            return operator=(operator*(n));
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o>
        inline math::number::division<bounded> 
        array<bounded, indexed, size, bit32, bit64, o>::divide(const bounded& n) const {
            return math::number::division<number<indexed, size, o, true>>::divide(*this, n);
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o>
        inline bounded array<bounded, indexed, size, bit32, bit64, o>::operator/(const bounded& n) const {
            return divide(n).Quotient;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o>
        inline bounded array<bounded, indexed, size, bit32, bit64, o>::operator%(const bounded& n) const {
            return divide(n).Remainder;
        }
    
        template <typename indexed, size_t size, endian::order o>
        bool number<indexed, size, o, false>::operator<(const number& n) const {
            words_type a = ray::words();
            words_type b = n.words();
            for (uint32_t i = 0; i < size; i++) {
                if (a[i] < b[i]) return true;
                if (a[i] > b[i]) return false;
            }
            
            return false;
        }
        
        template <typename indexed, size_t size, endian::order o>
        bool number<indexed, size, o, false>::operator<=(const number& n) const {
            words_type a = ray::words();
            words_type b = n.words();
            for (uint32_t i = 0; i < size; i++) {
                if (a[i] < b[i]) return true;
                if (a[i] > b[i]) return false;
            }
            
            return true;
        }
        
    }

}
#endif
