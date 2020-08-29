// Copyright (c) 2018-2020 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_VALID
#define DATA_VALID

#include <concepts>

#include <data/meta/equal.hpp>
#include <data/types.hpp>

namespace data {
    
    namespace interface {
        
        template <typename X>
        concept has_valid_method = requires(X x) {
            {x.valid()} -> std::same_as<bool>;
        };
        
        template <typename X>
        concept has_valid_member = requires(X x) {
            {x.Valid} -> std::same_as<bool>;
        };
        
    }
    
    namespace meta {
        
        template <typename X> struct is_valid {
            bool operator()(const X&) const {
                return true;
            }
            
            bool operator()(const X* x) const {
                return x == nullptr ? false : true;
            }
        };
        
        template <interface::has_valid_member X> struct is_valid<X> {
            bool operator()(const X& x) const {
                return x.Valid;
            }
            
            bool operator()(const X* x) const {
                return x == nullptr ? false : x->Valid;
            }
        };
        
        template <interface::has_valid_method X> struct is_valid<X> {
            bool operator()(const X& x) const {
                return x.valid();
            }
            
            bool operator()(const X* x) const {
                return x == nullptr ? false : x->valid();
            }
        };
    
    }

    template <typename X>
    inline bool valid(const X x) { 
        return meta::is_valid<X>{}(x);
    }

}

#endif
