// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_SIZE
#define DATA_SIZE

#include <concepts>
#include <data/math/infinite.hpp>

namespace data {
    
    namespace interface {
        
        template <typename X>
        concept has_finite_size_method = requires(X x) {
            { x.size() } -> std::same_as<size_t>;
        };
        
        template <typename X>
        concept has_infinite_size_method = requires(X x) {
            { x.size() } -> std::same_as<cardinality>;
        };
        
        template <typename X>
        concept has_size_method = has_finite_size_method<X> || has_infinite_size_method<X>;
        
    }
    
    namespace meta {
        
        template <typename X> struct size {
            math::unsigned_limit<size_t> operator()(const X&) {
                return 0;
            }
            
            math::unsigned_limit<size_t> operator()(const X*) {
                return 0;
            }
        };
        
        template <interface::has_finite_size_method X> struct size<X> {
            size_t operator()(const X& x) {
                return x.size();
            }
            
            size_t operator()(const X* x) {
                return x == nullptr ? 0 : x->size();
            }
        };
        
        template <interface::has_infinite_size_method X> struct size<X> {
            cardinality operator()(const X& x) {
                return x.size();
            }
            
            cardinality operator()(const X* x) {
                return x == nullptr ? 0 : x->size();
            }
        };
        
    }

    template <typename X>
    inline auto size(const X x) -> decltype(meta::size<X>{}(x)) {
        return meta::size<X>{}(x);
    }

}

#endif


