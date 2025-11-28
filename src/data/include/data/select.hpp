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
    requires ConstIterable<T>
    T inline select (const T &x, F &&satisfies) {
        T result;
        if constexpr (Queue<T>) {
            for (const auto &z : x) if (satisfies (z)) result <<= z;
            return result;
        } else if constexpr (Stack<T> ) {
            for (const auto &z : x) if (satisfies (z)) result >>= z;
            return reverse (result);
        } else if constexpr (Sack<T, decltype (*x.begin ())>) {
            for (const auto &z : x) if (satisfies (z)) result = insert (result, z);
            return result;
        // TODO there is a problem here because x.begin ()->Key doesn't necessarily exist.
        // if it doesn't, then this function won't compile even if another case is ok.
        /*
        } else if constexpr (Map<T, decltype (x.begin ()->Key), decltype (x.begin ()->Value)>) {
            for (const auto &[k, v] : x) if (satisfies (v)) result = insert (result, k, v);
            return result;*/
        } else {
            list<decltype (*x.begin ())> satisfied;
            for (const auto &z : x) if (satisfies (z)) satisfied <<= z;

            // NOTE: we don't actually know that we can make something of a given size like this.
            // this works for std::vectors and things like that.
            T z (size (x));
            for_each ([] (auto &&a, const auto &&b) {
                a = b;
            }, z, x);
            return z;
        }
    }

}

#endif
