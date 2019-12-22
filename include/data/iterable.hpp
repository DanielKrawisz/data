// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ITERABLE
#define DATA_ITERABLE

#include <data/interface.hpp>

namespace data {
    
    namespace interface {
    
        template <typename X>
        struct iterable {
            using iterator = decltype(std::declval<X>().begin());
            using const_iterator = decltype(std::declval<const X>().begin());
        private:
            using require_iterable = typename std::enable_if<meta::is_iterable<X, iterator>::value && meta::is_const_iterable<X, const_iterator>::value, bool>::type;
        };
    
        template <typename X>
        struct const_iterable {
            using const_iterator = decltype(std::declval<const X>().begin());
        private:
            using require_const_iterable = typename std::enable_if<meta::is_const_iterable<X, const_iterator>::value, bool>::type;
        };
        
        template <typename X, typename e>
        struct array : iterable<X>, indexed<X, e> {};
    }
    
    namespace container { 
        // A way of treating arrays as containers (std::array is not a container)
        // anything that behaves like an array can be consistently turned
        // into something that can be inhereted from. 
        template <typename indexed, typename elem, uint32 n>
        struct array {
            
            indexed Array;
            
            using iterator = typename interface::iterable<indexed>::iterator;
            using const_iterator = typename interface::iterable<indexed>::const_iterator;
            
            constexpr iterator begin() {
                return Array.begin();
            }
            
            constexpr iterator end() {
                return Array.end();
            }
            
            constexpr const_iterator begin() const {
                return Array.begin();
            }
            
            constexpr const_iterator end() const {
                return Array.end();
            }
            
            using index = size_t;
            
            static const index size = n;
            static const index last = n - 1;
            
            constexpr elem& operator[](index i) {
                return Array[i];
            }
            
            constexpr const elem& operator[](index i) const {
                return Array[i];
            }
                
            array() : Array{} {}
            array(const indexed& a) : Array{a} {}
            
            static array zero() {
                return array{};
            }
                
            operator bytes_view() const {
                return bytes_view{Array.data(), n};
            }
        };
    }
}

#endif

