// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_QUEUE
#define DATA_QUEUE

// a stack is the most basic functional data structure. 

#include <data/concepts.hpp>
#include <data/iterable.hpp>
#include <data/valid.hpp>
#include <data/function.hpp>

namespace data::interface {
    
    template <typename list, typename element>
    concept has_queue_constructor = requires (list x, element e) {
        { list (x, e) } -> Same<list>;
    } && requires (element e) {
        { list {e} } -> Same<list>;
    };
    
    template <typename list, typename element>
    concept has_append_method = requires (list x, element e) {
        { x.append (e) } -> ImplicitlyConvertible<const list>;
    };
    
    template <typename list>
    concept has_sort_method = requires (list x) {
        { x.sort () } -> ImplicitlyConvertible<const list>;
    };
    
}

namespace data {
    
    template <typename list, typename elem> requires interface::has_append_method<list, elem>
    inline list append (const list &x, elem e) {
        return x.append (e);
    }
    
    template <typename L>
    concept Queue = std::default_initializable<L> && Sequence<const L> && 
        interface::has_append_method<const L, decltype (std::declval<const L> ().first ())>;
    
}

namespace data::functional {
    
    template <Queue list> 
    list take_queue (const list &x, size_t n, const list &z = {});
    
    template <Queue list>
    list join_queue (const list &a, const list &b) {
        if (data::empty (b)) return a;
        return join_queue (append (a, first (b)), rest (b));
    }

    template <Queue list, typename elem>
    list erase_queue (const list &x, const elem &e) {
        list left {};
        list right = x;
        while (!empty (right)) {
            if (first (right) != e) left = append (left, first (right));
            right = rest (right);
        }
        return left;
    }
    
    template <Queue L> requires Ordered<decltype (std::declval<const L> ().first ())>
    L merge_queue (const L &a, const L &b, const L &n = {}) {
        if (data::empty (a) && data::empty(b)) return reverse (n);
        if (data::empty (a)) return merge_queue (a, rest (b), prepend (n, first (b)));
        if (data::empty (b)) return merge_queue (rest (a), b, prepend (n, first (a)));
        return first (a) < first(b) ?
            merge_queue (rest (a), b, prepend (n, first (a))):
            merge_queue (a, rest (b), prepend (n, first (b)));
    }

    template <Queue list>
    list remove_queue (const list &);
    
}

namespace data {

    template <typename F, typename... Args> concept Proposition = std::regular_invocable<F, Args...> && requires (const F f, Args &&...args) {
        { std::invoke (std::forward<F> (f) (std::forward<Args> (args)...)) } -> Same<bool>;
    };
    
    template <Queue list, Proposition<decltype (std::declval<const list> ().first ())> F>
    list select (list l, F satisfies, list found = {}) {
        if (data::empty (l)) return found;
        auto f0 = first (l);
        if (satisfies (f0)) select (rest (l), satisfies, append (found, f0));
        return select (rest (l), satisfies, found);
    }
    
    template <Queue L>
    L rotate_left (const L x) {
        size_t s = data::size (x);
        if (s == 0 || s == 1) return x; 
        
        return append (rest (x), first (x));
    }
    
    template <Queue L>
    L rotate_left (const L x, size_t n) {
        if (n == 0) return x;
        
        size_t s = size (x);
        if (s == 0 || s == 1) return x; 
        
        if (n > s) return rotate_left (x, n % s);
        return rotate_left (rotate_left (x, n - 1));
    }
    
    template <Queue L>
    inline L rotate_right (const L x) {
        return reverse (rotate_left (reverse (x)));
    }
    
    template <Queue L>
    L rotate_right (const L x, size_t n) {
        if (n == 0) return x;
        
        size_t s = size (x);
        if (s == 0 || s == 1) return x; 
        
        if (n > s) return rotate_right (x, n % s);
        return rotate_right (rotate_right (x, n - 1));
    }
    
}

#endif
