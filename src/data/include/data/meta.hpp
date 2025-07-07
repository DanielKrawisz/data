// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_META
#define DATA_META

#include <data/types.hpp>
#include <data/concepts.hpp>
#include <variant> // for std::monostate

namespace data {
    namespace meta {
        // wrapper enables std types to use references and void.
        template <typename X> struct wrapper;

        // insert has to do with values that we put into a data structure.
        template <typename X> struct insert;

        // this is for if you want a container to be able to contain void.
        template <typename X> struct retrieve;
    }
    
    template <typename X> using wrapped = meta::wrapper<X>::type;

    template <typename X> using inserted = meta::insert<X>::type;

    template <typename X> using retrieved = meta::retrieve<X>::type;
    
    namespace meta {
        template <typename X> struct wrapper {
            using type = X;
        };

        template <typename X> struct wrapper<X &> {
            using type = std::reference_wrapper<X>;
        };

        template <typename X> struct wrapper<const X &> {
            using type = std::reference_wrapper<const X>;
        };

        template <> struct wrapper<void> {
            using type = std::monostate;
        };

        template <typename X> struct retrieve {
            using type = X &;
        };

        template <> struct retrieve<void> {
            using type = void;
        };
        
        template <typename X> struct insert {
            using type = const X &;
        };

        template <typename X> struct insert<X &> {
            using type = X &;
        };

        template <typename X> struct insert<X *> {
            using type = X *;
        };

        template <std::integral X> struct insert<X> {
            using type = X;
        };

        template <> struct insert<void> {
            using type = void;
        };

    }

    static_assert (Same<inserted<const int &>, const int &>);

}

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

    template <typename X> struct setter {
        using type = const X &;
    };

    template <typename X> using set = setter<X>::type;

    template <std::integral X> struct setter<X> {
        using type = X;
    };

    template <> struct setter<bool> {
        using type = bool;
    };

    template <typename X> struct setter<X &> {
        using type = const X &;
    };

}

#endif
