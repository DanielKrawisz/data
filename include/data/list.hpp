#ifndef DATA_LIST_HPP
#define DATA_LIST_HPP

#include "types.hpp"

namespace data {
        
    namespace list {

        namespace definition {
                
            template <typename L>
            struct existence {
                bool empty(L l) const {
                        return l.empty();
                }
                    
                N size(L l) const {
                    return l.size();
                }
            };

            template <typename L>
            struct existence<L*> {
                bool empty(L l) const {
                    return l == nullptr;
                }

                N size(L l) const {
                    if (l == nullptr) return 0;
                    return l->size();
                }
            };

            template <typename L>
            struct existence<ptr<L>> {
                bool empty(L l) const {
                    return l == nullptr;
                }

                N size(L l) const {
                    if (l == nullptr) return 0;
                    return l->size();
                }
            };

            template <typename L, typename X>
            struct list : public existence<L> {
                X& first(L l) const {
                    return l.first();
                }
                    
                L rest(L l) const {
                    return l.rest();
                }
            };
                
            template <typename L, typename X>
            struct list<L*, X> : public existence<L*> {
                X& first(L l) const {
                    return l->First;
                }
                    
                L rest(L l) const {
                    if (l == nullptr) return nullptr;
                    return l->rest();
                }
            };
                
            template <typename L, typename X>
            struct list<ptr<L>, X> : public existence<ptr<L>> {
                X& first(L l) const {
                    return l->First;
                }
                    
                L rest(L l) const {
                    if (l == nullptr) return nullptr;
                    return l->rest();
                }
            };
                
            template <typename L, typename X>
            struct extendable : public list<L, X> {                
                L append(L l, X x) const {
                    return l + x;
                }
            };
                
            template <typename L, typename X>
            struct extendable<L*, X> : public list<L*, X> {
                L append(L l, X x) const {
                    if (l == nullptr) return x;
                    return l->append(x);
                }
            };
                
            template <typename L, typename X>
            struct extendable<ptr<L>, X> : public list<ptr<L>, X> {
                L append(L l, X x) const {
                    if (l == nullptr) return x;
                    return l->append(x);
                }
            };
                
            template <typename L, typename X, typename it>
            struct iterable : public list<L, X> {
                it begin(L l) const {
                    return l.begin();
                }
                
                it end(L l) const {
                    return l.end();
                }
            };
                
            template <typename L, typename X, typename it>
            struct iterable<L*, X, it> : public list<L*, X> {
                it begin(L l) const {
                    if (l == nullptr) return it{};
                    return l->begin();
                }
                
                it end(L l) const {
                    if (l == nullptr) return it{};
                    return l->end();
                }
            };
                
            template <typename L, typename X, typename it>
            struct iterable<ptr<L>, X, it> : public list<ptr<L>, X> {
                it begin(L l) const {
                    if (l == nullptr) return it{};
                    return l->begin();
                }
                
                it end(L l) const {
                    if (l == nullptr) return it{};
                    return l->end();
                }
            };
                
            template <typename L, typename X, typename it>
            struct complete : public extendable<L, X>, public iterable<L, X, it> {};
            
            template <typename L, typename X, typename it>
            struct complete<L*, X, it> : public extendable<L, X>, public iterable<L, X, it> {};
            
            template <typename L, typename X, typename it>
            struct complete<ptr<L>, X, it> : public extendable<L, X>, public iterable<L, X, it> {};
            
        }
            
        template <typename L> 
        inline bool empty(L l) {
            return definition::existence<L>{}.empty(l);
        }
            
        template <typename L> 
        inline bool size(L l) {
            return definition::existence<L>{}.size(l);
        }
            
        template <typename L, typename X> 
        inline X& first(L l) {
            return definition::list<L, X>{}.first(l);
        }
            
        template <typename L, typename X> 
        inline L rest(L l) {
            return definition::list<L, X>{}.rest(l);
        }

        template <typename L>
        L from(L l, N n) {
            if (n > l.size()) return {};
            L o = l;
            for (int i = 0; i < n; i++) o = rest(o);
            return o;
        }
        
        template<typename function, typename list, typename value>
        value reduce(function plus, list l) {
            if (empty(l)) return value{};
            
            return plus(reduce(plus, rest(l)), value{first(l)});
        }
            
        template<typename list, typename value>
        value reduce(list l) {
            if (empty(l)) return value{};
            
            return reduce(plus, rest(l)) + value{first(l)};
        }
                
        template <typename function, typename from, typename to> 
        to for_each(function f, from l) {
            if (empty(l)) return nullptr;
            return for_each(f, rest(l)) + f(*first(l));
        }
            
        template <typename function, typename from, typename to> 
        inline to for_each_indexed(N index, function f, from l) {
            if (empty(l)) return nullptr;
            return for_each_indexed(index + 1, f, rest(l)) + f(index, *first(l));
        }
            
        template <typename function, typename from, typename to> 
        inline to for_each_indexed(function f, from l) {
            return for_each_indexed(0, f, l);
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
            N size = l1.size();
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
                    
            iterator& operator++() {
                return operator=(iterator{rest(List)});
            }
                    
            E& operator*() {
                return first<L, E>(List);
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
                    
            node(X x, Y r) : First(x), Rest(r) {}
            node(X x) : First(x), Rest{} {}
                    
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
