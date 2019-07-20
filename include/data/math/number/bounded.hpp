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

namespace data::math::number {
    
    template <uint32_t size>
    struct bounded : public std::array<byte, size * 4> {
        using parent = std::array<byte, size * 4>;
        using word = data::number::endian::big<uint64>;
        using index = uint32;
    private:
        static const index last = size - 1;
        
        static uint32 lesser(word w);
        static uint32 greater(word w);
        static word extend(uint32);
        
        static bool overflow(word w) {
            return greater(w) != 0;
        }
        
        uint32& operator[](index i) const {
            if (i >= size) throw 0;
            return *(uint32*)(parent::data() + 4 * i);
        }
        
    public:
        bounded() : parent() {}
        bounded(uint32 x) : bounded() {
            operator[](last) = x;
        }
        
        bounded(const std::array<byte, 4 * size>& b);
        bounded(vector<byte>& v);
        
        static bounded zero() {
            return bounded{};
        }

        bool operator<(const bounded& d) const;
        bool operator<=(const bounded& d) const;
        bool operator>(const bounded& d) const;
        bool operator>=(const bounded& d) const;
        bool operator==(const bounded& d) const;
        bool operator!=(const bounded& d) const;
            
        bounded operator~() const;
        bounded operator|(const bounded&) const;
        bounded operator^(const bounded&) const;
            
        bounded operator-() const;
        bounded operator-(const bounded&) const;
        bounded operator+(const bounded&) const;
            
        bounded operator<<(uint32) const;
        bounded operator>>(uint32) const;
            
        bounded& operator|=(const bounded&);
        bounded& operator^=(const bounded&);
            
        bounded& operator+=(const bounded&);
        bounded& operator-=(const bounded&);
            
        bounded& operator<<=(uint32);
        bounded& operator>>=(uint32);
        
        static void minus(const bounded&, const bounded&, bounded&);
        static void plus(const bounded&, const bounded&, bounded&);
        
        using iterator = index_iterator<bounded&, uint32&>;

        iterator begin();
        iterator end();
        const iterator begin() const;
        const iterator end() const;
        
        constexpr static math::group::abelian<bounded> is_group{}; 
    };
    
    template <uint32_t size>
    inline bool bounded<size>::operator>(const bounded& d) const {
        return d <= *this;
    }
    
    template <uint32_t size>
    inline bool bounded<size>::operator>=(const bounded& d) const {
        return d < *this;
    }
    
    template <uint32_t size>
    inline bool bounded<size>::operator==(const bounded& d) const {
        return static_cast<const parent&>(*this) == static_cast<const parent&>(d);
    }
    
    template <uint32_t size>
    inline bool bounded<size>::operator!=(const bounded& d) const {
        return static_cast<const parent&>(*this) != static_cast<const parent&>(d);
    }
    
    template <uint32_t size>
    inline typename bounded<size>::iterator bounded<size>::begin() {
        return iterator{*this, uint(0)};
    }
    
    template <uint32_t size>
    inline typename bounded<size>::iterator bounded<size>::end() {
        return iterator{*this, size};
    }
    
    template <uint32_t size>
    inline const typename bounded<size>::iterator bounded<size>::begin() const {
        return iterator{*const_cast<bounded*>(this), uint(0)};
    }
    
    template <uint32_t size>
    inline const typename bounded<size>::iterator bounded<size>::end() const {
        return iterator{*const_cast<bounded*>(this), size};
    }
    
    template <uint32 size> 
    inline bounded<size> bounded<size>::operator-() const {
        return bounded<size>{} - *this;
    }
    
    namespace low {
    
        template <uint32 size> 
        inline void bit_not(bounded<size>& n) {
            for (uint32& u : n) u = ~u;
        }
    
        template <uint32 size> 
        inline void bit_and(const bounded<size>& a, const bounded<size>& b, bounded<size>& result) {
            for (uint32 i = 0; i < size; i++) result[i] = a[i]^b[i];
        }
    
        template <uint32 size> 
        inline void bit_or(const bounded<size>& a, const bounded<size>& b, bounded<size>& result) {
            for (uint32 i = 0; i < size; i++) result[i] = a[i]|b[i];
        }
    
    }
    
    template <uint32 size> 
    inline bounded<size> bounded<size>::operator~() const {
        bounded<size> n = *this;
        low::bit_not(n);
        return n;
    }
    
    template <uint32_t size>
    inline bounded<size>& bounded<size>::operator|=(const bounded& n) {
        low::bit_and(*this, n, *this);
        return *this;
    }
    
    template <uint32_t size>
    inline bounded<size>& bounded<size>::operator^=(const bounded& n) {
        low::bit_and(*this, n, *this);
        return *this;
    }
    
    template <uint32 size> 
    inline bounded<size> bounded<size>::operator+(const bounded& n) const {
        throw 0;
    }
    
    template <uint32 size> 
    inline bounded<size> bounded<size>::operator-(const bounded& n) const {
        throw 0;
    }
    
    template <uint32_t size>
    inline bounded<size>& bounded<size>::operator+=(const bounded& n) {
        return operator=(operator+(n));
    }
    
    template <uint32_t size>
    inline bounded<size>& bounded<size>::operator-=(const bounded& n) {
        return operator=(operator-(n));
    }

}

#endif
