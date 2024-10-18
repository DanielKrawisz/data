// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_BINARY_SEARCH_TREE
#define DATA_BINARY_SEARCH_TREE

#include <data/tools/linked_tree.hpp>
#include <data/meta.hpp>

namespace data {

    template <ordered value, functional::buildable_tree<value> tree> struct binary_search_tree;

    template <ordered key, typename value, functional::search_tree<data::entry<const key, value>> tree>
    requires interface::has_insert_method<tree, data::entry<const key, value>>
    struct binary_search_map;

    // the two trees are considered to be equal if they have
    // the same elements even if they are not the same tree.
    template <ordered value, functional::buildable_tree<value> tree, ordered X, functional::buildable_tree<X> T>
    requires std::equality_comparable_with<value, X>
    bool operator == (const binary_search_tree<value, tree> &a, const data::binary_search_tree<X, T> &b);

    template <typename key, std::equality_comparable value, typename tree>
    bool operator == (const binary_search_map<key, value, tree> &a, const binary_search_map<key, value, tree> &b);

    template <ordered value, functional::buildable_tree<value> tree>
    std::ostream &operator << (std::ostream &, binary_search_tree<value, tree>);

    template <ordered key, typename value, functional::search_tree<data::entry<const key, value>> tree>
    requires interface::has_insert_method<tree, data::entry<const key, value>>
    std::ostream &operator << (std::ostream &, binary_search_map<key, value, tree>);

    // we use a buildable tree to create a search tree.
    template <ordered value, functional::buildable_tree<value> tree>
    struct binary_search_tree : tree {
        binary_search_tree ();
        binary_search_tree (std::initializer_list<wrapped<value>> x);
        binary_search_tree (const tree &);
        binary_search_tree (tree &&);

        binary_search_tree left () const;
        binary_search_tree right () const;

        static bool sorted (binary_search_tree);

        bool valid () const;

        const unref<value> *contains (inserted<value>) const;

        tool::ordered_stack<linked_stack<const value &>> values () const;

        using iterator = functional::binary_search_iterator<value, tree>;

        iterator begin () const;
        iterator end () const;

        // if we have equivalent elements in the tree, the default behavior is to keep the old one.
        // however, you could also use functional::replace_new.
        binary_search_tree insert (inserted<value>,
            function<inserted<value> (inserted<value>, inserted<value>)> = &functional::keep_old<value>) const;
    };

    // we can use the above tree as the search tree with
    // insert method that we need to make the binary search map.
    // we can also use the rb::tree defined in rb.hpp, which
    // uses a balanced insert method.
    template <ordered key, typename value, functional::search_tree<data::entry<const key, value>> tree>
    requires interface::has_insert_method<tree, data::entry<const key, value>>
    struct binary_search_map : tree {
        using entry = data::entry<const key, value>;

        bool valid () const;

        binary_search_map ();
        binary_search_map (std::initializer_list<entry>);
        binary_search_map (const tree &);
        binary_search_map (tree &&);

        inserted<value> operator [] (inserted<key>) const;

        const unref<value> *contains (inserted<key>) const;
        bool contains (const entry &e) const;

        binary_search_map insert (const entry &) const;

        binary_search_map operator << (const entry &) const;
        binary_search_map operator <<= (const entry &);

        // try to insert something and call a function if it already exists.
        // (so that the user can decide whether he wants to replace it or not or combine them or whatever)
        // if a key already exists, the default behavior is to throw an exception.
        binary_search_map insert (const key &k, const value &v,
            function<inserted<value> (inserted<value> old_v, inserted<value> new_v)> already_exists = &default_key_already_exists) const;

        binary_search_map replace (const value &a, const value &b) const;
        binary_search_map replace_part (const key &k, const value &v) const;
        binary_search_map replace_part (const key &k, function<value (const value &)>) const;

        binary_search_map remove (const key &k) const;

