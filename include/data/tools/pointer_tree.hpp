#ifndef DATA_TOOLS_POINTER_TREE_HPP
#define DATA_TOOLS_POINTER_TREE_HPP

#include <data/tree.hpp>
#include <data/list/linked_list.hpp>
    
namespace data {

    template <typename value>
    struct pointer_tree final {
        value Value;
        pointer_tree* Left;
        pointer_tree* Right;
            
        value& root() const {
            return Value;
        }
            
        pointer_tree* left() const {
            return Left;
        } 
            
        pointer_tree* right() const {
            return Right;
        }
            
        ~pointer_tree() {
            delete Left;
            delete Right;
        }
            
        pointer_tree(const pointer_tree& n) : Value{n.Value}, Left{new pointer_tree{*Left}}, Right{new pointer_tree{*Right}} {}
            
        pointer_tree(pointer_tree&& n) {
            Value = n.Value;
            Left = n.Left;
            Right = n.Right;
            n.Left = nullptr;
            n.Right = nullptr;
        }
        
        pointer_tree(value v, pointer_tree* l, pointer_tree* r) : Value{v}, Left{l}, Right{r} {}
            
        using iterator = tree::iterator<pointer_tree*, linked_list<pointer_tree*&>, value>;
            
        iterator begin() const {
            return {*this};
        } 

        iterator end() const {
            return {};
        }
            
        constexpr static tree::definition::buildable<pointer_tree*> is_tree{};
    };

}

#endif
