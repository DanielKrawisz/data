// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_COMBINATORICS
#define DATA_COMBINATORICS

#include <data/math/permutation.hpp>
#include <data/math/figurate.hpp>
#include <data/tools.hpp>

namespace data {

    template <ordered elem> list<elem> range (elem from, elem to, elem by = 1);

    // a representation of the permutations of a list that can be iterated over.
    template <ordered elem> struct permutations {
        list<elem> List;

        operator list<list<elem>> () const;
        template <typename N> N size () const;

        bool operator == (const permutations &) const;

        struct iterator {
            list<elem &> operator * () const;
            bool operator == (const iterator &) const;
            iterator &operator ++ ();
            iterator operator ++ (int);

            iterator ();
            iterator (permutations &ls, const cross<int> &);

        private:
            permutations &Permutations;
            cross<int> Indices;
        };

        iterator begin ();
        iterator end ();
    };

    // a representation of the sublists of a list that can be iterated over.
    template <typename elem> struct sublists {
        list<elem> List;
        size_t FromSize;
        size_t ToSize;

        sublists (list<elem> a, size_t size);
        sublists (list<elem> a, size_t from, size_t to);

        template <typename N> N size () const;
        operator list<list<elem>> () const;

        bool operator == (const sublists &) const;

        struct iterator {
            list<elem &> operator * () const;
            bool operator == (const iterator &) const;
            iterator &operator ++ ();
            iterator operator ++ (int);

            iterator ();
            iterator (sublists &ls, const cross<int> &);

        private:
            sublists &Sublists;
            cross<int> Indices;
        };

        iterator begin ();
        iterator end ();

        static list<list<elem>> make (list<elem> a, size_t size);
    };

    // a representation of the partitions of a list that can be iterated over.
    template <typename elem> struct partitions {
        list<elem> List;
        size_t Size;
        size_t Offset;

        partitions (list<elem> a, size_t size);
        partitions (list<elem> a, size_t size, size_t offset);

        template <typename N> N size () const;
        operator list<list<elem>> () const;

        bool operator == (const partitions &) const;

        struct iterator {
            list<elem &> operator * () const;
            bool operator == (const iterator &) const;
            iterator &operator ++ ();
            iterator operator ++ (int);

            iterator ();
            iterator (list<elem> ls, int i);

        private:
            partitions &Partitions;
            int Index;
        };

        iterator begin ();
        iterator end ();
    };

    template <ordered elem> list<elem> inline range (elem from, elem to, elem by) {
        list<elem> x;
        for (elem e = from; e <= to; e += by) x <<= e;
        return x;
    }

    template <typename elem> inline sublists<elem>::sublists (list<elem> a, size_t size):
        List {a}, FromSize {size}, ToSize {size} {}

    template <typename elem> inline sublists<elem>::sublists (list<elem> a, size_t from, size_t to) :
        List {a}, FromSize {from}, ToSize {to} {}

    template <typename elem> inline partitions<elem>::partitions (list<elem> a, size_t size):
        List {a}, Size {size}, Offset {size} {}

    template <typename elem> inline partitions<elem>::partitions (list<elem> a, size_t size, size_t offset):
        List {a}, Size {size}, Offset {offset} {}

    template <ordered elem> template <typename N> N inline permutations<elem>::size () const {
        return factorial<N> (N (List.size ()));
    }

    template <typename elem> template <typename N> N inline sublists<elem>::size () const {
        N x = 0;
        for (N i = FromSize; i <= ToSize; i++) x += polytopic_number<N> (i, N (List.size ()) - i + 1);
        return x;
    }

