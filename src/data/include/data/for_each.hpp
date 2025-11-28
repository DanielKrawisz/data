// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_FOR_EACH
#define DATA_FOR_EACH

#include <data/io/exception.hpp>
#include <data/iterable.hpp>
#include <data/indexed.hpp>
#include <data/cycle.hpp>
#include <data/tree.hpp>
#include <data/map.hpp>
#include <data/set.hpp>

namespace data {

    template <typename F, typename A, typename ...As>
    requires ConstIterable<A> && Indexed<A, size_t> &&
        (ConstIterable<As> && ...) && (Indexed<As, size_t> && ...)
    void for_each (F&& f, As&&... as);

    template <typename F, typename X> void for_each (F&& f, const tree<X> &);
    template <typename F, typename X> void for_each (F&& f, const cycle<X> &);
    template <typename F, typename K, typename V> void for_each (F&& f, const map<K, V> &);
    template <typename F, typename X> void for_each (F&& f, const set<X> &);

    template <typename F, typename X> void for_each (F&& f, tree<X> &);
    template <typename F, typename X> void for_each (F&& f, cycle<X> &);
    template <typename F, typename K, typename V> void for_each (F&& f, map<K, V> &);

    template <typename F, typename A, typename ...As>
    requires ConstIterable<A> && Indexed<A, size_t> &&
        (ConstIterable<As> && ...) && (Indexed<As, size_t> && ...)
    void for_each_by (F&& f, As&&... as);

    template <typename F, typename X> void for_each_by (F&& f, const cycle<X> &);
    template <typename F, typename K, typename V> void for_each_by (F&& f, const map<K, V> &);

    template <typename F, typename X> void for_each_by (F&& f, cycle<X> &);
    template <typename F, typename K, typename V> void for_each_by (F&& f, map<K, V> &);

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

    template <typename F, typename X> void inline for_each (F&& f, const cycle<X> &x) {
        for_each (f, x.Cycle);
    }

    template <typename F, typename K, typename V> void inline for_each (F&& f, const map<K, V> &x) {
        for (const auto &[key, value]: x) f (value);
    }

    template <typename F, typename X> void inline for_each (F&& f, const set<X> &x) {
        for (const auto &z : x) f (z);
    }

    template <typename F, typename X> void for_each (F&& f, const tree<X> &x) {
        functional::for_each_infix ([&f] (const X &v) {
            f (v);
        }, x);
    }

    template <typename F, typename X> void inline for_each (F&& f, cycle<X> &x) {
        for_each (f, x.Cycle);
    }

    template <typename F, typename X> void for_each (F&& f, tree<X> &x) {
        functional::for_each_infix ([&f] (X &v) {
            f (v);
        }, x);
    }

    template <typename F, typename K, typename V> void inline for_each (F&& f, map<K, V> &x) {
        functional::for_each_infix ([&f] (entry<const K, V> &v) {
             f (v.Value);
        }, x);
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

    template <typename F, typename X> void inline for_each_by (F&& f, const cycle<X> &x) {
        for_each_by (f, x.Cycle);
    }

    template <typename F, typename K, typename V> void inline for_each_by (F&& f, const map<K, V> &x) {
        for (const auto &[key, value]: x) f (key, value);
    }

    template <typename F, typename X> void inline for_each_by (F&& f, cycle<X> &x) {
        for_each_by (f, x.Cycle);
    }

    template <typename F, typename K, typename V> void inline for_each_by (F&& f, map<K, V> &x) {
        functional::for_each_infix ([&f] (entry<const K, V> &v) {
            f (v.Key, v.Value);
        }, x);
    }
    
}

#endif
