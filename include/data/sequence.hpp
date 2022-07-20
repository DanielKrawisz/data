// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_SEQUENCE
#define DATA_SEQUENCE

#include <type_traits>
#include <data/empty.hpp>
#include <data/function.hpp>
#include <data/math/ordered.hpp>

namespace data {
    
    namespace interface {
        
        template <typename X, typename Y>
        concept convertible = std::convertible_to<X, Y> || std::convertible_to<Y, X>;
        
        template <typename list, typename elem> 
        concept has_first_method = requires (list x) {
            { x.first() } -> convertible<const elem>;
        };
        
        template <typename list> 
        concept has_rest_method = requires (list x) {
            { x.rest() } -> std::convertible_to<list>;
        };
        
    }
    
    template <typename L, typename elem = std::remove_reference_t<decltype(std::declval<L>().first())>> 
    concept sequence = 
        (interface::has_empty_method<L> || interface::has_size_method<L>) && 
        interface::has_rest_method<L> && 
        interface::has_first_method<L, elem>;
        
    // used to find the element of a sequence or container. 
    template <typename X> struct element;
    
    template <sequence X>
    struct element<X> {
        using type = std::remove_reference_t<decltype(std::declval<X>().first())>;
    };
    
    template <typename X> using element_of = element<X>::type;
    
    namespace meta {
        
        template <typename X> struct rest {
            X operator()(const X& x) {
                return x;
            }
            
            const X* operator()(const X* x) {
                return x;
            }
        };
        
        template <interface::has_rest_method X> struct rest<X> {
            X operator()(const X& x) {
                return x.rest();
            }
            
            const X* operator()(const X* x) {
                return x == nullptr ? nullptr : x->rest();
            }
        };
        
    }

    template <typename X>
    inline const decltype(std::declval<const X>().first()) first(const X& x) {
        return x.first();
    }

    template <typename X>
    inline X rest(const X& x) {
        return meta::rest<X>{}(x);
    }
    
    namespace functional {
        template <typename list, typename element> requires sequence<list, element>
        bool contains(const list& x, const element& e) {
            if (data::empty(x)) return false;
            if (data::first(x) == e) return true;
            return contains(data::rest(x), e);
        }
    
        template <sequence L> 
        std::ostream& write(std::ostream& o, L n) {
            o << string{"{"};
            if (!data::empty(n)) {
                o << n.first();
                L r = n.rest();
                while (!data::empty(r)) {
                    o << string{", "} << r.first();
                    r = r.rest();
                }
            } 
            o << string{"}"};
            return o;
        }
        
    }
    
    template <sequence list> 
    list drop(const list &x, uint32 n) {
        return data::empty(x) || n == 0 ? x : drop(rest(x), n - 1);
    }
    
    template <sequence X>
    X values(const X& x) {
        return x;
    }
    
    template <sequence L> requires ordered<element_of<L>>
    bool sorted(const L &x) {
        return size(x) < 2 ? true : first(x) <= first(rest(x)) && sorted(rest(x));
    }

}

#endif
