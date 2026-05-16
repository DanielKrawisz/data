// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TUPLE
#define DATA_TUPLE

/*
 *    Utility functions for std::tuple
 *
 *    We define data::tuple as std::tuple and provide utility functions for
 *    working with tuples.
 *
 *    ---------------------------------------------------------------------------
 *    for_each (tuple, f) -> void
 *    ---------------------------------------------------------------------------
 *
 *    Apply a function f to each element of a tuple. If the tuple type is
 *    non-const, then f may modify the tuple.
 *
 *    ---------------------------------------------------------------------------
 *    apply_at (tuple, f, i) -> return type of f
 *    ---------------------------------------------------------------------------
 *
 *    Apply a function f to position i of a tuple. If the tuple type is non-const
 *    then the function may modify the tuple.
 *
 *    ---------------------------------------------------------------------------
 *    lift (const tuple, f) -> tuple
 *    ---------------------------------------------------------------------------
 *
 *    Make a new tuple out of f applied to each of its elements.
 */

#include <data/types.hpp>
#include <data/exception.hpp>

namespace data {

    template <typename... X> using tuple = std::tuple<X...>;

    template <typename T>
    concept Tuple = requires {
        typename std::tuple_size<std::remove_cvref_t<T>>::type;
    };

    // apply F to each respective element of the given tuples.
    template <typename F, Tuple T, Tuple ...Ts> constexpr void for_each (F &&f, T &&t, Ts &&...ts);

    // apply a function to a part of a tuple and return the result.
    template <Tuple T, typename F>
    constexpr decltype (auto) apply_at (T &&t, F &&f, size_t i);

    // create a tuple out of the result of f applied to each element of t.
    template <typename F, typename ...X>
    constexpr auto lift (F &&f, const tuple<X...> &t) -> tuple<decltype (f (std::declval<X> ()))...>;

    // print a tuple to the screen if each element also supports printing with <<
    template <Tuple T> std::ostream &tuple_print (std::ostream &o, T &&t);

    template <Tuple T> std::ostream &tuple_print (std::ostream &o, T &&t) {
        using U = std::remove_cvref_t<T>;
        constexpr std::size_t N = std::tuple_size_v<U>;
        o << "{";
        if constexpr (N != 0) {
            [&]<std::size_t... I> (std::index_sequence<I...>) {
                ((operator << (operator << (o, std::get<I> (std::forward<T> (t))), ", ")), ...);
            } (std::make_index_sequence<N - 1> {});
            o << std::get<N - 1> (std::forward<T> (t));
        }
        return o << "}";
    }

    namespace {

        template <size_t I, typename Tuple, typename F>
        decltype (auto) tuple_apply_at_rec (Tuple &&t, F &&f, size_t idx) {
            if constexpr (I + 1 == std::tuple_size_v<std::remove_cvref_t<Tuple>>) {
                // last index
                if (idx == I) return std::forward<F> (f) (std::get<I> (t));
                throw exception {} << "tuple_apply_at index out of range: index " <<
                    idx << " out of " << std::tuple_size_v<std::remove_cvref_t<Tuple>> << " total" ;
            } else {
                if (idx == I) return std::forward<F> (f) (std::get<I> (t));
                else return tuple_apply_at_rec<I + 1, Tuple, F> (t, std::forward<F> (f), idx);
            }
        }

        template <std::size_t... Is, typename F, typename T, typename... Ts>
        constexpr void for_each_impl (std::index_sequence<Is...>, F &&f, T &&t, Ts &&...ts) {
            auto call_at_index = [&](auto I) {
                f (std::get<I> (std::forward<T> (t)),
                  std::get<I> (std::forward<Ts> (ts))...);
            };

            (call_at_index (std::integral_constant<std::size_t, Is> {}), ...);
        }

        template <typename F, typename... X, std::size_t... Is>
        constexpr auto lift_impl (F &&f, const tuple<X...> &t, std::index_sequence<Is...>)
            -> tuple<decltype (f (std::declval<X> ()))...> {
            return tuple<decltype (f (std::declval<X> ()))...> {
                f (std::get<Is> (t))...
            };
        }
    }

    // apply F to each respective element of the given tuples
    template <typename F, Tuple T, Tuple... Ts>
    constexpr void for_each (F &&f, T &&t, Ts &&...ts)
    {
        constexpr std::size_t N =
        std::tuple_size_v<std::remove_cvref_t<T>>;

        static_assert(((std::tuple_size_v<std::remove_cvref_t<Ts>> == N) && ...),
            "All tuples must have the same size");

        for_each_impl (std::make_index_sequence<N> {}, std::forward<F> (f),
            std::forward<T> (t),
            std::forward<Ts>(ts)...);
    }

    // apply a function to a part of a tuple and return the result.
    template <Tuple T, typename F>
    constexpr decltype (auto) apply_at (T &&t, F &&f, size_t i) {
        return tuple_apply_at_rec<0> (t, std::forward<F> (f), i);
    }

    template <typename F, typename... X>
    constexpr auto lift(F&& f, const tuple<X...>& t)
    -> tuple<decltype(f(std::declval<X>()))...>
    {
        return lift_impl(std::forward<F>(f),
                         t,
                         std::index_sequence_for<X...>{});
    }
}

#endif
