// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_FUNCTIONAL_STACK
#define DATA_FUNCTIONAL_STACK

// a stack is the most basic functional data structure. 

#include <type_traits>
#include <iterator>

#include <data/valid.hpp>
#include <data/iterable.hpp>

namespace data::functional {
    
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
        bool operator!=(const stack_iterator i) const;
        
        int operator-(const stack_iterator& i) const;
        
        stack_iterator(list n) : Next{n}, Index{0} {}
        stack_iterator(size_t i) : Next{}, Index{i} {}
    };
}

namespace std {
    struct input_output_iterator_tag : input_iterator_tag, output_iterator_tag {};
    
    template <typename list, typename elem> 
    struct iterator_traits<data::functional::stack_iterator<list, elem>> {
        using value_type = elem;
        using difference_type = int;
        using pointer = remove_reference_t<elem>*;
        using reference = elem&;
        using iterator_concept = input_output_iterator_tag;
    };
    
    template <typename list, typename elem> 
    struct iterator_traits<data::functional::stack_iterator<list, const elem>> {
        using value_type = elem;
        using difference_type = int;
        using pointer = const remove_reference_t<elem>*;
        using reference = const elem&;
        using iterator_concept = input_iterator_tag;
    };
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
    inline bool stack_iterator<L, element>::operator!=(const stack_iterator i) const {
        return Next != i.Next || Index != i.Index;
    }
    
    template <typename L, typename element> 
    inline int stack_iterator<L, element>::operator-(const stack_iterator& i) const {
        return static_cast<int>(Index) - i.Index;
    }
    
}

#endif
