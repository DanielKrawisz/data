// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_INFINITE
#define DATA_MATH_INFINITE

#include <data/types.hpp>

namespace data::math {
    
    template <typename X> struct unsigned_limit {
        X Value;
        byte Infinite;
        
        constexpr unsigned_limit(const X& x) : Value{x}, Infinite{0} {}
        constexpr unsigned_limit() : Value{0}, Infinite{1} {}
        
        constexpr bool is_infinite() const {
            return Infinite != 0;
        }
        
        constexpr static unsigned_limit infinity() {
            return {0, 1};
        }
        
        constexpr std::weak_ordering operator<=>(const unsigned_limit& x) {
            return is_infinite() ? (x.is_infinite() ? std::weak_ordering::equivalent : std::weak_ordering::greater) : Value<=>x;
        }
        
        constexpr unsigned_limit operator+(const X& x) const {
            if (is_infinite()) return infinity();
            return {Value + x};
        }
        
        constexpr unsigned_limit operator*(const X& x) const {
            if (is_infinite()) return infinity();
            return {Value * x};
        }
        
    private:
        unsigned_limit(const X& x, const byte b) : Value{x}, Infinite{b} {}
    };
    
    template <typename X> struct signed_limit {
        X Value;
        char Infinite;
        
        signed_limit(const X& x) : Value{x}, Infinite{0} {}
        
        bool is_infinite() const {
            return Infinite != 0;
        }
        
        bool is_positive_infinite() const {
            return Infinite > 0;
        }
        
        bool is_negative_infinite() const {
            return Infinite < 0;
        }
        
        std::weak_ordering operator<=>(const signed_limit& x) {
            return is_positive_infinite() ? (x.is_positive_infinite() ? std::weak_ordering::equivalent : std::weak_ordering::greater) : 
                is_negative_infinite() ? (x.is_negative_infinite() ? std::weak_ordering::equivalent : std::weak_ordering::less) : 
                Value<=>x;
        }
        
        static signed_limit infinity() {
            return {0, 1};
        }
        
        static signed_limit negative_infinity() {
            return {0, -1};
        }
        
    private:
        signed_limit(const X& x, const char b) : Value{x}, Infinite{b} {}
    };
    
    template <std::integral X, std::integral Y> 
    std::weak_ordering operator<=>(const unsigned_limit<X>& a, Y b) {
        return a.is_infinite() ? std::weak_ordering::greater : a.Value <=> b;
    }
    
    template <std::integral X, std::integral Y> 
    std::weak_ordering operator<=>(const signed_limit<X>& a, Y b) {
        return a.is_positive_infinite() ? std::weak_ordering::greater : 
            a.is_negative_infinite() ? std::weak_ordering::less : a.Value <=> b;
    }
    
    template <std::integral X, std::integral Y> 
    bool operator==(const unsigned_limit<X>& a, Y b) {
        return (a <=> b) == 0;
    }
    
    template <std::integral X, std::integral Y> 
    bool operator==(const signed_limit<X>& a, Y b) {
        return (a <=> b) == 0;
    }
}

template <typename X>
inline auto operator<=>(const data::math::unsigned_limit<X>& a, const data::math::unsigned_limit<X>& b) -> decltype(a.Value <=> b.Value) {
    if (a.is_infinite()) return b.is_infinite() ? 0 : 1;
    if (b.is_infinite()) return -1;
    return a.Value<=>b.Value;
}

template <typename X>
inline data::math::unsigned_limit<X> operator+(const data::math::unsigned_limit<X>& a, const data::math::unsigned_limit<X>& b) {
    if (a.is_infinite() || b.is_infinite()) return data::math::unsigned_limit<X>::infinity();
    return {a.Value + b.Value};
}

template <typename X>
inline auto operator<=>(const data::math::signed_limit<X>& a, const data::math::signed_limit<X>& b) -> decltype(a.Value <=> b.Value) {
    if (a.is_infinite()) return b.is_infinite() ? 0 : 1;
    if (a.is_negative_infinite()) return b.is_negative_infinite() ? 0 : -1;
    if (b.is_infinite()) return -1;
    if (b.is_negative_infinite()) return 1;
    return a.Value<=>b.Value;
}

namespace data {
    // we don't use all the cardinal numbers because
    // infinite is as big as we can get for now. 
    using cardinality = math::unsigned_limit<size_t>;
}

#endif 

