// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_LINKED_STACK
#define DATA_TOOLS_LINKED_STACK

#include <ostream>
#include <data/list.hpp>
#include <data/functional/stack.hpp>
    
namespace data::tool {
    template <typename elem>
    class linked_stack {
        
        using node = functional::stack_node<elem, linked_stack>;
        using next = ptr<node>;
        
        next Next;
        linked_stack(next n);
        
    public:
        linked_stack();
        linked_stack(const linked_stack& l);
        linked_stack(const elem& e, const linked_stack& l);
        linked_stack(const elem& e);
        
        template<typename ... P>
        linked_stack(const elem& a, const elem& b, P... p);
        
        linked_stack& operator=(const linked_stack& l);
        
        // if the list is empty, then this function
        // will dereference a nullptr. It is your
        // responsibility to check. 
        const elem& first() const;
        
        elem& first();
    
        bool empty() const;
        
        linked_stack rest() const;
        
        bool valid() const;
        
        bool contains(elem x) const;
        
        size_t size() const;
        
        linked_stack operator<<(elem x) const;
        linked_stack& operator<<=(elem x);
        
        linked_stack prepend(elem x) const;
        linked_stack prepend(linked_stack l) const;
        
        template <typename X, typename Y, typename ... P>
        linked_stack prepend(X x, Y y, P ... p) const;
        
        linked_stack operator^(linked_stack l) const;
        
        linked_stack from(uint32 n) const;
        
        const elem& operator[](uint32 n) const;
        
        using iterator = functional::stack_iterator<next, elem>;
        using const_iterator = functional::stack_iterator<next, const elem>;
        
        friend iterator;
        friend const_iterator;
        
        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;

        template <typename X> 
        bool operator==(const data::tool::linked_stack<X>& x) const {
            if ((void*)(Next.get()) == (void*)(x.Next.get())) return true;
            if (size() != x.size()) return false;
            if (Next == nullptr || x.Next == nullptr) return false;
            if (first() != x.first()) return false;
            return (rest() == x.rest());
        }

        template <typename X> 
        bool operator!=(const data::tool::linked_stack<X>& x) const {
            return !(*this == x);
        }
        
    };
    
    template <typename elem> inline std::ostream& operator<<(std::ostream& o, const linked_stack<elem>& x) {
        return functional::stack::write(o << "stack", x);
    }

    template <typename elem>
    inline linked_stack<elem>::linked_stack(next n) : Next{n} {}
    
    template <typename elem>
    inline linked_stack<elem>::linked_stack() : Next{nullptr} {}
    
    template <typename elem>
    inline linked_stack<elem>::linked_stack(const linked_stack& l) : linked_stack{l.Next} {}
    
    template <typename elem>
    inline linked_stack<elem>::linked_stack(const elem& e, const linked_stack& l) : linked_stack{std::make_shared<node>(e, l)} {}
    
    template <typename elem>
    inline linked_stack<elem>::linked_stack(const elem& e) : linked_stack{e, linked_stack{}} {}
    
    template <typename elem>
    template <typename ... P>
    inline linked_stack<elem>::linked_stack(const elem& a, const elem& b, P... p) : 
        linked_stack{a, linked_stack{b, linked_stack{p...}}} {} 
    
    template <typename elem>
    inline linked_stack<elem>& linked_stack<elem>::operator=(const linked_stack& l) {
        Next = l.Next;
        return *this;
    } 
    
    // if the list is empty, then this function
    // will dereference a nullptr. It is your
    // responsibility to check. 
    template <typename elem>
    inline const elem& linked_stack<elem>::first() const {
        return Next->First;
    }
    
    template <typename elem>
    inline elem& linked_stack<elem>::first() {
        return Next->First;
    }
    
    template <typename elem>
    inline bool linked_stack<elem>::empty() const {
        return Next == nullptr;
    }
    
    template <typename elem>
    inline linked_stack<elem> linked_stack<elem>::rest() const {
        if (empty()) return {};
        
        return Next->rest();
    }
    
    template <typename elem>
    inline bool linked_stack<elem>::valid() const {
        if (empty()) return true;
        if (!data::valid(first())) return false;
        return rest().valid();
    }
    
    template <typename elem>
    inline bool linked_stack<elem>::contains(elem x) const {
        if (empty()) return false;
            
        Next->contains(x);
    }
    
    template <typename elem>
    inline size_t linked_stack<elem>::size() const {
        if (empty()) return 0;
            
        return Next->size();
    }
    
    template <typename elem>
    inline linked_stack<elem> linked_stack<elem>::operator<<(elem x) const {
        return {x, *this};
    }
    
    template <typename elem>
    inline linked_stack<elem> linked_stack<elem>::prepend(elem x) const {
        return {x, *this};
    }
    
    template <typename elem>
    inline linked_stack<elem>& linked_stack<elem>::operator<<=(elem x) {
        return operator=(prepend(x));
    }
    
    template <typename elem>
    linked_stack<elem> linked_stack<elem>::prepend(linked_stack l) const {
        linked_stack x = *this;
        while (!l.empty()) {
            x = x + l.first();
            l = l.rest();
        }
        return x;
    }
    
    template <typename elem>
    template <typename X, typename Y, typename ... P>
    inline linked_stack<elem> linked_stack<elem>::prepend(X x, Y y, P ... p) const {
        return prepend(x).prepend(y, p...);
    }
    
    template <typename elem>
    inline linked_stack<elem> linked_stack<elem>::operator^(linked_stack l) const {
        return prepend(l);
    }
    
    template <typename elem>
    linked_stack<elem> linked_stack<elem>::from(uint32 n) const {
        if (empty()) return {};
        if (n == 0) return *this;
        return rest().from(n - 1);
    }
    
    template <typename elem>
    inline const elem& linked_stack<elem>::operator[](uint32 n) const {
        return from(n).first();
    }
    
    template <typename elem>
    inline typename linked_stack<elem>::iterator linked_stack<elem>::begin() {
        return iterator{Next};
    }
    
    template <typename elem>
    inline typename linked_stack<elem>::iterator linked_stack<elem>::end() {
        return iterator{size()};
    }
    
    template <typename elem>
    inline typename linked_stack<elem>::const_iterator linked_stack<elem>::begin() const {
        return const_iterator{Next};
    }
    
    template <typename elem>
    inline typename linked_stack<elem>::const_iterator linked_stack<elem>::end() const {
        return const_iterator{size()};
    }

}

#endif
