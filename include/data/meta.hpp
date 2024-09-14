// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_META
#define DATA_META

#include <variant>
#include <data/types.hpp>

namespace data::meta {

    template <typename X, typename Y> struct rule;

    template <typename X, typename... rule> struct replace_with;

    template <typename X, typename... rule> using replace = replace_with<X, rule...>::type;

    template <typename X, typename Y> struct replace_with<X, rule<X, Y>> {
        using type = Y;
        static constexpr bool replaced = true;
    };

    template <typename X, typename x, typename Y> struct replace_with<X, rule<x, Y>> {
        using type = X;
        static constexpr bool replaced = false;
    };

    template <typename X> struct container {
        using type = X;
    };

    template <typename X> struct retriever {
        using type = X &;
    };

    template <typename X> struct setter {
        using type = const X &;
    };

    template <typename X> using contain = container<X>::type;
    template <typename X> using retrieve = retriever<X>::type;
    template <typename X> using set = setter<X>::type;

    template <std::integral X> struct setter<X> {
        using type = X;
    };

    template <> struct setter<bool> {
        using type = bool;
    };

    template <> struct container<void> {
        using type = std::monostate;
    };

    template <> struct retriever<void> {
        using type = void;
    };

    template <typename X> struct container<X &> {
        using type = std::reference_wrapper<X>;
    };

    template <typename X> struct retriever<X &> {
        using type = X &;
    };

    template <typename X> struct setter<X &> {
        using type = const X &;
    };

}

#endif
