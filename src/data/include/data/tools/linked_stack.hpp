// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_LINKED_STACK
#define DATA_TOOLS_LINKED_STACK

#include <ostream>
#include <data/functional/stack.hpp>
    
namespace data {

    template <typename elem> class linked_stack;

    // stack two stacks together.
    template <typename elem> linked_stack<elem> operator + (linked_stack<elem>, linked_stack<elem>);

    template <typename elem>
    struct element<linked_stack<elem>> {
        using type = elem;
    };
    
    template <typename elem>
    class linked_stack {
        
        using node = functional::stack_node<elem, linked_stack>;
        using next = ptr<node>;
        
        next Next;
        linked_stack (next n);
        
    public:
        linked_stack ();
        explicit linked_stack (inserted<elem> e, const linked_stack &l);
        explicit linked_stack (inserted<elem> e);
        
        linked_stack (std::initializer_list<wrapped<elem>> init) {
            for (int i = init.size () - 1; i >= 0; i--) *this = prepend (*(init.begin () + i));
        }
        
        // if the list is empty, then this function
        // will dereference a nullptr. It is your
        // responsibility to check. 
        const elem &first () const;
        
        elem &first ();
    
        bool empty () const;
        
        linked_stack rest () const;
        
        bool valid () const;
        
        bool contains (elem x) const;
        
        size_t size () const;
        
        linked_stack operator << (inserted<elem> x) const;
        linked_stack &operator <<= (inserted<elem> x);
        
        linked_stack prepend (inserted<elem> x) const;
        linked_stack prepend (linked_stack l) const;
        
        template <typename X, typename Y, typename ... P>
        linked_stack prepend (X x, Y y, P ... p) const;
        
        linked_stack operator ^ (linked_stack l) const;
        
        linked_stack from (uint32 n) const;
        
        const elem &operator [] (uint32 n) const;
        
        using iterator = sequence_iterator<const linked_stack<elem>>;
        using sentinel = data::sentinel<const linked_stack<elem>>;
        
        iterator begin () const;
        sentinel end () const;
        
        template <data::sequence X> requires std::equality_comparable_with<elem, data::element_of<X>>
        bool operator == (const X &x) const;

        // automatic conversions 
        template <typename X> requires implicitly_convertible_to<elem, X>
        operator linked_stack<X> () const;

        // explicit conversions
        template <typename X> requires explicitly_convertible_to<elem, X>
        explicit operator linked_stack<X> () const;
        
    };

    template <typename elem> linked_stack<elem> operator + (linked_stack<elem> a, linked_stack<elem> b) {
        return b.prepend (data::reverse (a));
    }
    
    // a bidirectional iterator in case you need one. 
    template <typename elem>
    class linked_stack_iterator : public sequence_iterator<linked_stack<elem>> {
        linked_stack<const linked_stack<elem> &> Prev;
        
        linked_stack_iterator (const linked_stack<elem> &s, linked_stack<elem> n, int i, linked_stack<const linked_stack<elem> &> p) :
            sequence_iterator<linked_stack<elem>> {s, n, i}, Prev {p} {}
        
    public:
        linked_stack_iterator (): sequence_iterator<linked_stack<elem>> {}, Prev {} {}
        
        linked_stack_iterator operator ++ (int);
        linked_stack_iterator &operator ++ ();
        
        linked_stack_iterator operator -- (int);
        linked_stack_iterator &operator -- ();
        
        linked_stack_iterator (const linked_stack<elem> &s) : sequence_iterator<linked_stack<elem>> {s}, Prev {} {}
    };
}

namespace std {
    template <typename elem> 
    struct iterator_traits<data::linked_stack_iterator<elem>> {
        using value_type = remove_const_t<elem>;
        using difference_type = int;
        using pointer = const remove_reference_t<elem>*;
        using reference = const elem &;
        using iterator_concept = forward_iterator_tag;
    };
}

namespace data {
    
    template <typename elem> requires requires (std::ostream &o, const elem &e) {
        { o << e } -> std::same_as<std::ostream &>;
    } std::ostream inline &operator << (std::ostream &o, const linked_stack<elem> &x) {
        return functional::write (o << "stack", x);
    }
    
    template <typename elem>
    inline linked_stack<elem>::linked_stack (next n) : Next {n} {}
    
    template <typename elem>
    inline linked_stack<elem>::linked_stack () : Next {nullptr} {}
    
    template <typename elem>
    inline linked_stack<elem>::linked_stack (inserted<elem> e, const linked_stack &l) : linked_stack {std::make_shared<node> (e, l)} {}
    
    template <typename elem>
    inline linked_stack<elem>::linked_stack (inserted<elem> e) : linked_stack {e, linked_stack {}} {}
    
