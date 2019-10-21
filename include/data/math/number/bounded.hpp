// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BOUNDED
#define DATA_MATH_NUMBER_BOUNDED

#include <type_traits>
#include <data/encoding/words.hpp>
#include <data/math/group.hpp>
#include <data/math/sign.hpp>
#include <data/math/ordered.hpp>
#include <data/math/division.hpp>
#include <data/container.hpp>
#include <data/math/number/gmp/N.hpp>
#include <data/math/number/gmp/Z.hpp>
#include <data/encoding/integer.hpp>
#include <data/io/unimplemented.hpp>

namespace data {
        
    namespace bounded {
    
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
            endian::order o>    // endianness. 
        struct array : public ordered<indexed, n, o> {
            using ordered<indexed, n, o>::ordered;
            using ordered<indexed, n, o>::Array;
            using ordered<indexed, n, o>::endian;
            using ordered<indexed, n, o>::opposite_endian;
            
            using word = endian::ordered<bit32, endian>;
            using words_type = data::encoding::words<n, n / 4, 0, bit32, o>;
            using methods = ::data::encoding::methods<n, endian::ordered<bit32, endian>, words_type>;
            
            words_type words() {
                return words_type{slice<byte, n>{Array}};
            }
            
            const words_type words() const {
                return words_type::make(slice<byte, n>::make(Array));
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
            
            math::division<bounded> divide(const bounded&) const;
            bounded operator/(const bounded&) const;
            bounded operator%(const bounded&) const;
            
            constexpr static math::group::abelian<bounded> is_group{}; 
        };
    }
    
    namespace math::number {
        
        template <typename indexed, size_t size, endian::order o, bool is_signed> struct bounded;
        
        template <typename indexed, size_t size, endian::order o>
        struct bounded<indexed, size, o, false> : 
            public data::bounded::array<bounded<indexed, size, o, false>, indexed, size, uint32, o> {
            using bit32 = uint32;
            using bit64 = uint64;
            
            using ray = data::bounded::array<bounded, indexed, size, bit32, o>;
            using ray::ray;
            using typename ray::words_type;
            using typename ray::methods;
            
            bounded(uint64 x) : ray{} {
                uint32 l = lesser(x);
                uint32 g = greater(x);
                ray::words().set(0, endian::ordered<uint32, o>::as(l));
                ray::words().set(1, endian::ordered<uint32, o>::as(g));
            }
            
            bounded(bounded<indexed, size, ray::opposite_endian, false> n) : 
                bounded{static_cast<data::bounded::ordered<indexed, size, ray::opposite_endian>>(n)} {}
            
            explicit bounded(string_view s);
            
            // power
            bounded operator^(const bounded&) const;
            bounded& operator^=(const bounded&);
            
            bool operator<(const bounded&) const;
            bool operator<=(const bounded&) const;
                
            bounded& operator+=(const bounded&);
            bounded& operator-=(const bounded&);
            bounded& operator*=(const bounded&);
            
            bounded& operator+=(const uint32&);
            bounded& operator-=(const uint32&);
            bounded& operator*=(const uint32&);
            
            static bounded max() {
                throw method::unimplemented{};
            }
            
            static bounded min() {
                throw method::unimplemented{};
            }
            
            bounded& operator++() {
                operator+=(1);
                return *this;
            }
            
            bounded& operator--() {
                operator-=(1);
                return *this;
            }
            
            bounded operator++(int) {
                bounded n = *this;
                ++(*this);
                return n;
            }
            
            bounded operator--(int) {
                bounded n = *this;
                ++(*this);
                return n;
            }
                
            bounded& operator<<=(uint32);
            bounded& operator>>=(uint32);
                
            bounded& operator<<=(int32);
            bounded& operator>>=(int32);
            
        private:
            explicit bounded(gmp::N n) {
                throw method::unimplemented{};
            }
        };
        
        template <typename indexed, size_t size, endian::order o>
        struct bounded<indexed, size, o, true> : 
            public data::bounded::array<bounded<indexed, size, o, true>, indexed, size, int32, o> {
            using bit32 = int32;
            using bit64 = int64;
            
            using ray = data::bounded::array<bounded, indexed, size, bit32, o>;
            using ray::ray;
            using typename ray::words_type;
            using typename ray::methods;
            
            bounded(int64 x) : ray{} {
                if (x < 0) operator--();
                ray::words().set(0, endian::ordered<int32, o>::as(lesser(x)));
                ray::words().set(1, endian::ordered<int32, o>::as(greater(x)));
            }
            
            bounded(bounded<indexed, size, ray::opposite_endian, true> n) : 
                bounded{static_cast<data::bounded::ordered<indexed, size, ray::opposite_endian>>(n)} {}
            
            explicit bounded(string_view s);
            
            // power
            bounded operator^(const bounded<indexed, size, o, false>&) const;
            bounded& operator^=(const bounded<indexed, size, o, false>&);
            
            bool operator<(const bounded& d) const;
            bool operator<=(const bounded& d) const;
                
            bounded& operator+=(const bounded&);
            bounded& operator-=(const bounded&);
            bounded& operator*=(const bounded&);
                
            bounded& operator+=(const int32&);
            bounded& operator-=(const int32&);
            bounded& operator*=(const int32&);
            
            static bounded max() {
                throw method::unimplemented{};
            }
            
            static bounded min() {
                throw method::unimplemented{};
            }
            
            bounded& operator++() {
                operator+=(1);
                return *this;
            }
            
            bounded& operator--() {
                operator-=(1);
                return *this;
            }
            
            bounded operator++(int) {
                bounded z = *this;
                ++(*this);
                return z;
            }
            
            bounded operator--(int) {
                bounded z = *this;
                ++(*this);
                return z;
            }
                
            bounded& operator<<=(uint32);
            bounded& operator>>=(uint32);
                
            bounded& operator<<=(int32);
            bounded& operator>>=(int32);
            
        private:
            explicit bounded(gmp::Z z) {
                throw method::unimplemented{};
            }
        };
        
    }
    