    template <typename elem> template <typename N> N inline partitions<elem>::size () const {
        N size {List.size ()};
        math::division<N> d = math::divide<N> {} (size, N (Offset));
        return d.Remainder < Size - Offset ? d.Quotient - 1 : d.Quotient;
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

    template <ordered elem> permutations<elem>::operator list<list<elem>> () const {
        if (empty (List)) return {};
        if (size (List) == 1) return {List};
        return for_each ([x = List] (const math::permutation<elem> &p) -> list<elem> {
            return for_each (p, x);
        }, static_cast<list<math::permutation<elem>>> (rules (List, List)));
    }

    template <typename elem> sublists<elem>::operator list<list<elem>> () const {
        return reduce (join<list<list<elem>>>, for_each ([x = List] (size_t size) -> list<list<elem>> {
            return make (x, size);
        }, range (FromSize, ToSize)));
    }

    template <typename elem> partitions<elem>::operator list<list<elem>> () const {
        if (Offset <= 0) throw exception {} << "offset cannot be " << Offset;
        if (Size > List.size ()) return {};
        return list<list<elem>> {take (List, Size)} + partition (drop (List, Offset), Size, Offset);
    }

    template <typename elem> list<list<elem>> sublists<elem>::make (list<elem> a, size_t size) {
        if (size > a.size () || size < 0) return {};
        if (size == 0) return {{}};
        list<list<elem>> x {};
        for (int ii = 0; ii <= a.size () - size; ii++) {
            const elem &e = a[ii];
            x = x + for_each ([e] (list<elem> a) -> list<elem> {
                return prepend (a, e);
            }, make (drop (a, ii + 1), size - 1));
        }
        return x;
    }

    template <ordered elem> inline permutations<elem>::iterator::operator ++ (int) {
        auto x = *this;
        (*this)++;
        return x;
    }

    template <template elem> inline sublists<elem>::iterator::operator ++ (int) {
        auto x = *this;
        (*this)++;
        return x;
    }

    template <template elem> inline paritions<elem>::iterator::operator ++ (int) {
        auto x = *this;
        (*this)++;
        return x;
    }

    template <ordered elem> inline permutations<elem>::iterator (): List {}, Indices {} {}
    template <ordered elem> inline permutations<elem>::iterator (list<elem> ls, const cross<int> &i) : List {ls}, Indices {i} {}

    template <template elem> inline sublists<elem>::iterator (): List {}, Indices {} {}
    template <template elem> inline sublists<elem>::iterator (list<elem> ls, const cross<int> &i) : List {ls}, Indices {i} {}

    template <template elem> inline partitions<elem>::iterator (): List {}, Index {0} {}
    template <template elem> inline partitions<elem>::iterator (list<elem> ls, int i): List {ls}, Index {i} {}

    template <ordered elem> bool inline permutations<elem>::iterator::operator == (const iterator &i) const {
        return Permutations == i.Permutations && Indices == i.Indices;
    }

    template <template elem> bool inline sublists<elem>::iterator::operator == (const iterator &i) const {
        return Sublists == i.Sublists && Indices == i.Indices;
    }

    template <template elem> bool inline partitions<elem>::iterator::operator == (const iterator &i) const {
        return Partitions == i.Partitions && Index == i.Index;
    }

    template <ordered elem> bool inline permutations<elem>::operator == (const permutations &p) const {
        return List == p.List;
    }

    template <template elem> bool inline sublists<elem>::operator == (const sublists &p) const {
        return List == p.List && FromSize == p.FromSize && ToSize == p.ToSize;
    }

    template <template elem> bool inline partitions<elem>::operator == (const partitions &p) const {
        return List == p.List && Size == p.Size && Offset == p.Offset;
    }

    template <ordered elem> inline permutations<elem>::iterator permutations<elem>::begin () {
        cross<int> ind (List.size ());
        for (int i = 0; i < ind.size (); i++) ind[i] = i;
        return iterator {*this, ind};
    }

    template <ordered elem> inline permutations<elem>::iterator permutations<elem>::end () {
        return iterator {*this, cross<int> (List.size (), -1)};
    }

    template <template elem> inline sublists<elem>::iterator sublists<elem>::begin () {
        cross<int> ind (FromSize);
        for (int i = 0; i < FromSize; i++) ind[i] = i;
        return iterator {*this, ind};
    }

    template <template elem> inline sublists<elem>::iterator sublists<elem>::end () {
        cross<int> ind (ToSize + 1)
        for (int i = 0; i < ToSize + 1; i++) ind[i] = i;
        return iterator {*this, ind };
    }

    template <template elem> inline partitions<elem>::iterator partitions<elem>::begin () {
        return iterator {*this, 0};
    }

    template <template elem> inline partitions<elem>::iterator partitions<elem>::end () {
        auto o = List.size () / Offset;
        auto n = List.size () % Offset;
        return iterator {*this, n < Size ? o * Offset : (o + 1) * Offset};
    }

    template <ordered elem> permutations<elem>::iterator &permutations<elem>::operator ++ () {
        int i = Indices.size () - 1;
        while (i >= 0) {
            int x = -1;
            for (int j = 0; j < Indices.size (); j++)
                if (x == -1 && Indices[j] == i) x = j;
                else if (Indices[j] == -1) {
                    Indices[j] = i;
                    Indices[x] = -1;
                    goto out;
                }
            if (x == -1) return *this;
            Indices[x] = -1;
            i--;
        }
        return *this;
        out;
        for (int j = 0; j < Indices.size (); j++) if (Indices[j] == -1) Indices[j] = ++i;
        return *this;
    }

    template <template elem> sublists<elem>::iterator &sublists<elem>::operator ++ () {
        int i = Indices.size () - 1;
        while (true) {
            Indices[i]++;
            if (Indices[i] < Sublists.List.size () - Indices.size () + i + 1)) {
                int j = Indices[i];
                do {
                    i++;
                    j++;
                    Indices[i] = j;
                } while (i < Indices.size () - 1);
                return *this;
            }
            i--;
            if (i == -1) {
                Indices.resize (Indices.size () + 1);
                for (i = 0; i < Indices.size (): i++) Indices[i] = i;
                return *this;
            }
        }
    }

    template <template elem> inline partitions<elem>::iterator &partitions<elem>::operator ++ () {
        Index += Partition.Offset;
        return *this;
    }

    template <ordered elem> list<elem &> permutations<elem>::iterator::operator * () const {
        list<elem &> ls;
        for (int i = 0; i < Indices.size (); i++) ls <<= Permutations.List[Indices[i]];
        return ls;
    }

    template <ordered elem> list<elem &> sublists<elem>::iterator::operator * () const {
        list<elem &> ls;
        for (int i = 0; i < Indices.size (); i++) ls <<= Permutations.List[Indices[i]];
        return ls;
    }

    template <ordered elem> list<elem &> partitions<elem>::iterator::operator * () const {
        list<elem &> ls;
        for (int i = 0; i < Partition.Size; i++) ls <<= Permutations.List[i + Index];
        return ls;
    }
}

#endif
