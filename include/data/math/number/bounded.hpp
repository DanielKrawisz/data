// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BOUNDED
#define DATA_MATH_NUMBER_BOUNDED

#include <data/words.hpp>
#include <data/math/group.hpp>
#include <data/math/sign.hpp>
#include <data/math/number/division.hpp>
#include <data/tools/managed.hpp>

namespace data {
        
    namespace math::number::bounded {
    
        template <typename bounded, uint32 n, typename bit32, typename bit64>
        struct array : managed::bytes<n> {
            using parent = managed::bytes<n>;
            using word = ordered<bit64, endian::order::big>;
            using index = uint32;
            
            static const index size = n;
            static const index last = n - 1;
            
            array() : parent(std::array<byte, size>{}) {}
            
            array(const array& a) : parent{static_cast<const parent&>(a)} {}
            array(const std::array<byte, size>& d) : parent{d} {}
            array(array&& a) : parent{static_cast<const parent&&>(a)} {}
            array(std::array<byte, size>&& d) : parent{d} {}
            array(vector<byte>& v);
            
            static array zero() {
                return array{};
            }
            
            array& operator=(const array& a);
            
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
            
            using words_type = ::data::words<size, bit32, bit64>;
            using parent::value;
            
            words_type words() {
                return words_type{value()};
            }
            
            const words_type words() const {
                return words_type{value()};
            }
            
            array(bit32 x) : array() {
                words()[last] = x;
            }
            
            operator std::array<byte, size>&() {
                return value();
            }
            
            operator const std::array<byte, size>&() {
                return value();
            }
            
            constexpr static math::group::abelian<bounded> is_group{}; 
        };
        
        template <uint32 size, bool is_signed> struct number;
        
        template <uint32 size>
        struct number<size, false> : public array<number<size, false>, size, uint32, uint64> {
            using bit32 = uint32;
            using bit64 = uint64;
            using words_type = data::words<size, bit32, bit64>;
            
            using ray = array<number, size, bit32, bit64>;
            using ray::operator-=;
            using ray::operator+=;
            using ray::operator*=;
            using ray::operator<<=;
            using ray::operator>>=;
            
            number() : ray{} {}
            number(bit32 x) : ray{x} {}
            number(const number& n) : ray{static_cast<const ray&>(n)} {}
            number(number&& n) : ray{static_cast<ray&&>(n)} {}
            number(const std::array<byte, size>& a) : ray{a} {};
            number(std::array<byte, size>&& a) : ray{a} {};
            number(vector<byte>& v) : ray{v} {};
            
            number& operator=(const number&) const;
            
            // power
            number operator^(const number&) const;
            number& operator^=(const number&);
            
            bool operator<(const number&) const;
            bool operator<=(const number&) const;
        };
        
        template <uint32 size>
        struct number<size, true> : public array<number<size, true>, size, int32, int64> {
            using bit32 = int32;
            using bit64 = int64;
            using words_type = data::words<size, bit32, bit64>;
            
            using ray = array<number, size, bit32, bit64>;
            using ray::operator-=;
            using ray::operator+=;
            using ray::operator*=;
            using ray::operator<<=;
            using ray::operator>>=;
            
            number() : ray{} {}
            number(bit32 x) : ray{x} {}
            number(const number& n) : ray{static_cast<const ray&>(n)} {}
            number(number&& n) : ray{static_cast<ray&&>(n)} {}
            number(const std::array<byte, size>& a) : ray{a} {};
            number(std::array<byte, size>&& a) : ray{a} {};
            number(vector<byte>& v) : ray{v} {};
            
            number& operator=(const number&) const;
            
            // power
            number operator^(const number<size, false>&) const;
            number& operator^=(const number<size, false>&);
            
            bool operator<(const number& d) const;
            bool operator<=(const number& d) const;
        };
        
        template <uint32 size> using uint = number<size, false>;
        
    }
    
    using uint160 = math::number::bounded::uint<20>;
    using uint256 = math::number::bounded::uint<32>;
    using uint512 = math::number::bounded::uint<64>;
    
    using int160 = math::number::bounded::number<20, true>;
    using int256 = math::number::bounded::number<32, true>;
    using int512 = math::number::bounded::number<64, true>;

    namespace math::number::bounded {
        
        template <typename bounded, uint32 size, typename bit32, typename bit64>
        inline bool array<bounded, size, bit32, bit64>::operator>(const bounded& d) const {
            return d <= bounded{parent::value()};
        }
        
        template <typename bounded, uint32 size, typename bit32, typename bit64>
        inline bool array<bounded, size, bit32, bit64>::operator>=(const bounded& d) const {
            return d < bounded{parent::value()};
        }
        
        template <typename bounded, uint32 size, typename bit32, typename bit64>
        inline bool array<bounded, size, bit32, bit64>::operator==(const bounded& d) const {
            return static_cast<const parent&>(*this) == static_cast<const parent&>(d);
        }
        
        template <typename bounded, uint32 size, typename bit32, typename bit64>
        inline bool array<bounded, size, bit32, bit64>::operator!=(const bounded& d) const {
            return static_cast<const parent&>(*this) != static_cast<const parent&>(d);
        }
        
