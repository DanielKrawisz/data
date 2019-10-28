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
            using methods = ::data::encoding::methods<n / 4, word, words_type>;
            
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
                ray::words().set(0, endian::ordered<uint32, o>{lesser(x)});
                ray::words().set(1, endian::ordered<uint32, o>{greater(x)});
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
            
            static bounded max();
            static bounded min();
            static gmp::N modulus();
            
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
            explicit bounded(const gmp::N& n);
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
                ray::words().set(0, endian::ordered<int32, o>{lesser(x)});
                ray::words().set(1, endian::ordered<int32, o>{greater(x)});
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
            
            static bounded max();
            static bounded min();
            
            static gmp::N modulus() {
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
                
            bounded& operator<<=(uint32);
            bounded& operator>>=(uint32);
                
            bounded& operator<<=(int32);
            bounded& operator>>=(int32);
            
        private:
            explicit bounded(const gmp::Z& z);
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
        bool bounded<indexed, size, o, true>::operator<(const bounded& n) const {
            words_type a = ray::words();
            words_type b = n.words();
            int last = words_type::last;
            
            if (a[last] < b[last]) return true;
            if (a[last] > b[last]) return false;
            
            for (int i = words_type::last - 1; i >= 0; i--) {
                if (uint32(a[i]) < uint32(b[i])) return true;
                if (uint32(a[i]) > uint32(b[i])) return false;
            }
            
            return false;
        }
        
        template <typename indexed, size_t size, endian::order o>
        bool bounded<indexed, size, o, true>::operator<=(const bounded& n) const {
            words_type a = ray::words();
            words_type b = n.words();
            int last = words_type::last;
            
            if (a[last] < b[last]) return true;
            if (a[last] > b[last]) return false;
            
            for (int i = words_type::last - 1; i >= 0; i--) {
                if (uint32(a[i]) < uint32(b[i])) return true;
                if (uint32(a[i]) > uint32(b[i])) return false;
            }
            
            return true;
        }
        
        template <typename indexed, size_t size, endian::order o>
        bounded<indexed, size, o, false>::bounded(string_view s) : bounded{} {
            if (!encoding::natural::valid(s)) throw std::invalid_argument{"not a natural number"};
            
            if (encoding::hexidecimal::valid(s) && s.size() > (2 + 2 * size)) throw std::invalid_argument{"string too long"};
            *this = bounded{gmp::N{s}};
            
        }
        
        template <typename indexed, size_t size, endian::order o>
        bounded<indexed, size, o, true>::bounded(string_view s) : bounded{} {
            if (!encoding::integer::valid(s)) throw std::invalid_argument{"not an integer"};
            
            // Is there a minus sign? 
            bool minus_sign = encoding::integer::negative(s);
            bool hexidecimal = encoding::hexidecimal::valid(minus_sign ? s.substr(1) : s);
            
            if (hexidecimal && s.size() > (2 + 2 * size)) throw std::invalid_argument{"string too long"};
            
            gmp::Z z{s};
            if (z > gmp::Z{max()}) z -= modulus();
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
            for (int i = 0; i <= words_type::last; i++) w.set(i, 0xffffffff);
            return b;
        }
            
        
        template <typename indexed, size_t size, endian::order o>
        gmp::N bounded<indexed, size, o, false>::modulus() {
            std::string one = std::string{"0x01"} + encoding::hexidecimal::write(bounded{0}).substr(2);
            return gmp::N{one};
        }
        
        template <typename indexed, size_t size, endian::order o>
        bounded<indexed, size, o, true> bounded<indexed, size, o, true>::min() {
            bounded b{};
            words_type w = b.words();
            w.set(words_type::last, 0x80000000);
            //for (int i = 0; i < words_type::last; i++) w.set(i, 0);
            return b;
        }
        
        template <typename indexed, size_t size, endian::order o>
        bounded<indexed, size, o, true> bounded<indexed, size, o, true>::max() {
            bounded b{};
            words_type w = b.words();
            w.set(words_type::last, 0x7fffffff);
            for (int i = 0; i < words_type::last; i++) w.set(i, 0xffffffff);
            return b;
        }
        
        namespace low {
            
            template <typename gmp_limb, typename bit32> struct mpz_to_words;
            
            template <typename bit32> struct mpz_to_words<uint64, bit32> {
                template <size_t size, endian::order o>
                void operator()(const gmp::Z& z, encoding::words<4 * size, size, 0, bit32, o> w) {
                    if (z.size() * 2 <= size) {
                        int j = 0;
                        for (auto i = z.begin(); i != z.end(); i++) {
                            w.set(j, lesser(*i));
                            w.set(j + 1, greater(*i));
                            j += 2;
                        }
                    } else {
                        for (int j = 0; j <= size - 2; j += 2) {
                            uint64 word = z[j/2];
                            w.set(j, lesser(word));
                            w.set(j + 1, greater(word));
                        }
                        if (size % 2 == 1) w.set(size - 1, lesser(z[size / 2]));
                    } 
                }
            };
            
        }
        
        template <typename indexed, size_t size, endian::order o>
        bounded<indexed, size, o, false>::bounded(const gmp::N& n) : bounded{0} {
            if (n > gmp::N{max()}) throw std::invalid_argument{"too high."};
            
            low::mpz_to_words<gmp::gmp_uint, uint32>{}(n, ray::words());
        }
        
        template <typename indexed, size_t size, endian::order o>
        bounded<indexed, size, o, true>::bounded(const gmp::Z& n) : bounded{0} {
            gmp::Z m{max()};
            if (n > m) throw std::invalid_argument{"too high."};
            if (n < gmp::Z{min()}) throw std::invalid_argument{"too low."};
            
            low::mpz_to_words<gmp::gmp_uint, int32>{}(n < 0 ? n + modulus() : n, ray::words());
        }
        
        namespace low {
            template <size_t size, data::endian::order o>
            void write_hex(std::ostream& s,
                const math::number::bounded<std::array<data::byte, size>, size, o, false>& n) {
                s << "0x";
                using words_type = typename math::number::bounded<std::array<data::byte, size>, size, o, false> ::words_type;
                words_type w = n.words();
                for (int i = words_type::last; i >= 0; i--) s << encoding::hex::write(uint32(w[i]));
            }
            
            template <size_t size, data::endian::order o>
            void write_hex(std::ostream& s,
                const math::number::bounded<std::array<data::byte, size>, size, o, true>& n) {
                s << "0x";
                using words_type = typename math::number::bounded<std::array<data::byte, size>, size, o, true>::words_type;
                words_type w = n.words();
                for (int i = words_type::last; i >= 0; i--) s << encoding::hex::write(uint32(int32(w[i])));
            }
            
            template <size_t size, data::endian::order o>
            void write_dec(std::ostream& s, 
                const math::number::bounded<std::array<data::byte, size>, size, o, false>& n) {
                s << std::dec << gmp::N{n};
            }
            
            template <size_t size, data::endian::order o>
            void write_dec(std::ostream& s,
                const math::number::bounded<std::array<data::byte, size>, size, o, true>& n) {
                s << std::dec << gmp::Z{n};
            }
        }
        
    }

}

template <unsigned long size, data::endian::order o, bool is_signed>
std::ostream& operator<<(std::ostream& s, 
    const data::math::number::bounded<std::array<data::byte, size>, size, o, is_signed>& n) {
    if (s.flags() & std::ios::hex) {
        data::math::number::low::write_hex(s, n);
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
