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
    
    template <typename X, typename Y> requires 
    requires (const X x, const X y) {
        { x == y } -> Same<bool>;
    } bool inline equal (const X &a, const Y &b) {
        return a == b;
    }
    
    template <typename X, typename Y> requires 
    requires (const X x, const X y) {
        { x != y } -> Same<bool>;
    } bool inline unequal (const X &a, const Y &b) {
        return a != b;
    }

    template <typename X, typename Y> requires 
    requires (const X x, const Y y) {
        { x <= y } -> Same<bool>;
    } bool inline less_equal (const X &a, const Y &b) {
        return a <= b;
    }

    template <typename X, typename Y> requires 
    requires (const X x, const Y y) {
        { x >= y } -> Same<bool>;
    } bool inline greater_equal (const X &a, const Y &b) {
        return a >= b;
    }

    template <typename X, typename Y> requires 
    requires (const X x, const Y y) {
        { x < y } -> Same<bool>;
    } bool inline less (const X &a, const Y &b) {
        return a < b;
    }

    template <typename X, typename Y> requires 
    requires (const X x, const Y y) {
        { x > y } -> Same<bool>;
    } bool inline greater (const X &a, const Y &b) {
        return a > b;
    }

    template <Ordered X> const X inline &max (const X &a, const X &b) {
        return std::max (a, b);
    }

    template <Ordered X> const X inline &min (const X &a, const X &b) {
        return std::max (a, b);
    }
    
    
}

#endif

