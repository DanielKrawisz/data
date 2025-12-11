// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_FLATTEN
#define DATA_FLATTEN

/**
 * Flatten nested data structures.
 *
 * The `flatten` family of functions converts certain nested container
 * structures into simpler, “flattened” forms. The exact behavior depends on
 * the container type and whether a compile-time `depth` parameter is supplied.
 *
 *     template <typename X> auto flatten (stack<X>);
 *     template <typename X> auto flatten (list<X>);
 *     template <typename X> auto flatten (const cross<X> &);
 *
 * recursively descend into the container **only as long as the same
 * container type continues**.
 *
 * For example:
 * - Flattening a `stack<stack<T>>` produces a single `stack<T>`.
 * - Flattening a `list<list<T>>` produces a `list<T>`.
 * - Flattening stops at the first level whose elements are *not* of the same
 *   container type (e.g., `list<stack<T>>` does not flatten across the change
 *   in structure).
 *
 *
 * ---
 * ## Depth-Limited Flattening
 *
 * The overloads
 *
 *     template <size_t depth, typename X> auto flatten(stack<X>);
 *     template <size_t depth, typename X> auto flatten(list<X>);
 *     template <size_t depth, typename X> auto flatten(const cross<X> &);
 *
 * flatten exactly `depth` layers of the same container type.
 *
 * Requirements:
 * - It is an error if the structure cannot be unwrapped at least `depth` times
 *   while maintaining the same container type.
 *
 * Examples:
 * - `flatten<1>(stack<stack<T>>)` → `stack<T>`
 * - `flatten<2>(stack<stack<stack<T>>>)` → `stack<T>`
 * - `flatten<2>(stack<list<T>>)` → error (structure switches type after 1 level)
 *
 *
 * ---
 * ## Flattening Arrays
 *
 * Arrays work differently because they store compile-time fixed shapes:
 *
 *     template <typename X, size_t... size>
 *     auto flatten(const array<X, size...> &);
 *
 *     template <size_t depth, typename X, size_t... size>
 *     auto flatten(const array<X, size...> &);
 *
 * Rules for arrays:
 *
 * - `flatten(array<X, s0, s1, ..., sn>)` produces a **one-dimensional**
 *   `array<X, s0 * s1 * ... * sn>`.
 *
 * - `flatten<depth>(array<...>)` removes the first `depth` shape dimensions:
 *     - Result has dimension count `sizeof...(size) - depth`.
 *     - It is an error if `depth >= sizeof...(size)`.
 *
 * Arrays are *not* recursively flattened by inspecting the element type;
 * structural flattening applies **only to the outer array dimensions**.
 *
 *
 * ---
 * ## Constness
 *
 * `flatten` never mutates the original structure. The result is always a new,
 * independent structure, regardless of const or non-const input.
 */

#include <data/stack.hpp>
#include <data/list.hpp>
#include <data/cross.hpp>
#include <data/array.hpp>
#include <data/get.hpp>
#include <data/for_each.hpp>

namespace data {

    template <typename X> decltype (auto) inline flatten (auto &&x) {
        return x;
    }

    template <typename X> auto flatten (stack<X>);
    template <typename X> auto flatten (list<X>);
    template <typename X> auto flatten (const cross<X> &b);
    template <typename X, size_t... size> array<X, array<X, size...>::Size> flatten (const array<X, size...> &);

    template <size_t depth, typename X> auto flatten (stack<X>);
    template <size_t depth, typename X> auto flatten (list<X>);
    template <size_t depth, typename X> auto flatten (const cross<X> &);
    template <size_t depth, typename X, size_t... size> auto flatten (const array<X, size...> &);

    namespace {
        // Primary template: any type that is not the same container
        template <typename Container, typename Inner>
        struct get_depth_same {
            static constexpr size_t value = 0;
        };

        // Specialization for when Container<X> contains the same Container type
        template <template <typename> class Container, typename X, typename Y>
        struct get_depth_same<Container<X>, Container<Y>> {
            static constexpr size_t value = 1 + get_depth_same<Container<X>, Y>::value;
        };

        // Helper alias
        template <typename T>
        inline constexpr size_t get_depth = get_depth_same<T, T>::value;

        // Primary template: handles mismatched Container/Inner or too deep recursion
        template <typename Container, typename Inner, size_t depth>
        struct flatten_result {
            static_assert (sizeof (Container) == 0,
                "flatten_result_type: depth exceeds available nested homogeneous layers");
        };

        template <typename Container, typename Inner>
        struct flatten_result<Container, Inner, 0> {
            using type = Inner;
        };

        // Specialization for when Container<X> contains the same Container type
        template <template <typename> class Container, typename X, typename Y, size_t depth>
        requires (depth > 0)
        struct flatten_result<Container<X>, Container<Y>, depth> {
            using type = flatten_result<Container<X>, Y, depth - 1>::type;
        };

        // Helper alias
        template <typename T, size_t depth>
        using flatten_result_type = typename flatten_result<T, T, depth>::type;

        // Compute product of first N elements of a parameter pack
        template <size_t N, size_t... Sizes>
        struct product_first_n;

        template <size_t N, size_t First, size_t... Rest>
        struct product_first_n<N, First, Rest...> {
            static constexpr size_t value = First * product_first_n<N - 1, Rest...>::value;
        };

        // Base case: N == 0
        template <size_t First, size_t... Rest>
        struct product_first_n<0, First, Rest...> {
            static constexpr size_t value = 1;
        };
    }

    template <typename X> auto inline flatten (stack<X> x) {
        return data::flatten<get_depth<stack<X>> - 1> (x);
    }

    template <typename X> auto inline flatten (list<X> x) {
        return data::flatten<get_depth<list<X>> - 1> (x);
    }

    template <typename X> auto inline flatten (const cross<X> &x) {
        return data::flatten<get_depth<cross<X>> - 1> (x);
    }

    template <size_t depth, typename X> auto flatten (stack<X> x) {
        flatten_result_type<stack<X>, depth> result;
        for_each<depth> ([&result] (auto &&n) {
            result <<= n;
        }, x);
        return result;
    }

    template <size_t depth, typename X> auto flatten (list<X> x) {
        flatten_result_type<list<X>, depth> result;
        for_each<depth> ([&result] (auto &&n) {
            result <<= n;
        }, x);
        return result;
    }

    template <size_t depth, typename X> auto flatten (const cross<X> &x) {
        flatten_result_type<cross<X>, depth> result;
        for_each<depth> ([&result] (auto &&n) {
            result <<= n;
        }, x);
        return result;
    }

    template <typename X, size_t... size> array<X, array<X, size...>::Size> flatten (const array<X, size...> &x) {
        array<X, array<X, size...>::Size> result;
        for_each ([](auto &&r, auto &&i) {
            r = i;
        }, result, x);
        return result;
    }
/*
    template <size_t depth, typename X, size_t... size> auto flatten (const array<X, size...> &x) {
        constexpr size_t N = sizeof... (size);
        static_assert (depth < N, "cannot flatten an array out of existence");
        // result_type is array<X, product of first depth + 1 parameters, remaining size parameters...>
        result_type result;
        for_each ([](auto &&r, auto &&i) {
            r = i;
        }, result, x);
        return result;
    }*/
}

#endif
