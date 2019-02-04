#ifndef DATA_TREE_HPP
#define DATA_TREE_HPP

#include <data/types.hpp>
#include <data/list.hpp>
#include <data/set.hpp>
#include <data/list/linked_list.hpp>
#include <data/tools/iterator_list.hpp>
    
namespace data {

    namespace tree {
        
        namespace definition {
            
            template <typename T>
            struct tree : public set::definition::existence<T>, public is_iterable<T> {
                
                using element = typename is_iterable<T>::element;
                
                element& root(const T t) const {
                    return t.root();
                }
                
                T left(const T t) const {
                    return t.left();
                } 
                
                T right(const T t) const {
                    return t.right();
                }
                
            };
            
            template <typename T>
            struct tree<T*> : public set::definition::existence<T*>, public is_iterable<T*> {
                
                using element = typename is_iterable<T>::element;
                
                element& root(const T* t) const {
                    return t->root();
                }
                
                T* left(const T* t) const {
                    return t->left();
                } 
                
                T* right(const T* t) const {
                    return t->right();
                }
            };
            
            template <typename T>
            struct tree<ptr<T>> : virtual public tree<T*>, virtual public set::definition::existence<ptr<T>>, public is_iterable<ptr<T>> {
                using element = typename is_iterable<ptr<T>>::element;
            };
            
            template <typename T>
            struct buildable : public tree<T> {
                using element = typename tree<T>::element;
                T make(element e, T left, T right) {
                    return {e, left, right};
                }
            };
            
            template <typename T>
            struct buildable<T*> : virtual public tree<T*> {
                using element = typename tree<T*>::element;
                T* make(element e, T left, T right) {
                    return new T{e, left, right};
                };
            };
            
            template <typename T>
            struct buildable<ptr<T>> : public buildable<T*>, public tree<ptr<T>> {};
            
        }
        
        template <typename T>
        inline bool empty(T t) {
            return definition::tree<T>{}.empty(t);
        }
        
        template <typename T>
        inline typename definition::tree<T>::element& root(T t) {
            return definition::tree<T>{}.root(t);
        }
        
        template <typename T>
        inline T left(T t) {
            return definition::tree<T>{}.left(t);
        }
        
        template <typename T>
        inline T right(T t) {
            return definition::tree<T>{}.right(t);
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
