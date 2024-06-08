// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MAYBE
#define DATA_MAYBE

#include <optional>
#include <data/types.hpp>

namespace data {

    template <typename X> struct maybe : std::optional<X> {
        using std::optional<X>::optional;
        maybe (std::optional<X> &&x) : std::optional<X> {x} {}
    };

    template <typename X> requires requires (const X &a, const X &b) {
        { a == b } -> std::convertible_to<bool>;
    } bool operator == (const maybe<X> &a, const maybe<X> &b) {
        return bool (a) != bool (b) ? false : !bool (a) ? true : *a == *b;
    }

    template <typename X> requires requires (const X &a, const X &b) {
        { a == b } -> std::convertible_to<bool>;
    } bool operator == (const maybe<X> &a, const X &b) {
        return bool (a) ? *a == b : false;
    }

    template <typename X> requires requires (const X &a, const X &b) {
        { a + b } -> std::convertible_to<bool>;
    } maybe<X> operator + (const maybe<X> &a, const maybe<X> &b) {
        return bool (a) ? (bool (b) ? maybe<X> {*a + *b} : a) : bool (b) ? b : maybe<X> {};
    }

    template <typename X> requires requires (const X &a, const X &b) {
        { a + b } -> std::convertible_to<bool>;
    } maybe<X> operator + (const maybe<X> &a, const X &b) {
        return bool (a) ? maybe<X> {*a + b} : maybe<X> {b};
    }

    template <typename X> requires requires (const X &a, const X &b) {
        { a * b } -> std::convertible_to<bool>;
    } maybe<X> operator * (const maybe<X> &a, const maybe<X> &b) {
        return bool (a) && bool (b) ? maybe<X> {*a * *b} : maybe<X> {};
    }

    template <typename X> requires requires (const X &a, const X &b) {
        { a * b } -> std::convertible_to<bool>;
    } maybe<X> operator * (const maybe<X> &a, const X &b) {
        return bool (a) ? maybe<X> {*a * b} : maybe<X> {};
    }

    template <typename X> requires requires (std::ostream &o, const X &x) {
        { o << x } -> std::same_as<std::ostream &>;
    } std::ostream inline &operator << (std::ostream &o, const maybe<X> &x) {
        o << "*";
        if (bool (x)) o << *x;
        return o;
    }

}

#endif
