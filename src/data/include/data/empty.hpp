// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_EMPTY
#define DATA_EMPTY

#include <data/iterable.hpp>
#include <data/size.hpp>

namespace data {
    
    template <typename X> bool empty (const X x);
    
    namespace interface {
        
        template <typename X> concept has_empty_method = requires (X x) {
            { x.empty () } -> Same<bool>;
        };
        
    }
    
    namespace meta {
        
        template <typename X> struct is_empty {};
        
        template <typename X> 
        requires interface::has_empty_method<const X> || interface::has_size_method<const X> || ConstIterable<X>
        struct is_empty<X> {
            bool operator () (const X &x) {
                if constexpr (interface::has_empty_method<const X>) {
                    return x.empty ();
                } else if constexpr (interface::has_size_method<const X>) {
                    return x.size () == 0;
                } else if constexpr (ConstIterable<X>) {
                    return std::empty (x);
                }
            }
            
            bool operator () (const X *x) {
                if (x == nullptr) return true;
                return is_empty<X> {} (*x);
            }
            
            bool operator () (const ptr<X> *x) {
                if (x == nullptr) return true;
                return is_empty<X> {} (*x);
            }
        };
        
    }

    template <typename X> bool inline empty (const X x) {
        return meta::is_empty<X> {} (x);
    }

}

#endif

