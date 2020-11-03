// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_LIST
#define DATA_LIST

#include <data/stack.hpp>

namespace data::interface {
    
    template <typename Q>
    struct list : stack<Q> {
        using element = typename std::enable_if<
            meta::has_append_method<Q, typename sequence<Q>::element>::value, typename sequence<Q>::element>::type;
    }; 
}

namespace data::functional::list {
    
    template <typename L>
    L rotate_left(const L x) {
        size_t s = size(x);
        if (s == 0 || s == 1) return x; 
        
        return data::append(data::rest(x), data::first(x));
    }
    
    template <typename L>
    inline L rotate_right(const L x) {
        return data::reverse(rotate_left(data::reverse(x)));
    }
    
    template <typename L>
    L rotate_left(const L x, uint32 n) {
        if (n == 0) return x;
        
        size_t s = size(x);
        if (s == 0 || s == 1) return x; 
        
        if (n > s) return rotate_left(x, n % s);
        return rotate_left(rotate_left(x, n - 1));
    }
    
    template <typename L>
    L rotate_right(const L x, uint32 n) {
        if (n == 0) return x;
        
        size_t s = size(x);
        if (s == 0 || s == 1) return x; 
        
        if (n > s) return rotate_right(x, n % s);
        return rotate_right(rotate_right(x, n - 1));
    }
}

namespace data {
    // TODO if this is a stack, need to reverse.
    // take care of this when we use c++20. 
    template <typename list>
    list take(list l, uint32 x) {
        if (x >= l.size()) return l;
        list n = l;
        list r{};
        while (x > 0) {
            r = r << n.first();
            n = n.rest();
            x--;
        }
        return r;
    }
    
    template <typename list>
    list join(const list&a, const list& b) {
        if (b.empty()) return a;
        return join(a << b.first(), b.rest());
    }
}

#endif

