// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_FUNCTIONAL_STACK
#define DATA_FUNCTIONAL_STACK

// a stack is the most basic functional data structure. 

#include <type_traits>
#include <iterator>

#include <data/valid.hpp>
#include <data/sequence.hpp>
#include <data/iterable.hpp>
#include <data/function.hpp>

namespace data::interface {
    
    template <typename list, typename element>
    concept has_stack_constructor = requires (list x, element e) {
        { list{e, x} } -> std::same_as<list>;
    } && requires (element e) {
        { list{e} } -> std::same_as<list>;
    };
    
    template <typename list, typename element>
    concept has_prepend_method = requires (list x, element e) {
        { x.prepend(e) } -> std::convertible_to<list>;
    };
    
    template <typename list>
    concept has_reverse_method = requires (list x) {
        { x.reverse() } -> std::same_as<list>;
    };
    
}

namespace data::functional { 
    
    template <typename L, typename elem = std::remove_reference_t<decltype(std::declval<L>().first())>>
    concept stack = sequence<const L, elem> && interface::has_prepend_method<const L, elem> && 
        interface::has_stack_constructor<L, elem> && std::default_initializable<L>;
    
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
    
    // Requires element to be ordered. 
    // used in merge sort. 
    template <typename X> X merge(const X& a, const X& b) {
        X r{};
        while (!empty(a) && !empty(b)) {
            if (first(a) > first(b)) {
                r = r + first(a);
                b = rest(b);
            } else {
                r = r + first(b);
                b = rest(b);
            }
        }
        
        if (empty(a)) r = prepend(r, b);
        else if (empty(b)) r = prepend(r, a);
        return reverse(r);
    }
    
    template <stack L>
    L reverse(L list) {        
        struct inner {
            L operator()(L reversed, L list) {
                if (data::empty(list)) return reversed;
                return inner{}(reversed << first(list), rest(list));
            }
        };
        
        return inner{}(L{}, list);
    }
    
    // This is a node that you could perhaps use to make a list. 
    template <typename X, typename Y>
    struct stack_node {
        X First;
        Y Rest;
        size_t Size;
        
        stack_node(X x, Y r);
        stack_node(X x);
        
        X& first();
        const X& first() const;
        Y rest() const;
        
        bool valid() const;
        size_t size() const;
        bool contains(X x) const;
    };
    
    // an iterator that could go with a list. 
    template <typename list, typename element> 
    class stack_iterator {
        list Next;
        size_t Index;
        
        stack_iterator(list n, size_t i) : Next{n}, Index{i} {}
        
    public:
        stack_iterator() : Next{nullptr}, Index{0} {}
        
        stack_iterator& operator=(const stack_iterator& i);
        
        stack_iterator operator++(int);
        stack_iterator& operator++();
        
        element& operator*() const;
        
        bool operator==(const stack_iterator i) const;
        
        int operator-(const stack_iterator& i) const;
        
        stack_iterator(list n) : Next{n}, Index{0} {}
        stack_iterator(size_t i) : Next{}, Index{i} {}
    };

    template <stack X> 
    inline std::ostream& operator<<(std::ostream& o, const X& x) {
        return data::functional::write(o, x);
    }
}

namespace std {
    struct input_output_iterator_tag : input_iterator_tag, output_iterator_tag {};
    
    template <typename list, typename elem> 
    struct iterator_traits<data::functional::stack_iterator<list, elem>> {
        using value_type = remove_const_t<elem>;
        using difference_type = int;
        using pointer = remove_reference_t<elem>*;
        using reference = elem&;
        using iterator_concept = input_output_iterator_tag;
    };
    
    template <typename list, typename elem> 
    struct iterator_traits<data::functional::stack_iterator<list, const elem>> {
        using value_type = remove_const_t<elem>;
        using difference_type = int;
        using pointer = const remove_reference_t<elem>*;
        using reference = const elem&;
        using iterator_concept = input_iterator_tag;
    };
}

