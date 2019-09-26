// Copyright (c) 2018-2019 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_VALID
#define DATA_VALID

#include <data/meta/which.hpp>
#include "types.hpp"

namespace data {
    
    namespace low {
        
        template <typename X>
        struct has_valid_method {
            bool operator()(const X x) const {
                return x.valid();
            }
            
            bool operator()(const X* x) const {
                return x->valid();
            }
        };
        
        template <typename X>
        struct has_valid_member {
            bool operator()(const X x) const {
                return x.Valid;
            }
            
            bool operator()(const X* x) const {
                return x->Valid;
            }
        };
        
        template <typename X>
        struct default_valid {
            bool operator()(const X) const {
                return true;
            }
            
            bool operator()(const X*) const {
                return true;
            }
        };
    
        template <typename X>
        struct check_valid {
            bool operator()(const X x) const {
                return typename data::meta::Which<has_valid_method<X>, default_valid<X>>::result{}(x);
            }
        };
        
        template <typename X>
        struct check_valid<X*> {
            bool operator()(const X* x) const {
                return x != nullptr && typename data::meta::Which<has_valid_method<X>, default_valid<X>>::result{}(x);
            }
        };
        
        template <typename X>
        struct check_valid<ptr<X>> {
            bool operator()(const ptr<X> x) const {
                return x != nullptr && typename data::meta::Which<has_valid_method<X>, default_valid<X>>::result{}(x);
            }
        };
    
    }

    template <typename X>
    inline bool valid(const X x) {      
        return low::check_valid<X>{}(x);
    }

}

#endif
