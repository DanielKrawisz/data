// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_BOUNDED
#define DATA_MATH_NUMBER_BOUNDED

#include <type_traits> 
#include <data/math/group.hpp>
#include <data/math/sign.hpp>
#include <data/math/ordered.hpp>
#include <data/math/division.hpp>
#include <data/iterable.hpp>
#include <data/math/number/abs.hpp>
#include <data/math/number/gmp/gmp.hpp>
#include <data/math/number/bytes/N.hpp>
#include <data/math/number/bytes/Z.hpp>
#include <data/encoding/integer.hpp>

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
            
            constexpr static endian::order endian = o;
            constexpr static endian::order opposite_endian = endian::opposite(o);
            
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

            bool operator==(const bounded& d) const;
            bool operator!=(const bounded& d) const;
            
            bounded operator~() const;
            bounded operator|(const bounded&) const;
            bounded operator^(const bounded&) const;
            
            bool operator>(const bounded& d) const;
            bool operator>=(const bounded& d) const;
            
            math::sign sign() const {
                if (bounded{Array} < 0) return math::negative;
                if (bounded{Array} > 0) return math::positive;
                return math::zero;
            }
            
            bounded operator-(const bounded&) const;
            bounded operator+(const bounded&) const;
            bounded operator*(const bounded&) const;
            
            bounded operator-(const bit32&) const;
            bounded operator+(const bit32&) const;
            bounded operator*(const bit32&) const;
            
            bounded operator<<(int32) const;
            bounded operator>>(int32) const;
            
            constexpr static math::group::abelian<bounded> is_group{}; 
            
        protected:
            using word = endian::ordered<bit32, endian>;
            using words_type = arithmetic::fixed_words<n, n / 4, n % 4, bit32, o>;
            using methods = arithmetic::unoriented<words_type, word>;
            
            words_type words() {
                return words_type{slice<byte, n>{Array}};
            }
            
            const words_type words() const {
                return words_type::make(slice<byte, n>::make(Array));
            }
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
                ray::words().set(0, endian::ordered<uint32, o>{lesser_half(x)});
                ray::words().set(1, endian::ordered<uint32, o>{greater_half(x)});
            }
            
            bounded(bounded<indexed, size, ray::opposite_endian, false> n) : 
                bounded{static_cast<data::bounded::ordered<indexed, size, ray::opposite_endian>>(n)} {}
            
            explicit bounded(string_view s);
            
            bounded& operator=(const bounded& d) {
                throw method::unimplemented{"bounded::operator="};
            }
            
            operator slice<byte, size>() const;
            
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
            
            static bounded max();
            static bounded min();
            static N_bytes<o> modulus();
            
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
                
            bounded& operator<<=(int32);
            bounded& operator>>=(int32);
            
            math::division<bounded> divide(const bounded&) const;
            
            bounded operator/(const bounded& n) const {
                return divide(n).Quotient;
            }
            
            bounded operator%(const bounded& n) const {
                return divide(n).Remainder;
            }
            
        private:
            explicit bounded(const N_bytes<o>& n) {
                if (n > N_bytes<o>{max()}) throw std::out_of_range{"N_bytes too big"};
                throw method::unimplemented{"bounded{N_bytes}"};
            }
            
            bounded(const bounded<indexed, size, o, true>) {
                throw method::unimplemented{"boundedbounded<indexed, size, o, false>{bounded<indexed, size, o, true>}"};
            }
            
            friend struct abs<bounded<indexed, size, o, false>, bounded<indexed, size, o, true>>;
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
                ray::words().set(0, endian::ordered<int32, o>{lesser_half(x)});
                ray::words().set(1, endian::ordered<int32, o>{greater_half(x)});
            }
            
            bounded(bounded<indexed, size, ray::opposite_endian, true> n) : 
                bounded{static_cast<data::bounded::ordered<indexed, size, ray::opposite_endian>>(n)} {}
            bounded(const bounded<indexed, size, o, false>&) {
                throw method::unimplemented{"signed bounded from unsigned bounded"};
            }
            
            explicit bounded(string_view s);
            
            operator slice<byte, size>() const;
            
            bounded& operator=(const bounded& d) {
                throw method::unimplemented{"bounded::operator="};
            }
            
            bounded operator-() const;
            using ray::operator-;
            
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
            
            static bounded max();
            static bounded min();
            
            static N_bytes<o> modulus() {
                return bounded<indexed, size, o, false>::modulus();
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
                --(*this);
                return z;
            }
                
            bounded& operator<<=(int32);
            bounded& operator>>=(int32);
            
            math::division<bounded> divide(const bounded&) const;
            
            bounded operator/(const bounded& n) const {
                return divide(n).Quotient;
            }
            
            bounded operator%(const bounded& n) const {
                return divide(n).Remainder;
            }
            
        private:
            explicit bounded(const Z_bytes<o>& z) {
                if (z > Z_bytes<o>{max()} || z < Z_bytes<o>{min()}) throw std::out_of_range{"Z_bytes too big"};
                throw method::unimplemented{"bounded{Z_bytes}"};
            }
        };

        template <typename indexed, size_t size, endian::order o, bool is_signed> 
        struct arg<bounded<indexed, size, o, is_signed>> {
            bounded<indexed, size, o, false> operator()(const bounded<indexed, size, o, is_signed>& i) {
                if (i == 0) throw division_by_zero{};
                return i.sign() == math::negative ? -1 : 1;
            }
        };
        
    }
    
    template <size_t size> using uint = math::number::bounded<std::array<byte, size>, size, endian::big, false>;
    template <size_t size> using integer = math::number::bounded<std::array<byte, size>, size, endian::big, true>;
    using uint160 = uint<20>;
    using uint256 = uint<32>;
    using uint512 = uint<64>;
    
    using int160 = integer<20>;
    using int256 = integer<32>;
    using int512 = integer<64>;
    
    namespace encoding::hexidecimal { 
        
        template <typename indexed, size_t size, endian::order o, bool is_signed>
        string write(const math::number::bounded<indexed, size, o, is_signed>& n);
        
    }
    
    namespace encoding::decimal {
        
        template <typename indexed, size_t size, endian::order o, bool is_signed>
        string write(const math::number::bounded<indexed, size, o, is_signed>& n);
        
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
        inline bounded array<bounded, indexed, size, bit32, o>::operator~() const {
            bounded n{};
            methods::bit_not(words_type::Last, words(), n.words());
            return n;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, endian::order o> 
        inline bounded array<bounded, indexed, size, bit32, o>::operator-(const bounded& n) const {
            bounded result;
            methods::minus(words_type::Last, words(), n.words(), result.words());
            return result;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, endian::order o> 
        inline bounded array<bounded, indexed, size, bit32, o>::operator+(const bounded& n) const {
            bounded result;
            methods::plus(words_type::Last, words(), n.words(), result.words());
            return result;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, endian::order o>
        inline bounded array<bounded, indexed, size, bit32, o>::operator*(const bounded& n) const {
            bounded result;
            methods::times(words_type::Last, words(), n.words(), result.words());
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
            methods::plus(words_type::Last, words(), n, result.words());
            return result;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, endian::order o>
        inline bounded array<bounded, indexed, size, bit32, o>::operator*(const bit32& n) const {
            bounded result;
            methods::times(words(), n, result.words());
            return result;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, endian::order o> 
        inline bounded array<bounded, indexed, size, bit32, o>::operator<<(int32 bits) const {
            bounded result;
            words_type w = result.words();
            if ((bits < 0 && o == endian::little) || (bits >= 0 && o == endian::big)) 
                methods::up(size, words(), bits, w);
            else methods::down(size, words(), bits, w);
            return result;
        }
        
        template <typename bounded, typename indexed, size_t size, typename bit32, endian::order o>
        inline bounded array<bounded, indexed, size, bit32, o>::operator>>(int32 bits) const {
            bounded result;
            words_type w = result.words();
            if ((bits < 0 && o == endian::little) || (bits >= 0 && o == endian::big)) 
                methods::down(size, words(), bits, w);
            else methods::up(size, words(), bits, w);
            return result;
        }
        
    }
    
    namespace math::number {
        
        template <typename indexed, size_t size, endian::order o>
        inline bounded<indexed, size, o, false>& 
        bounded<indexed, size, o, false>::operator<<=(int32 bits) {
            return operator=(ray::operator<<(bits));
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline bounded<indexed, size, o, false>& 
        bounded<indexed, size, o, false>::operator>>=(int32 bits) {
            return operator=(ray::operator>>(bits));
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline bounded<indexed, size, o, true>& 
        bounded<indexed, size, o, true>::operator<<=(int32 bits) {
            return operator=(ray::operator<<(bits));
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline bounded<indexed, size, o, true>& 
        bounded<indexed, size, o, true>::operator>>=(int32 bits) {
            return operator=(ray::operator>>(bits));
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline bounded<indexed, size, o, false>&
        bounded<indexed, size, o, false>::operator+=(const bounded<indexed, size, o, false>& n) {
            methods::plus(size, ray::words(), n.words(), ray::words());
            return *this;
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline bounded<indexed, size, o, true>&
        bounded<indexed, size, o, true>::operator+=(const bounded<indexed, size, o, true>& n) {
            methods::plus(size, ray::words(), n.words(), ray::words());
            return *this;
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline bounded<indexed, size, o, false>&
        bounded<indexed, size, o, false>::operator-=(const bounded<indexed, size, o, false>& n) {
            methods::plus(size, ray::words(), n.words(), ray::words());
            return *this;
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline bounded<indexed, size, o, true>&
        bounded<indexed, size, o, true>::operator-=(const bounded<indexed, size, o, true>& n) {
            methods::plus(ray::words_type::Last, ray::words(), n.words(), ray::words());
            return *this;
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline bounded<indexed, size, o, false>&
        bounded<indexed, size, o, false>::operator*=(const bounded<indexed, size, o, false>& n) {
            return operator=(operator*(n));
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline bounded<indexed, size, o, false>& 
        bounded<indexed, size, o, false>::operator+=(const bit32& n) {
            return methods::plus(size, ray::words(), n, ray::words());
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline bounded<indexed, size, o, true> 
        bounded<indexed, size, o, true>::operator-() const {
            return ray::operator~() + 1;
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline bounded<indexed, size, o, true>& 
        bounded<indexed, size, o, true>::operator+=(const bit32& n) {
            return methods::plus(size, ray::words(), n, ray::words());
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline bounded<indexed, size, o, false>& 
        bounded<indexed, size, o, false>::operator-=(const bit32& n) {
            methods::minus(size, ray::words(), n, ray::words());
            return *this;
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline bounded<indexed, size, o, true>& 
        bounded<indexed, size, o, true>::operator-=(const bit32& n) {
            words_type out = ray::words();
            methods::minus(size, ray::words(), n, out);
            return *this;
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline bounded<indexed, size, o, false>& 
        bounded<indexed, size, o, false>::operator*=(const bit32& n) {
            methods::times(size, ray::words(), n, ray::words());
            return *this;
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline math::division<bounded<indexed, size, o, false>> 
        bounded<indexed, size, o, false>::divide(const bounded& n) const {
            return math::number::natural::divide(*this, n);
        }
        
        template <typename indexed, size_t size, endian::order o>
        inline math::division<bounded<indexed, size, o, true>> 
        bounded<indexed, size, o, true>::divide(const bounded& n) const {
            return math::number::integer::divide(*this, n);
        }
    
        template <typename indexed, size_t size, endian::order o>
        bool bounded<indexed, size, o, false>::operator<(const bounded& n) const {
            return methods::less(words_type::Last, ray::words(), n.words());
        }
        
        template <typename indexed, size_t size, endian::order o>
        bool bounded<indexed, size, o, false>::operator<=(const bounded& n) const {
            return methods::less_equal(words_type::Last, ray::words(), n.words());
        }
    
        template <typename indexed, size_t size, endian::order o>
        bool bounded<indexed, size, o, true>::operator<(const bounded& n) const {
            return methods::less(words_type::Last, ray::words(), n.words());
        }
        
        template <typename indexed, size_t size, endian::order o>
        bool bounded<indexed, size, o, true>::operator<=(const bounded& n) const {
            return methods::less_equal(words_type::Last, ray::words(), n.words());
        }
        
        template <typename indexed, size_t size, endian::order o>
        bounded<indexed, size, o, false>::bounded(string_view s) : bounded{} {
            if (!encoding::natural::valid(s)) throw std::invalid_argument{"not a natural number"};
            
            if (encoding::hexidecimal::valid(s) && s.size() > (2 + 2 * size)) throw std::invalid_argument{"string too long"};
            *this = bounded{N_bytes<o>{s}};
        }
        
        template <typename indexed, size_t size, endian::order o>
        bounded<indexed, size, o, true>::bounded(string_view s) : bounded{} {
            if (!encoding::integer::valid(s)) throw std::invalid_argument{"not an integer"};
            
            // Is there a minus sign? 
            bool minus_sign = encoding::integer::negative(s);
            bool hexidecimal = encoding::hexidecimal::valid(minus_sign ? s.substr(1) : s);
            
            if (hexidecimal && s.size() > (2 + 2 * size)) throw std::invalid_argument{"string too long"};
            
            Z_bytes<o> z{s};
            if (z > Z_bytes<o>{max()}) z -= modulus();
            *this = bounded{z};
        }
        
        template <typename indexed, size_t size, endian::order o>
        bounded<indexed, size, o, false> bounded<indexed, size, o, false>::min() {
            return 0;
        }
        
        template <typename indexed, size_t size, endian::order o>
        bounded<indexed, size, o, false> bounded<indexed, size, o, false>::max() {
            bounded b{};
            words_type w = b.words();
            for (int i = 0; i <= words_type::Last; i++) w.set(i, 0xffffffff);
            return b;
        }
        
        template <typename indexed, size_t size, endian::order o>
        N_bytes<o> bounded<indexed, size, o, false>::modulus() {
            std::string one = std::string{"0x01"} + encoding::hexidecimal::write(bounded{0}).substr(2);
            return N_bytes<o>{one};
        }
        
        template <typename indexed, size_t size, endian::order o>
        bounded<indexed, size, o, true> bounded<indexed, size, o, true>::min() {
            bounded b{};
            words_type w = b.words();
            w.set(words_type::Last, 0x80000000);
            return b;
        }
        
        template <typename indexed, size_t size, endian::order o>
        bounded<indexed, size, o, true> bounded<indexed, size, o, true>::max() {
            bounded b{};
            words_type w = b.words();
            w.set(words_type::Last, 0x7fffffff);
            for (int i = 0; i < words_type::Last; i++) w.set(i, 0xffffffff);
            return b;
        }
        
        namespace low {
            
            template <size_t size, data::endian::order o>
            void write_dec(std::ostream& s, 
                const math::number::bounded<std::array<data::byte, size>, size, o, false>& n) {
                s << std::dec << N_bytes<o>{n};
            }
            
            template <size_t size, data::endian::order o>
            void write_dec(std::ostream& s,
                const math::number::bounded<std::array<data::byte, size>, size, o, true>& n) {
                s << std::dec << Z_bytes<o>{n};
            }
        }
        
    }

}

template <unsigned long size, data::endian::order o, bool is_signed>
std::ostream& operator<<(std::ostream& s, 
    const data::math::number::bounded<std::array<data::byte, size>, size, o, is_signed>& n) {
    if (s.flags() & std::ios::hex) {
        data::encoding::hexidecimal::write(s, data::bytes_view(n.Array.data(), size), o);
        return s;
    } 
    if (s.flags() & std::ios::dec) {
        data::math::number::low::write_dec(s, n);
        return s;
    }
    return s;
}

// Explicit instantiation of the above template function. My understanding is that
// this should not be necessary but I get linker errors when these are not here. 
// I would like to be able to remove these. 
// Note: the reason probably has to do with the order in which the object files are linked. 
template std::ostream& operator<<<8, data::endian::big, true>(std::ostream& s, const data::math::number::bounded<std::array<data::byte, 8>, 8, data::endian::big, true>& n);

template std::ostream& operator<<<8, data::endian::big, false>(std::ostream& s, const data::math::number::bounded<std::array<data::byte, 8>, 8, data::endian::big, false>& n);

template std::ostream& operator<<<8, data::endian::little, true>(std::ostream& s, const data::math::number::bounded<std::array<data::byte, 8>, 8, data::endian::little, true>& n);

template std::ostream& operator<<<8, data::endian::little, false>(std::ostream& s, const data::math::number::bounded<std::array<data::byte, 8>, 8, data::endian::little, false>& n);

namespace data {
    
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
    
}

#endif
