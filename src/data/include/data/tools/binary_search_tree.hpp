// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_BINARY_SEARCH_TREE
#define DATA_BINARY_SEARCH_TREE

#include <data/tools/linked_tree.hpp>
#include <data/meta.hpp>

namespace data {

    template <sortable value, functional::tree<value> tree>
    struct binary_search_tree : tree {
        binary_search_tree ();
        binary_search_tree (const tree &);
        binary_search_tree (tree &&);

        binary_search_tree left () const;
        binary_search_tree right () const;

        static bool sorted (binary_search_tree);

        bool valid () const;

        binary_search_tree insert (const value &) const;
        const value *contains (const value &) const;

        binary_search_tree (std::initializer_list<wrapped<value>> x);

        tool::ordered_stack<linked_stack<const value &>> values () const;

        // the two trees are considered to be equal if they have
        // the same elements even if they are not the same tree.
        template <sortable X, functional::tree<value> T>
        requires std::equality_comparable_with<value, X>
        bool operator == (const data::binary_search_tree<X, T> &x) const;

        // an iterator for a tree that treats it as a binary search tree
        // since there are other ways that the elements of a tree could
        // be organized, we don't have any natural iterator for a tree.
        struct iterator {
            using sentinel = data::sentinel<tree>;

            using value_type = value;
            using difference_type = int;

            const tree *Tree;
            tree Next;
            linked_stack<const tree> Last;

            // we need this constructor in order to satisfy some
            // std concepts but it's not really good for anything.
            iterator ();

            // for the beginning, the second argument should be the tree itself.
            // for the end, the second argument should be the empty tree.
            iterator (const tree *m, const tree &n);

            iterator operator ++ (int);
            iterator &operator ++ ();

            const value &operator * () const;
            const value *operator -> () const;

            bool operator == (const iterator &i) const;

            bool operator == (const sentinel i) const;

            int operator - (const iterator &i) const;

        private:
            void go_left ();
        };

        iterator begin () const;
        iterator end () const;
    };

    template <sortable value, functional::tree<value> tree>
    binary_search_tree<value, tree>::iterator
    binary_search_tree<value, tree>::iterator::operator ++ (int) {
        auto x = *this;
        ++(*this);
        return x;
    }

    template <sortable value, functional::tree<value> tree>
    binary_search_tree<value, tree>::iterator
    &binary_search_tree<value, tree>::iterator::operator ++ () {
        if (Next.empty ()) return *this;

        if (!Next.right ().empty ()) {
            Next = Next.right ();
            go_left ();
            return *this;
        }

        if (!data::empty (Last)) {
            Next = Last.first ();
            Last = Last.rest ();
            return *this;
        }

        Next = tree {};
        return *this;
    }

    template <sortable value, functional::tree<value> tree>
    const value inline &binary_search_tree<value, tree>::iterator::operator * () const {
        return Next.root ();
    }

    template <sortable value, functional::tree<value> tree>
    const value inline *binary_search_tree<value, tree>::iterator::operator -> () const {
        return &Next.root ();
    }

    template <sortable value, functional::tree<value> tree>
    bool inline binary_search_tree<value, tree>::iterator::operator == (const iterator &i) const {
        return Tree == i.Tree && Next == i.Next;
    }

    template <sortable value, functional::tree<value> tree>
    void binary_search_tree<value, tree>::iterator::go_left () {
        if (Next.empty ()) return;
        while (!Next.left ().empty ()) {
            Last = Last << Next;
            Next = Next.left ();
        }
    }

    template <sortable value, functional::tree<value> tree>
    inline binary_search_tree<value, tree>::iterator::iterator () : Tree {nullptr}, Next {}, Last {} {}

    template <sortable value, functional::tree<value> tree>
    inline binary_search_tree<value, tree>::iterator::iterator (const tree *m, const tree &n) : Tree {m}, Next {n}, Last {} {
        go_left ();
    }

    template <sortable value, functional::tree<value> tree>
    template <sortable X, functional::tree<value> T>
    requires std::equality_comparable_with<value, X>
    bool binary_search_tree<value, tree>::operator == (const data::binary_search_tree<X, T> &x) const {
        if (this->size () != x.size ()) return false;
        auto bb = x.begin ();

        for (auto ab = begin (); ab != end (); ab++) {
            if (*ab != *bb) return false;
            bb++;
        }

        return true;
    }

    template <sortable value, functional::tree<value> tree>
    inline binary_search_tree<value, tree>::binary_search_tree (): tree {} {}

    template <sortable value, functional::tree<value> tree>
    inline binary_search_tree<value, tree>::binary_search_tree (const tree &t): tree {t} {}

    template <sortable value, functional::tree<value> tree>
    inline binary_search_tree<value, tree>::binary_search_tree (tree &&t): tree {t} {}

    template <sortable value, functional::tree<value> tree>
    binary_search_tree<value, tree> inline binary_search_tree<value, tree>::left () const {
        return static_cast<const tree *> (this)->left ();
    }

    template <sortable value, functional::tree<value> tree>
    binary_search_tree<value, tree> inline binary_search_tree<value, tree>::right () const {
        return static_cast<const tree *> (this)->right ();
    }

    template <sortable value, functional::tree<value> tree>
    bool inline binary_search_tree<value, tree>::valid () const {
        return tree::valid () && sorted (*this);
    }

    template <sortable value, functional::tree<value> tree>
    inline binary_search_tree<value, tree>::binary_search_tree (std::initializer_list<wrapped<value>> x) {
        for (const wrapped<value> z : x) *this = this->insert (z);
    }

    template <sortable value, functional::tree<value> tree>
    tool::ordered_stack<linked_stack<const value &>> inline binary_search_tree<value, tree>::values () const {
        linked_stack<const value &> st;
        for (const value &v : *this) st <<= v;
        return tool::ordered_stack<linked_stack<const value &>> {reverse (st)};
    }

    template <sortable value, functional::tree<value> tree>
    bool binary_search_tree<value, tree>::sorted (binary_search_tree x) {
        if (data::empty (x)) return true;

        auto z = x.begin ();
        auto last = z;
        z++;
        for (; z != x.end (); z++) {
            if (!(*last < *z)) return false;
            last = z;
        }

        return true;
    }

    template <sortable value, functional::tree<value> tree>
    const value inline *binary_search_tree<value, tree>::contains (const value &v) const {
        return data::empty (*this) ? nullptr: this->root () == v ? &this->root ():
            this->root () < v ? this->left ().contains (v): this->right ().contains (v);
    }

    template <sortable value, functional::tree<value> tree>
    binary_search_tree<value, tree> inline binary_search_tree<value, tree>::insert (const value &v) const {
        return data::empty (*this) ?
            tree {v}:
            v < this->root () ?
                tree {this->root (), left ().insert (v), right ()}:
                tree {this->root (), left (), right ().insert (v)};
    }


    template <sortable value, functional::tree<value> tree>
    binary_search_tree<value, tree>::iterator inline binary_search_tree<value, tree>::begin () const {
        return iterator {this, *this};
    }

    template <sortable value, functional::tree<value> tree>
    binary_search_tree<value, tree>::iterator inline binary_search_tree<value, tree>::end () const {
        return iterator {this, tree {}};
    }

}

#endif

