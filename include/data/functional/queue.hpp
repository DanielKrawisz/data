// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_QUEUE
#define DATA_QUEUE

// a stack is the most basic functional data structure. 

#include <type_traits>
#include <data/iterable.hpp>
#include <data/valid.hpp>

namespace data::interface {
    
    template <typename list, typename element>
    concept has_queue_constructor = requires (list x, element e) {
        { list{x, e} } -> std::same_as<list>;
    } && requires (element e) {
        { list{e} } -> std::same_as<list>;
    };
    
    template <typename list, typename element>
    concept has_append_method = requires (list x, element e) {
        { x.append(e) } -> std::convertible_to<list>;
    };
    
    template <typename list>
    concept has_sort_method = requires (list x) {
        { x.sort() } -> std::convertible_to<list>;
    };
    
}

namespace data::functional {
    
    template <typename L, typename elem = decltype(std::declval<L>().first())> 
    concept queue = sequence<const L, elem> && interface::has_append_method<const L, elem> && 
        interface::has_queue_constructor<L, elem> && std::default_initializable<L>;
    
}

namespace data {
    
    template <typename list, typename elem> requires interface::has_append_method<list, elem>
    inline list append(const list& x, const elem& e) {
        return x.append(e);
    }

    template <typename X> X merge_sort(const X& a);
    
    template <functional::queue L>
    L rotate_left(const L x) {
        size_t s = size(x);
        if (s == 0 || s == 1) return x; 
        
        return append(rest(x), first(x));
    }
    
    template <functional::queue L>
    L rotate_left(const L x, uint32 n) {
        if (n == 0) return x;
        
        size_t s = size(x);
        if (s == 0 || s == 1) return x; 
        
        if (n > s) return rotate_left(x, n % s);
        return rotate_left(rotate_left(x, n - 1));
    }
    
    template <functional::queue L>
    inline L rotate_right(const L x) {
        return reverse(rotate_left(reverse(x)));
    }
    
    template <functional::queue L>
    L rotate_right(const L x, uint32 n) {
        if (n == 0) return x;
        
        size_t s = size(x);
        if (s == 0 || s == 1) return x; 
        
        if (n > s) return rotate_right(x, n % s);
        return rotate_right(rotate_right(x, n - 1));
    }
    
    template <interface::has_sort_method X>
    inline X sort(const X x) {
        return x.sort();
    }
    
    template <functional::queue L>
    L sort(L list) {
        return merge_sort(list);
    }
    
}

#endif
