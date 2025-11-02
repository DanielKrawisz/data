// Copyright (c) 2019-2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TYPES
#define DATA_TYPES

#include <vector>
#include <array>
#include <string>
#include <memory>
#include <stdint.h>
#include <algorithm>
#include <random>
#include <functional>
#include <expected>

#ifndef UINT8_MAX
static_assert (false, "Must support bytes");
#endif
#ifndef UINT16_MAX
static_assert (false, "Must support uint16");
#endif
#ifndef UINT32_MAX
static_assert (false, "Must support uint32");
#endif
#ifndef UINT64_MAX
static_assert (false, "Must support uint64");
#endif

#ifndef INT8_MAX
static_assert (false, "Must support int8");
#endif
#ifndef INT16_MAX
static_assert (false, "Must support int16");
#endif
#ifndef INT32_MAX
static_assert (false, "Must support int32");
#endif
#ifndef INT64_MAX
static_assert (false, "Must support int64");
#endif

namespace data {

    using byte = std::uint8_t;
    using uint16 = std::uint16_t;
    using uint32 = std::uint32_t;
    using uint64 = std::uint64_t;

    using int8 = std::int8_t ;
    using int16 = std::int16_t;
    using int32 = std::int32_t;
    using int64 = std::int64_t;

    template <typename X> using ptr = std::shared_ptr<X>;

    template <typename X> using function = std::function<X>;

    template <typename... X> using handler = std::function<void (X...)>;

    template <typename X> using view = std::basic_string_view<X>;

    using string_view = std::basic_string_view<char>;

    template <typename X, typename Y = X> using pair = std::pair<X, Y>;
    template <typename X, typename Y> using expected = std::expected<X, Y>;

    template <typename X> using unref = std::remove_reference_t<X>;
    template <typename X> using unconst = std::remove_const_t<X>;

    template <typename... X> using tuple = std::tuple<X...>;

    // apply to each element of a tuple.
    template <typename Tuple, typename F>
    requires requires {
        typename std::tuple_size<std::remove_cvref_t<Tuple>>::type;
    } constexpr void for_each (Tuple &&t, F &&f) {
        using U = std::remove_cvref_t<Tuple>;
        constexpr std::size_t N = std::tuple_size_v<U>;
        [&]<std::size_t... I> (std::index_sequence<I...>) {
            (f(std::get<I> (std::forward<Tuple> (t))), ...);
        } (std::make_index_sequence<N> {});
    }

}

#endif
