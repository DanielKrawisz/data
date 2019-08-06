// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TREE
#define DATA_TREE

#include <data/types.hpp>
#include <data/list.hpp>
#include <data/set.hpp>
#include <data/list/linked.hpp>
#include <data/tools/iterator_list.hpp>
    
namespace data {

    namespace tree {
        
        namespace definition {
            
            template <typename T, typename E>
            struct tree : public set::definition::existence<T> {                
                
                E& root(const T t) const {
                    return t.root();
                }
                
                T left(const T t) const {
                    return t.left();
                } 
                
                T right(const T t) const {
                    return t.right();
                }
                
            };
            
            template <typename T, typename E>
            struct tree<T*, E> : public set::definition::existence<T*> {
                
                E& root(const T* t) const {
                    return t->root();
                }
                
                T* left(const T* t) const {
                    return t->left();
                } 
                
                T* right(const T* t) const {
                    return t->right();
                }
            };
            
            template <typename T, typename E>
            struct tree<ptr<T>, E> : virtual public tree<T*, E>, virtual public set::definition::existence<ptr<T>> {
                using element = typename tree<T*, E>::element;
            };
            
            template <typename T, typename E>
            struct buildable : public tree<T, E> {
                T make(E e, T left, T right) {
                    return {e, left, right};
                }
            };
            
            template <typename T, typename E>
            struct buildable<T*, E> : virtual public tree<T*, E> {
                T* make(E e, T left, T right) {
                    return new T{e, left, right};
                };
            };
            
            template <typename T, typename E>
            struct buildable<ptr<T>, E> : public buildable<T*, E>, public tree<ptr<T>, E> {};
            
        }
        
        template <typename T>
        struct is_tree {
            using element = typename std::remove_reference<typename std::invoke_result<decltype(&T::root), T>::type>::type;
            constexpr static definition::tree<T, element> IsList{};
        };
        
        template <typename T>
        inline bool empty(T t) {
            return list::empty(t);
        }
        
        template <typename T, typename E>
        inline typename definition::tree<T, E>::element& root(T t) {
            return definition::tree<T, E>{}.root(t);
        }
        
        template <typename T, typename E>
        inline T left(T t) {
            return definition::tree<T, E>{}.left(t);
        }
        
        template <typename T, typename E>
        inline T right(T t) {
            return definition::tree<T, E>{}.right(t);
        }
        
        template <typename T, typename X>
        T insert(T t, X x) {
            if (empty(t)) return T{x, T{}, T{}};
            X& r = root(t);
            if (x == r) return t;
            if (x < r) return T{r, insert(left(t), x), right(t)};
            if (x > r) return T{r, left(t), insert(right(t))};
        }
        
        template <typename T, typename L, typename E>
        struct iterator {
            static T Invalid;
            
            T& Tree;
            L List;
            
            constexpr static list::definition::extendable<L, T&> is_list{};
            
            iterator() : Tree{Invalid}, List{} {}
            iterator(T& t) : Tree{t}, List{} {}
            iterator(T& t, L l) : Tree{t}, List{l} {}
            
            bool valid() const {
                return &Tree != &Invalid;
            }

            E& operator*() {
                return root(Tree);
            }

            iterator& operator=(iterator i) {
                List = i.List;
                Tree = i.Tree;
                return *this;
            }
            
            iterator next() {
                if (empty(left(Tree))) {
                    if (empty(right(Tree))) {
                        if (list::empty(List)) return {};
                        else return {list::first(List), list::rest(List)};
                    } else return {right(Tree), List};
                } else return {left(Tree), empty(right(Tree)) ? List : List + right(Tree)};
            }
                    
            iterator& operator++() {
                return operator=(next());
            }
     
            bool operator==(const iterator i) const {
                return Tree == i.Tree && List == i.List;
            }
     
            bool operator!=(const iterator i) const {
                return Tree != i.Tree || List != i.List;
            }
                
        };

        template <typename value, typename tree>
        struct node final {
            value Value;
            tree Left;
            tree Right;
            
            template <typename L>
            iterator<tree, L, value> begin() const {
                return {*this};
            } 
            
            template <typename L>
            iterator<tree, L, value> end() const {
                return {};
            }
            
        };
    
    }

}

#endif
