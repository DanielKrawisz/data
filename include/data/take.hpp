// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TAKE
#define DATA_TAKE

#include <data/functional/list.hpp>
#include <data/reverse.hpp>

namespace data::functional { 
    
    template <stack list> 
    list take_stack (const list &x, size_t n, const list &z) {
        return data::empty (x) || n == 0 ? reverse (z) : take_stack (rest (x), n - 1, prepend (z, first (x)));
    }
    
    template <queue list> 
    list take_queue (const list &x, size_t n, const list &z) {
        return data::empty (x) || n == 0 ? z : take_queue (rest (x), n - 1, append (z, first (x)));
    }
}

namespace data {
    
    template <functional::pendable list> 
    list inline take (const list &l, size_t size) {
        if constexpr (functional::queue<list>) return functional::take_queue (l, size);
        return functional::take_stack (l, size);
    }

    template <iterable T> 
    T take (const T &n, size_t size) {
        if (size > data::size (n)) {
            auto z = n;
            return z;
        }
        
        // note: we don't actually know that we can make something of a given size like this. 
        T z (size);
        std::copy (n.begin (), n.begin () + size, z.begin ());
        return z;
    }
    
}

#endif
