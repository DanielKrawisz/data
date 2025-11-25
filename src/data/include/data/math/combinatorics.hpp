// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_COMBINATORICS
#define DATA_COMBINATORICS

#include <data/list.hpp>
#include <data/stack.hpp>
#include <data/lift.hpp>
#include <data/sign.hpp>
#include <data/abs.hpp>
#include <data/math/figurate.hpp>
#include <data/divmod.hpp>

namespace data {

    template <Ordered elem> stack<elem> range (elem from, elem to, elem by = 1);

    // a representation of the permutations of a list that can be iterated over.
    template <typename elem> struct permutations {
        cross<elem> List;

        // count the number of permutations.
        template <typename N> constexpr N count () const;

        // get list of all permutations. (hard on memory,
        // better to iterate over them if there are a lot.)
        operator list<cross<elem>> () const;

        bool operator == (const permutations &) const;

        // forward iterator
        struct iterator;

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

        template <typename N> N count () const;
        operator list<stack<elem>> () const;

        bool operator == (const sublists &) const;

        // forward iterator
        struct iterator {
            using difference_type = int;
            using value_type = stack<elem>;

            stack<elem> operator * () const;
            bool operator == (const iterator &) const;
            // preincrement
            iterator &operator ++ ();
            // postincrement
            iterator operator ++ (int);
            iterator &operator += (uint32);
            iterator operator + (uint32);

            iterator ();
            iterator (const sublists &ls, const cross<int> &);

        private:
            sublists Sublists;
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

        template <typename N> N count () const;
        operator list<stack<elem>> () const;

        bool operator == (const partitions &) const;

        // forward iterator
        struct iterator {
            using difference_type = int;
            using value_type = stack<elem>;

            stack<elem> operator * () const;
            bool operator == (const iterator &) const;
            // preincrement
            iterator &operator ++ ();
            // postincrement
            iterator operator ++ (int);
            iterator &operator += (uint32);
            iterator operator + (uint32);

            iterator ();
            iterator (const partitions &, size_t i);

        private:
            partitions const *Partitions;
            size_t Index;
        };

        iterator begin () const;
        iterator end () const;
    };

    template <typename elem>
    struct permutations<elem>::iterator {
        using difference_type = int;
        using value_type = cross<elem>;

        const value_type &operator * () const;
        const value_type *operator -> () const;

        bool operator == (const iterator &) const;

        // preincrement
        iterator &operator ++ ();
        // postincrement
        iterator operator ++ (int);
        iterator &operator += (uint32);
        iterator operator + (uint32);

        iterator ();
        iterator (const cross<elem> &, bool end = false);

    private:
        using face = math::sign;
        constexpr static const face left = math::negative;
        constexpr static const face right = math::positive;

        cross<elem> Permuted;

        struct SJT_complex {
            face Face;
            size_t Value;
            bool operator == (const SJT_complex &x) const {
                return Face == x.Face && Value == x.Value;
            }
        };

        cross<SJT_complex> SJT;

        void swap (int i, int j) {
            std::swap (SJT[i], SJT[j]);
            std::swap (Permuted[i], Permuted[j]);
        }

        size_t size () const {
            return Permuted.size ();
        }
    };

    template <typename elem>
    permutations<elem>::iterator inline permutations<elem>::begin () const {
        return iterator {List, false};
    }

    template <typename elem>
    permutations<elem>::iterator inline permutations<elem>::end () const {
        return iterator {List, true};
    }

    template <Ordered elem> stack<elem> inline range (elem from, elem to, elem by) {
        stack<elem> x;
        for (elem e = from; e <= to; e += by) x >>= e;
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

    template <typename elem> template <typename N> constexpr N inline permutations<elem>::count () const {
        if (List.size () == 0) return 0;
        return math::factorial<N> (N (List.size ()));
    }

    template <typename elem> template <typename N> N inline sublists<elem>::count () const {
        N x = 0;
        for (N i = FromSize; i <= ToSize; i++) x += math::polytopic_number<N> (i, N (List.size ()) - i + 1);
        return x;
    }

    template <typename elem> template <typename N> N inline partitions<elem>::count () const {
        N b {List.size () - (Size.Value - Offset.Value)};
        division<N> d = divmod (b, math::nonzero<N> {N (Offset.Value)});
        return d.Remainder == 0 || d.Remainder >= Size.Value - Offset.Value ? d.Quotient : d.Quotient - 1;
    }

    template <typename elem> permutations<elem>::operator list<cross<elem>> () const {
        auto i = begin ();
        auto e = end ();
        list<cross<elem>> result;
        while (i != e) {
            result <<= *i;
            i++;
        }
        return result;
    }

    template <typename elem> sublists<elem>::operator list<stack<elem>> () const {
        auto x = List;
        return reduce<list<stack<elem>>, list<list<stack<elem>>>> (&join<list<stack<elem>>>,
            lift ([x] (size_t size) -> list<stack<elem>> {
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
            x = x + lift ([e] (stack<elem> y) -> stack<elem> {
                return prepend (y, e);
            }, make (drop (a, ii + 1), size - 1));
        }

        return x;
    }

    template <typename elem> permutations<elem>::iterator inline permutations<elem>::iterator::operator ++ (int) {
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

    template <typename elem> inline permutations<elem>::iterator::iterator (const cross<elem> &init, bool end):
        Permuted {init}, SJT {} {
        SJT.resize (size ());
        face initial_dir = end ? right : left;
        for (size_t i = 0; i < size (); i++) SJT[i] = {initial_dir, i};
    }

    template <typename elem> inline sublists<elem>::iterator::iterator ():
        Sublists {nullptr}, Indices {} {}

    template <typename elem> inline sublists<elem>::iterator::iterator (const sublists &x, const cross<int> &i) :
        Sublists {x}, Indices {i} {}

    template <typename elem> inline partitions<elem>::iterator::iterator ():
        Partitions {nullptr}, Index {0} {}

    template <typename elem> inline partitions<elem>::iterator::iterator (const partitions &p, size_t i):
        Partitions {&p}, Index {i} {}

    template <typename elem> bool inline permutations<elem>::iterator::operator == (const iterator &i) const {
        return SJT == i.SJT && Permuted == i.Permuted;
    }

    template <typename elem> bool inline sublists<elem>::iterator::operator == (const iterator &i) const {
        return Sublists == i.Sublists && Indices == i.Indices;
    }

    template <typename elem> bool inline partitions<elem>::iterator::operator == (const iterator &i) const {
        return Partitions == i.Partitions && Index == i.Index;
    }

    template <typename elem> bool inline permutations<elem>::operator == (const permutations &p) const {
        return List == p.List;
    }

    template <typename elem> bool inline sublists<elem>::operator == (const sublists &p) const {
        return List == p.List && FromSize == p.FromSize && ToSize == p.ToSize;
    }

    template <typename elem> bool inline partitions<elem>::operator == (const partitions &p) const {
        return List == p.List && Size == p.Size && Offset == p.Offset;
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
        size_t z = count<size_t> ();
        return iterator {*this, z * Offset.Value};
    }

    // iterate through all permutations using adjacent swaps, ensuring that the
    // signature of the permutation changes with each iteration.
    template <typename elem> permutations<elem>::iterator &permutations<elem>::iterator::operator ++ () {
        int max_mobile_index = -1;
        for (int i = 0; i < size (); i++) {
            // an object is mobile if it points to a lesser object.
            int adjacent_index = i + int8 (SJT[i].Face);
            // it's not mobile if it points outside the array.
            if (adjacent_index < 0 || adjacent_index >= size ()) continue;

            if (SJT[i].Value > SJT[adjacent_index].Value &&
                (max_mobile_index == -1 || SJT[max_mobile_index].Value < SJT[i].Value))
                    max_mobile_index = i;
        }

        // If no king was found, we have to detect
        // whether this is the last valid permutation.
        // If it is, we increment to the end () state.
        if (max_mobile_index == -1) {
            if (size () == 1) SJT[0].Face = right;
            else if (size () > 1 && SJT[0].Value == 1) {
                // take us to the end () state.
                swap (0, 1);
                SJT[0].Face = right;
                SJT[1].Face = right;
            };

        } else {
            // flip the king with the adjacent element in his preferred direction.
            size_t king_value = SJT[max_mobile_index].Value;
            swap (max_mobile_index, max_mobile_index + int8 (SJT[max_mobile_index].Face));
            // flip every higher value
            for (int i = 0; i < size (); i++)
                if (SJT[i].Value > king_value) SJT[i].Face = -SJT[i].Face;
        }

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
            if (Indices[i] <= Sublists.List.size () - Indices.size () + i) {
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

    template <typename elem> const cross<elem> inline &permutations<elem>::iterator::operator * () const {
        return Permuted;
    }

    template <typename elem> stack<elem> sublists<elem>::iterator::operator * () const {
        stack<elem> ls;
        for (int i = 0; i < Indices.size (); i++)
            ls >>= Sublists.List[Indices[i]];
        return reverse (ls);
    }

    template <typename elem> stack<elem> partitions<elem>::iterator::operator * () const {
        stack<elem> ls;
        for (int i = 0; i < Partitions->Size.Value; i++) ls >>= Partitions->List[i + Index];
        return reverse (ls);
    }

    template <typename elem> permutations<elem>::iterator inline &permutations<elem>::iterator::operator += (uint32 u) {
        for (uint32 i = 0; i < u; i++) ++(*this);
        return *this;
    }

    template <typename elem> permutations<elem>::iterator inline permutations<elem>::iterator::operator + (uint32 u) {
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
