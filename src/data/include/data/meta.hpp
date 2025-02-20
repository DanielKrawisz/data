// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_META
#define DATA_META

#include <data/types.hpp>
namespace data {
    namespace meta {
        // wrapper enables std types to use references and void.
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
    }

    template <typename X> using wrapped = meta::wrapper<X>::type;

    namespace meta {
        // avoid copying certain types when we retrieve the
        // value from a data structure.
        template <typename X> struct retriever {
            using type = X &;
        };

        template <> struct retriever<void> {
            using type = void;
        };

        template <typename X> struct retriever<X &> {
            using type = X &;
        };
    }

    template <typename X> using retrieved = meta::retriever<X>::type;

    namespace meta {
        // insert has to do with values that put into a data structure.
        template <typename X> struct insert {
            using type = const X &;
        };

        template <typename X> struct insert<X &> {
            using type = X &;
        };

        template <typename X> struct insert<const X &> {
            using type = const X &;
        };

        template <typename X> struct insert<X *> {
            using type = X *;
        };

        template <typename X> struct insert<const X *> {
            using type = const X *;
        };

        template <typename X> struct insert<X *const> {
            using type = X *const;
        };
    }

    template <typename X> using inserted = meta::insert<X>::type;

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