        template <typename bounded, uint32 size, typename bit32, typename bit64> 
        inline bounded array<bounded, size, bit32, bit64>::operator-() const {
            return bounded{0} - *this;
        }
        
        template <typename bounded, uint32 size, typename bit32, typename bit64> 
        inline bounded array<bounded, size, bit32, bit64>::operator~() const {
            bounded n = *this;
            words_type{n}.bit_negate();
            return n;
        }
        
        template <typename bounded, uint32 size, typename bit32, typename bit64>
        inline bounded& array<bounded, size, bit32, bit64>::operator|=(const bounded& n) {
            words_type::bit_and(*this, n, *this);
            return *this;
        }
        
        template <typename bounded, uint32 size, typename bit32, typename bit64>
        inline bounded& array<bounded, size, bit32, bit64>::operator^=(const bounded& n) {
            words_type::bit_and(*this, n, *this);
            return *this;
        }
        
        template <typename bounded, uint32 size, typename bit32, typename bit64> 
        inline bounded array<bounded, size, bit32, bit64>::operator+(const bounded& n) const {
            bounded result;
            words_type::plus(*this, n, result);
            return result;
        }
        
        template <typename bounded, uint32 size, typename bit32, typename bit64> 
        inline bounded array<bounded, size, bit32, bit64>::operator<<(uint32 bits) const {
            bounded result;
            words_type::bit_shift_left(*this, bits, result);
            return result;
        }
    
        template <typename bounded, uint32 size, typename bit32, typename bit64>
        inline bounded array<bounded, size, bit32, bit64>::operator>>(uint32 bits) const {
            bounded result;
            words_type::bit_shift_right(*this, bits, result);
            return result;
        }
        
        template <typename bounded, uint32 size, typename bit32, typename bit64> 
        inline bounded array<bounded, size, bit32, bit64>::operator<<(int32 bits) const {
            bounded result;
            words_type::bit_shift_left(*this, bits, result);
            return result;
        }
    
        template <typename bounded, uint32 size, typename bit32, typename bit64>
        inline bounded array<bounded, size, bit32, bit64>::operator>>(int32 bits) const {
            bounded result;
            words_type::bit_shift_right(*this, bits, result);
            return result;
        }
        
        template <typename bounded, uint32 size, typename bit32, typename bit64>
        inline bounded& array<bounded, size, bit32, bit64>::operator<<=(uint32 bits) {
            words_type::bit_shift_left(*this, bits, *this);
            return *this;
        }
        
        template <typename bounded, uint32 size, typename bit32, typename bit64>
        inline bounded& array<bounded, size, bit32, bit64>::operator>>=(uint32 bits) {
            words_type::bit_shift_right(*this, bits, *this);
            return *this;
        }
        
        template <typename bounded, uint32 size, typename bit32, typename bit64>
        inline bounded& array<bounded, size, bit32, bit64>::operator<<=(int32 bits) {
            words_type::bit_shift_left(*this, bits, *this);
            return *this;
        }
        
        template <typename bounded, uint32 size, typename bit32, typename bit64>
        inline bounded& array<bounded, size, bit32, bit64>::operator>>=(int32 bits) {
            words_type::bit_shift_right(*this, bits, *this);
            return *this;
        }
        
        template <typename bounded, uint32 size, typename bit32, typename bit64> 
        inline bounded array<bounded, size, bit32, bit64>::operator-(const bounded& n) const {
            bounded result;
            words_type::minus(*this, n, result);
            return result;
        }
    
        template <typename bounded, uint32 size, typename bit32, typename bit64>
        inline bounded array<bounded, size, bit32, bit64>::operator*(const bounded& n) const {
            bounded result;
            array::times(*this, n, result);
            return result;
        }
        
        template <typename bounded, uint32 size, typename bit32, typename bit64>
        inline bounded& array<bounded, size, bit32, bit64>::operator+=(const bounded& n) {
            return array::plus(*this, n,*this);
        }
        
        template <typename bounded, uint32 size, typename bit32, typename bit64>
        inline bounded& array<bounded, size, bit32, bit64>::operator-=(const bounded& n) {
            return array::plus(*this, n,*this);
        }
        
        template <typename bounded, uint32 size, typename bit32, typename bit64>
        inline bounded& array<bounded, size, bit32, bit64>::operator*=(const bounded& n) {
            return operator=(operator*(n));
        }
        
        template <typename bounded, uint32 size, typename bit32, typename bit64>
        inline math::number::division<bounded> 
        array<bounded, size, bit32, bit64>::divide(const bounded& n) const {
            return math::number::division<number<size, true>>::divide(*this, n);
        }
        
        template <typename bounded, uint32 size, typename bit32, typename bit64>
        inline bounded array<bounded, size, bit32, bit64>::operator/(const bounded& n) const {
            return divide(n).Quotient;
        }
        
        template <typename bounded, uint32 size, typename bit32, typename bit64>
        inline bounded array<bounded, size, bit32, bit64>::operator%(const bounded& n) const {
            return divide(n).Remainder;
        }
        
    }

}
#endif
