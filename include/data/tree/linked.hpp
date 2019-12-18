// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TREE_LINKED
#define DATA_TREE_LINKED

#include <data/tree.hpp>
#include <data/list/linked.hpp>
#include <data/io/unimplemented.hpp>
    
namespace data::tree {

    template <typename value>
    struct linked {
        
        using node = ptr<tree::node<value, linked<value>>>;
        
        node Node;
        
        bool empty() const {
            return Node == nullptr;
        }
        
        const value& root() const {
            return Node->Value;
        }
        
        linked<value> left() const {
            return Node == nullptr ? linked{} : Node->Left;
        } 
        
        linked<value> right() const {
            return Node == nullptr ? linked{} : Node->Left;
        }
        
        bool contains(const value& v) const {
            if (Node == nullptr) return false;
            if (Node->Value == v) return true;
            if (Node->Left.contains(v)) return true;
            return Node->Right.contains(v);
        }
        
        uint32 size() const {
            return Node == nullptr ? 0 : 1 + left().size() + right().size();
        }
        
        linked(const linked<value>& n) : Node{n.Node} {}
        
        linked(linked<value>&& n) {
            node m = n.Node;
            n.Node = Node;
            Node = m;
        }
        
        linked() : Node{nullptr} {}
        linked(value v, linked<value> l, linked<value> r) : Node{new tree::node<value, linked<value>>{v, l, r}} {}
        
        bool operator==(const linked<value>& t) {
            if (Node == t.Node) return true;
            if (Node == nullptr || t.Node == nullptr) return false;
            return Node->operator==(*t.Node);
        }
        
        bool operator!=(const linked<value>& t) {
            return ! operator==(t);
        }
        
        list::linked<value> values() const {
            throw method::unimplemented{"linked tree values"};
        }
        
        using iterator = list::iterator<list::linked<value>>;
        
        iterator begin() const {
            return values().begin();
        } 
        
        iterator end() const {
            return iterator{};
        }
    };

}

#endif
