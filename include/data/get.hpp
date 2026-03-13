// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_GET
#define DATA_GET

/*
 *    ---------------------------------------------------------------------------
 *    data::get
 *    ---------------------------------------------------------------------------
 *
 *    Part of:  functional data structures
 *
 *    Provides: a higher-order accessor for nested container types.
 *
 *    See also:
 *        data/for_each.hpp
 *        data/lift.hpp
 *
 *    Takes a value x and a parameter pack of indices k, krest.... At each step
 *    select one dimension inside the type of x and then recurse on the remainder
 *    of the indices.
 *
 *    For a value z and index k such that
 *
 *        data::Indexed<decltype (z), decltype (k)>    ,
 *
 *    we have
 *
 *        get (z, k, rest...)   →   get (z[k], rest... )
 *
 *    for an array<X, size_t ...sizes> a and size_ts k_0, k_1... we have
 *
 *        get (a, k_0, k_1, ..., k_(A-1), rest...) →
 *            get (a[k_0, k1, ..., k_(A-1)], rest... )
 *
 *    ---------------------------------------------------------------------------
*/

#include <data/indexed.hpp>
#include <data/array.hpp>
#include <data/meta.hpp>

namespace data {
    template <typename T>
    decltype (auto) inline get (T &&x) {
        return std::forward<T> (x);
    }

    template <typename T, typename key, typename ...args>
    decltype (auto) get (T &&x, const key &k, args &&...a);

    // You don't need to read past this point.
    namespace {
        template <
            std::size_t Last,                   // final dimension (stride factor)
            typename A>
        constexpr decltype (auto) inline
        get_array_helper (std::index_sequence<Last>, std::size_t index, A&& array) {
            if (index >= Last) throw exception {} << "array index " << index << " too big for size " << Last;
            return array.Values[index];
        }

        template <
            std::size_t Last,                   // final dimension (stride factor)
            std::size_t... RecursiveSizes,      // remaining dimensions
            typename A,
            typename Next,
            typename... Remaining>
        constexpr decltype (auto)
        get_array_helper (std::index_sequence<Last, RecursiveSizes...>, size_t index, A&& array, Next next, Remaining... remaining) {
            if (next >= Last) throw exception {} << "array index " << next << " too big for size " << Last;
            size_t next_index = index * Last + static_cast<size_t> (next);
            if constexpr (sizeof... (RecursiveSizes) == 0) {
                return get (array.Values[next_index], remaining...);
            } else {
                return get_array_helper<RecursiveSizes...> (
                    std::index_sequence<RecursiveSizes...> {},
                    next_index, array, remaining...);
            }
        }
    }

    template <typename T, typename key, typename ...args>
    decltype (auto) get (T &&x, const key &k, args &&...a) {
        if constexpr (requires {
            typename array_info<std::remove_cvref_t<T>>::dims;
        }) {
            return get_array_helper (
                typename array_info<std::remove_cvref_t<T>>::dims {},
                size_t (0), std::forward<T> (x), static_cast<size_t> (k), std::forward<args> (a)...);
        } else {
            return get (std::forward<T> (x)[k], std::forward<args> (a)...);
        }
    }

    namespace {
        template <typename X, size_t N, size_t ...J>
        decltype (auto) inline paste_into_get (X &&x, const array<size_t, N> &ind, std::index_sequence<J...>) {
            return get (x, ind[J]...);
        }

        template <typename X, size_t N> decltype (auto) inline paste_into_get (X &&x, const array<size_t, N> &ind) {
            return paste_into_get (x, ind, std::make_index_sequence<N> {});
        }
    }

    // TODO this should be in array.hpp but we used paste_int_get, so it is here for now.
    template <typename F, typename X, size_t ...sizes> void array_for_each_by (F &&f, const array<X, sizes...> &x) {
        constexpr static const size_t N = sizeof... (sizes);
        array<size_t, N> limit {sizes...};
        array<size_t, N> index {};

        while (true) {
            f (index, paste_into_get (x, index));

            int current_index = N - 1;
            while (true) {
                if (current_index < 0) return;
                if (++index[current_index] != limit[current_index]) break;
                index[current_index--] = 0;
            }
        }
    }

}

#endif
