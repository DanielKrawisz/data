// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ORDERED
#define DATA_MATH_ORDERED

#include <type_traits>
#include <algorithm>
#include <data/concepts.hpp>

namespace data {
    
    template <typename X> concept Ordered = std::totally_ordered<X>;
    template <typename X> concept Sortable = requires (const X &a, const X &b) {
        {a < b} -> ImplicitlyConvertible<bool>;
    };

    // we use this in the priority queue.
    template <typename element>
    concept Prioritized = Copyable<element> && requires (const element &a, const element &b) {
        {a <= b} -> ImplicitlyConvertible<bool>;
    };
    
    template <typename X, typename Y> requires 
    requires (const X x, const X y) {
        { x == y } -> Same<bool>;
    } constexpr bool inline equal (const X &a, const Y &b) {
        return a == b;
    }
    
    template <typename X, typename Y> requires 
    requires (const X x, const X y) {
        { x != y } -> Same<bool>;
    } constexpr bool inline unequal (const X &a, const Y &b) {
        return a != b;
    }

    template <typename X, typename Y> requires 
    requires (const X x, const Y y) {
        { x <= y } -> Same<bool>;
    } constexpr bool inline less_equal (const X &a, const Y &b) {
        return a <= b;
    }

    template <typename X, typename Y> requires 
    requires (const X x, const Y y) {
        { x >= y } -> Same<bool>;
    } constexpr bool inline greater_equal (const X &a, const Y &b) {
        return a >= b;
    }

    template <typename X, typename Y> requires 
    requires (const X x, const Y y) {
        { x < y } -> Same<bool>;
    } constexpr bool inline less (const X &a, const Y &b) {
        return a < b;
    }

    template <typename X, typename Y> requires 
    requires (const X x, const Y y) {
        { x > y } -> Same<bool>;
    } constexpr bool inline greater (const X &a, const Y &b) {
        return a > b;
    }

    template <Ordered X> constexpr const X inline &max (const X &a, const X &b) {
        return std::max (a, b);
    }

    template <Ordered X> constexpr const X inline &min (const X &a, const X &b) {
        return std::max (a, b);
    }

    // asks whether min <= num && max >= num
    template <typename A> constexpr A inline within (const A &num, const A &min, const A &max) {
        return greater_equal (num, min) && less_equal (num, max);
    }
    
    
}

#endif

