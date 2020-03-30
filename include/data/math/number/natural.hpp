// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_NATURAL
#define DATA_MATH_NUMBER_NATURAL

#include <data/types.hpp>
#include <data/math/countable.hpp>
#include <data/math/arithmetic.hpp>
#include <data/math/ordered.hpp>
#include <data/math/ring.hpp>
#include <data/math/division.hpp>
#include <data/math/commutative.hpp>
#include <data/math/associative.hpp>

namespace data::interface {
    template <typename N>
    struct natural : 
        countable<N>, ordered<N>, 
        math::associative<plus<N>, N>, 
        math::commutative<plus<N>, N>, 
        math::associative<times<N>, N>, 
        math::commutative<times<N>, N> {
    private:
        using require_plus_operator = typename std::enable_if<meta::has_plus_operator<N>::value, void>::type;
        using require_minus_operator = typename std::enable_if<meta::has_minus_operator<N>::value, void>::type;
        using require_times_operator = typename std::enable_if<meta::has_times_operator<N>::value, void>::type;
        using require_division_operator = typename std::enable_if<meta::has_divide_operator<N, N, N>::value, void>::type;
        using require_mod_operator = typename std::enable_if<meta::has_mod_operator<N, N, N>::value, void>::type;
    };
}

namespace data::math::number::natural {
    
    // Generic division algorithm. 
    template <typename N>
    static division<N> divide(const N Dividend, const N Divisor) {
        if (Divisor == 0) throw division_by_zero{};
        if (Divisor == 1) return {Dividend, 0};
        
        N pow = 1;
        N exp = Divisor;
        N remainder = Dividend;
        N quotient = 0;
        uint64 digits = 1;
        
        while (exp <= remainder) { 
            exp<<=digits;
            pow<<=digits;
            digits<<=1;
        } 
        
        while(true) {
            digits >>= 1;
            if (digits == 0) break;
            if (exp > remainder) {
                exp>>=digits;
                pow>>=digits;
            } else { 
                exp<<=digits;
                pow<<=digits;
            }
        }
        
        while (pow > 0) {
            while (exp > remainder) {
                exp>>=1;
                pow>>=1;
                if (pow == 0) goto out;
            }
            
            quotient += pow;
            remainder -= exp;
        }
        out: 
        
        return {quotient, remainder};
    }
    
}

namespace data {
    
    template <size_t size> struct decimal {
        char Value[size] = {};
        bool Valid{false};
        
        constexpr decimal(const char (&input)[size]) noexcept {
            if (input[0] < '1' || input[0] > '9') return;
            for (size_t i{1}; i < size - 1; ++i) {
                if (input[i] < '1' || input[i] > '9') return;
            }
            if (input[size - 1] != 0) return;
            Valid = true;
            for (size_t i{0}; i < size; ++i) Value[i] = input[i];
        }
        
        constexpr operator uint64() const {
            if (size > 20) return 0;
            uint64 x{0};
            uint64 digit{1};
            for (int i{size-2}; i >= 0; --i) {
                x += digit * static_cast<uint64>(Value[i] - '0');
                digit *= 10;
            }
            return x;
        }
    };

    template <> struct decimal<0>;
    template <> struct decimal<1>;
    
    template <> struct decimal<2> {
        char Value[2] = {};
        bool Valid{false};
        
        constexpr decimal(const char (&input)[2]) noexcept {
            if (input[0] < '0' || input[0] > '9') return;
            if (input[1] != 0) return;
            Valid = true;
            Value[0] = input[0];
            Value[1] = input[1];
        }
        
        constexpr operator uint64() const {
            return static_cast<uint64>(Value[0] - '0');
        }
    };
    
    template <size_t N> decimal(const char (&)[N]) -> decimal<N>;
    template <size_t N> decimal(decimal<N>) -> decimal<N>;
}

#endif
