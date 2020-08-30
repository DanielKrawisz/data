// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_LIST
#define DATA_LIST

#include <data/functional/stack.hpp>
#include <data/functional/queue.hpp>

namespace data::functional {
    
    template <typename Q, typename elem = std::remove_reference_t<decltype(std::declval<Q>().first())>>
    concept list = stack<Q, elem> && queue<Q, elem>;
    
}

namespace data::meta {
    
    template <functional::queue list>
    list take_queue(list l, uint32 x) {
        if (x >= l.size()) return l;
        list n = l;
        list r{};
        while (x > 0) {
            r = data::append(r, n.first());
            n = n.rest();
            x--;
        }
        return r;
    }
    
    template <functional::stack list>
    list take_stack(list l, uint32 x) {
        if (x >= l.size()) return l;
        list n = l;
        list r{};
        while (x > 0) {
            r = data::prepend(r, n.first());
            n = n.rest();
            x--;
        }
        return data::reverse(r);
    }
    
    template <typename list>
    struct take {
        list operator()(const list& l, uint32 x) {
            return take_stack(l, x);
        }
    };
    
    template <functional::list list>
    struct take<list> {
        list operator()(const list& l, uint32 x) {
            return take_queue(l, x);
        }
    };
    
    template <functional::queue list>
    struct take<list> {
        list operator()(const list& l, uint32 x) {
            return take_queue(l, x);
        }
    };
    
    template <typename L, typename engine>
    L shuffle(const L x, engine& e) {
        L q = x;
        L z{};
        while (!data::empty(q)) {
            q = rotate_left(q, std::uniform_int_distribution<int>(0, q.size() - 1)(e));
            z = z << q.first();
            q = q.rest();
        }
        return z;
    }
    
    template <typename L>
    L shuffle(const L x) {
        return shuffle(x, get_random_engine());
    }
}

namespace data {
    
    template <functional::list L>
    inline L rotate_right(const L x) {
        size_t s = size(x);
        if (s == 0 || s == 1) return x; 
        
        return prepend(rest(x), first(x));
    }
    
    template <functional::list list> 
    list take(list l, uint32 x) {
        return meta::take<list>{}(l, x);
    }
    
    template <functional::list list>
    list join(const list&a, const list& b) {
        if (b.empty()) return a;
        return join(a << b.first(), b.rest());
    }
}

#endif

