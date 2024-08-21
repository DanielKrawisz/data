// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_COMBINATORICS
#define DATA_COMBINATORICS

#include <data/math/permutation.hpp>
#include <data/math/figurate.hpp>
#include <data/tools.hpp>

namespace data {

    template <ordered elem> stack<elem> range (elem from, elem to, elem by = 1);

    // a representation of the permutations of a list that can be iterated over.
    // NOTE: The iteration order is not the same as what you get if you generate
    // the whole list.
    template <ordered elem> struct permutations {
        stack<elem> List;

        operator list<list<elem>> () const;
        template <typename N> N size () const;

        bool operator == (const permutations &) const;

        // forward iterator
        struct iterator {
            using difference_type = int;
            using value_type = stack<elem>;

            stack<elem> operator * () const;
            bool operator == (const iterator &) const;
            iterator &operator ++ ();
            iterator operator ++ (int);
            iterator &operator += (uint32);
            iterator operator + (uint32);

            iterator ();
            iterator (const permutations &ls, const cross<int> &);

        private:
            permutations const *Permutations;
            cross<int> Indices;
        };

        iterator begin () const;
        iterator end () const;
    };

    // a representation of the sublists of a list that can be iterated over.
    // a sublist is defined here as a list containing elements of the given
    // list in the same order. It is not a sublist in the sense that it as
    // a whole is contained in the given list.
    template <typename elem> struct sublists {
        stack<elem> List;
        size_t FromSize;
        size_t ToSize;

        sublists (const stack<elem> &a, size_t size);
        sublists (const stack<elem> &a, size_t from, size_t to);

        template <typename N> N size () const;
        operator list<stack<elem>> () const;

        bool operator == (const sublists &) const;

        // forward iterator
        struct iterator {
            using difference_type = int;
            using value_type = stack<elem>;

            stack<elem> operator * () const;
            bool operator == (const iterator &) const;
            iterator &operator ++ ();
            iterator operator ++ (int);
            iterator &operator += (uint32);
            iterator operator + (uint32);

            iterator ();
            iterator (const sublists &ls, const cross<int> &);

        private:
            sublists const *Sublists;
            cross<int> Indices;
        };

        iterator begin () const;
        iterator end () const;

        // make a list of all sublists of a list for a given size.
        static list<stack<elem>> make (stack<elem> a, size_t size);
    };

    // a representation of the partitions of a list that can be iterated over.
    template <typename elem> struct partitions {
        stack<elem> List;
        math::nonzero<size_t> Size;
        math::nonzero<size_t> Offset;

        partitions (const stack<elem> &a, math::nonzero<size_t> size);
        partitions (const stack<elem> &a, math::nonzero<size_t> size, math::nonzero<size_t> offset);

        template <typename N> N size () const;
        operator list<stack<elem>> () const;

        bool operator == (const partitions &) const;

        // forward iterator
        struct iterator {
            using difference_type = int;
            using value_type = stack<elem>;

            stack<elem> operator * () const;
            bool operator == (const iterator &) const;
            iterator &operator ++ ();
            iterator operator ++ (int);
            iterator &operator += (uint32);
            iterator operator + (uint32);

            iterator ();
            iterator (const partitions &, int i);

        private:
            partitions const *Partitions;
            int Index;
        };

        iterator begin () const;
        iterator end () const;
    };

    template <ordered elem> stack<elem> inline range (elem from, elem to, elem by) {
        stack<elem> x;
        for (elem e = from; e <= to; e += by) x <<= e;
        return reverse (x);
    }

    template <typename elem> inline sublists<elem>::sublists (const stack<elem> &a, size_t size):
        List {a}, FromSize {size}, ToSize {size} {}

    template <typename elem> inline sublists<elem>::sublists (const stack<elem> &a, size_t from, size_t to) :
        List {a}, FromSize {from}, ToSize {to} {}

    template <typename elem> inline partitions<elem>::partitions (const stack<elem> &a, math::nonzero<size_t> size):
        partitions (a, size, size) {}

    template <typename elem> inline partitions<elem>::partitions (const stack<elem> &a, math::nonzero<size_t> size, math::nonzero<size_t> offset):
        List {a}, Size {size}, Offset {offset} {
        if (Offset.Value <= 0 || Size.Value <= 0) throw exception {} << "offset cannot be " << Offset;
    }

    template <ordered elem> template <typename N> N inline permutations<elem>::size () const {
        if (List.size () == 0) return 0;
        return math::factorial<N> (N (List.size ()));
    }

