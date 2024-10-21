// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_BINARY_SEARCH_TREE
#define DATA_BINARY_SEARCH_TREE

#include <data/tools/linked_tree.hpp>
#include <data/meta.hpp>

namespace data {

    template <ordered value, functional::buildable_tree<value> tree>
    struct binary_search_tree : tree {
        binary_search_tree ();
        binary_search_tree (const tree &);
        binary_search_tree (tree &&);

        binary_search_tree left () const;
        binary_search_tree right () const;

        static bool sorted (binary_search_tree);

        bool valid () const;

        binary_search_tree insert (const value &) const;
        const unref<value> *contains (const value &) const;
        unref<value> *contains (const value &);

        binary_search_tree (std::initializer_list<wrapped<value>> x);

        tool::ordered_stack<linked_stack<const value &>> values () const;

        // the two trees are considered to be equal if they have
        // the same elements even if they are not the same tree.
        template <ordered X, functional::buildable_tree<value> T>
        requires std::equality_comparable_with<value, X>
        bool operator == (const data::binary_search_tree<X, T> &x) const;

        using iterator = functional::binary_search_iterator<value, tree>;

        iterator begin () const;
        iterator end () const;
    };

    template <ordered value, functional::buildable_tree<value> tree>
    inline binary_search_tree<value, tree>::binary_search_tree (): tree {} {}

    template <ordered value, functional::buildable_tree<value> tree>
    inline binary_search_tree<value, tree>::binary_search_tree (const tree &t): tree {t} {}

    template <ordered value, functional::buildable_tree<value> tree>
    inline binary_search_tree<value, tree>::binary_search_tree (tree &&t): tree {t} {}

    template <ordered value, functional::buildable_tree<value> tree>
    binary_search_tree<value, tree> inline binary_search_tree<value, tree>::left () const {
        return static_cast<const tree *> (this)->left ();
    }

    template <ordered value, functional::buildable_tree<value> tree>
    binary_search_tree<value, tree> inline binary_search_tree<value, tree>::right () const {
        return static_cast<const tree *> (this)->right ();
    }

    template <ordered value, functional::buildable_tree<value> tree>
    bool inline binary_search_tree<value, tree>::valid () const {
        return tree::valid () && sorted (*this);
    }

    template <ordered value, functional::buildable_tree<value> tree>
    inline binary_search_tree<value, tree>::binary_search_tree (std::initializer_list<wrapped<value>> x) {
        for (const wrapped<value> z : x) *this = this->insert (z);
    }

    template <ordered value, functional::buildable_tree<value> tree>
    tool::ordered_stack<linked_stack<const value &>> inline binary_search_tree<value, tree>::values () const {
        linked_stack<const value &> st;
        for (const value &v : *this) st <<= v;
        return tool::ordered_stack<linked_stack<const value &>> {reverse (st)};
    }

    template <ordered value, functional::buildable_tree<value> tree>
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

    template <ordered value, functional::buildable_tree<value> tree>
    const unref<value> inline *binary_search_tree<value, tree>::contains (const value &v) const {
        return functional::contains (*this, v);
    }

    template <ordered value, functional::buildable_tree<value> tree>
    binary_search_tree<value, tree> inline binary_search_tree<value, tree>::insert (const value &v) const {
        return functional::insert (static_cast<const tree &> (*this), v);
    }


    template <ordered value, functional::buildable_tree<value> tree>
    binary_search_tree<value, tree>::iterator inline binary_search_tree<value, tree>::begin () const {
        return iterator {this, *this};
    }

    template <ordered value, functional::buildable_tree<value> tree>
    binary_search_tree<value, tree>::iterator inline binary_search_tree<value, tree>::end () const {
        return iterator {this, tree {}};
    }

    template <ordered value, functional::buildable_tree<value> tree>
    template <ordered X, functional::buildable_tree<value> T>
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

}

#endif

