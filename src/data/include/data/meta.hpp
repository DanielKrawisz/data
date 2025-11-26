// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_META
#define DATA_META

#include <data/types.hpp>
#include <data/concepts.hpp>
#include <variant> // for std::monostate

namespace data {
    template <class X>
    using ref_to_ptr =
        std::conditional_t<
            std::is_reference_v<X>,
                std::add_pointer_t<std::remove_reference_t<X>>,
                X>;

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

        static_assert (Same<inserted<const int &>, const int &>);

        // strange but true
        using intref = int &;
        static_assert (Same<const intref &, int &>);

    }

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

    // get type out of a parameter pack by index
    template <std::size_t I, typename... Ts>
    struct get_type;

    template <typename T, typename... Ts>
    struct get_type<0, T, Ts...> {
        using type = T;
    };

    template <std::size_t I, typename T, typename... Ts>
    struct get_type<I, T, Ts...> {
        using type = typename get_type<I - 1, Ts...>::type;
    };

    // get value out of a parameter pack by index.
    template <std::size_t I, auto... Ts>
    struct get_value;

    template <auto T, auto... Ts>
    struct get_value<0, T, Ts...> {
        constexpr static const auto value = T;
    };

    template <std::size_t I, auto T, auto... Ts>
    struct get_value<I, T, Ts...> {
        constexpr static const auto value = get_value<I - 1, Ts...>::value;
    };

    template <auto V, std::size_t... I>
    constexpr auto repeat_value_impl (std::index_sequence<I...>)
        -> std::integer_sequence<decltype (V), ((void)I, V)...>;

    template <auto V, std::size_t N>
    using repeat_value = decltype (repeat_value_impl<V> (std::make_index_sequence<N> {}));

    template <auto ...types> struct last_value;

    template <auto T> struct last_value<T> {
        constexpr static const decltype (T) value = T;
    };

    template <auto first, auto ...rest> struct last_value<first, rest...> {
        constexpr static const auto value = last_value<rest...>::value;
    };

    template<class seq> struct count;

    template<size_t... I> struct count<std::index_sequence<I...>> {
        constexpr static const size_t result = sizeof... (I);
    };

    template<class Seq, size_t I>
    struct get;

    template<size_t... Ns, size_t I>
    struct get<std::index_sequence<Ns...>, I> {
        static constexpr size_t value = std::array<size_t, sizeof... (Ns)> {Ns...}[I];
    };

    template<class... Seqs>
    struct concat;

    template<>
    struct concat<> {
        using type = std::index_sequence<>;
    };

    template<class S>
    struct concat<S> {
        using type = S;
    };

    template<size_t... A, size_t... B, class... Rest>
    struct concat<std::index_sequence<A...>, std::index_sequence<B...>, Rest...> {
        using type = typename concat<std::index_sequence<A..., B...>, Rest...>::type;
    };

    template<typename seq> struct for_each;
    template<> struct for_each<std::index_sequence<>> {
        template <typename F> void operator () (F &&f) {}
    };

    template<typename seq> struct for_each;
    template<size_t first, size_t ...rest> struct for_each<std::index_sequence<first, rest...>> {
        template <typename F> void operator () (F &&f) {
            f (first);
            for_each<std::index_sequence<rest...>> {} (f);
        }
    };

    template<class sequence, size_t index> struct prepend;

    template<size_t ...indices, size_t index> struct prepend<std::index_sequence<indices...>, index> {
        using result = std::index_sequence<index, indices...>;
    };

    template<class sequence> struct sort;

    template<> struct sort<std::index_sequence<>> {
        using result = std::index_sequence<>;
    };

    template<size_t index> struct sort<std::index_sequence<index>> {
        using result = std::index_sequence<index>;
    };

    template<size_t a, size_t b, size_t ...rest>
    requires (b < a)
    struct sort<std::index_sequence<a, b, rest...>> {
        using result = prepend<typename sort<std::index_sequence<a, rest...>>::result, b>::result;
    };

    template<size_t a, size_t b, size_t ...rest> struct sort<std::index_sequence<a, b, rest...>> {
        using result = prepend<typename sort<std::index_sequence<b, rest...>>::result, a>::result;
    };

    // (indices must be sorted)
    template<class sequence, class remove_indices, size_t index = 0>
    struct remove_at;

    template<class sequence, size_t index>
    struct remove_at<sequence, std::index_sequence<>, index> {
        using result = sequence;
    };

    template<size_t next, size_t ...rest, size_t index, size_t ...remaining>
    struct remove_at<std::index_sequence<next, rest...>, std::index_sequence<index, remaining...>, index> {
        using result = remove_at<std::index_sequence<rest...>, std::index_sequence<remaining...>, index + 1>::result;
    };

    template<size_t next, size_t ...rest, size_t next_index, size_t ...remaining, size_t index>
    struct remove_at<std::index_sequence<next, rest...>, std::index_sequence<next_index, remaining...>, index> {
        using result = prepend<
            typename remove_at<std::index_sequence<rest...>, std::index_sequence<next_index, remaining...>, index + 1>::result,
            next>::result;
    };

}

#endif