        tool::ordered_stack<linked_stack<const key &>> keys () const;

        tool::ordered_stack<linked_stack<const entry &>> values () const;

        template <typename X, typename T>
        requires implicitly_convertible_to<value, X>
        operator binary_search_map<key, X, T> () const;

        template <typename X, typename T>
        requires explicitly_convertible_to<value, X>
        explicit operator binary_search_map<key, X, T> () const;

        binary_search_map operator & (binary_search_map x) const;
        binary_search_map operator | (binary_search_map x) const;
        binary_search_map operator ^ (binary_search_map x) const;

        struct iterator : tree::iterator {
            using tree::iterator::iterator;

            using value_type = entry;

            // pre increment
            iterator &operator ++ ();

            // post increment
            iterator operator ++ (int);

            bool operator == (const iterator i) const;
        };

        iterator begin () const;
        iterator end () const;

        struct key_does_not_exist : std::logic_error {
            key_does_not_exist () : std::logic_error {"key does not exist"} {}
        };

        struct key_already_exists : std::logic_error {
            key_already_exists () : std::logic_error {"key already exists"} {}
        };

    private:
        static inserted<value> default_key_already_exists (inserted<value> old_v, inserted<value> new_v);

        static tree insert (const tree, inserted<key>, inserted<value>,
            function<inserted<value> (inserted<value> old_v, inserted<value> new_v)> already_exists = &default_key_already_exists);
    };

    template <ordered value, functional::buildable_tree<value> tree>
    inline binary_search_tree<value, tree>::binary_search_tree (): tree {} {}

