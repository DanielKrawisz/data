// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_COMBINATORICS
#define DATA_COMBINATORICS

#include <data/math/permutation.hpp>
#include <data/math/figurate.hpp>

namespace data {

    template <ordered elem> list<elem> range (elem from, elem to, elem by = 1);
    template <ordered elem> list<list<elem>> permutations (list<elem>);
    template <typename elem> list<list<elem>> sublists (list<elem> a, size_t size);
    template <typename elem> list<list<elem>> sublists (list<elem> a, size_t from, size_t to);
    template <typename elem> list<list<elem>> partition (list<elem> a, size_t size);
    template <typename elem> list<list<elem>> partition (list<elem> a, size_t size, size_t offset);

    template <typename N> N sublists_count (const N &list_size, N sub_size) {
        N x = 0;
        for (N n = 1; n <= size; n++) x += polytopic_number (n, list_size - sub_size);
        return x;
    }

    template <typename> N permutations_count (const N &list_size) {
        return factorial<N> (list_size);
    }

    template <typename elem> list<list<elem>> inline sublists (list<elem> a, size_t from, size_t to) {
        return reduce (join<list<list<elem>>>, for_each ([a] (size_t size) -> list<list<elem>> {
            return sublists (a, size);
        }, range (from, to)));
    }

    template <typename elem> list<list<elem>> inline partition (list<elem> a, size_t size) {
        return partitions (a, size, size);
    }

    template <ordered elem> list<elem> inline range (elem from, elem to, elem by) {
        list<elem> x;
        for (elem e = from; e <= to; e += by) x <<= e;
        return x;
    }

    template <typename elem> list<list<elem>> sublists (list<elem> a, size_t size) {
        if (size > a.size () || size < 0) return {};
        if (size == 0) return {{}};
        list<list<elem>> x {};
        for (int ii = 0; ii <= a.size () - size; ii++) {
            const elem &e = a[ii];
            x = x + for_each ([e] (list<elem> a) -> list<elem> {
                return prepend (a, e);
            }, sublists (drop (a, ii + 1), size - 1));
        }
        return x;
    }

    template <typename elem> list<list<elem>> partition (list<elem> a, size_t size, size_t offset) {
        if (offset <= 0) throw exception {} << "offset cannot be " << offset;
        if (size > a.size ()) return {};
        return list<list<elem>> {take (a, size)} + partition (drop (a, offset), size, offset);
    }

    namespace {
        template <typename elem>
        list<std::list<entry<elem, elem>>> rules (list<elem> a, list<elem> b) {
            if (a.size () != b.size ()) throw exception {} << "lists must have the same size";
            if (a.size () == 0) return {};
            if (a.size () == 1) return {std::list<entry<elem, elem>> {entry<elem, elem> {first (a), first (b)}}};
            list<std::list<entry<elem, elem>>> x;
            for (int i = 0; i < b.size (); i++) {
                entry<elem, elem> fst {a.first (), b[i]};
                x = x + for_each ([fst] (const std::list<entry<elem, elem>> &x) -> std::list<entry<elem, elem>> {
                    auto z = x;
                    z.push_front (fst);
                    return z;
                }, rules (rest (a), remove_index (b, i)));
            }
            return x;
        }
    }

    template <ordered elem> list<list<elem>> permutations (list<elem> ls) {
        if (empty (ls)) return {};
        if (size (ls) == 1) return {ls};
        return for_each ([ls] (const math::permutation<elem> &p) -> list<elem> {
            return for_each (p, ls);
        }, static_cast<list<math::permutation<elem>>> (rules (ls, ls)));
    }
}

#endif
