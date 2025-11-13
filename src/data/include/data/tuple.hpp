// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TUPLE
#define DATA_TUPLE

#include <data/types.hpp>
#include <data/io/exception.hpp>

namespace data {

    template <typename... X> using tuple = std::tuple<X...>;

    template <typename T>
    concept Tuple = requires {
        typename std::tuple_size<std::remove_cvref_t<T>>::type;
    };

    // apply F to each element of a tuple.
    template <Tuple T, typename F> constexpr void for_each (T &&t, F &&f);

    // apply a function to a part of a tuple and return the result.
    template <Tuple T, typename F>
    constexpr decltype (auto) apply_at (T &&t, F &&f, int i);

    // apply to each element of a tuple.
    template <Tuple T, typename F>
    constexpr void for_each (T &&t, F &&f) {
        using U = std::remove_cvref_t<T>;
        constexpr std::size_t N = std::tuple_size_v<U>;
        [&]<std::size_t... I> (std::index_sequence<I...>) {
            (f(std::get<I> (std::forward<T> (t))), ...);
        } (std::make_index_sequence<N> {});
    }

    namespace {

        template <std::size_t I, typename Tuple, typename F>
        decltype (auto) tuple_apply_at_rec (Tuple &&t, F &&f, std::size_t idx) {
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
    }

    // apply a function to a part of a tuple and return the result.
    template <Tuple T, typename F>
    constexpr decltype (auto) apply_at (T &&t, F &&f, int i) {
        return tuple_apply_at_rec<0> (t, std::forward<F> (f), i);
    }
}

#endif
