// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_PRIORITY_QUEUE
#define DATA_TOOLS_PRIORITY_QUEUE

#include <data/functional/tree.hpp>
#include <data/stack.hpp>
#include <data/tools/linked_tree.hpp>
#include <data/ordered_sequence.hpp>
#include <data/ordered.hpp>
    
namespace data::tool {
    
    template <functional::buildable_tree tree, Prioritized element> struct priority_queue;

    template <functional::buildable_tree tree, Prioritized element>
    bool empty (const priority_queue<tree, element> x);

    template <functional::buildable_tree tree, Prioritized element>
    size_t size (const priority_queue<tree, element> x);

    template <functional::buildable_tree tree, Prioritized element>
    priority_queue<tree, element> insert (const priority_queue<tree, element> x, inserted<element>);

    template <functional::buildable_tree tree, Prioritized element>
    ordered_sequence<element> values (const priority_queue<tree, element> pq);
    
    template <functional::buildable_tree tree, Prioritized element>
    priority_queue<tree, element> merge (const priority_queue<tree, element> a, const priority_queue<tree, element> b);

    template <functional::buildable_tree tree, Prioritized element>
    priority_queue<tree, element> operator & (const priority_queue<tree, element> a, const priority_queue<tree, element> b);

    template <functional::buildable_tree tree, Prioritized element>
    priority_queue<tree, element> operator | (const priority_queue<tree, element> a, const priority_queue<tree, element> b);

    template <functional::buildable_tree tree, Prioritized element>
    priority_queue<tree, element> operator ^ (const priority_queue<tree, element> a, const priority_queue<tree, element> b);

    template <functional::buildable_tree tree, Prioritized element>
    priority_queue<tree, element> operator << (const priority_queue<tree, element> p, inserted<element> elem);

    template <functional::buildable_tree tree, Prioritized element>
    priority_queue<tree, element> operator <<= (priority_queue<tree, element> &p, inserted<element> elem);
    
    template <functional::buildable_tree tree, Prioritized element>
    struct priority_queue : tree {
        
        priority_queue () : tree {} {}
        priority_queue (inserted<element> e);
        
        bool valid () const;
        
        const element &first () const;
        priority_queue rest () const;
        
        priority_queue insert (inserted<element> elem) const;

        priority_queue (std::initializer_list<wrapped<element>> init): priority_queue {} {
            for (int i = init.size () - 1; i >= 0; i--) *this = insert (*(init.begin () + i));
        }
        
        template <typename list> requires Sequence<list, element>
        priority_queue insert (list l) const;
        
        template <typename list> requires Sequence<list, element>
        priority_queue (list l);

        template <Sequence X> requires std::equality_comparable_with<element, decltype (std::declval<X> ().first ())>
        bool operator == (const X &x) const {
            return sequence_equal (*this, x);
        }

        priority_queue (tree t) : tree {t} {}

        static tree merge (const tree &left, const tree right) {
            if (left.empty ()) return right;
            if (right.empty ()) return left;
            if (left.root () <= right.root ())
                return tree {left.root (), left.left (), merge (left.right (), right)};
            else return tree {right.root (), right.left (), merge (left, right.right ())};
        }

        static bool contains (const tree &t, const inserted<element> e) {
            if (data::empty (t)) return false;
            const auto r = root (t);
            if (e == r) return true;
            if (e <= r) return false;
            if (contains (left (t), e)) return true;
            return contains (right (t), e);
        }

        bool contains (const inserted<element> e) const {
            return contains (static_cast<const tree &> (*this), e);
        }

        stack<const element &> values () const {
            stack<const element &> result;
            auto pq = *this;
            while (!pq.empty ()) {
                result <<= pq.first ();
                pq = pq.rest ();
            }
            return reverse (result);
        }
    };

    template <functional::buildable_tree tree, Prioritized element>
    size_t size (const priority_queue<tree, element> x) {
        return x.size ();
    }
    
    template <functional::buildable_tree tree, Prioritized element>
    priority_queue<tree, element> inline operator << (const priority_queue<tree, element> p, inserted<element> elem) {
        return p.insert (elem);
    }

    template <functional::buildable_tree tree, Prioritized element>
    priority_queue<tree, element> inline operator <<= (priority_queue<tree, element> &p, inserted<element> elem) {
        return p = p.insert (elem);
    }

    template <functional::buildable_tree tree, Prioritized element>
    ordered_sequence<element> values (const priority_queue<tree, element> pq) {
        stack<element> vals;

        while (!empty (pq)) {
            vals >>= first (pq);
            pq = rest (pq);
        }

        return reverse (vals);
    }
    
    template <functional::buildable_tree tree, Prioritized element>
    bool inline empty (const priority_queue<tree, element> x) {
        return x.empty ();
    }

    template <functional::buildable_tree tree, Prioritized element>
    priority_queue<tree, element> inline sort (const priority_queue<tree, element> x) {
        return x;
    }
    
    template <functional::buildable_tree tree, Prioritized element>
    priority_queue<tree, element> inline merge (const priority_queue<tree, element> a, const priority_queue<tree, element> b) {
        return priority_queue<tree, element>::merge (a, b);
    }

    template <functional::buildable_tree tree, Prioritized element>
    priority_queue<tree, element> inline operator & (const priority_queue<tree, element> a, const priority_queue<tree, element> b) {
        return merge (a, b);
    }
    
    template <functional::buildable_tree tree, typename element> requires Prioritized<element>
    inline priority_queue<tree, element>::priority_queue (inserted<element> e) : priority_queue {priority_queue {}.insert (e)} {}
    
    template <functional::buildable_tree tree, typename element> requires Prioritized<element>
    const element inline &priority_queue<tree, element>::first () const {
        if (this->empty ()) throw empty_sequence_exception {};
        return root (*this);
    }
    
    template <functional::buildable_tree tree, typename element> requires Prioritized<element>
    priority_queue<tree, element> priority_queue<tree, element>::rest () const {
        if (this->empty ()) return *this;
        return {merge (left (*this), right (*this))};
    }
    
    template <functional::buildable_tree tree, typename element> requires Prioritized<element>
    priority_queue<tree, element> inline priority_queue<tree, element>::insert (inserted<element> elem) const {
        return {merge (tree {elem}, static_cast<const tree &> (*this))};
    }
    
    template <functional::buildable_tree tree, typename element> requires Prioritized<element>
    template <typename list> requires Sequence<list, element>
    priority_queue<tree, element> priority_queue<tree, element>::insert (list l) const {
        if (l.empty ()) return *this;
        return insert (l.first ()).insert (l.rest ());
    }
    
    template <functional::buildable_tree tree, typename element> requires Prioritized<element>
    template <typename list> requires Sequence<list, element>
    inline priority_queue<tree, element>::priority_queue (list l) : priority_queue {priority_queue {}.insert (l)} {}
    
    template <functional::buildable_tree tree, typename element> requires Prioritized<element>
    bool inline priority_queue<tree, element>::valid () const {
        // TODO this is wrong. We also need to check that the tree is ordered.
        return static_cast<const tree &> (*this).valid ();
    }
    
}

#endif
