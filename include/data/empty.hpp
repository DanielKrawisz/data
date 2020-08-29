// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_EMPTY
#define DATA_EMPTY

#include <concepts>
#include <data/types.hpp>

namespace data {
    
    namespace interface {
        
        template <typename X>
        concept has_empty_method = requires(X x) {
            { x.empty() } -> std::same_as<bool>;
        };
        
    }
    
    namespace meta {
        
        template <typename X> struct is_empty {
            bool operator()(const X&) {
                return true;
            }
            
            bool operator()(const X*) {
                return true;
            }
        };
        
        template <interface::has_empty_method X> struct is_empty<X> {
            bool operator()(const X& x) {
                return x.empty();
            }
            
            bool operator()(const X* x) {
                return x == nullptr ? true : x->empty();
            }
        };
        
    }

    template <typename X>
    inline bool empty(const X x) {
        return meta::is_empty<X>{}(x);
    }

}

#endif

