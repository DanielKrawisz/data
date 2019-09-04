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
    
    template <typename L>
    struct is_iterable {
        using iterator = decltype(std::declval<L>().begin());
        using const_iterator = decltype(std::declval<const L>().begin());
        constexpr static iterable<L, iterator, const_iterator> Requirement{};
    };
    
    // A way of treating arrays as containers (std::array is not a container)
    // anything that behaves like an array can be consistently turned
    // into something that can be inhereted from. 
    template <typename indexed, typename elem, uint32 n>
    struct array : public container::is_iterable<indexed> {
        indexed Array;
        
        using iterator = typename container::is_iterable<indexed>::iterator;
        using const_iterator = typename container::is_iterable<indexed>::const_iterator;
        using returned = typename returned<elem>::type;
        
        iterator begin() {
            return Array.begin();
        }
        
        iterator end() {
            return Array.end();
        }
            
        const_iterator begin() const {
            return Array.begin();
        }
            
        const_iterator end() const {
            return Array.end();
        }
            
        using index = uint32;
        
        static const index size = n;
        static const index last = n - 1;
        
        returned operator[](index i) {
            return Array[i];
        }
            
        const returned operator[](index i) const {
            return Array[i];
        }
            
        array() : Array{} {}
        array(indexed& a) : Array{a} {}
        
        static array zero() {
            return array{};
        }
    };
}

#endif

