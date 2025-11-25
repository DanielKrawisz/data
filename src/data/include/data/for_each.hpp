// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_FOR_EACH
#define DATA_FOR_EACH

#include <data/io/exception.hpp>
#include <data/iterable.hpp>
#include <data/indexed.hpp>
#include <data/size.hpp>

namespace data {

    template <typename F, typename A, typename ...As>
    requires ConstIterable<A> && Indexed<A, size_t> &&
        (ConstIterable<As> && ...) && (Indexed<As, size_t> && ...)
    void for_each (F&& f, As&&... as);

    template <typename F, typename A, typename ...As>
    requires ConstIterable<A> && Indexed<A, size_t> &&
        (ConstIterable<As> && ...) && (Indexed<As, size_t> && ...)
    void for_each_by (F&& f, As&&... as);

    template <typename F, typename A, typename ...As>
    requires ConstIterable<A> && Indexed<A, size_t> &&
        (ConstIterable<As> && ...) && (Indexed<As, size_t> && ...)
    void for_each (F &&f, A &&a, As &&...as) {
        size_t sizes = data::size (a);
        if (!((sizes == data::size (as)) && ...)) throw exception {} << "unequal sizes";

        // --- lockstep iteration ---
        auto begins = std::tuple (a.begin (), as.begin ()...);

        for (std::size_t i = 0; i < sizes; ++i) {
            std::apply ([&] (auto &...it){
                f ((*it)...);    // call f on all dereferenced iterators
                (++it, ...);    // increment all
            }, begins);
        }
    }

    template <typename F, typename A, typename ...As>
    requires ConstIterable<A> && Indexed<A, size_t> &&
        (ConstIterable<As> && ...) && (Indexed<As, size_t> && ...)
    void for_each_by (F &&f, A &&a, As &&...as) {
        size_t sizes = data::size (a);
        if (!((sizes == data::size (as)) && ...)) throw exception {} << "unequal sizes";

        // --- lockstep iteration ---
        auto begins = std::tuple (a.begin (), as.begin ()...);

        for (size_t i = 0; i < sizes; ++i) {
            std::apply ([&] (auto &...it) {
                // call f on all dereferenced iterators with index
                f (i, (*it)...);
                (++it, ...);    // increment all
            }, begins);
        }
    }
    
}

#endif