    template <ordered key, typename value, functional::search_tree<data::entry<const key, value>> tree>
    requires interface::has_insert_method<tree, data::entry<const key, value>>
    inline binary_search_map<key, value, tree>::binary_search_map (): tree {} {}

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
    inline binary_search_tree<value, tree>::binary_search_tree (std::initializer_list<wrapped<value>> init) {
        for (const wrapped<value> z : init) *this = this->insert (z);
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
    const unref<value> inline *binary_search_tree<value, tree>::contains (inserted<value> v) const {
        return functional::contains (*this, v);
    }

    template <ordered key, typename value, functional::search_tree<data::entry<const key, value>> tree>
    requires interface::has_insert_method<tree, data::entry<const key, value>>
    const unref<value> inline *binary_search_map<key, value, tree>::contains (inserted<key> k) const {
        if (data::empty (*this)) return nullptr;
        const auto &e = data::root (*this);
        return e.Key == k ? &e.Value: k < e.Key ? data::left (*this).contains (k) : data::right (*this).contains (k);
    }

    template <ordered value, functional::buildable_tree<value> tree>
    binary_search_tree<value, tree> inline binary_search_tree<value, tree>::insert (inserted<value> v,
        function<inserted<value> (inserted<value>, inserted<value>)> already) const {
        return functional::insert (static_cast<const tree &> (*this), v, already);
    }


    template <ordered key, typename value, functional::search_tree<data::entry<const key, value>> tree>
    requires interface::has_insert_method<tree, data::entry<const key, value>>
    tree insert (const tree t, inserted<key> k, inserted<value> v,
        function<inserted<value> (inserted<value> old_v, inserted<value> new_v)> already_exists) {
        if (data::empty (t)) return tree {entry {k, v}, tree {}, tree {}};
        const auto &e = data::root (t);
        return k == e.Key ? tree {entry {k, already_exists (e.Value, v)}, data::left (t), data::right (t)}:
            k < e.Key ? tree {e, insert (data::left (t), k, v, already_exists), data::right (t)}:
                tree {e, data::left (t), insert (data::right (t), k, v, already_exists)};
    }

    template <ordered key, typename value, functional::search_tree<data::entry<const key, value>> tree>
    requires interface::has_insert_method<tree, data::entry<const key, value>>
    binary_search_map<key, value, tree> inline
    binary_search_map<key, value, tree>::insert (const key &k, const value &v,
        function<inserted<value> (inserted<value> old_v, inserted<value> new_v)> already_exists) const {
        return insert (*this, k, v, already_exists);
    }

    template <ordered value, functional::buildable_tree<value> tree>
    binary_search_tree<value, tree>::iterator inline binary_search_tree<value, tree>::begin () const {
        return iterator {this, *this};
    }

    template <ordered value, functional::buildable_tree<value> tree>
    binary_search_tree<value, tree>::iterator inline binary_search_tree<value, tree>::end () const {
        return iterator {this, tree {}};
    }

    template <ordered value, functional::buildable_tree<value> tree, ordered X, functional::buildable_tree<X> T>
    requires std::equality_comparable_with<value, X>
    bool operator == (const binary_search_tree<value, tree> &a, const data::binary_search_tree<X, T> &b) {
        if (a.size () != b.size ()) return false;
        auto bb = b.begin ();
        for (auto ab = a.begin (); ab != a.end (); ab++) if (*ab != *bb) return false;
        else bb++;
        return true;
    }

    template <ordered key, typename value, functional::search_tree<data::entry<const key, value>> tree>
    requires interface::has_insert_method<tree, data::entry<const key, value>>
    inline binary_search_map<key, value, tree>::binary_search_map (std::initializer_list<entry> init) : tree {} {
        for (const auto &p : init) *this = insert (p);
    }

    template <typename key, std::equality_comparable value, typename tree>
    bool operator == (const binary_search_map<key, value, tree> &a, const binary_search_map<key, value, tree> &b) {
        if (a.size () != b.size ()) return false;
        auto bi = b.begin ();
        for (auto ai = a.begin (); ai != a.end (); ai++) if (ai->Key != bi->Key || ai->Value != bi->Value) return false;
        else bi++;
        return true;
    }

    template <ordered key, typename value, functional::search_tree<data::entry<const key, value>> tree>
    requires interface::has_insert_method<tree, data::entry<const key, value>>
    bool inline binary_search_map<key, value, tree>::contains (const data::entry<const key, value> &e) const {
        const value *v = contains (e.Key);
        return bool (v) ? *v == e.Value : false;
    }

    template <ordered key, typename value, functional::search_tree<data::entry<const key, value>> tree>
    requires interface::has_insert_method<tree, data::entry<const key, value>>
    template <typename X, typename T> requires implicitly_convertible_to<value, X>
    binary_search_map<key, value, tree>::operator binary_search_map<key, X, T> () const {
        binary_search_map<key, X, T> m;
        for (const auto &e : *this) m = m.insert (e);
        return m;
    }

    template <ordered key, typename value, functional::search_tree<data::entry<const key, value>> tree>
    requires interface::has_insert_method<tree, data::entry<const key, value>>
    template <typename X, typename T> requires explicitly_convertible_to<value, X>
    binary_search_map<key, value, tree>::operator binary_search_map<key, X, T> () const {
        binary_search_map<key, X, T> m;
        for (const auto &e : *this) m = m.insert (e);
        return m;
    }

    template <ordered key, typename value, functional::search_tree<data::entry<const key, value>> tree>
    requires interface::has_insert_method<tree, data::entry<const key, value>>
    binary_search_map<key, value, tree> binary_search_map<key, value, tree>::operator & (binary_search_map x) const {
        auto m = *this;
        for (const auto &e : x) m = m.insert (e);
        return m;
    }

    template <ordered key, typename value, functional::search_tree<data::entry<const key, value>> tree>
    requires interface::has_insert_method<tree, data::entry<const key, value>>
    inserted<value> inline binary_search_map<key, value, tree>::default_key_already_exists (inserted<value>, inserted<value>) {
        throw key_already_exists {};
    }

    template <ordered key, typename value, functional::search_tree<data::entry<const key, value>> tree>
    requires interface::has_insert_method<tree, data::entry<const key, value>>
    tool::ordered_stack<linked_stack<const key &>>
    binary_search_map<key, value, tree>::keys () const {
        linked_stack<const key &> kk {};

        for (const entry &e : *this) kk <<= e.Key;

        tool::ordered_stack<linked_stack<const key &>> x {};
        for (const auto &k : data::reverse (kk)) x = x << k;
        return x;
    }

    template <ordered key, typename value, functional::search_tree<data::entry<const key, value>> tree>
    requires interface::has_insert_method<tree, data::entry<const key, value>>
    tool::ordered_stack<linked_stack<const data::entry<const key, value> &>>
    binary_search_map<key, value, tree>::values () const {
        linked_stack<const entry &> kk {};

        for (const entry &e : *this) kk <<= e;

        tool::ordered_stack<linked_stack<const entry &>> x {};
        for (const auto &e : data::reverse (kk)) x = x << e;
        return x;
    }

    template <ordered key, typename value, functional::search_tree<data::entry<const key, value>> tree>
    requires interface::has_insert_method<tree, data::entry<const key, value>>
    binary_search_map<key, value, tree>::iterator inline
    binary_search_map<key, value, tree>::begin () const {
        return iterator {this, *this};
    }

    template <ordered key, typename value, functional::search_tree<data::entry<const key, value>> tree>
    requires interface::has_insert_method<tree, data::entry<const key, value>>
    binary_search_map<key, value, tree>::iterator inline
    binary_search_map<key, value, tree>::end () const {
        return iterator {this, tree {}};
    }

    template <ordered key, typename value, functional::search_tree<data::entry<const key, value>> tree>
    requires interface::has_insert_method<tree, data::entry<const key, value>>
    inserted<value> binary_search_map<key, value, tree>::operator [] (inserted<key> k) const {
        const auto *v = contains (k);
        if (!bool (v)) throw key_does_not_exist {};
        return *v;
    }

    template <ordered value, functional::buildable_tree<value> tree>
    std::ostream &operator << (std::ostream &o, binary_search_tree<value, tree> t) {
        o << "{";

        auto i = t.begin ();
        if (i != t.end ()) {
            o << *i;

            while (true) {
                i++;
                if (i == t.end ()) break;
                o << ", " << *i;
            }
        }

        return o << "{";
    }

    template <ordered key, typename value, functional::search_tree<data::entry<const key, value>> tree>
    requires interface::has_insert_method<tree, data::entry<const key, value>>
    std::ostream &operator << (std::ostream &o, binary_search_map<key, value, tree> m) {
        o << "{";

        auto i = m.begin ();
        if (i != m.end ()) {
            o << *i;

            while (true) {
                i++;
                if (i == m.end ()) break;
                o << ", " << *i;
            }
        }

        return o << "{";
    }


    template <ordered key, typename value, functional::search_tree<data::entry<const key, value>> tree>
    requires interface::has_insert_method<tree, data::entry<const key, value>>
    binary_search_map<key, value, tree>::iterator inline binary_search_map<key, value, tree>::iterator::operator ++ (int) {
        auto x = *this;
        ++(*this);
        return x;
    }

    template <ordered key, typename value, functional::search_tree<data::entry<const key, value>> tree>
    requires interface::has_insert_method<tree, data::entry<const key, value>>
    binary_search_map<key, value, tree>::iterator inline &binary_search_map<key, value, tree>::iterator::operator ++ () {
        ++static_cast<tree::iterator &> (*this);
        return *this;
    }

    template <ordered key, typename value, functional::search_tree<data::entry<const key, value>> tree>
    requires interface::has_insert_method<tree, data::entry<const key, value>>
    bool inline binary_search_map<key, value, tree>::iterator::operator == (const iterator i) const {
        return static_cast<const tree::iterator &> (*this) == static_cast<const tree::iterator &> (i);
    }
}

#endif

