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

namespace data::math::number::natural {
    
    // Generic division algorithm. 
    template <typename N>
    static division<N> divide (const N Dividend, const N Divisor) {
        if (Divisor == 0) throw division_by_zero {};
        if (Divisor == 1) return {Dividend, 0u};
        if (Divisor == 2) return {Dividend >> 1, Dividend & 1u};
        
        N pow {1};
        N exp {Divisor};
        N remainder {Dividend};
        N quotient {0};
        uint64 digits {1};
        
        while (exp <= remainder) { 
            exp <<= digits;
            pow <<= digits;
            digits <<= 1;
        } 

        while (true) {
            digits >>= 1;
            if (digits == 0) break;
            if (exp > remainder) {
                exp >>= digits;
                pow >>= digits;
            } else { 
                exp <<= digits;
                pow <<= digits;
            }
        }

        while (pow > 0) {
            while (exp > remainder) {
                exp >>= 1;
                pow >>= 1;
                if (pow == 0) goto out;
            }
            
            quotient += pow;
            remainder -= exp;
        }
        out: 
        return {quotient, remainder};
    }
    
    template <typename N> bool divides (const N &dividend, const N divisor) {
        return divide<N> (dividend, divisor).Remainder == 0;
    }
    
}

namespace data {
    
    template <size_t size> struct decimal {
        char Value[size] = {};
        bool Valid {false};
        
        constexpr decimal (const char (&input)[size]) noexcept {
            if (input[0] < '1' || input[0] > '9') return;
            for (size_t i {1}; i < size - 1; ++i) {
                if (input[i] < '1' || input[i] > '9') return;
            }
            if (input[size - 1] != 0) return;
            Valid = true;
            for (size_t i {0}; i < size; ++i) Value[i] = input[i];
        }
        
        constexpr operator uint64 () const {
            if (size > 20) return 0;
            uint64 x {0};
            uint64 digit {1};
            for (int i {size-2}; i >= 0; --i) {
                x += digit * static_cast<uint64> (Value[i] - '0');
                digit *= 10;
            }
            return x;
        }
    };

    template <> struct decimal<0>;
    template <> struct decimal<1>;
    
    template <> struct decimal<2> {
        char Value[2] = {};
        bool Valid {false};
        
        constexpr decimal (const char (&input)[2]) noexcept {
            if (input[0] < '0' || input[0] > '9') return;
            if (input[1] != 0) return;
            Valid = true;
            Value[0] = input[0];
            Value[1] = input[1];
        }
        
        constexpr operator uint64 () const {
            return static_cast<uint64> (Value[0] - '0');
        }
    };
    
    template <size_t N> decimal (const char (&)[N]) -> decimal<N>;
    template <size_t N> decimal (decimal<N>) -> decimal<N>;
}

// Peano axioms. 
namespace data::math::number::peano {
    
    constexpr decimal zero {"0"};
    
    template <auto&> struct 
    number;
    
    template <typename> struct 
    natural;
    
    // axiom 1: zero is a natural number. 
    template <> struct 
    natural<number<zero>> {};
    
    template <typename, typename> struct 
    equal;
    
    // axiom 2: equality is reflexive.
    template <typename x> struct 
    equal<x, x> {};
    
    // axiom 3: equality is symmetric. 
    template <typename x, typename y> 
    equal<y, x> symmetric_equal (equal<x, y>);
    
    // axiom 4: equality is transitive. 
    template <typename x, typename y, typename z> 
    equal<x, z> transitive_equal (equal<x, y>, equal<y, z>);
    
    template <typename x, typename y> 
    natural<y> closed_equal (equal<x, y>, natural<x>);
    
    template <typename> struct 
    suc;
    
    template <typename x> 
    natural<suc<x>> closed_successor (natural<x>);
    
    template <typename x, typename y>
    equal<suc<x>, suc<y>> injection_up (equal<x, y>);
    
    template <typename x, typename y>
    equal<x, y> injection_down (equal<suc<x>, suc<y>>);
    
    template <template<typename> typename predicate, typename x, typename y> 
    predicate<x> induction (natural<x>, predicate<number<zero>>, predicate<suc<y>> (*) (natural<y>, predicate<y>));
    
    template <typename, typename> struct add;
    
    template <typename x> struct
    equal<add<x, number<zero>>, x> {};
    
    template <typename x, typename y> struct
    equal<add<x, suc<y>>, suc<add<x, y>>> {};
    
    template <typename, typename> struct mul;
    
    template <typename x> struct
    equal<mul<x, number<zero>>, number<zero>> {};
    
    template <typename x, typename y> struct
    equal<mul<x, suc<y>>, add<x, mul<x, y>>> {};
    
}

#endif