    template <size_t size> using uint = 
        math::number::bounded<std::array<byte, size>, size, endian::order::big, false>;
    template <size_t size> using integer = 
        math::number::bounded<std::array<byte, size>, size, endian::order::big, true>;
    
    using uint160 = uint<20>;
    using uint256 = uint<32>;
    using uint512 = uint<64>;
    
    using int160 = integer<20>;
    using int256 = integer<32>;
    using int512 = integer<64>;
    
    namespace encoding::hexidecimal { 
        
        template <typename indexed, size_t size, endian::order o, bool is_signed>
        string write(const math::number::bounded<indexed, size, o, is_signed>& n) {
            std::stringstream ss;
            ss << std::hex << n;
            return ss.str();
        }
        
    }
    
    namespace encoding::decimal {
        
        template <typename indexed, size_t size, endian::order o, bool is_signed>
        string write(const math::number::bounded<indexed, size, o, is_signed>& n) {
            std::stringstream ss;
            ss << std::dec << n;
            return ss.str();
        }
        
    }
    
    namespace bounded {
        
        template <typename bounded, typename indexed, size_t size, typename bit32, endian::order o>
        inline bool array<bounded, indexed, size, bit32, o>::operator>(const bounded& d) const {
            return d <= bounded{Array};
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, endian::order o>
        inline bool array<bounded, indexed, size, bit32, o>::operator>=(const bounded& d) const {
            return d < bounded{Array};
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, endian::order o>
        inline bool array<bounded, indexed, size, bit32, o>::operator==(const bounded& d) const {
            return Array == d.Array;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, endian::order o>
        inline bool array<bounded, indexed, size, bit32, o>::operator!=(const bounded& d) const {
            return Array != d.Array;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, endian::order o> 
        inline bounded array<bounded, indexed, size, bit32, o>::operator-() const {
            return bounded{0} - *this;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, endian::order o> 
        inline bounded array<bounded, indexed, size, bit32, o>::operator~() const {
            bounded n = *this;
            methods::bit_negate(n.words());
            return n;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, endian::order o> 
        inline bounded array<bounded, indexed, size, bit32, o>::operator<<(uint32 bits) const {
            bounded result;
            methods::bit_shift_left(words(), bits, result.words());
            return result;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, endian::order o>
        inline bounded array<bounded, indexed, size, bit32, o>::operator>>(uint32 bits) const {
            bounded result;
            methods::bit_shift_right(words(), bits, result.words());
            return result;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, endian::order o> 
        inline bounded array<bounded, indexed, size, bit32, o>::operator<<(int32 bits) const {
            bounded result;
            methods::bit_shift_left(words(), bits, result.words());
            return result;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, endian::order o>
        inline bounded array<bounded, indexed, size, bit32, o>::operator>>(int32 bits) const {
            bounded result;
            methods::bit_shift_right(words(), bits, result.words());
            return result;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, endian::order o> 
        inline bounded array<bounded, indexed, size, bit32, o>::operator-(const bounded& n) const {
            bounded result;
            words_type::minus(words(), n, result.words());
            return result;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, endian::order o> 
        inline bounded array<bounded, indexed, size, bit32, o>::operator+(const bounded& n) const {
            bounded result;
            words_type::plus(words(), n, result.words());
            return result;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, endian::order o>
        inline bounded array<bounded, indexed, size, bit32, o>::operator*(const bounded& n) const {
            bounded result;
            words_type::times(words(), n, result.words());
            return result;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, endian::order o> 
        inline bounded array<bounded, indexed, size, bit32, o>::operator-(const bit32& n) const {
            bounded result;
            methods::minus(*this, n, result);
            return result;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, endian::order o> 
        inline bounded array<bounded, indexed, size, bit32, o>::operator+(const bit32& n) const {
            bounded result;
            methods::plus(words(), n, result.words());
            return result;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, endian::order o>
        inline bounded array<bounded, indexed, size, bit32, o>::operator*(const bit32& n) const {
            bounded result;
            methods::times(words(), n, result.words());
            return result;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, endian::order o>
        inline math::division<bounded> 
        array<bounded, indexed, size, bit32, o>::divide(const bounded& n) const {
            return math::division<bounded>::divide(*this, n);
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, endian::order o>
        inline bounded array<bounded, indexed, size, bit32, o>::operator/(const bounded& n) const {
            return divide(n).Quotient;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, endian::order o>
        inline bounded array<bounded, indexed, size, bit32, o>::operator%(const bounded& n) const {
            return divide(n).Remainder;
        }
    }
    
    namespace math::number {
        
        template <typename indexed, size_t size, endian::order o>
        inline bounded<indexed, size, o, false>& 
        bounded<indexed, size, o, false>::operator<<=(uint32 bits) {
            methods::bit_shift_left(ray::words(), bits, ray::words);
            return *this;
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline bounded<indexed, size, o, false>& 
        bounded<indexed, size, o, false>::operator>>=(uint32 bits) {
            methods::bit_shift_right(ray::words(), bits, ray::words);
            return *this;
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline bounded<indexed, size, o, false>& 
        bounded<indexed, size, o, false>::operator<<=(int32 bits) {
            methods::bit_shift_left(ray::words(), bits, ray::words());
            return *this;
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline bounded<indexed, size, o, false>& 
        bounded<indexed, size, o, false>::operator>>=(int32 bits) {
            methods::bit_shift_right(ray::words(), bits, ray::words());
            return *this;
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline bounded<indexed, size, o, false>&
        bounded<indexed, size, o, false>::operator+=(const bounded<indexed, size, o, false>& n) {
            return methods::plus(ray::words(), n, ray::words());
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline bounded<indexed, size, o, false>&
        bounded<indexed, size, o, false>::operator-=(const bounded<indexed, size, o, false>& n) {
            return methods::plus(ray::words(), n, ray::words());
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline bounded<indexed, size, o, true>&
        bounded<indexed, size, o, true>::operator-=(const bounded<indexed, size, o, true>& n) {
            return methods::plus(ray::words(), n, ray::words());
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline bounded<indexed, size, o, false>&
        bounded<indexed, size, o, false>::operator*=(const bounded<indexed, size, o, false>& n) {
            return operator=(operator*(n));
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline bounded<indexed, size, o, false>& 
        bounded<indexed, size, o, false>::operator+=(const bit32& n) {
            return methods::plus(ray::words(), n, ray::words());
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline bounded<indexed, size, o, true>& 
        bounded<indexed, size, o, true>::operator+=(const bit32& n) {
            return methods::plus(ray::words(), n, ray::words());
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline bounded<indexed, size, o, false>& 
        bounded<indexed, size, o, false>::operator-=(const bit32& n) {
            methods::minus(ray::words(), n, ray::words());
            return *this;
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline bounded<indexed, size, o, true>& 
        bounded<indexed, size, o, true>::operator-=(const bit32& n) {
            methods::minus(ray::words(), n, ray::words());
            return *this;
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline bounded<indexed, size, o, false>& 
        bounded<indexed, size, o, false>::operator*=(const bit32& n) {
            methods::times(ray::words(), n, ray::words());
            return *this;
        }
    
        template <typename indexed, size_t size, endian::order o>
        bool bounded<indexed, size, o, false>::operator<(const bounded& n) const {
            words_type a = ray::words();
            words_type b = n.words();
            for (uint32_t i = 0; i < size; i++) {
                if (a[i] < b[i]) return true;
                if (a[i] > b[i]) return false;
            }
            
            return false;
        }
        
        template <typename indexed, size_t size, endian::order o>
        bool bounded<indexed, size, o, false>::operator<=(const bounded& n) const {
            words_type a = ray::words();
            words_type b = n.words();
            for (uint32_t i = 0; i < size; i++) {
                if (a[i] < b[i]) return true;
                if (a[i] > b[i]) return false;
            }
            
            return true;
        }
        
        template <typename indexed, size_t size, endian::order o>
        bounded<indexed, size, o, false>::bounded(string_view s) : bounded{} {
            if (!encoding::natural::valid(s)) throw std::invalid_argument{"not a natural number"};
            
            if (encoding::hexidecimal::valid(s) && s.size() > (2 + 2 * size)) throw std::invalid_argument{"string too long"};
            
            gmp::N n{s};
            if (n > gmp::N{max()}) throw std::invalid_argument{"number too big"};
            
            *this = bounded{n};
            
        }
        
        template <typename indexed, size_t size, endian::order o>
        bounded<indexed, size, o, true>::bounded(string_view s) : bounded{} {
            if (!encoding::integer::valid(s)) throw std::invalid_argument{"not an integer"};
            
            // Is there a minus sign? 
            bool negative = encoding::integer::negative(s);
            bool hexidecimal = encoding::hexidecimal::valid(negative ? s.substr(1) : s);
            
            if (hexidecimal && s.size() > (2 + 2 * size)) throw std::invalid_argument{"string too long"};
            
            gmp::Z z{s};
            if (z > gmp::Z{max()} || z < gmp::Z{min()}) throw std::invalid_argument{"number out of range"};
            
            *this = bounded{z};
            
        }
        
    }

}
#endif