    // if the list is empty, then this function
    // will dereference a nullptr. It is your
    // responsibility to check. 
    template <typename elem>
    const elem inline &linked_stack<elem>::first () const {
        return Next->First;
    }
    
    template <typename elem>
    elem inline &linked_stack<elem>::first () {
        return Next->First;
    }
    
    template <typename elem>
    bool inline linked_stack<elem>::empty () const {
        return Next == nullptr;
    }
    
    template <typename elem>
    linked_stack<elem> inline linked_stack<elem>::rest () const {
        if (empty ()) return {};
        
        return Next->rest ();
    }
    
    template <typename elem>
    inline bool linked_stack<elem>::valid () const {
        if (empty ()) return true;
        if (!data::valid (first ())) return false;
        return rest ().valid ();
    }
    
    template <typename elem>
    bool inline linked_stack<elem>::contains (elem x) const {
        if (empty ()) return false;
            
        return Next->contains (x);
    }
    
    template <typename elem>
    inline size_t linked_stack<elem>::size () const {
        if (empty ()) return 0;
            
        return Next->size ();
    }
    
    template <typename elem>
    linked_stack<elem> inline linked_stack<elem>::operator <<(inserted<elem> x) const {
        return linked_stack {x, *this};
    }
    
    template <typename elem>
    linked_stack<elem> inline linked_stack<elem>::prepend (inserted<elem> x) const {
        return linked_stack {x, *this};
    }
    
    template <typename elem>
    linked_stack<elem> inline &linked_stack<elem>::operator <<= (inserted<elem> x) {
        return operator = (prepend (x));
    }
    
    template <typename elem>
    linked_stack<elem> linked_stack<elem>::prepend (linked_stack l) const {
        linked_stack x = *this;
        while (!l.empty ()) {
            x = x << l.first ();
            l = l.rest ();
        }
        return x;
    }
    
    template <typename elem>
    template <typename X, typename Y, typename ... P>
    linked_stack<elem> inline linked_stack<elem>::prepend (X x, Y y, P ... p) const {
        return prepend (x).prepend (y, p...);
    }
    
    template <typename elem>
    linked_stack<elem> inline linked_stack<elem>::operator ^ (linked_stack l) const {
        return prepend (l);
    }
    
    template <typename elem>
    linked_stack<elem> linked_stack<elem>::from (uint32 n) const {
        if (empty ()) return {};
        if (n == 0) return *this;
        return rest ().from (n - 1);
    }
    
    template <typename elem>
    const elem inline &linked_stack<elem>::operator [] (uint32 n) const {
        return from (n).first();
    }
    
    template <typename elem>
    linked_stack<elem>::iterator inline linked_stack<elem>::begin () const {
        return iterator {*this};
    }
    
    template <typename elem>
    linked_stack<elem>::sentinel inline linked_stack<elem>::end () const {
        return sentinel {*this};
    }
    
    template <typename elem>
    linked_stack_iterator<elem> linked_stack_iterator<elem>::operator ++ (int) {
        auto n = *this;
        ++(*this);
        return n;
    }
    
    template <typename elem>
    linked_stack_iterator<elem> linked_stack_iterator<elem>::operator -- (int) {
        auto n = *this;
        --(*this);
        return n;
    }
    
    template <typename elem>
    linked_stack_iterator<elem> &linked_stack_iterator<elem>::operator ++ () {
        Prev <<= sequence_iterator<linked_stack<elem>>::Next;
        sequence_iterator<linked_stack<elem>>::operator ++ ();
    }
    
    template <typename elem>
    linked_stack_iterator<elem> &linked_stack_iterator<elem>::operator -- () {
        if (data::empty (Prev)) return *this;
        return *this = linked_stack_iterator {
            *sequence_iterator<linked_stack<elem>>::Sequence, 
            first (sequence_iterator<linked_stack<elem>>::Prev),
            sequence_iterator<linked_stack<elem>>::Index - 1, 
            rest (sequence_iterator<linked_stack<elem>>::Prev)};
    }

    template <typename elem>
    template <data::sequence X> requires std::equality_comparable_with<elem, data::element_of<X>>
    bool inline linked_stack<elem>::operator == (const X &x) const {
        return sequence_equal (*this, x);
    }

    template <typename elem>
    template <typename X> requires implicitly_convertible_to<elem, X>
    inline linked_stack<elem>::operator linked_stack<X> () const {
        if (size () == 0) return linked_stack<X> {};
        return linked_stack<X> {rest ()}.prepend (X (first ()));
    }

    template <typename elem>
    template <typename X> requires explicitly_convertible_to<elem, X>
    inline linked_stack<elem>::operator linked_stack<X> () const {
        if (size () == 0) return linked_stack<X> {};
        return linked_stack<X> {rest ()}.prepend (X (first ()));
    }

}

#endif
