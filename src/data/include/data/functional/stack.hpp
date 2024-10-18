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
        { list (e, x) } -> std::same_as<list>;
    } && requires (element e) {
        { list {e} } -> std::same_as<list>;
    };
    
    template <typename list, typename element>
    concept has_prepend_method = requires (list x, element e) {
        { x.prepend (e) } -> implicitly_convertible_to<list>;
    };
    
}

namespace data { 
    template <typename list, typename elem> requires interface::has_prepend_method<list, elem>
    inline list prepend (const list &x, const elem &e) {
        return x.prepend (e);
    }
}

namespace data::functional { 
    
    template <typename L, typename elem = std::remove_reference_t<decltype (std::declval<L> ().first ())>>
    concept stack = sequence<const L, elem> && interface::has_prepend_method<const L, elem> && 
        interface::has_stack_constructor<L, elem> && std::default_initializable<L>;
    
    template <stack list> 
    list take_stack (const list &x, size_t n, const list &z = {});
    
    template <functional::stack list>
    list join_stack (const list &a, const list &b);

    // merges two stacks sorted by < into a single sorted stack.
    template <stack L> requires ordered<element_of<L>>
    L merge_stack (const L &a, const L &b);
    /*
    template <stack L> requires ordered<element_of<L>>
    L merge_stack (const L &a, const L &b, const L &n = {});*/
    
    template <typename times, typename L1, typename L2, typename plus, typename value>
    value inner (times t, L1 l1, L2 l2, plus p);
}

namespace data {
/*
    template <functional::stack list> 
    list reverse (const list &given, const list &reversed = {});*/

    template <functional::stack list> list reverse (const list &);
}

namespace data::functional { 
    
    template <functional::stack list>
    list join_stack (const list &a, const list &b) {
        if (data::empty (a)) return b;
        return prepend (join_stack (rest (a), b), first (a));
    }
    /*
    template <stack L> requires ordered<element_of<L>>
    L merge_stack (const L &a, const L &b, const L &n) {
        std::cout << " merging two lists of size " << data::size (a) << " and " << data::size (b) << " with " << data::size (n) << " complete." << std::endl;
        if (data::empty (a) && data::empty (b)) return reverse (n);
        if (data::empty (a)) return merge_stack (a, rest (b), prepend (n, first (b)));
        if (data::empty (b)) return merge_stack (rest (a), b, prepend (n, first (a)));
        return first (a) < first (b) ?
            merge_stack (rest (a), b, prepend (n, first (a))):
            merge_stack (a, rest (b), prepend (n, first (b)));
    }*/

    template <stack L> requires ordered<element_of<L>>
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
        
        stack_node (X x, Y r);
        stack_node (X x);
        
        X& first ();
        const X& first() const;
        Y rest () const;
        
        bool valid () const;
        size_t size () const;
        bool contains (X x) const;
    };
    
    template <typename X, typename Y>
    inline stack_node<X, Y>::stack_node (X x, Y r) : First (x), Rest(r), Size {data::size (r) + 1} {}
    
    template <typename X, typename Y>
    inline stack_node<X, Y>::stack_node (X x) : First (x), Rest {}, Size {1} {}
    
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
        
        return data::functional::contains (Rest, x);
    }

}

#endif
