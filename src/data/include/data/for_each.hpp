// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_FOR_EACH
#define DATA_FOR_EACH

/*
 *  -------------------------------------------------------------------------
 *  for_each.hpp
 *  -------------------------------------------------------------------------
 *
 *  A family of traversal functions that apply a user-supplied function `f`
 *  over one or more data structures of identical shape.  These functions
 *  handle sequences, maps, trees, and any other data structure that exposes
 *  a consistent structure.
 *
 *  All input structures must have the same shape.
 *      - Sequences must have the same length.
 *      - Maps must have the same keys.
 *      - Trees must have the same branching structure.
 *      - Tuples, arrays, and other fixed-shape containers must match exactly.
 *
 *  The reference categories of the provided data structures are preserved:
 *
 *      - If a structure is const, its elements are passed as const&.
 *      - If a structure is non-const, its elements are passed as non-const&.
 *
 *  This allows deep modification of non-const structures while guaranteeing
 *  that const structures are never written to.
 *
 *  NOTE: not all cases are implemented. TODO for version 1.
 *
 *  -------------------------------------------------------------------------
 *  for_each (f, a, b, ...)
 *  -------------------------------------------------------------------------
 *
 *  Applies `f` to each corresponding element across all arguments:
 *
 *  Example pattern:
 *      Z1, Z2, ... : data structures of identical shape
 *      z1, z2, ... : elements of those data structures
 *      f           : function taking references to the corresponding elements
 *
 *      for_each (f, Z1, Z2, ...)
 *
 *  calls
 *
 *      f (z1, z2, ...)
 *
 *  on all the corresponding parts of Z1, Z2, ...
 *
 *  If any of the Zs is non-const, then the element passed to f will be a
 *  non-const reference, allowing mutation.  Likewise for any other non-const
 *  structure.
 *
 *  -------------------------------------------------------------------------
 *  for_each_by (f, a, b, ...)
 *  -------------------------------------------------------------------------
 *
 *  Same as `for_each`, but provides the index or key as the first argument
 *  to `f`.
 *
 *      for_each_by (f, S1, S2, ...)
 *
 *  The index type depends on the structure:
 *      - size_t for sequences
 *      - map key type for maps
 *      - fixed index for tuples or arrays
 *      - does not work on trees (but stay tuned)
 *
 *  The call to f receives (key, element1, element2, ...).
 *
 *  -------------------------------------------------------------------------
 *  for_each<depth> (f, a, b, ...)
 *  -------------------------------------------------------------------------
 *
 *  Recursively descends exactly `depth` levels into the structure and applies
 *  f to the elements found at that depth.
 *
 *  The arguments passed to f are references into the original containers,
 *  preserving constness as above.
 *
 *  -------------------------------------------------------------------------
 *  for_each_by<depth> (f, a, b, ...)
 *  -------------------------------------------------------------------------
 *
 *  Same as for_each<depth>, but f receives the full path of indices/keys as
 *  its first argument.
 *
 *      for_each_by<d> (f, S1, S2, ...)
 *
 *  The index path is returned as a tuple holding each index/key used along
 *  the descent. The subsequent arguments to f are references to the elements
 *  at depth d from each structure.
 *
 *  -------------------------------------------------------------------------
 */

#include <data/io/exception.hpp>
#include <data/iterable.hpp>
#include <data/indexed.hpp>
#include <data/cycle.hpp>
#include <data/tree.hpp>
#include <data/map.hpp>
#include <data/set.hpp>
#include <data/array.hpp>
#include <data/get.hpp>

namespace data {

    template <typename F, typename A, typename ...As>
    requires ConstIterable<A> && Indexed<A, size_t> &&
        (ConstIterable<As> && ...) && (Indexed<As, size_t> && ...)
    void for_each (F &&f, As &&...as);

    template <size_t depth, typename F, typename A, typename ...As>
    requires ConstIterable<A> && Indexed<A, size_t> &&
        (ConstIterable<As> && ...) && (Indexed<As, size_t> && ...)
    void for_each (F &&f, As &&...as);

    template <typename F, typename X> void for_each (F &&f, const tree<X> &);
    template <typename F, typename X> void for_each (F &&f, const cycle<X> &);
    template <typename F, typename K, typename V> void for_each (F &&f, const map<K, V> &);
    template <typename F, typename X> void for_each (F &&f, const set<X> &);
    template <typename F, typename X, size_t ...sizes> void for_each (F &&f, const array<X, sizes...> &);

    template <typename F, typename X> void for_each (F &&f, tree<X> &);
    template <typename F, typename X> void for_each (F &&f, cycle<X> &);
    template <typename F, typename K, typename V> void for_each (F &&f, map<K, V> &);
    template <typename F, typename X, size_t ...sizes> void for_each (F &&f, array<X, sizes...> &);

    template <typename F, typename A, typename ...As>
    requires ConstIterable<A> && Indexed<A, size_t> &&
        (ConstIterable<As> && ...) && (Indexed<As, size_t> && ...)
    void for_each_by (F &&f, As&&... as);

