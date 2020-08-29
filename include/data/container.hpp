// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CONTAINER
#define DATA_CONTAINER

#include <data/sequence.hpp>

namespace data {
    
    namespace interface {
        
        template <typename X, typename E> 
        concept has_values_method = requires (X x) {
            { x.values() } -> sequence<E>;
        };
        
        template <typename X, typename element> 
        concept has_contains_method = requires (X x, const element e) {
            { x.contains(e) } -> std::same_as<bool>;
        };
    
        template <typename X, typename element>
        concept has_insert_method = requires (X x, const element e) {
            { x.insert(e) } -> std::convertible_to<X>;
        };
        
        template <typename X, typename element>
        concept has_remove_method = requires (X x, const element e) {
            { x.remove(e) } -> std::convertible_to<X>;
        };
        
    }
        
    template <typename X, typename E> 
    concept container = sequence<X, E> || (interface::has_size_method<X> && interface::has_contains_method<X, E>);
    
    namespace meta {
        
        template <typename X, typename E, 
            bool has_contains = interface::has_contains_method<X, E>, 
            bool is_sequence = sequence<X, E>, 
            bool is_iterable = std::ranges::range<X>> struct contains;
        
        template <typename X, typename E> 
        struct contains<X, E, false, false, false> {
            bool operator()(const X& x, const E& e) {
                return false;
            }
            
            bool operator()(const X* x, const E& e) {
                return false;
            }
        };
        
        template <typename X, typename E, bool is_sequence, bool is_iterable> 
        struct contains<X, E, true, is_sequence, is_iterable> {
            bool operator()(const X& x, const E& e) {
                return x.contains(e);
            }
            
            bool operator()(const X* x, const E& e) {
                return x == nullptr ? false : x->contains(e);
            }
        };
        
        template <typename X, typename E, bool is_iterable> 
        struct contains<X, E, false, true, is_iterable> {
            bool operator()(const X& x, const E& e) {
                return functional::contains(x, e);
            }
        };
        
        template <typename X, typename E> 
        struct contains<X, E, false, false, true> {
            bool operator()(const X& x, const E& e) {
                return std::find(x.begin(), x.end(), e) != x.end();
            }
        };
        
    }

    template <typename X>
    inline auto values(const X& x) -> decltype(x.values()) {
        return x.values();
    }

    template <typename X, typename E>
    requires container<X, E> || std::ranges::range<X>
    inline bool contains(const X& x, const E& e) {
        return meta::contains<X, E>{}(x, e);
    }

}

#endif

