#ifndef DATA_TREE_LINKED_TREE_HPP
#define DATA_TREE_LINKED_TREE_HPP

#include <data/tree.hpp>
    
namespace data {

    template <typename value>
    struct linked_tree {
        constexpr static tree::definition::buildable<linked_tree<value>, value> is_tree{};
        
        using node = ptr<tree::node<value, linked_tree<value>>>;
        
        node Node;
        
        bool empty() const {
            return Node == nullptr;
        }
            
        value& root() const {
            return Node->Value;
        }
            
        linked_tree<value> left() const {
            return Node->Left;
        } 
            
        linked_tree<value> right() const {
            return Node->Right;
        }
            
        linked_tree(const linked_tree<value>& n) : Node{n.Node} {}
            
        linked_tree(linked_tree<value>&& n) {
            node m = n.Node;
            n.Node = Node;
            Node = m;
        }
        
        linked_tree() : Node{nullptr} {}
        linked_tree(value v, linked_tree<value> l, linked_tree<value> r) : Node{new tree::node<value, linked_tree<value>>{v, l, r}} {}
            
        using iterator = tree::iterator<linked_tree<value>, linked_list<linked_tree<value>&>, value>;

        iterator begin() const {
            return {*this};
        } 

        iterator end() const {
            return {};
        }
        
        bool operator==(const linked_tree<value>& t) {
            if (Node == t.Node) return true;
            if (Node == nullptr || t.Node == nullptr) return false;
            return Node->operator==(*t.Node);
        }
        
        bool operator!=(const linked_tree<value>& t) {
            return ! operator==(t);
        }
    };

}

#endif
