// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_LIST
#define DATA_LIST

#include <type_traits>
#include <data/meta/which.hpp>
#include "container.hpp"

namespace data::list {
    namespace definition {
    
        template <typename L>
        struct retractable : public container::existence<L> {
            L rest(const L l) const {
                return l.rest();
            }
        };
        
        template <typename L>
        struct retractable<L*> : virtual public container::existence<L*> {
            L rest(const L* l) const {
                if (l == nullptr) return nullptr;
                return l->rest();
            }
        };
        
        template <typename L>
        struct retractable<ptr<L>> : 
            virtual public container::existence<ptr<L>>, 
            virtual public retractable<L*> {};
        
        template <typename L, typename R>
        struct list : public retractable<L> {
            R first(const L l) const {
                return l.first();
            }
        };
        
        template <typename L, typename R>
        struct list<L*, R> : virtual public retractable<L*> {
            R first(const L* const l) const {
                return l->first();
            }
        };
        
        template <typename L, typename R>
        struct list<ptr<L>, R> : virtual public retractable<ptr<L>>, virtual public list<L*, R> {};
        
        template <typename L, typename X>
        struct extendable : virtual public container::existence<L> {
            L make_empty() const {
                return {};
            }
            
            const L prepend(const L l, const X x) const {
                return l + x;
            }
        };
        
        template <typename L, typename X>
        struct extendable<L*, X> : virtual public container::existence<L*> {
            const L prepend(const L* const l, X x) const {
                if (l == nullptr) return x;
                return l->append(x);
            }
        };
        
        template <typename L, typename X>
        struct extendable<ptr<L>, X> : virtual public extendable<L*, X> {};
        
        template <typename L, typename X, typename R>
        struct buildable :
            virtual public list<L, R>, 
            public extendable<L, X> {};
        
        template <typename L, typename R, typename it, typename const_it>
        struct iterable : 
            virtual public list<L, R>, 
            public container::iterable<L, it, const_it> {};
        
        template <typename L, typename R, typename it, typename const_it>
        struct iterable<L*, R, it, const_it> : 
            virtual public list<L*, R>, 
            public container::iterable<L*, it, const_it> {};
        
        template <typename L, typename R, typename it, typename const_it>
        struct iterable<ptr<L>, R, it, const_it> : 
            virtual public list<ptr<L>, R>, 
            public container::iterable<ptr<L>, it, const_it> {};
            
        template <typename L, typename X, typename R, typename it, typename const_it>
        struct complete : public buildable<L, X, R>, public iterable<L, R, it, const_it> {};
        
        // cannot use references with iterators. 
        template <typename L, typename X, typename it, typename const_it>
        struct complete<L, X&, X&, it, const_it> : virtual public list<L*, X&> {
            complete() = delete;
        };
        
    }
    
    template <typename L>
    struct is_list {
        using returned = decltype(std::declval<L>().first());
        constexpr static definition::list<L, returned> is{};
    };
    
    template <typename L>
    struct is_list<L*> {
        using returned = decltype(std::declval<L>().first());
        constexpr static definition::list<L*, returned> IsList{};
    };
    
    template <typename L>
    struct is_list<ptr<L>> : public is_list<L*> {};
    
    template <typename L, typename X>
    struct is_buildable : public is_list<L> {
        using returned = typename is_list<L>::returned;
        constexpr static definition::buildable<L, X, returned> IsBuildableList{};
    };
    
    template <typename L>
    struct is_iterable : public is_list<L> {
        using iterator = decltype(std::declval<L>().begin());
        using const_iterator = decltype(std::declval<const L>().begin());
        constexpr static definition::iterable<L, typename is_list<L>::returned, iterator, const_iterator> IsIterableList{};
    };
    
    template <typename L> 
    inline bool empty(L l) {
        return container::existence<L>{}.empty(l);
    }
    
    template <typename L> 
    inline uint32 size(L l) {
        return container::existence<L>{}.size(l);
    }
    
    template <typename L> 
    inline typename is_list<L>::returned first(const L l) {
        return definition::list<L, typename is_list<L>::returned>{}.first(l);
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
        if (list::empty(list)) return L{{value}};
        
        return append(first(list), append(rest(list), value));
    }
    
    template <typename L>
    L reverse(L list) {
        using requirement = definition::extendable<L, typename is_list<L>::returned>;
        constexpr static requirement satisfied{};
        
        struct inner {
            L operator()(L reversed, L list) {
                if (list::empty(list)) return reversed;
                return inner{}(reversed + first(list), rest(list));
            }
        };
        
        return inner{}(L{}, list);
    }
    
    template <typename L>
    L from(L l, uint32 n) {
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
        uint32 size = l1.size();
        if (size != l2.size()) return value{};
        if (size == 0) return value{};
        
        return p(t(l1.first(), l2.first()), inner(t, rest(l1), rest(l2), p));
    }
    
    // this is a const iterator that could go with a list. 
    template <typename L>
    struct iterator {
        L List;
        
        using requirement = is_iterable<L>;
        using returned = typename requirement::returned;
        constexpr static requirement Satisfied{};
        
        iterator& operator=(iterator i) {
            this->List = i.List;
            return *this;
        }
        
        iterator operator++(int) { // Postfix
            iterator n = *this;
            operator=(iterator{rest(List)});
            return n;
        }
        
        iterator& operator++() { // Prefix
            return operator=(iterator{rest(List)});
        }
        
        const returned operator*() const {
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
    template <typename X, typename Y, typename R>
    struct node {
        X First;
        Y Rest;
        uint32 Size;
        
        node(X x, Y r) : First(x), Rest(r), Size{list::size(r) + 1} {}
        node(X x) : First(x), Rest{}, Size{1} {}
        
        const R first() const {
            return First;
        }
        
        R first() {
            return First;
        }
        
        Y rest() const {
            return Rest;
        }
        
        uint32 size() const {
            return Size;
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

#endif
