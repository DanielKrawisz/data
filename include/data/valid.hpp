// Copyright (c) 2018-2019 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_VALID
#define DATA_VALID

#include <data/meta/equal.hpp>
#include "types.hpp"

namespace data {
    
    namespace meta {
        
        template <typename X, bool has_valid_member, bool has_valid_method> struct is_valid;
        
        template <typename X, bool has_valid_method> struct is_valid<X, true, has_valid_method> {
            bool operator()(const X x) const {
                return x.Valid;
            }
            
            bool operator()(const X* x) const {
                return x->Valid;
            }
        };
        
        template <typename X> struct is_valid<X, false, true> {
            bool operator()(const X x) const {
                return x.valid();
            }
            
            bool operator()(const X* x) const {
                return x->valid();
            }
        };
        
        template <typename X> struct is_valid<X, false, false> {
            bool operator()(const X) const {
                return true;
            }
            
            bool operator()(const X*) const {
                return true;
            }
        };
        
        template <typename X, typename = int> struct has_valid_member {
            constexpr static bool value = false;
        };

        template <typename X>
        struct has_valid_member<X, decltype((void) X::Valid, 0)> {
            constexpr static bool value = true;
        };
        
        template <typename X, typename = int> struct has_valid_method {
            constexpr static bool value = false;
        };

        template <typename X>
        struct has_valid_method<X, decltype((void) X::valid, 0)> {
            constexpr static bool value = true;
        };
        
        template <typename X> 
        struct valid {
            bool operator()(const X x) {
                return is_valid<X, has_valid_member<X>::value, has_valid_method<X>::value>{}(x);
            }
        };
        
        template <typename X> 
        struct valid<X*> {
            bool operator()(const X* x) {
                return is_valid<X, has_valid_member<X>::value, has_valid_method<X>::value>{}(x);
            }
        };
    
    }

    template <typename X>
    inline bool valid(const X x) { 
        return meta::valid<X>{}(x);
    }

}

#endif
