// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TREE_LINKED
#define DATA_TREE_LINKED

#include <data/functional/tree.hpp>
    
namespace data {

    template <Copyable value> struct linked_tree;
    
    template <typename value> bool empty (linked_tree<value> x);
    template <typename value> size_t size (linked_tree<value> x);

    template <Copyable value> struct linked_tree {
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
    
    template <Copyable value>
    inline bool linked_tree<value>::empty () const {
        return Node == nullptr;
    }
    
    template <Copyable value>
    const value inline &linked_tree<value>::root () const {
        if (Node == nullptr) throw data::empty_sequence_exception {};
        return Node->Value;
    }

    template <Copyable value>
    value inline &linked_tree<value>::root () {
        if (Node == nullptr) throw data::empty_sequence_exception {};
        return Node->Value;
    }
    
    template <Copyable value>
    inline linked_tree<value> linked_tree<value>::left () const {
        return Node == nullptr ? linked_tree {} : Node->Left;
    } 
    
    template <Copyable value>
    inline linked_tree<value> linked_tree<value>::right () const {
        return Node == nullptr ? linked_tree {} : Node->Right;
    }
    
    template <Copyable value>
    bool linked_tree<value>::contains (inserted<value> v) const {
        if (Node == nullptr) return false;
        if (Node->Value == v) return true;
        if (Node->Left.contains (v)) return true;
        return Node->Right.contains (v);
    }
    
    template <Copyable value>
    inline size_t linked_tree<value>::size () const {
        return Size;
    }
    
    template <Copyable value>
    inline linked_tree<value>::linked_tree () : Node {nullptr}, Size {0} {}
    
    template <Copyable value>
    inline linked_tree<value>::linked_tree (inserted<value> v, linked_tree l, linked_tree r) :
        Node {std::make_shared<node> (v, l, r)}, Size {1 + l.size () + r.size ()} {}
    
    template <Copyable value>
    inline linked_tree<value>::linked_tree (inserted<value> v) : linked_tree {v, linked_tree {}, linked_tree {}} {}
    
    template <Copyable value>
    std::ostream &linked_tree<value>::write (std::ostream &o) const {
        if (Size == 0) return o << "{}";
        if (Size == 1) return o << "{" << root () << "}";
        return right ().write (left ().write (o << "{" << root () << ", ") << ", ") << "}";
    }

    template <Copyable value>
    template <typename X> requires std::equality_comparable_with<value, X>
    bool linked_tree<value>::operator == (const data::linked_tree<X> &x) const {
        if (Node == x.Node) return true;
        if (Node == nullptr || x.Node == nullptr) return false;
        if (root () != x.root ()) return false;
        if (left () != x.left ()) return false;
        return right () == x.right ();
    }

    template <Copyable value>
    template <typename X> requires ImplicitlyConvertible<value, X>
    inline linked_tree<value>::operator linked_tree<X> () const {
        return empty () ? linked_tree <X> {} : linked_tree<X> {X (root ()), linked_tree<X> {left ()}, linked_tree<X> {right ()}};
    }

    template <Copyable value>
    template <typename X> requires ExplicitlyConvertible<value, X>
    inline linked_tree<value>::operator linked_tree<X> () const {
        return empty () ? linked_tree <X> {} : linked_tree<X> {X (root ()), linked_tree<X> {left ()}, linked_tree<X> {right ()}};
    }

}

#endif
