// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_QUEUE
#define DATA_QUEUE

// a stack is the most basic functional data structure. 

#include <data/concepts.hpp>
#include <data/iterable.hpp>
#include <data/valid.hpp>

namespace data::interface {
    
    template <typename list, typename element>
    concept has_queue_constructor = requires (list x, element e) {
        { list (x, e) } -> std::same_as<list>;
    } && requires (element e) {
        { list {e} } -> std::same_as<list>;
    };
    
    template <typename list, typename element>
    concept has_append_method = requires (list x, element e) {
        { x.append (e) } -> implicitly_convertible_to<list>;
    };
    
    template <typename list>
    concept has_sort_method = requires (list x) {
        { x.sort () } -> implicitly_convertible_to<list>;
    };
    
}

namespace data {
    
    template <typename list, typename elem> requires interface::has_append_method<list, elem>
    inline list append (const list &x, const elem &e) {
        return x.append (e);
    }
    
}

namespace data::functional {
    
    template <typename L, typename elem = decltype (std::declval<L> ().first ())>
    concept queue = sequence<const L, elem> && interface::has_append_method<const L, elem> && 
        interface::has_queue_constructor<L, elem> && std::default_initializable<L>;
    
    template <queue list> 
    list take_queue (const list &x, size_t n, const list &z = {});
    
    template <queue list>
    list join_queue(const list&a, const list& b) {
        if (data::empty (b)) return a;
        return join_queue (append(a, first(b)), rest(b));
    }
    
    template <queue L> requires ordered<element_of<L>>
    L merge_queue (const L &a, const L &b, const L &n = {}) {
        if (data::empty (a) && data::empty(b)) return reverse (n);
        if (data::empty (a)) return merge_queue (a, rest (b), prepend (n, first (b)));
        if (data::empty (b)) return merge_queue (rest (a), b, prepend (n, first (a)));
        return first (a) < first(b) ?
            merge_queue (rest (a), b, prepend (n, first (a))):
            merge_queue (a, rest (b), prepend (n, first (b)));
    }
    
}

namespace data {
    
    template <functional::queue list>
    list select (list l, function<bool (element_of<list>)> satisfies, list found = {}) {
        if (data::empty (l)) return found;
        auto f0 = first (l);
        if (satisfies (f0)) select (rest (l), satisfies, append (found, f0));
        return select (rest (l), satisfies, found);
    }
    
    template <functional::queue L>
    L rotate_left (const L x) {
        size_t s = data::size (x);
        if (s == 0 || s == 1) return x; 
        
        return append (rest (x), first (x));
    }
    
    template <functional::queue L>
    L rotate_left (const L x, uint32 n) {
        if (n == 0) return x;
        
        size_t s = size (x);
        if (s == 0 || s == 1) return x; 
        
        if (n > s) return rotate_left (x, n % s);
        return rotate_left (rotate_left (x, n - 1));
    }
    
    template <functional::queue L>
    inline L rotate_right (const L x) {
        return reverse (rotate_left (reverse (x)));
    }
    
    template <functional::queue L>
    L rotate_right (const L x, uint32 n) {
        if (n == 0) return x;
        
        size_t s = size (x);
        if (s == 0 || s == 1) return x; 
        
        if (n > s) return rotate_right (x, n % s);
        return rotate_right (rotate_right (x, n - 1));
    }
    
}

#endif
