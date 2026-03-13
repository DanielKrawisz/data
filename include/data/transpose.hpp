// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TRANSPOSE
#define DATA_TRANSPOSE

#include <data/array.hpp>
#include <data/get.hpp>
#include <data/lift.hpp>
#include <data/math/permutation.hpp>

namespace data {

/*
    ------------------------------------------------------------------------------
    transpose<N...>

    Index-permutation transpose for arrays.

    Interpretation of template arguments:
      transpose<n1, n2, ..., nk>
      means: the original index n1 becomes the first index,
             the original index n2 becomes the second index,
             ...
             the original index nk becomes the k-th index.
    Opposite to Mathematica's Transpose: here the argument list
    describes "which original index moves into which slot", rather than
    "where the current index goes".

    Default behavior:
      transpose<>  is interpreted as transpose<0, 1>
      and for 2-dimensional structures performs the usual matrix transpose.

    ------------------------------------------------------------------------------
*/

    template <typename X, size_t A, size_t B> array<X, B, A> transpose (const array<X, A, B> &z);

    template <size_t... I, typename X, size_t ...sizes> auto transpose (const array<X, sizes...> &z);

    template <typename X, size_t A, size_t B> array<X, B, A> inline transpose (const array<X, A, B> &z) {
        return transpose<1, 0> (z);
    }

    namespace {
        // true iff I... is a permutation of {0,1,...,N-1}
        template <std::size_t N, std::size_t... rest> struct is_valid_permutation;

        template <std::size_t N, std::size_t first, std::size_t... rest>
        struct is_valid_permutation<N, first, rest...>;

        template <std::size_t N> struct is_valid_permutation<N> : std::true_type {};

        template <std::size_t N, std::size_t first, std::size_t... rest>
        struct is_valid_permutation<N, first, rest...>;

        template <std::size_t...>
        struct all_distinct_from_first;

        template <std::size_t N, std::size_t first, std::size_t... rest>
        struct is_valid_permutation<N, first, rest...> : std::bool_constant<
            first < N && all_distinct_from_first<first, rest...>::value && is_valid_permutation<N, rest...>::value> {};

        template <std::size_t first>
        struct all_distinct_from_first<first> : std::true_type {};

        template <std::size_t first, std::size_t next, std::size_t... rest>
        struct all_distinct_from_first<first, next, rest...>
            : std::conditional_t<
                (first != next),
                all_distinct_from_first<first, rest...>,
                std::false_type
            > {};

        template <typename permutation, typename list>
        struct transpose_result;

        template <typename from, typename indices>
        struct get_all_sequence;

        template <typename from, typename indices>
        using get_all = get_all_sequence<from, indices>::result;

        template <size_t ...sizes>
        struct get_all_sequence<std::index_sequence<sizes...>, std::index_sequence<>> {
            using result = std::index_sequence<>;
        };

        template <size_t ...sizes, size_t i, size_t ...I>
        struct get_all_sequence<std::index_sequence<sizes...>, std::index_sequence<i, I...>> {
            using result = meta::prepend<
                get_all<std::index_sequence<sizes...>, std::index_sequence<I...>>,
                meta::get<std::index_sequence<sizes...>, i>>;
        };

        template <typename X, size_t ...sizes>
        struct transpose_result<X, std::index_sequence<sizes...>> {
            using type = array<X, sizes...>;
        };

        template <typename permutation, typename list>
        using transpose_result_type = typename transpose_result<permutation, list>::type;
    }

    template <size_t... I, typename X, size_t ...sizes> auto transpose (const array<X, sizes...> &z) {

        constexpr static const size_t perm_size = sizeof... (I);

        static_assert (sizeof...(sizes) >= perm_size,
                        "Index permutation longer than array rank");

        static_assert (is_valid_permutation<sizeof... (sizes), I...>::value,
                        "Indices must form a valid permutation");

        using extents = std::index_sequence<sizes...>;

        // construct the result that will be returned.
        transpose_result_type<X,
            meta::cat<
                get_all<meta::take<extents, perm_size>, std::index_sequence<I...>>,
                meta::drop<extents, perm_size>
            >
        > result;

        constexpr static const size_t N = sizeof... (sizes);

        array<size_t, perm_size> ordering {I...};

        // now we do a multi-dimensional iteration.
        for_each_by ([&result, &ordering] (const array<size_t, N> &indices, const X &x) {
            array<size_t, N> result_indices;

            // apply the permutation to the result_indices
            for (int i = 0; i < N; i++) if (i >= perm_size) result_indices[i] = indices[i];
            else result_indices[i] = indices[ordering[i]];

            paste_into_get (result, result_indices) = x;

        }, z);
        return result;
    }

}

#endif
