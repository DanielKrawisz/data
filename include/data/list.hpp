// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_LIST
#define DATA_LIST

#include <type_traits>
#include "types.hpp"

namespace data {
        
    namespace list {

        namespace definition {
                
            template <typename L>
            struct existence {
                bool empty(const L l) const {
                    return l.empty();
                }
                    
                uint size(const L l) const {
                    return l.size();
                }
            };

            template <typename L>
            struct existence<L*> {
                bool empty(const L* l) const {
                    return l == nullptr;
                }

                uint size(const L* l) const {
                    if (l == nullptr) return 0;
                    return l->size();
                }
            };

            template <typename L>
            struct existence<ptr<L>> : virtual public existence<L*> {};

            template <typename L>
            struct retractable : public existence<L> {
                L rest(const L l) const {
                    return l.rest();
                }
            };
                
            template <typename L>
            struct retractable<L*> : virtual public existence<L*> {
                L rest(const L* l) const {
                    if (l == nullptr) return nullptr;
                    return l->rest();
                }
            };
                
            template <typename L>
            struct retractable<ptr<L>> : virtual public existence<ptr<L>>, virtual public retractable<L*> {};

            template <typename L, typename X>
            struct list : public retractable<L> {
                X first(L l) {
                    return l.first();
                }
            };
                
            template <typename L, typename X>
            struct list<L*, X> : virtual public retractable<L*> {
                X first(L* l) {
                    return l->First;
                }
            };
                
            template <typename L, typename X>
            struct list<ptr<L>, X> : virtual public retractable<ptr<L>>, virtual public list<L*, X> {};
                
            template <typename L, typename X>
            struct extendable : public existence<L> {
                L make_empty() const {
                    return {};
                }
                
                L prepend(L l, X x) const {
                    return l + x;
                }
            };
                
            template <typename L, typename X>
            struct extendable<L*, X> : virtual public existence<L*> {
                L prepend(const L* l, X x) const {
                    if (l == nullptr) return x;
                    return l->append(x);
                }
            };
                
            template <typename L, typename X>
            struct extendable<ptr<L>, X> : virtual public extendable<L*, X> {};

            template <typename L, typename X, typename it>
            struct iterable : public list<L, X> {
                it begin(L l) {
                    return l.begin();
                }
                
                it end(L l) {
                    return l.end();
                }
            };
                
            template <typename L, typename X, typename it>
            struct iterable<L*, X, it> : virtual public list<L*, X> {
                it begin(L* l) {
                    if (l == nullptr) return it{};
                    return l->begin();
                }
                
                it end(L* l) {
                    if (l == nullptr) return it{};
                    return l->end();
                }
            };
                
            template <typename L, typename X, typename it>
            struct iterable<ptr<L>, X, it> : public list<ptr<L>, X>, public iterable<L*, X, it> {};
                
            template <typename L, typename X, typename it>
            struct complete : public extendable<L, X>, public iterable<L, X, it> {};
            
            template <typename L, typename X, typename it>
            struct complete<L*, X, it> : virtual public extendable<L*, X>, public iterable<L*, X, it> {};
            
            template <typename L, typename X, typename it>
            struct complete<ptr<L>, X, it> : complete<L*, X, it>, public extendable<ptr<L>, X>, public iterable<ptr<L>, X, it> {};
            
        }
            
        template <typename L>
        struct is_list {
            using element = typename std::remove_reference<typename std::invoke_result<decltype(&L::first), L>::type>::type;
            constexpr static definition::list<L, element> IsList{};
        };
        
        template <typename L>
        struct is_list<L*> {
            using element = typename std::remove_reference<typename std::invoke_result<decltype(&L::first), L>::type>::type;
            constexpr static definition::list<L, element> IsList{};
        };
        
        template <typename L>
        struct is_list<ptr<L>> : public is_list<L*> {
            using element = typename is_list<L*>::element;
        };
            
        template <typename L>
        struct is_buildable : public is_list<L> {
            constexpr static definition::extendable<L, typename is_list<L>::element> IsExtendableList{};
        };
        
        template <typename L>
        struct is_iterable : public is_list<L> {
            using iterator = typename std::invoke_result<typename L::first>::type;
            constexpr static definition::iterable<L, typename is_list<L>::Element, iterator> IsIterableList{};
        };
            
        template <typename L> 
        inline bool empty(L l) {
            return definition::existence<L>{}.empty(l);
        }
            
        template <typename L> 
        inline uint size(L l) {
            return definition::existence<L>{}.size(l);
        }
            
        template <typename L> 
        inline typename is_list<L>::element& first(L l) {
            return definition::list<L, typename is_list<L>::element>{}.first(l);
        }
            
        template <typename L> 
        inline L rest(L l) {
            return definition::retractable<L>{}.rest(l);
        }
        
        template <typename L, typename X> 
        inline L prepend(L list, X value) {
            return definition::extendable<L, X>{}.prepend(list, value);
        }
        
        template <typename L, typename X>
        L append(L list, X value) {
            if (empty(list)) return L{{value}};
            
            return append(first(list), append(rest(list), value));
        }
        
        template <typename L>
        L reverse(L list) {
            using requirement = is_buildable<L>;
            constexpr static requirement satisfied{};
            
            struct inner {
                L operator()(L reversed, L list) {
                    if (empty(list)) return reversed;
                    return inner{}(reversed + first(list), rest(list));
                }
            };
            
            return inner{}(L{}, list);
        }

        template <typename L>
        L from(L l, uint n) {
            if (n > l.size()) return {};
            L o = l;
            for (int i = 0; i < n; i++) o = rest(o);
            return o;
        }
            
        template <typename function, typename list, typename value>
        value find(function satisfies, list l) {
            if (empty(l)) return value{};
            auto f0 = first(l);
            if (satisfies(f0)) return f0;
            return rest(l);
        }
            
        template <typename times, typename L1, typename L2, typename plus, typename value>
        value inner(times t, L1 l1, L2 l2, plus p) {
            uint size = l1.size();
            if (size != l2.size()) return value{};
            if (size == 0) return value{};
                
            return p(t(l1.first(), l2.first()), inner(t, rest(l1), rest(l2), p));
        }
        
        // this is an iterator that could go with a list. 
        template <typename L, typename E>
        struct iterator {
            L List;
                    
            iterator& operator=(iterator i) {
                this->List = i.List;
                return *this;
            }
                    
            iterator operator++(int i) { // Postfix
                iterator n = *this;
                operator=(iterator{rest(List)});
                return n;
            }

            iterator& operator++() { // Prefix
                return operator=(iterator{rest(List)});
            }

            const E operator*() const{
                return first(List);
            }
     
            bool operator==(const iterator i) const {
                return List == i.List;
            }
     
            bool operator!=(const iterator i) const {
                return List != i.List;
            }
                
        };
            
        // This is a node that you could perhaps use to make a list. 
        template <typename X, typename Y>
        struct node {
            X First;
            Y Rest;
            uint Size;
                    
            node(X x, Y r) : First(x), Rest(r), Size{size(r) + 1} {}
            node(X x) : First(x), Rest{}, Size{1} {}
                    
            X first() const {
                return First;
            }
                    
            Y rest() const {
                return Rest;
            }
                        
            bool contains(X x) const {
                if (x == First) return true;
                            
                return contains(Rest, x);
            }
                    
            bool operator==(const node& n) const {
                return First == n.First && Rest == n.Rest;
            }
                        
        };
    
    }

}

#endif