    template <size_t depth, typename F, typename A, typename ...As>
    requires ConstIterable<A> && Indexed<A, size_t> &&
        (ConstIterable<As> && ...) && (Indexed<As, size_t> && ...)
    void for_each_by (F &&f, As&&... as);

    template <typename F, typename X> void for_each_by (F &&f, const cycle<X> &);
    template <typename F, typename K, typename V> void for_each_by (F &&f, const map<K, V> &);

    template <typename F, typename X> void for_each_by (F &&f, cycle<X> &);
    template <typename F, typename K, typename V> void for_each_by (F &&f, map<K, V> &);
    template <typename F, typename X, size_t sizes> void for_each_by (F &&f, const array<X, sizes> &);
    template <typename F, typename X, size_t ...sizes> void for_each_by (F &&f, const array<X, sizes...> &);

    template <typename F, typename A, typename ...As>
    requires ConstIterable<A> && Indexed<A, size_t> &&
        (ConstIterable<As> && ...) && (Indexed<As, size_t> && ...)
    void for_each (F &&f, A &&a, As &&...as) {
        size_t sizes = data::size (a);
        if (!((sizes == data::size (as)) && ...)) throw exception {} << "unequal sizes";

        // --- lockstep iteration ---
        auto begins = std::tuple (a.begin (), as.begin ()...);

        for (std::size_t i = 0; i < sizes; ++i) {
            std::apply ([&] (auto &...it) {
                f ((*it)...);    // call f on all dereferenced iterators
                (++it, ...);    // increment all
            }, begins);
        }
    }

    template <size_t depth, typename F, typename A, typename ...As>
    requires ConstIterable<A> && Indexed<A, size_t> &&
        (ConstIterable<As> && ...) && (Indexed<As, size_t> && ...)
    void for_each (F &&f, A &&a, As &&...as) {
        if constexpr (depth == 0)
            return data::for_each (
                std::forward<F> (f),
                std::forward<A> (a),
                std::forward<As> (as)...);
        else return data::for_each ([&f] (auto &&...n) {
            for_each<depth - 1> (f, n...);
        }, std::forward<A> (a), std::forward<As> (as)...);
    }

    template <typename F, typename X> void inline for_each (F &&f, const cycle<X> &x) {
        for_each (f, x.Cycle);
    }

    template <typename F, typename K, typename V> void inline for_each (F &&f, const map<K, V> &x) {
        for (const auto &[key, value]: x) f (value);
    }

    template <typename F, typename X> void inline for_each (F &&f, const set<X> &x) {
        for (const auto &z : x) f (z);
    }

    template <typename F, typename X> void for_each (F &&f, const tree<X> &x) {
        functional::for_each_infix ([&f] (const X &v) {
            f (v);
        }, x);
    }

    template <typename F, typename X> void inline for_each (F &&f, cycle<X> &x) {
        for_each (f, x.Cycle);
    }

    template <typename F, typename X> void for_each (F &&f, tree<X> &x) {
        functional::for_each_infix ([&f] (X &v) {
            f (v);
        }, x);
    }

    template <typename F, typename K, typename V> void inline for_each (F &&f, map<K, V> &x) {
        if (data::empty (x)) return;

        map<K, V> Left = left (x);
        for_each (f, Left);

        // we have to use const_cast here because RB::tree cannot have
        // non-const access since it is sorted. However, when
        // we use entry<K, V>, changing the value of V does not change
        // the sorting, so it's ok to change it.
        f (const_cast<data::entry<const K, V> &> (x.root ()).Value);

        map<K, V> Right = right (x);
        for_each (f, Right);
    }

    template <typename F, typename X, size_t ...sizes> void inline for_each (F &&f, const array<X, sizes...> &z) {
        for (const X &x : z) f (x);
    }

    template <typename F, typename X, size_t ...sizes> void inline for_each (F &&f, array<X, sizes...> &z) {
        for (X &x : z) f (x);
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

    template <typename F, typename X> void inline for_each_by (F &&f, const cycle<X> &x) {
        for_each_by (f, x.Cycle);
    }

    template <typename F, typename K, typename V> void inline for_each_by (F &&f, const map<K, V> &x) {
        for (const auto &[key, value]: x) f (key, value);
    }

    template <typename F, typename X> void inline for_each_by (F &&f, cycle<X> &x) {
        for_each_by (f, x.Cycle);
    }

    template <typename F, typename K, typename V> void inline for_each_by (F &&f, map<K, V> &x) {
        if (data::empty (x)) return;

        map<K, V> Left = left (x);
        for_each_by (f, Left);

        // we have to use const_cast here because RB::tree cannot have
        // non-const access since it is sorted. However, when
        // we use entry<K, V>, changing the value of V does not change
        // the sorting, so it's ok to change it.
        auto r = const_cast<data::entry<const K, V> &> (x.root ());
        f (r.Key, r.Value);

        map<K, V> Right = right (x);
        for_each_by (f, Right);
    }

    template <typename F, typename X, size_t ...sizes> void inline for_each_by (F &&f, const array<X, sizes...> &x) {
        return array_for_each_by (std::forward<F> (f), x);
    }
    
}

#endif
