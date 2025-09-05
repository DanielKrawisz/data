// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_FUNCTIONAL_STACK
#define DATA_FUNCTIONAL_STACK

// a stack is the most basic functional data structure. 

#include <iterator>

#include <data/concepts.hpp>
#include <data/valid.hpp>
#include <data/sequence.hpp>
#include <data/iterable.hpp>
#include <data/function.hpp>
#include <data/size.hpp>

#include <data/io/exception.hpp>

namespace data::interface {
    
    template <typename list, typename element>
    concept has_stack_constructor = requires (list x, element e) {
        { list (e, x) } -> Same<list>;
    } && requires (element e) {
        { list {e} } -> Same<list>;
    };
    
    template <typename list, typename element>
    concept has_prepend_method = requires (list x, element e) {
        { x.prepend (e) } -> ImplicitlyConvertible<const list>;
    };
    
}

namespace data { 
    template <typename list, typename elem> requires interface::has_prepend_method<list, elem>
    inline list prepend (const list &x, elem e) {
        return x.prepend (e);
    }

    template <typename L, typename elem = first_return_type<L>>
    concept Stack = std::default_initializable<L> && Sequence<const L, elem> &&
        interface::has_prepend_method<const L, elem>;

    template <Stack list> list reverse (const list &);
}

namespace data::functional { 
    
    template <Stack list> 
    list take_stack (const list &x, size_t n, const list &z = {});
    
    template <Stack list>
    list join_stack (const list &a, const list &b);

    // merges two stacks sorted by < into a single sorted stack.
    template <Stack L> requires Ordered<decltype (std::declval<L> ().first ())>
    L merge_stack (const L &a, const L &b);
    
    template <typename times, typename L1, typename L2, typename plus, typename value>
    value inner (times t, L1 l1, L2 l2, plus p);
    
    template <Stack list>
    list join_stack (const list &a, const list &b) {
        if (data::empty (a)) return b;
        return prepend (join_stack (rest (a), b), first (a));
    }

    template <Stack list, typename elem>
    list erase_stack (const list &x, const elem &e) {
        list left {};
        list right = x;
        while (!empty (right)) {
            if (first (right) != e) left = prepend (left, first (right));
            right = rest (right);
        }
        return reverse (left);
    }

    template <Stack L> requires Ordered<decltype (std::declval<L> ().first ())>
    L merge_stack (const L &a, const L &b) {
        L l = a;
        L r = b;
        L n {};

        while (true) {
            if (data::empty (l) && data::empty (r)) return reverse (n);
            else if (data::empty (l)) {
                n = prepend (n, first (r));
                r = rest (r);
            } else if (data::empty (r)) {
                n = prepend (n, first (l));
                l = rest (l);
            } else if (first (l) < first (r)) {
                n = prepend (n, first (l));
                l = rest (l);
            } else {
                n = prepend (n, first (r));
                r = rest (r);
            }
        }
    }
    
    template <typename times, typename L1, typename L2, typename plus, typename value>
    value inner (times t, L1 l1, L2 l2, plus p) {
        uint32 size = l1.size ();
        if (size != l2.size ()) exception {"lists must be the same size"};
        if (size == 0) return value {};
        
        return p (t (l1.first (), l2.first ()), inner (t, rest (l1), rest (l2), p));
    }
    
    // This is a node that you could perhaps use to make a list. 
    template <typename X, typename Y>
    struct stack_node {
        X First;
        Y Rest;
        size_t Size;
        
        stack_node (inserted<X> x, Y r);
        stack_node (inserted<X> x);
        
        X &first ();
        const X &first () const;
        Y rest () const;
        
        bool valid () const;
        size_t size () const;
        bool contains (X x) const;
    };
    
    template <typename X, typename Y>
    inline stack_node<X, Y>::stack_node (inserted<X> x, Y r) : First (x), Rest (r), Size {data::size (r) + 1} {}
    
    template <typename X, typename Y>
    inline stack_node<X, Y>::stack_node (inserted<X> x) : First (x), Rest {}, Size {1} {}
    
    template <typename X, typename Y>
    inline const X& stack_node<X, Y>::first () const {
        return First;
    }
    
    template <typename X, typename Y>
    inline Y stack_node<X, Y>::rest () const {
        return Rest;
    }
    
    template <typename X, typename Y>
    inline bool stack_node<X, Y>::valid () const {
        return data::valid (First) && data::valid (Rest);
    }
    
    template <typename X, typename Y>
    inline size_t stack_node<X, Y>::size () const {
        return Size;
    }
    
    template <typename X, typename Y>
    inline bool stack_node<X, Y>::contains (X x) const {
        if (x == First) return true;
        
        return data::contains (Rest, x);
    }

}

#endif
