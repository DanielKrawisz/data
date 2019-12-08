// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_LIST
#define DATA_LIST

#include <type_traits>
#include <data/valid.hpp>
#include <data/iterable.hpp>

namespace data::meta {
    
    template <typename list, typename element>
    class has_list_constructor {
        template <typename X> static auto test(int) -> decltype((void)(X{std::declval<const element>(), std::declval<const X>()}), yes());
        template <typename> static no test(...);
    public:
        static constexpr bool value = std::is_same<decltype(test<list>(0)), yes>::value;
    };
}

namespace data::interface {
    
    // TODO need constructor. 
    // TODO no prepend method needed. 
    template <typename L>
    struct list : sequence<L> {
        using element = typename sequence<L>::element;
    private:
        using require_default_constructable = typename std::enable_if<std::is_default_constructible<L>::value, bool>::type;
        //using require_list_constructor = typename std::enable_if<meta::has_list_constructor<L, element>::value, bool>::type;
    }; 
}

namespace data::list {
    
    template <typename L>
    L reverse(L list) {
        constexpr static interface::list<L> Required{};
        
        struct inner {
            L operator()(L reversed, L list) {
                if (data::empty(list)) return reversed;
                return inner{}(reversed << first(list), rest(list));
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
        if (data::empty(l)) return value{};
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
        constexpr static interface::list<L> Requirement1{};
        constexpr static interface::iterable<L> Requirement2{};
        using element = typename interface::list<L>::element;
        
        L List;
        
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
        
        const element operator*() const {
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
        const X First;
        Y Rest;
        uint32 Size;
        
        node(X x, Y r) : First(x), Rest(r), Size{data::size(r) + 1} {}
        node(X x) : First(x), Rest{}, Size{1} {}
        
        const X& first() const {
            return First;
        }
        
        Y rest() const {
            return Rest;
        }
        
        bool valid() const {
            return data::valid(First) && data::valid(Rest);
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

    template <typename L> 
    std::ostream& write(std::ostream& o, L n) {
        o << string{"{"};
        if (!data::empty(n)) {
            o << n.first();
            L r = n.rest();
            while (!data::empty(r)) {
                o << string{", "} << r.first();
                r = r.rest();
            }
        } 
        o << string{"}"};
        return o;
    }

}

namespace data::meta {
    
    template <typename list>
    class has_reverse_method {
        template <typename X> static auto test(int) -> typename 
            std::enable_if<std::is_same<decltype(std::declval<const X>().reverse()), list>::value, yes>::type;
        template <typename> static no test(...);
    public:
        static constexpr bool value = std::is_same<decltype(test<list>(0)), yes>::value;
    };
    
    template <typename list>
    class has_sort_method {
        template <typename X> static auto test(int) -> typename 
            std::enable_if<std::is_same<decltype(std::declval<const X>().sort()), list>::value, yes>::type;
        template <typename> static no test(...);
    public:
        static constexpr bool value = std::is_same<decltype(test<list>(0)), yes>::value;
    };
    
    template <typename X, bool is_list, bool has_reverse_method> struct reverse {
        X operator()(const X& x){
            return x;
        }
    };
    
    template <typename X, bool has_reverse_method> struct reverse<X, true, has_reverse_method> {
        X operator()(const X& x){
            return list::reverse(x);
        }
    };
    
    template <typename X> struct reverse<X, true, true> {
        X operator()(const X& x){
            return x.reverse();
        }
    };
}

namespace data {
    
    // list doesn't need a prepend method. 
    template <typename L, typename X>
    inline L prepend(const L& l, const X& x) {
        return l.prepend(x);
    }
    
    template <typename X>
    inline X reverse(const X x) {
        return list::reverse(x);
    }
}

#endif
