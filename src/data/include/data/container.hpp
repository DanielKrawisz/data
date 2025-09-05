// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CONTAINER
#define DATA_CONTAINER

#include <data/sequence.hpp>
#include <data/iterable.hpp>
#include <data/functional/list.hpp>

namespace data {
    
    namespace interface {
        
        template <typename X> 
        concept has_values_method = requires (const X x) {
            { x.values () } -> Sequence<>;
        };
        
        template <typename X, typename element> 
        concept has_contains_method = requires (const X x, const element e) {
            { x.contains (e) } -> ImplicitlyConvertible<bool>;
        };
    
        template <typename X, typename element>
        concept has_insert_method = requires (const X x, const element e) {
            { x.insert (e) } -> ImplicitlyConvertible<const X>;
        };
        
        template <typename X, typename element>
        concept has_remove_method = requires (const X x, const element e) {
            { x.remove (e) } -> ImplicitlyConvertible<const X>;
        };
        
    }

    template <typename X, typename E> 
    concept Container = Sequence<X, E> || (interface::has_size_method<X> && interface::has_contains_method<X, E>);

    template <typename X> requires interface::has_values_method<X>
    auto values (const X &x) {
        return x.values ();
    }

    template <typename X, typename E>
    requires (!Sequence<X>) && (Container<X, E> || (ConstIterable<X> && requires (const X x, const E e) {
        { *x.begin () == e } -> Same<bool>;
    }))
    bool inline contains (const X &x, const E &e) {
        if constexpr (requires () {
            { x.contains (e) } -> Same<bool>;
        }) {
            return x.contains (e);
        } else if constexpr (has_rest_method<X> && requires () {
            { x.first () == e } -> Same<bool>;
        }) {
            auto xx = x;
            while (!data::empty (xx)) {
                if (first (xx) == e) return true;
                xx = rest (xx);
            }
            return false;
        } if constexpr (ConstIterable<X>) {
            return std::find (x.begin (), x.end (), e) != x.end ();
        } else {
            throw "cannot construct contains method";
        }
    }
    
    // we already have a remove for Pendable types.
    template <typename X, typename E> 
    X remove (const X &x, const E &e) {
        if constexpr (requires () {
            { x.remove (e) } -> ImplicitlyConvertible<const X>;
        }) {
            return x.remove (e);
        } else {
            throw data::exception {} << "cannot construct remove method";
        }
    }

}

#endif

