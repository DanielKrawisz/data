// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TREE_LINKED
#define DATA_TREE_LINKED

#include <data/functional/tree.hpp>
    
namespace data {

    template <Element value> struct linked_tree;
    
    template <typename value> bool empty (linked_tree<value> x);
    template <typename value> size_t size (linked_tree<value> x);

    template <Element value> struct linked_tree {
        using node = functional::tree_node<value, linked_tree>;
        using next = ptr<node>;

        bool empty () const;
        
        const value &root () const;
        value &root ();

        linked_tree left () const;
        linked_tree right () const;
        
        bool contains (inserted<value> v) const;
        size_t size () const;
        
        bool valid () const {
            return Size == 0 || (data::valid (root ()) && left ().valid () && right ().valid ());
        }
        
        linked_tree ();
        linked_tree (inserted<value> v, linked_tree l, linked_tree r);
        linked_tree (inserted<value> v);
        
        template <typename X> requires std::equality_comparable_with<value, X>
        bool operator == (const data::linked_tree<X> &x) const;

        template <typename X> requires ImplicitlyConvertible<value, X>
        operator linked_tree<X> () const;

        template <typename X> requires ExplicitlyConvertible<value, X>
        explicit operator linked_tree<X> () const;
        
        std::ostream &write (std::ostream &o) const;

        linked_tree for_each (function<value (inserted<value>)> f) const {
            if (empty ()) return *this;
            auto r = root ();
            value mutated = f (r);
            if (mutated != r) return linked_tree {mutated, left ().for_each (f), right ().for_each (f)};
            linked_tree ll = left ();
            linked_tree mutated_left = ll.for_each (f);
            if (ll.Node != mutated_left.Node) return linked_tree {r, mutated_left, right ().for_each (f)};
            linked_tree rr = right ();
            linked_tree mutated_right = rr.for_each (f);
            if (rr.Node != mutated_right.Node) return linked_tree {r, ll, mutated_right};
            return *this;
        }

    private:
        next Node;
        size_t Size;
    };

    template <typename value> 
    inline std::ostream &operator << (std::ostream &o, const linked_tree<value> &x) {
        return x.write (o << "tree ");
    }

    template <typename value> 
    bool inline empty (linked_tree<value> x) {
        return x.empty ();
    }

    template <typename value> 
    size_t inline size (linked_tree<value> x) {
        return x.size ();
    }
    
    template <Element value>
    inline bool linked_tree<value>::empty () const {
        return Node == nullptr;
    }
    
    template <Element value>
    const value inline &linked_tree<value>::root () const {
        return Node->Value;
    }

    template <Element value>
    value inline &linked_tree<value>::root () {
        return Node->Value;
    }
    
    template <Element value>
    inline linked_tree<value> linked_tree<value>::left () const {
        return Node == nullptr ? linked_tree {} : Node->Left;
    } 
    
    template <Element value>
    inline linked_tree<value> linked_tree<value>::right () const {
        return Node == nullptr ? linked_tree {} : Node->Right;
    }
    
    template <Element value>
    bool linked_tree<value>::contains (inserted<value> v) const {
        if (Node == nullptr) return false;
        if (Node->Value == v) return true;
        if (Node->Left.contains (v)) return true;
        return Node->Right.contains (v);
    }
    
    template <Element value>
    inline size_t linked_tree<value>::size () const {
        return Size;
    }
    
    template <Element value>
    inline linked_tree<value>::linked_tree () : Node {nullptr}, Size {0} {}
    
    template <Element value>
    inline linked_tree<value>::linked_tree (inserted<value> v, linked_tree l, linked_tree r) :
        Node {std::make_shared<node> (v, l, r)}, Size {1 + l.size () + r.size ()} {}
    
    template <Element value>
    inline linked_tree<value>::linked_tree (inserted<value> v) : linked_tree {v, linked_tree {}, linked_tree {}} {}
    
    template <Element value>
    std::ostream &linked_tree<value>::write (std::ostream &o) const {
        if (Size == 0) return o << "{}";
        if (Size == 1) return o << "{" << root () << "}";
        return right ().write (left ().write (o << "{" << root () << ", ") << ", ") << "}";
    }

    template <Element value>
    template <typename X> requires std::equality_comparable_with<value, X>
    bool linked_tree<value>::operator == (const data::linked_tree<X> &x) const {
        if (Node == x.Node) return true;
        if (Node == nullptr || x.Node == nullptr) return false;
        if (root () != x.root ()) return false;
        if (left () != x.left ()) return false;
        return right () == x.right ();
    }

    template <Element value>
    template <typename X> requires ImplicitlyConvertible<value, X>
    inline linked_tree<value>::operator linked_tree<X> () const {
        return empty () ? linked_tree <X> {} : linked_tree<X> {X (root ()), linked_tree<X> {left ()}, linked_tree<X> {right ()}};
    }

    template <Element value>
    template <typename X> requires ExplicitlyConvertible<value, X>
    inline linked_tree<value>::operator linked_tree<X> () const {
        return empty () ? linked_tree <X> {} : linked_tree<X> {X (root ()), linked_tree<X> {left ()}, linked_tree<X> {right ()}};
    }

}

#endif
