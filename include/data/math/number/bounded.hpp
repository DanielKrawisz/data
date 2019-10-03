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
#include <data/math/number/gmp/N.hpp>
#include <data/math/number/gmp/Z.hpp>
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
            using methods = ::data::encoding::methods<n, bit32, bit64, endian>;
            using words_type = typename methods::words;
            
            words_type words() {
                return words_type{Array};
            }
            
            const words_type words() const {
                return words_type{Array};
            }

            bool operator==(const bounded& d) const;
            bool operator!=(const bounded& d) const;
            
            bounded operator~() const;
            bounded operator|(const bounded&) const;
            bounded operator^(const bounded&) const;
            
            bounded operator<<(uint32) const;
            bounded operator>>(uint32) const;
            
            bounded operator<<(int32) const;
            bounded operator>>(int32) const;
            
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
            
            bounded operator-(const bit32&) const;
            bounded operator+(const bit32&) const;
            bounded operator*(const bit32&) const;
            
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
            using typename ray::words_type;
            using typename ray::methods;
            
            number(uint64 x) : ray{} {
                ray::words()[0] = lesser(x);
                ray::words()[1] = greater(x);
            }
            
            number(number<indexed, size, ray::opposite_endian, false> n) : 
                number{static_cast<ordered<indexed, size, ray::opposite_endian>>(n)} {}
            
            explicit number(string& s) : number{gmp::N{s}} {}
            
            // power
            number operator^(const number&) const;
            number& operator^=(const number&);
            
            bool operator<(const number&) const;
            bool operator<=(const number&) const;
                
            number& operator+=(const number&);
            number& operator-=(const number&);
            number& operator*=(const number&);
                
            number& operator+=(const uint32&);
            number& operator-=(const uint32&);
            number& operator*=(const uint32&);
            
            static number max();
            static number min();
            
            number& operator++() {
                operator+=(1);
                return *this;
            }
            
            number& operator--() {
                operator-=(1);
                return *this;
            }
            
            number operator++(int) {
                number n = *this;
                ++(*this);
                return n;
            }
            
            number operator--(int) {
                number n = *this;
                ++(*this);
                return n;
            }
                
            number& operator<<=(uint32);
            number& operator>>=(uint32);
                
            number& operator<<=(int32);
            number& operator>>=(int32);
            
        private:
            explicit number(gmp::N n);
        };
        
        template <typename indexed, size_t size, endian::order o>
        struct number<indexed, size, o, true> : public array<number<indexed, size, o, true>, indexed, size, int32, int64, o> {
            using bit32 = int32;
            using bit64 = int64;
            
            using ray = array<number, indexed, size, bit32, bit64, o>;
            using ray::ray;
            using typename ray::words_type;
            using typename ray::methods;
            
            number(int64 x) : ray{} {
                if (x < 0) operator--();
                ray::words()[0] = lesser(x);
                ray::words()[1] = greater(x);
            }
            
            number(number<indexed, size, ray::opposite_endian, true> n) : 
                number{static_cast<ordered<indexed, size, ray::opposite_endian>>(n)} {}
            
            explicit number(string& s) : number{gmp::N{s}} {}
            
            // power
            number operator^(const number<indexed, size, o, false>&) const;
            number& operator^=(const number<indexed, size, o, false>&);
            
            bool operator<(const number& d) const;
            bool operator<=(const number& d) const;
                
            number& operator+=(const number&);
            number& operator-=(const number&);
            number& operator*=(const number&);
                
            number& operator+=(const int32&);
            number& operator-=(const int32&);
            number& operator*=(const int32&);
            
            static number max();
            static number min();
            
            number& operator++() {
                operator+=(1);
                return *this;
            }
            
            number& operator--() {
                operator-=(1);
                return *this;
            }
            
            number operator++(int) {
                number z = *this;
                ++(*this);
                return z;
            }
            
            number operator--(int) {
                number z = *this;
                ++(*this);
                return z;
            }
                
            number& operator<<=(uint32);
            number& operator>>=(uint32);
                
            number& operator<<=(int32);
            number& operator>>=(int32);
            
        private:
            explicit number(gmp::Z z);
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
            methods::bit_negate(n.words());
            return n;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o> 
        inline bounded array<bounded, indexed, size, bit32, bit64, o>::operator<<(uint32 bits) const {
            bounded result;
            methods::bit_shift_left(words(), bits, result.words());
            return result;
        }
    
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o>
        inline bounded array<bounded, indexed, size, bit32, bit64, o>::operator>>(uint32 bits) const {
            bounded result;
            methods::bit_shift_right(words(), bits, result.words());
            return result;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o> 
        inline bounded array<bounded, indexed, size, bit32, bit64, o>::operator<<(int32 bits) const {
            bounded result;
            methods::bit_shift_left(words(), bits, result.words());
            return result;
        }
    
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o>
        inline bounded array<bounded, indexed, size, bit32, bit64, o>::operator>>(int32 bits) const {
            bounded result;
            methods::bit_shift_right(words(), bits, result.words());
            return result;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o> 
        inline bounded array<bounded, indexed, size, bit32, bit64, o>::operator-(const bounded& n) const {
            bounded result;
            words_type::minus(words(), n, result.words());
            return result;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o> 
        inline bounded array<bounded, indexed, size, bit32, bit64, o>::operator+(const bounded& n) const {
            bounded result;
            words_type::plus(words(), n, result.words());
            return result;
        }
    
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o>
        inline bounded array<bounded, indexed, size, bit32, bit64, o>::operator*(const bounded& n) const {
            bounded result;
            words_type::times(words(), n, result.words());
            return result;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o> 
        inline bounded array<bounded, indexed, size, bit32, bit64, o>::operator-(const bit32& n) const {
            bounded result;
            methods::minus(*this, n, result);
            return result;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o> 
        inline bounded array<bounded, indexed, size, bit32, bit64, o>::operator+(const bit32& n) const {
            bounded result;
            methods::plus(words(), n, result.words());
            return result;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, typename bit64, endian::order o>
        inline bounded array<bounded, indexed, size, bit32, bit64, o>::operator*(const bit32& n) const {
            bounded result;
            methods::times(words(), n, result.words());
            return result;
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
        inline number<indexed, size, o, false>& 
        number<indexed, size, o, false>::operator<<=(uint32 bits) {
            methods::bit_shift_left(ray::words(), bits, ray::words);
            return *this;
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline number<indexed, size, o, false>& 
        number<indexed, size, o, false>::operator>>=(uint32 bits) {
            methods::bit_shift_right(ray::words(), bits, ray::words);
            return *this;
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline number<indexed, size, o, false>& 
        number<indexed, size, o, false>::operator<<=(int32 bits) {
            methods::bit_shift_left(ray::words(), bits, ray::words());
            return *this;
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline number<indexed, size, o, false>& 
        number<indexed, size, o, false>::operator>>=(int32 bits) {
            methods::bit_shift_right(ray::words(), bits, ray::words());
            return *this;
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline number<indexed, size, o, false>&
        number<indexed, size, o, false>::operator+=(const number<indexed, size, o, false>& n) {
            return methods::plus(ray::words(), n, ray::words());
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline number<indexed, size, o, false>&
        number<indexed, size, o, false>::operator-=(const number<indexed, size, o, false>& n) {
            return methods::plus(ray::words(), n, ray::words());
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline number<indexed, size, o, false>&
        number<indexed, size, o, false>::operator*=(const number<indexed, size, o, false>& n) {
            return operator=(operator*(n));
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline number<indexed, size, o, false>& 
        number<indexed, size, o, false>::operator+=(const bit32& n) {
            return methods::plus(ray::words(), n, ray::words());
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline number<indexed, size, o, false>& 
        number<indexed, size, o, false>::operator-=(const bit32& n) {
            methods::minus(ray::words(), n, ray::words());
            return *this;
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline number<indexed, size, o, false>& 
        number<indexed, size, o, false>::operator*=(const bit32& n) {
            methods::times(ray::words(), n, ray::words());
            return *this;
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
