// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MAYBE
#define DATA_MAYBE

#include <optional>
#include <data/concepts.hpp>
#include <data/meta.hpp>

namespace data {

    template <typename X> struct maybe : std::optional<wrapped<X>> {
        using std::optional<wrapped<X>>::optional;
        constexpr maybe (std::optional<wrapped<X>> &&x) : std::optional<wrapped<X>> {x} {}

        constexpr retrieved<X> operator * () {
            return (retrieved<X>) *static_cast<std::optional<wrapped<X>> &> (*this);
        }

        constexpr const retrieved<X> operator * () const {
            return (const retrieved<X>) *static_cast<const std::optional<wrapped<X>> &> (*this);
        }
    };

    template <> struct maybe<void> {
        maybe () = delete;
    };

    template <typename X> requires requires (const X &a, const X &b) {
        { a == b } -> implicitly_convertible_to<bool>;
    } bool operator == (const maybe<X> &a, const maybe<X> &b) {
        return bool (a) != bool (b) ? false : !bool (a) ? true : *a == *b;
    }

    template <typename X> requires requires (const X &a, const X &b) {
        { a == b } -> implicitly_convertible_to<bool>;
    } bool operator == (const maybe<X> &a, const X &b) {
        return bool (a) ? *a == b : false;
    }

    template <typename X> requires requires (const X &a, const X &b) {
        { a + b } -> implicitly_convertible_to<bool>;
    } maybe<X> operator + (const maybe<X> &a, const maybe<X> &b) {
        return bool (a) ? (bool (b) ? maybe<X> {*a + *b} : a) : bool (b) ? b : maybe<X> {};
    }

    template <typename X> requires requires (const X &a, const X &b) {
        { a + b } -> implicitly_convertible_to<bool>;
    } maybe<X> operator + (const maybe<X> &a, const X &b) {
        return bool (a) ? maybe<X> {*a + b} : maybe<X> {b};
    }

    template <typename X> requires requires (const X &a, const X &b) {
        { a * b } -> implicitly_convertible_to<bool>;
    } maybe<X> operator * (const maybe<X> &a, const maybe<X> &b) {
        return bool (a) && bool (b) ? maybe<X> {*a * *b} : maybe<X> {};
    }

    template <typename X> requires requires (const X &a, const X &b) {
        { a * b } -> implicitly_convertible_to<bool>;
    } maybe<X> operator * (const maybe<X> &a, const X &b) {
        return bool (a) ? maybe<X> {*a * b} : maybe<X> {};
    }

    template <typename X> requires requires (std::ostream &o, const X &x) {
        { o << x } -> std::same_as<std::ostream &>;
    } std::ostream inline &operator << (std::ostream &o, const maybe<X> &x) {
        o << std::string{"*"};
        if (bool (x)) o << *x;
        return o;
    }

}

#endif
