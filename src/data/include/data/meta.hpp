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

    // TODO this is pretty lazy since we could use integer_sequence
    template <size_t ...sizes> using index_sequence = std::index_sequence<sizes...>;

    template <auto V, std::size_t... I>
    constexpr auto repeat_value_impl (index_sequence<I...>)
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

    template<class seq> struct count_sequence;

    template<size_t ...I> struct count_sequence<index_sequence<I...>> {
        constexpr static const size_t value = sizeof... (I);
    };

    template <typename sequence> constexpr static const size_t count = count_sequence<sequence>::value;

    template<class Seq, size_t I>
    struct get_sequence;

    template<size_t... Ns, size_t I>
    struct get_sequence<index_sequence<Ns...>, I> {
        static constexpr size_t value = std::array<size_t, sizeof... (Ns)> {Ns...}[I];
    };

    template <typename sequence, size_t i> constexpr static size_t get = get_sequence<sequence, i>::value;

    template<class... Seqs> struct concat_sequence;

    template <typename ...sequence> using cat = concat_sequence<sequence...>::result;

    template<> struct concat_sequence<> {
        using result = index_sequence<>;
    };

    template<class S> struct concat_sequence<S> {
        using result = S;
    };

    template<size_t... A, size_t... B, class... Rest>
    struct concat_sequence<index_sequence<A...>, index_sequence<B...>, Rest...> {
        using result = cat<index_sequence<A..., B...>, Rest...>;
    };

    template<typename seq> struct for_each;
    template<> struct for_each<index_sequence<>> {
        template <typename F> void operator () (F &&f) {}
    };

    template<typename seq> struct for_each;
    template<size_t first, size_t ...rest> struct for_each<index_sequence<first, rest...>> {
        template <typename F> void operator () (F &&f) {
            f (first);
            for_each<index_sequence<rest...>> {} (f);
        }
    };

    template<class sequence, size_t index> struct prepend_sequence;

    template <typename sequence, size_t i> using prepend = prepend_sequence<sequence, i>::result;

    template<size_t ...indices, size_t index> struct prepend_sequence<index_sequence<indices...>, index> {
        using result = index_sequence<index, indices...>;
    };

    template<class sequence, size_t index> struct take_sequence;

    template <typename sequence, size_t i> using take = take_sequence<sequence, i>::result;

    template<size_t ...indices> struct take_sequence<index_sequence<indices...>, 0> {
        using result = index_sequence<>;
    };

    template<size_t first, size_t ...indices, size_t index>
    requires (index > 0)
    struct take_sequence<index_sequence<first, indices...>, index> {
        using result = prepend<take<index_sequence<indices...>, index - 1>, first>;
    };

    template<class sequence, size_t index> struct drop_sequence;

    template <typename sequence, size_t i> using drop = drop_sequence<sequence, i>::result;

    template<size_t ...indices> struct drop_sequence<index_sequence<indices...>, 0> {
        using result = index_sequence<indices...>;
    };

    template<size_t first, size_t ...indices, size_t index>
    requires (index > 0)
    struct drop_sequence<index_sequence<first, indices...>, index> {
        using result = drop<index_sequence<indices...>, index - 1>;
    };

    // data::meta::sort_sequence<std::integer_sequence<long unsigned int, 1, 2> >
    template<class sequence> struct sort_sequence;

    template <typename sequence> using sort = sort_sequence<sequence>::result;

    template<> struct sort_sequence<index_sequence<>> {
        using result = index_sequence<>;
    };

    template<size_t index> struct sort_sequence<index_sequence<index>> {
        using result = index_sequence<index>;
    };

    template<size_t a, size_t b, size_t ...rest>
    requires (b < a)
    struct sort_sequence<index_sequence<a, b, rest...>> {
        using result = prepend<sort<index_sequence<a, rest...>>, b>;
    };

    template<size_t a, size_t b, size_t ...rest> struct sort_sequence<index_sequence<a, b, rest...>> {
        using result = prepend<sort<index_sequence<b, rest...>>, a>;
    };

    // (indices must be sorted)
    template<class sequence, class remove_indices, size_t index = 0> struct remove_sequence;

    template <typename sequence, typename removed> using remove = remove_sequence<sequence, removed>::result;

    template<class sequence, size_t index>
    struct remove_sequence<sequence, index_sequence<>, index> {
        using result = sequence;
    };

    template<size_t next, size_t ...rest, size_t index, size_t ...remaining>
    struct remove_sequence<index_sequence<next, rest...>, index_sequence<index, remaining...>, index> {
        using result = typename remove_sequence<index_sequence<rest...>, index_sequence<remaining...>, index + 1>::result;
    };

    template<size_t next, size_t ...rest, size_t next_index, size_t ...remaining, size_t index>
    struct remove_sequence<index_sequence<next, rest...>, index_sequence<next_index, remaining...>, index> {
        using result = prepend<typename remove_sequence<
                index_sequence<rest...>,
                index_sequence<next_index, remaining...>, index + 1>::result,
            next>;
    };

    template<class sequence, class reversed = index_sequence<>> struct reverse_sequence;

    template <typename sequence> using reverse = reverse_sequence<sequence>::result;

    template <size_t ...reversed>
    struct reverse_sequence<index_sequence<>, index_sequence<reversed...>> {
        using result = index_sequence<reversed...>;
    };

    template <std::size_t first, size_t ...rest, size_t ...reversed>
    struct reverse_sequence<index_sequence<first, rest...>, index_sequence<reversed...>> {
        using result = reverse_sequence<index_sequence<rest...>, index_sequence<reversed..., first>>::result;
    };

}

#endif