    template <typename elem> template <typename N> N inline sublists<elem>::size () const {
        N x = 0;
        for (N i = FromSize; i <= ToSize; i++) x += math::polytopic_number<N> (i, N (List.size ()) - i + 1);
        return x;
    }

    template <typename elem> template <typename N> N inline partitions<elem>::size () const {
        N b {List.size () - (Size.Value - Offset.Value)};
        math::division<N> d = math::divide<N> {} (b, math::nonzero<N> {N (Offset.Value)});
        return d.Remainder == 0 || d.Remainder >= Size.Value - Offset.Value ? d.Quotient : d.Quotient - 1;
    }

    namespace {
        template <typename elem>
        list<std::list<entry<elem, elem>>> rules (stack<elem> a, stack<elem> b) {
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
        if (data::empty (List)) return {};
        if (data::size (List) == 1) return {List};
        return for_each ([x = List] (const math::permutation<elem> &p) -> list<elem> {
            return for_each (p, x);
        }, static_cast<list<math::permutation<elem>>> (rules (List, List)));
    }

    template <typename elem> sublists<elem>::operator list<stack<elem>> () const {
        auto x = List;
        return reduce<list<stack<elem>>, list<list<stack<elem>>>> (&join<list<stack<elem>>>,
            for_each ([x] (size_t size) -> list<stack<elem>> {
                return make (x, size);
            }, range (FromSize, ToSize)));
    }

    template <typename elem> partitions<elem>::operator list<stack<elem>> () const {
        if (Size.Value > List.size ()) return {};
        return prepend (static_cast<list<stack<elem>>> (partitions (drop (List, Offset.Value), Size, Offset)), take (List, Size.Value));
    }

    template <typename elem> list<stack<elem>> sublists<elem>::make (stack<elem> a, size_t size) {
        if (size > a.size () || size < 0) return {};
        if (size == 0) return {{}};

        list<stack<elem>> x {};
        for (int ii = 0; ii <= a.size () - size; ii++) {
            const elem &e = a[ii];
            x = x + for_each ([e] (stack<elem> y) -> stack<elem> {
                return prepend (y, e);
            }, make (drop (a, ii + 1), size - 1));
        }

        return x;
    }

    template <ordered elem> permutations<elem>::iterator inline permutations<elem>::iterator::operator ++ (int) {
        auto x = *this;
        ++(*this);
        return x;
    }

    template <typename elem> sublists<elem>::iterator inline sublists<elem>::iterator::operator ++ (int) {
        auto x = *this;
        ++(*this);
        return x;
    }

    template <typename elem> partitions<elem>::iterator inline partitions<elem>::iterator::operator ++ (int) {
        auto x = *this;
        ++(*this);
        return x;
    }

    template <ordered elem> inline permutations<elem>::iterator::iterator ():
        Permutations {nullptr}, Indices {} {}

    template <ordered elem> inline permutations<elem>::iterator::iterator (const permutations &p, const cross<int> &i) :
        Permutations {&p}, Indices {i} {}

    template <typename elem> inline sublists<elem>::iterator::iterator ():
        Sublists {nullptr}, Indices {} {}

    template <typename elem> inline sublists<elem>::iterator::iterator (const sublists &x, const cross<int> &i) :
        Sublists {&x}, Indices {i} {}

    template <typename elem> inline partitions<elem>::iterator::iterator ():
        Partitions {nullptr}, Index {0} {}

    template <typename elem> inline partitions<elem>::iterator::iterator (const partitions &p, int i):
        Partitions {&p}, Index {i} {}

    template <ordered elem> bool inline permutations<elem>::iterator::operator == (const iterator &i) const {
        return Permutations == i.Permutations && Indices == i.Indices;
    }

    template <typename elem> bool inline sublists<elem>::iterator::operator == (const iterator &i) const {
        return Sublists == i.Sublists && Indices == i.Indices;
    }

    template <typename elem> bool inline partitions<elem>::iterator::operator == (const iterator &i) const {
        return Partitions == i.Partitions && Index == i.Index;
    }

    template <ordered elem> bool inline permutations<elem>::operator == (const permutations &p) const {
        return List == p.List;
    }

    template <typename elem> bool inline sublists<elem>::operator == (const sublists &p) const {
        return List == p.List && FromSize == p.FromSize && ToSize == p.ToSize;
    }

    template <typename elem> bool inline partitions<elem>::operator == (const partitions &p) const {
        return List == p.List && Size == p.Size && Offset == p.Offset;
    }

    template <ordered elem> inline permutations<elem>::iterator permutations<elem>::begin () const {
        cross<int> ind (List.size ());
        for (int i = 0; i < ind.size (); i++) ind[i] = i;
        return iterator {*this, ind};
    }

