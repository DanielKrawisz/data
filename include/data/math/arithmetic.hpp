// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ARITHMETIC
#define DATA_MATH_ARITHMETIC

#include <data/iterable.hpp>
#include <type_traits>

namespace data {
    
    namespace meta {
        
        template <typename X>
        class has_plus_operator {
            template <typename U> static auto test(int) -> typename 
                std::enable_if<std::is_same<decltype(std::declval<const U>() + std::declval<const U>()), U>::value, yes>::type;
            template <typename> static no test(...);
        public:
            static constexpr bool value = std::is_same<decltype(test<X>(0)), yes>::value;
        };
        
        template <typename X, bool has_plus_operator> struct plus;
        
        template <typename X> struct plus<X, true> {
            
            X operator()(const X& a, const X& b) {
                return a + b;
            }
        };
        
        template <typename X>
        class has_times_operator {
            template <typename U> static auto test(int) -> typename 
                std::enable_if<std::is_same<decltype(std::declval<const U>() * std::declval<const U>()), U>::value, yes>::type;
            template <typename> static no test(...);
        public:
            static constexpr bool value = std::is_same<decltype(test<X>(0)), yes>::value;
        };
        
        template <typename X, bool has_times_operator> struct times;
        
        template <typename X> struct times<X, true> {
            
            X operator()(const X& a, const X& b) {
                return a * b;
            }
        };
        
        template <typename X>
        class has_minus_operator {
            template <typename U> static auto test(int) -> typename 
                std::enable_if<std::is_same<decltype(std::declval<const U>() - std::declval<const U>()), U>::value, yes>::type;
            template <typename> static no test(...);
        public:
            static constexpr bool value = std::is_same<decltype(test<X>(0)), yes>::value;
        };
        
        template <typename X, bool has_minus_operator> struct minus;
        
        template <typename X> struct minus<X, true> {
            
            X operator()(const X& a, const X& b) {
                return a - b;
            }
        };
        
        template <typename dividend, typename divisor, typename quotient>
        class has_divide_operator {
            template <typename U> static auto test(int) -> typename 
                std::enable_if<std::is_same<decltype(std::declval<const U>() / std::declval<const divisor>()), quotient>::value, yes>::type;
            template <typename> static no test(...);
        public:
            static constexpr bool value = std::is_same<decltype(test<dividend>(0)), yes>::value;
        };
        
        template <typename quotient, typename dividend, typename divisor, bool has_divide_operator> struct divide;
        
        template <typename quotient, typename dividend, typename divisor> 
        struct divide<quotient, dividend, divisor, true> {
            
            quotient operator()(const dividend& a, const divisor& b) {
                return a / b;
            }
        };
        
        template <typename dividend, typename divisor, typename quotient>
        class has_mod_operator {
            template <typename U> static auto test(int) -> typename 
                std::enable_if<std::is_same<decltype(std::declval<const U>() % std::declval<const divisor>()), quotient>::value, yes>::type;
            template <typename> static no test(...);
        public:
            static constexpr bool value = std::is_same<decltype(test<dividend>(0)), yes>::value;
        };
        
        template <typename quotient, typename dividend, typename divisor, bool has_divide_operator> struct mod;
        
        template <typename quotient, typename dividend, typename divisor> 
        struct mod<quotient, dividend, divisor, true> {
            
            quotient operator()(const dividend& a, const divisor& b) {
                return a % b;
            }
        };
        
        template <typename F, typename Q>
        class has_quadrance_method {
            template <typename U> static auto test(int) -> typename 
                std::enable_if<std::is_same<decltype(std::declval<const U>().quadrance()), Q>::value, yes>::type;
            template <typename> static no test(...);
        public:
            static constexpr bool value = std::is_same<decltype(test<F>(0)), yes>::value;
        };
        
    }

    template <typename X> struct plus {
        X operator()(const X& a, const X& b) {
            return meta::plus<X, meta::has_plus_operator<X>::value>{}(a, b);
        }
        
        static const X identity() {
            static X Identity{0};
            return Identity;
        }
    };

    template <typename X> struct times {
        X operator()(const X& a, const X& b) {
            return meta::times<X, meta::has_times_operator<X>::value>{}(a, b);
        }
        
        static const X identity() {
            static X Identity{1};
            return Identity;
        }
    };

    template <typename X> struct minus {
        X operator()(const X& a, const X& b) {
            return meta::minus<X, meta::has_minus_operator<X>::value>{}(a, b);
        }
    };
/*
    template <typename quotient, typename dividend, typename divisor> struct divide {
        quotient operator()(const dividend& a, const divisor& b) {
            return meta::divide<quotient, dividend, divisor, 
                meta::has_divide_operator<quotient, dividend, divisor>::value>{}(a, b);
        }
    };

    template <typename quotient, typename dividend, typename divisor> struct mod {
        quotient operator()(const dividend& a, const divisor& b) {
            return meta::mod<quotient, dividend, divisor, 
                meta::has_divide_operator<quotient, dividend, divisor>::value>{}(a, b);
        }
    };*/

}

namespace data::math::arithmetic {
    
    template <typename word, endian::order r> struct number;
    
    template <typename word> struct number<word, endian::big> {
        slice<word> Data;
        
        constexpr static endian::order endian = endian::big;
        constexpr static endian::order opposite = endian::little;
        
        const word& operator[](uint32) const;
        
        word& operator[](uint32);
        
    };
    
    template <typename word> struct number<word, endian::little> {
        slice<word> Data;
        
        constexpr static endian::order endian = endian::little;
        constexpr static endian::order opposite = endian::big;
        
        const word& operator[](uint32) const;
        
        word& operator[](uint32);
    };
    
    template <typename word, endian::order r>
    void bit_negate(number<word, r>, const number<word, r> a);
    
    template <typename word, endian::order r>
    void negate(number<word, r>, const number<word, r> a);
    
    template <typename word, endian::order r>
    void bit_and(number<word, r>, const number<word, r> a, const number<word, r> b);
    
    template <typename word, endian::order r>
    void bit_or(number<word, r>, const number<word, r> a, const number<word, r> b);
    
    template <typename word, endian::order r>
    void plus(number<word, r>, const number<word, r> a, word b);
    
    template <typename word, endian::order r>
    void times(number<word, r>, const number<word, r> a, word b);
    
    template <typename word, endian::order r>
    void minus(number<word, r>, const number<word, r> a, word b);
    
    template <typename word, endian::order r>
    void plus(number<word, r>, const number<word, r> a, number<word, r> b);
    
    template <typename word, endian::order r>
    void times(const number<word, r> a, number<word, r> b);
    
    template <typename word, endian::order r>
    void minus(const number<word, r> a, number<word, r> b);
    
};

#endif

