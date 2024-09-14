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

namespace data {

    using byte = uint8_t;
    using uint16 = uint16_t;
    using uint32 = uint32_t;
    using uint64 = uint64_t;

    using int8 = int8_t ;
    using int16 = int16_t;
    using int32 = int32_t;
    using int64 = int64_t;

    template <typename X> using ptr = std::shared_ptr<X>;

    template <typename X> using function = std::function<X>;

    template <typename... X> using handler = std::function<void (X...)>;

    template <typename... X> using tuple = std::tuple<X...>;

    template<typename X>
    using view = std::basic_string_view<X>;

    using string_view = std::basic_string_view<char>;

    using bytes_view = view<byte>;

    // TODO replace this with something good.
    using random_engine = std::default_random_engine;

    random_engine& get_random_engine ();

}

#endif
