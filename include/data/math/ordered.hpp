// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ORDERED
#define DATA_MATH_ORDERED

#include <type_traits>
#include <data/types.hpp>

namespace data::meta {
    
    template <typename X>
    class has_less_operator {
        template <typename U> static auto test(int) -> typename 
            std::enable_if<std::is_same<decltype(std::declval<const U>().operator<(std::declval<const U>())), bool>::value, yes>::type;
        template <typename> static no test(...);
    public:
        static constexpr bool value = std::is_same<decltype(test<X>(0)), yes>::value;
    };
    
    template <typename X>
    class has_greater_operator {
        template <typename U> static auto test(int) -> typename 
            std::enable_if<std::is_same<decltype(std::declval<const U>().operator>(std::declval<const U>())), bool>::value, yes>::type;
        template <typename> static no test(...);
    public:
        static constexpr bool value = std::is_same<decltype(test<X>(0)), yes>::value;
    };
    
    template <typename X>
    class has_less_equal_operator {
        template <typename U> static auto test(int) -> typename 
            std::enable_if<std::is_same<decltype(std::declval<const U>().operator<=(std::declval<const U>())), bool>::value, yes>::type;
        template <typename> static no test(...);
    public:
        static constexpr bool value = std::is_same<decltype(test<X>(0)), yes>::value;
    };
    
    template <typename X>
    class has_greater_equal_operator {
        template <typename U> static auto test(int) -> typename 
            std::enable_if<std::is_same<decltype(std::declval<const U>().operator>=(std::declval<const U>())), bool>::value, yes>::type;
        template <typename> static no test(...);
    public:
        static constexpr bool value = std::is_same<decltype(test<X>(0)), yes>::value;
    };
    
    template <typename X>
    class has_equal_operator {
        template <typename U> static auto test(int) -> typename 
            std::enable_if<std::is_same<decltype(std::declval<const U>().operator==(std::declval<const U>())), bool>::value, yes>::type;
        template <typename> static no test(...);
    public:
        static constexpr bool value = std::is_same<decltype(test<X>(0)), yes>::value;
    };
    
    template <typename X>
    class has_unequal_operator {
        template <typename U> static auto test(int) -> typename 
            std::enable_if<std::is_same<decltype(std::declval<const U>().operator!=(std::declval<const U>())), bool>::value, yes>::type;
        template <typename> static no test(...);
    public:
        static constexpr bool value = std::is_same<decltype(test<X>(0)), yes>::value;
    };
}

namespace data::interface {
    
    template <typename Q>
    struct ordered {
    private:
        using require_less_operator = typename std::enable_if<meta::has_less_operator<Q>::value, void>::type;
        using require_greater_operator = typename std::enable_if<meta::has_greater_operator<Q>::value, void>::type;
        using require_less_equal_operator = typename std::enable_if<meta::has_less_equal_operator<Q>::value, void>::type;
        using require_greater_equal_operator = typename std::enable_if<meta::has_greater_equal_operator<Q>::value, void>::type;
        using require_equal_operator = typename std::enable_if<meta::has_equal_operator<Q>::value, void>::type;
        using require_unequal_operator = typename std::enable_if<meta::has_unequal_operator<Q>::value, void>::type;
    }; 

}

namespace data::meta {
    template <typename X, bool has_equal_operator> struct is_equal;
    
    template <typename X, bool has_unequal_operator> struct is_unequal {
        bool operator()(const X& a, const X& b) {
            return !(a == b);
        }
    };
    
    template <typename X, bool has_less_equal_operator> struct is_less_equal {
        bool operator()(const X& a, const X& b) {
            return b > a;
        }
    };
    
    template <typename X, bool has_greater_equal_operator> struct is_greater_equal {
        bool operator()(const X& a, const X& b) {
            return b < a;
        }
    };
    
    template <typename X, bool has_less_equal_operator> struct is_less {
        bool operator()(const X& a, const X& b) {
            return b >= a;
        }
    };
    
    template <typename X, bool has_greater_equal_operator> struct is_greater {
        bool operator()(const X& a, const X& b) {
            return b <= a;
        }
    };
    
    template <typename X> struct is_equal<X, true> {
        bool operator()(const X& a, const X& b) {
            return a == b;
        }
    };
    
    template <typename X> struct is_unequal<X, true> {
        bool operator()(const X& a, const X& b) {
            return a != b;
        }
    };
    
    template <typename X> struct is_less_equal<X, true> {
        bool operator()(const X& a, const X& b) {
            return a <= b;
        }
    };
    
    template <typename X> struct is_greater_equal<X, true> {
        bool operator()(const X& a, const X& b) {
            return a >= b;
        }
    };
    
    template <typename X> struct is_less<X, true> {
        bool operator()(const X& a, const X& b) {
            return a < b;
        }
    };
    
    template <typename X> struct is_greater<X, true> {
        bool operator()(const X& a, const X& b) {
            return a > b;
        }
    };
}

namespace data {
    
    template <typename X> bool equal(const X& a, const X& b) {
        return meta::is_equal<X, meta::has_equal_operator<X>::value>{}(a, b);
    }
    
    template <typename X> bool unequal(const X& a, const X& b) {
        return meta::is_equal<X, meta::has_unequal_operator<X>::value>{}(a, b);
    }
    
    template <typename X> bool less_equal(const X& a, const X& b) {
        return meta::is_equal<X, meta::has_less_equal_operator<X>::value>{}(a, b);
    }
    
    template <typename X> bool greater_equal(const X& a, const X& b) {
        return meta::is_equal<X, meta::has_greater_equal_operator<X>::value>{}(a, b);
    }
    
    template <typename X> bool less(const X& a, const X& b) {
        return meta::is_equal<X, meta::has_less_operator<X>::value>{}(a, b);
    }
    
    template <typename X> bool greater(const X& a, const X& b) {
        return meta::is_equal<X, meta::has_greater_operator<X>::value>{}(a, b);
    }
    
}

#endif

