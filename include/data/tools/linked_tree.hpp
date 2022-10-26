// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TREE_LINKED
#define DATA_TREE_LINKED

#include <data/functional/tree.hpp>
    
namespace data {

    template <typename value>
    struct linked_tree {
        
        using node = functional::tree_node<value, linked_tree>;
        using next = ptr<node>;
        
        next Node;
        size_t Size;
        
        bool empty() const;
        
        const value &root() const;
        value &root();
        
        linked_tree left() const;
        linked_tree right() const;
        
        bool contains(const value& v) const;
        size_t size() const;
        
        bool valid() const {
            return data::valid(root()) && left().valid() && right().valid();
        }
        
        linked_tree();
        linked_tree(const value& v, linked_tree l, linked_tree r);
        linked_tree(const value& v);
        linked_tree(const linked_tree& t);
        
        linked_tree& operator=(const linked_tree& t);
        
        template <typename X> requires std::equality_comparable_with<value, X>
        bool operator==(const data::linked_tree<X>& x) const {
            if (Node == x.Node) return true;
            if (Node == nullptr || x.Node == nullptr) return false;
            if (root() != x.root()) return false;
            if (left() != x.left()) return false;
            return right() == x.right();
        }
        
        using iterator = functional::tree_iterator<linked_tree>;
        using sentinel = data::sentinel<linked_tree>;
        
        iterator begin() const;
        sentinel end() const;
        
        std::ostream& write(std::ostream& o) const;
    };

    template <typename X> 
    inline std::ostream& operator<<(std::ostream& o, const linked_tree<X>& x) {
        return x.write(o << "tree");
    }
    
    template <typename value>
    inline bool linked_tree<value>::empty() const {
        return Node == nullptr;
    }
    
    template <typename value>
    inline const value &linked_tree<value>::root() const {
        return Node->Value;
    }
    
    template <typename value>
    inline value &linked_tree<value>::root() {
        return Node->Value;
    }
    
    template <typename value>
    inline linked_tree<value> linked_tree<value>::left() const {
        return Node == nullptr ? linked_tree{} : Node->Left;
    } 
    
    template <typename value>
    inline linked_tree<value> linked_tree<value>::right() const {
        return Node == nullptr ? linked_tree{} : Node->Right;
    }
    
    template <typename value>
    bool linked_tree<value>::contains(const value& v) const {
        if (Node == nullptr) return false;
        if (Node->Value == v) return true;
        if (Node->Left.contains(v)) return true;
        return Node->Right.contains(v);
    }
    
    template <typename value>
    inline size_t linked_tree<value>::size() const {
        return Size;
    }
    
    template <typename value>
    inline linked_tree<value>::linked_tree() : Node{nullptr}, Size{0} {}
    
    template <typename value>
    inline linked_tree<value>::linked_tree(const value& v, linked_tree l, linked_tree r) : 
        Node{std::make_shared<node>(v, l, r)}, Size{1 + l.size() + r.size()} {}
    
    template <typename value>
    inline linked_tree<value>::linked_tree(const value& v) : linked_tree{v, linked_tree{}, linked_tree{}} {}
    
    template <typename value>
    inline linked_tree<value>::iterator linked_tree<value>::begin() const {
        return iterator{*this};
    } 
    
    template <typename value>
    inline linked_tree<value>::sentinel linked_tree<value>::end() const {
        return sentinel{*this};
    }
    
    template <typename value>
    inline linked_tree<value>::linked_tree(const linked_tree& t) {
        Node = t.Node;
        Size = t.Size;
    }
    
    template <typename value>
    inline linked_tree<value>& linked_tree<value>::operator=(const linked_tree& t) {
        Node = t.Node;
        Size = t.Size;
        return *this;
    }
    
    template <typename value>
    std::ostream& linked_tree<value>::write(std::ostream& o) const {
        if (Size == 0) return o << "{}";
        if (Size == 1) return o << "{" << root() << "}";
        return right().write(left().write(o << "{" << root() << ", ") << ", ") << "}";
    }
}

#endif
