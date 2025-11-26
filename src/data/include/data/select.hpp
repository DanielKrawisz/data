// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_SELECT
#define DATA_SELECT

#include <data/list.hpp>
#include <data/take.hpp>
#include <data/for_each.hpp>

namespace data {

    template <typename T, typename F>
    requires Pendable<T> && ConstIterable<T>
    T inline select (const T &x, F &&satisfies) {
        T result;
        if constexpr (Queue<T>) {
            for (const auto &z : x) if (satisfies (z)) result <<= z;
            return result;
        } else if constexpr (Stack<T> ) {
            for (const auto &z : x) if (satisfies (z)) result >>= z;
            return reverse (result);
        } else {
            list<decltype (*x.begin ())> satisfied;
            for (const auto &z : x) if (satisfies (z)) satisfied <<= z;

            // NOTE: we don't actually know that we can make something of a given size like this.
            // this works for std::vectors and things like that.
            T z (size (satisfies));
            for_each ([] (auto &&a, const auto &&b) {
                a = b;
            }, z, x);
            return z;
        }
    }

}

#endif
