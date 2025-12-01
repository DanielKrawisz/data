// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CONTAINER
#define DATA_CONTAINER

#include <data/sequence.hpp>
#include <data/iterable.hpp>
#include <data/functional/list.hpp>
#include <data/functional/tree.hpp>

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
    concept Set = (interface::has_size_method<X> && interface::has_contains_method<X, E>);

    template <typename X, typename E> 
    concept Container = Sequence<X, E> || Set<X, E>;

    template <typename X, typename E>
    concept Sack = Container<X, E> && std::default_initializable<X> &&
    interface::has_insert_method<X, E>;

    template <typename X, typename element>
    concept OrderedSet = Set<X, element> && interface::has_values_method<X> && Sortable<element>;

    template <typename X, typename element>
    concept Heap = Tree<X, element> && Set<X, element> &&
        interface::has_values_method<X> && Prioritized<element> &&
        Sequence<X, element> && Sack<X, element>;

    template <typename M> concept IterableSet =
        ConstIterable<M> && Set<M, decltype (*std::declval<const M> ().begin ())>;

    template <typename M> concept IterableStack =
        ConstIterable<M> && Stack<M, decltype (*std::declval<const M> ().begin ())>;

    template <typename M> concept IterableQueue =
        ConstIterable<M> && Queue<M, decltype (*std::declval<const M> ().begin ())>;

    template <typename M> concept IterableList =
        ConstIterable<M> && List<M, decltype (*std::declval<const M> ().begin ())>;

    template <typename M> concept IterableSack =
        ConstIterable<M> && Sack<M, decltype (*std::declval<const M> ().begin ())>;

    template <typename X> requires interface::has_values_method<X>
    auto values (const X &x) {
        return x.values ();
    }

    // we have a contains method for sequences, so we have to
    // specifically say not to use it.
    template <typename X, typename E> requires (!Sequence<X>)
    bool inline contains (const X &x, E &&e) {
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
            throw exception {} << "cannot construct contains method";
        }
    }

}

#endif