namespace data::meta {
    template <typename list> struct reverse;
    
    template <interface::has_reverse_method X> struct reverse<X> {
        X operator()(const X& x) {
            return x.reverse();
        }
    };
    
    template <typename X> requires interface::has_reverse_method<X> && functional::stack<X>
    struct reverse<X> {
        X operator()(const X& x) {
            return x.reverse();
        }
    };
    
    template <functional::stack X> struct reverse<X> {
        X operator()(const X& x) {
            return functional::reverse(x);
        }
    };
}

// These functions will work on any stack-like object. 
namespace data {
    template <typename list, typename elem> requires interface::has_prepend_method<list, elem>
    inline list prepend(const list& x, const elem& e) {
        return x.prepend(e);
    }
    
    template <typename X>
    inline X reverse(const X x) {
        return meta::reverse<X>{}(x);
    }
    
    template <typename list>
    list join(const list&a, const list& b) {
        if (b.empty()) return a;
        return join(a << b.first(), b.rest());
    }
    
    template <typename list, typename prop> requires functional::stack<list> && function<prop, element_of<list>, bool>
    list select(const list& a, const prop p) {
        return a.empty() ? list{} : p(a.first()) ? list{a.first()} << select(a.rest(), p) : select(a.rest(), p);
    }
    
    template <functional::stack list> 
    list take(list x, uint32 n) {
        list z{};
        while (size(x) > 0 && n > 0) {
            z = prepend(z, x.first());
            x = rest(x);
            n--;
        }
        return reverse(z);
    }
}

namespace data::functional {
    
    template <typename X, typename Y>
    inline stack_node<X, Y>::stack_node(X x, Y r) : First(x), Rest(r), Size{data::size(r) + 1} {}
    
    template <typename X, typename Y>
    inline stack_node<X, Y>::stack_node(X x) : First(x), Rest{}, Size{1} {}
    
    template <typename X, typename Y>
    inline const X& stack_node<X, Y>::first() const {
        return First;
    }
    
    template <typename X, typename Y>
    inline Y stack_node<X, Y>::rest() const {
        return Rest;
    }
    
    template <typename X, typename Y>
    inline bool stack_node<X, Y>::valid() const {
        return data::valid(First) && data::valid(Rest);
    }
    
    template <typename X, typename Y>
    inline size_t stack_node<X, Y>::size() const {
        return Size;
    }
    
    template <typename X, typename Y>
    inline bool stack_node<X, Y>::contains(X x) const {
        if (x == First) return true;
        
        return data::functional::contains(Rest, x);
    }
    
    // an iterator that could go with a list. 
    template <typename L, typename element> 
    inline stack_iterator<L, element>& stack_iterator<L, element>::operator=(const stack_iterator& i) {
        Next = i.Next;
        Index = i.Index;
        return *this;
    }
    
    template <typename L, typename element> 
    inline stack_iterator<L, element>& stack_iterator<L, element>::operator++() { // Prefix
        if (Next == nullptr) return *this;
        if (Next->Size == 1) return operator=(stack_iterator{Index + 1});
        return operator=(stack_iterator{Next->Rest.Next, Index + 1});
    }
    
    template <typename L, typename element> 
    inline stack_iterator<L, element> stack_iterator<L, element>::operator++(int) { // Postfix
        stack_iterator n = *this;
        operator++();
        return n;
    }
    
    template <typename L, typename element> 
    inline element& stack_iterator<L, element>::operator*() const {
        return Next->First;
    }
    
    template <typename L, typename element> 
    inline bool stack_iterator<L, element>::operator==(const stack_iterator i) const {
        return Next == i.Next && Index == i.Index;
    }
    
    template <typename L, typename element> 
    inline int stack_iterator<L, element>::operator-(const stack_iterator& i) const {
        return static_cast<int>(Index) - i.Index;
    }

}

#endif
