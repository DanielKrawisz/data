// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TREE_LINKED
#define DATA_TREE_LINKED

#include <data/tree.hpp>
#include <data/tools/linked_stack.hpp>
#include <data/io/unimplemented.hpp>
    
namespace data::functional::tree {

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
        
        size_t size() const {
            return Node == nullptr ? 0 : 1 + left().size() + right().size();
        }
        
        linked() : Node{nullptr} {}
        linked(value v, linked<value> l, linked<value> r) : Node{new tree::node<value, linked>{v, l, r}} {}
        linked(value v) : linked{v, {}, {}} {}
        
        bool operator==(const linked& t) {
            if (Node == t.Node) return true;
            if (Node == nullptr || t.Node == nullptr) return false;
            return Node->operator==(*t.Node);
        }
        
        bool operator!=(const linked& t) {
            return ! operator==(t);
        }
        
        tool::linked_stack<value> values() const {
            throw method::unimplemented{"linked tree values"};
        }
        
        using iterator = stack::iterator<tool::linked_stack<value>>;
        
        iterator begin() const {
            return values().begin();
        } 
        
        iterator end() const {
            return iterator{};
        }
    };

}

#endif