    template <ordered elem> inline permutations<elem>::iterator permutations<elem>::end () const {
        return iterator {*this, cross<int> (List.size (), -1)};
    }

    template <typename elem> sublists<elem>::iterator sublists<elem>::begin () const {
        size_t indices_size = FromSize > List.size () ? List.size () + 1 : FromSize;
        cross<int> ind (indices_size);
        for (int i = 0; i < indices_size; i++) ind[i] = i;
        return iterator {*this, ind};
    }

    template <typename elem> sublists<elem>::iterator sublists<elem>::end () const {
        size_t indices_size = ToSize > List.size () ? List.size () + 1 : ToSize + 1;
        cross<int> ind (indices_size);
        for (int i = 0; i < indices_size; i++) ind[i] = i;
        return iterator {*this, ind};
    }

    template <typename elem> inline partitions<elem>::iterator partitions<elem>::begin () const {
        return iterator {*this, 0};
    }

    template <typename elem> inline partitions<elem>::iterator partitions<elem>::end () const {
        size_t z = size<size_t> ();
        return iterator {*this, z * Offset.Value};
    }

    template <ordered elem> permutations<elem>::iterator &permutations<elem>::iterator::operator ++ () {
        int i = Indices.size () - 1;
        while (i >= 0) {
            int x = -1;
            for (int j = 0; j < Indices.size (); j++)
                if (x == -1) {
                    if (Indices[j] == i) x = j;
                } else if (Indices[j] == -1) {
                    Indices[j] = i;
                    Indices[x] = -1;
                    goto out;
                }
            if (x == -1) return *this;
            Indices[x] = -1;
            i--;
        }
        return *this;
        out:
        for (int j = 0; j < Indices.size (); j++) if (Indices[j] == -1) Indices[j] = ++i;
        return *this;
    }

    template <typename elem> sublists<elem>::iterator &sublists<elem>::iterator::operator ++ () {
        int i = Indices.size () - 1;
        while (true) {
            if (i == -1) {
                Indices.resize (Indices.size () + 1);
                for (i = 0; i < Indices.size (); i++) Indices[i] = i;
                return *this;
            }
            Indices[i]++;
            if (Indices[i] <= Sublists->List.size () - Indices.size () + i) {
                int j = Indices[i];
                while (i < Indices.size () - 1) {
                    i++;
                    j++;
                    Indices[i] = j;
                }
                return *this;
            }
            i--;
        }
    }

    template <typename elem> inline partitions<elem>::iterator &partitions<elem>::iterator::operator ++ () {
        Index += Partitions->Offset.Value;
        return *this;
    }

    template <ordered elem> stack<elem> permutations<elem>::iterator::operator * () const {
        stack<elem> ls;
        for (int i = 0; i < Indices.size (); i++) ls <<= Permutations->List[Indices[i]];
        return reverse (ls);
    }

    template <ordered elem> stack<elem> sublists<elem>::iterator::operator * () const {
        stack<elem> ls;
        for (int i = 0; i < Indices.size (); i++)
            ls <<= Sublists->List[Indices[i]];
        return reverse (ls);
    }

    template <ordered elem> stack<elem> partitions<elem>::iterator::operator * () const {
        stack<elem> ls;
        for (int i = 0; i < Partitions->Size.Value; i++) ls <<= Partitions->List[i + Index];
        return reverse (ls);
    }

    template <ordered elem> permutations<elem>::iterator inline &permutations<elem>::iterator::operator += (uint32 u) {
        for (uint32 i = 0; i < u; i++) ++(*this);
        return *this;
    }

    template <ordered elem> permutations<elem>::iterator inline permutations<elem>::iterator::operator + (uint32 u) {
        auto n = *this;
        for (uint32 i = 0; i < u; i++) ++n;
        return n;
    }

    template <typename elem> sublists<elem>::iterator inline &sublists<elem>::iterator::operator += (uint32 u) {
        for (uint32 i = 0; i < u; i++) ++(*this);
        return *this;
    }

    template <typename elem> sublists<elem>::iterator inline sublists<elem>::iterator::operator + (uint32 u) {
        auto n = *this;
        for (uint32 i = 0; i < u; i++) ++n;
        return n;
    }

    template <typename elem> partitions<elem>::iterator inline &partitions<elem>::iterator::operator += (uint32 u) {
        for (uint32 i = 0; i < u; i++) ++(*this);
        return *this;
    }

    template <typename elem> partitions<elem>::iterator inline partitions<elem>::iterator::operator + (uint32 u) {
        auto n = *this;
        for (uint32 i = 0; i < u; i++) ++n;
        return n;
    }
}

#endif
