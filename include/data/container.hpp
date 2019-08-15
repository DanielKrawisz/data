// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CONTAINER
#define DATA_CONTAINER

#include <data/types.hpp>

namespace data::container {
    
    template <typename elem> struct returned {
        using type = elem&;
    };
    
    template <typename elem> struct returned<elem&> {
        using type = elem&;
    };
    
    template <typename elem> struct returned<elem*> {
        using type = elem*;
    };
    
    template <typename elem> struct returned<ptr<elem>> {
        using type = ptr<elem>;
    };
    
    template <typename X>
    struct existence {
        bool empty(const X x) const {
            return x.empty();
        }
        
        uint32 size(const X x) const {
            return x.size();
        }
    };
    
    template <typename X>
    struct existence<X*> {
        bool empty(const X* const x) const {
            return x == nullptr;
        }
        
        uint32 size(const X* const x) const {
            if (x == nullptr) return 0;
            return x->size();
        }
    };
    
    template <typename X>
    struct existence<ptr<X>> : virtual public existence<X*> {};
    
    template <typename X, typename it, typename const_it>
    struct iterable {
        using iterator = it;
        using const_iterator = const_it;
        
        iterator begin(X x) {
            return x.begin();
        }
        
        iterator end(X x) {
            return x.end();
        }
        
        const_iterator begin(const X x) const {
            return x.begin();
        }
        
        const_iterator end(const X x) const {
            return x.end();
        }
    };
    
    template <typename X, typename it, typename const_it>
    struct iterable<X*, it, const_it> {
        using iterator = it;
        using const_iterator = const_it;
        
        iterator begin(X* x) {
            if (x == nullptr) return iterator{};
            return x->begin();
        }
        
        iterator end(X* x) {
            if (x == nullptr) return iterator{};
            return x->end();
        }
        
        const_iterator begin(const X* const x) const {
            if (x == nullptr) return const_iterator{};
            return x->begin();
        }
        
        const_iterator end(const X* const x) const {
            if (x == nullptr) return const_iterator{};
            return x->end();
        }
    };
    
    template <typename L, typename it, typename const_it>
    struct iterable<ptr<L>, it, const_it> : public iterable<L*, it, const_it> {};
    
}

#endif

