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
#include <data/math/number/extended_euclidian.hpp>

namespace data::math::number::bounded {
    
    template <uint32_t size, typename bounded, typename bit32, typename bit64>
    struct array : public std::array<byte, size * 4> {
        using parent = std::array<byte, size * 4>;
        using word = data::number::endian::big<bit64>;
        using index = bit32;
        
        static const index last = size - 1;
        
        static bit32 lesser(word w);
        static bit32 greater(word w);
        
        bit32& operator[](index i) const {
            if (i >= size) throw 0;
            return *(uint32*)(parent::data() + 4 * i);
        }
        
        static word extend(uint32);
        
        static bool overflow(word w) {
            return greater(w) != 0;
        }
        
        array() : parent() {}
        array(bit32 x) : array() {
            operator[](last) = x;
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
            
        bounded& operator<<=(uint32);
        bounded& operator>>=(uint32);
        
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
        
        static void minus(const bounded&, const bounded&, bounded&);
        static void plus(const bounded&, const bounded&, bounded&);
        
        using iterator = index_iterator<bounded&, bit32&>;

        iterator begin();
        iterator end();
        const iterator begin() const;
        const iterator end() const;
        
        constexpr static math::group::abelian<bounded> is_group{}; 
    };
    
    template <uint32_t size, bool is_signed> struct number;
    
    template <uint32_t size>
    struct number<size, false> : public array<size, number<size, false>, uint32, uint64> {
        using array = array<size, number<size, false>, uint32, uint64>;
        using array::operator[];
        using array::operator=;
        using array::operator|=;
        using array::operator^=;
        using array::operator<<=;
        using array::operator>>=;
        
        bool operator<(const number<size, false>& d) const;
        bool operator<=(const number<size, false>& d) const;
        
        math::number::division<number<size, false>> divide(const number<size, false>&) const;
        number<size, false> operator/(const number<size, false>&) const;
        number<size, false> operator%(const number<size, false>&) const;
    };
    
    template <uint32_t size>
    struct number<size, true> : public array<size, number<size, true>, uint32, uint64> {
        using array = array<size, number<size, true>, uint32, uint64>;
        using array::operator[];
        using array::operator=;
        using array::operator|=;
        using array::operator^=;
        using array::operator<<=;
        using array::operator>>=;
        
        bool operator<(const number<size, true>& d) const;
        bool operator<=(const number<size, true>& d) const;
        
        math::number::division<number<size, true>> divide(const number<size, true>&) const;
        number<size, true> operator/(const number<size, true>&) const;
        number<size, true> operator%(const number<size, true>&) const;
    };
    
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
    
    template <uint32_t size, typename bounded, typename bit32, typename bit64>
    inline typename array<size, bounded, bit32, bit64>::iterator array<size, bounded, bit32, bit64>::begin() {
        return iterator{*this, uint(0)};
    }
    
    template <uint32_t size, typename bounded, typename bit32, typename bit64>
    inline typename array<size, bounded, bit32, bit64>::iterator array<size, bounded, bit32, bit64>::end() {
        return iterator{*this, size};
    }
    
    template <uint32_t size, typename bounded, typename bit32, typename bit64>
    inline const typename array<size, bounded, bit32, bit64>::iterator array<size, bounded, bit32, bit64>::begin() const {
        return iterator{*const_cast<array*>(this), uint(0)};
    }
    
    template <uint32_t size, typename bounded, typename bit32, typename bit64>
    inline const typename array<size, bounded, bit32, bit64>::iterator array<size, bounded, bit32, bit64>::end() const {
        return iterator{*const_cast<array*>(this), size};
    }
    
    template <uint32 size, typename bounded, typename bit32, typename bit64> 
    inline bounded array<size, bounded, bit32, bit64>::operator-() const {
        return bounded{0} - *this;
    }
    
    namespace low {
    
        template <uint32 size, typename bounded, typename bit32, typename bit64> 
        inline void bit_not(bounded& n) {
            for (bit32& u : n) u = ~u;
        }
    
        template <uint32 size, typename bounded, typename bit32, typename bit64> 
        inline void bit_and(const bounded& a,
                            const bounded& b, 
                            bounded& result) {
            for (uint32 i = 0; i < size; i++) result[i] = a[i]^b[i];
        }
    
        template <uint32 size, typename bounded, typename bit32, typename bit64> 
        inline void bit_or(const bounded& a,
                           const bounded& b,
                           bounded& result) {
            for (uint32 i = 0; i < size; i++) result[i] = a[i]|b[i];
        }
    
    }
    
    template <uint32 size, typename bounded, typename bit32, typename bit64> 
    inline bounded array<size, bounded, bit32, bit64>::operator~() const {
        bounded n = *this;
        low::bit_not(n);
        return n;
    }
    
    template <uint32_t size, typename bounded, typename bit32, typename bit64>
    inline bounded& array<size, bounded, bit32, bit64>::operator|=(const bounded& n) {
        low::bit_and(*this, n, *this);
        return *this;
    }
    
    template <uint32_t size, typename bounded, typename bit32, typename bit64>
    inline bounded& array<size, bounded, bit32, bit64>::operator^=(const bounded& n) {
        low::bit_and(*this, n, *this);
        return *this;
    }
    
    template <uint32 size, typename bounded, typename bit32, typename bit64> 
    inline bounded array<size, bounded, bit32, bit64>::operator+(const bounded& n) const {
        throw 0;
    }
    
    template <uint32 size, typename bounded, typename bit32, typename bit64> 
    inline bounded array<size, bounded, bit32, bit64>::operator-(const bounded& n) const {
        throw 0;
    }
    
    template <uint32_t size, typename bounded, typename bit32, typename bit64>
    inline bounded& array<size, bounded, bit32, bit64>::operator+=(const bounded& n) {
        return operator=(operator+(n));
    }
    
    template <uint32_t size, typename bounded, typename bit32, typename bit64>
    inline bounded& array<size, bounded, bit32, bit64>::operator-=(const bounded& n) {
        return operator=(operator-(n));
    }
    
    template <uint32_t size>
    inline math::number::division<number<size, true>> number<size, true>::divide(const number<size, true>& n) const {
        return math::number::extended_euclidian<number<size, true>>::divide(*this, n);
    }
    
    template <uint32_t size>
    inline math::number::division<number<size, false>> number<size, false>::divide(const number<size, false>& n) const {
        return math::number::extended_euclidian<number<size, false>, number<size, true>>::divide(*this, n);
    }
    
    template <uint32_t size>
    inline number<size, false> number<size, false>::operator/(const number<size, false>& n) const {
        return divide(n).Quotient;
    }
    
    template <uint32_t size>
    inline number<size, false> number<size, false>::operator%(const number<size, false>& n) const {
        return divide(n).Remainder;
    }
    
    template <uint32_t size>
    inline number<size, true> number<size, true>::operator/(const number<size, true>& n) const {
        return divide(n).Quotient;
    }
    
    template <uint32_t size>
    inline number<size, true> number<size, true>::operator%(const number<size, true>& n) const {
        return divide(n).Remainder;
    }

}

#endif
