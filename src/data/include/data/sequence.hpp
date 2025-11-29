// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_SEQUENCE
#define DATA_SEQUENCE

#include <data/concepts.hpp>
#include <data/empty.hpp>
#include <data/function.hpp>
#include <data/ordered.hpp>
#include <data/iterable.hpp>

namespace data {
    
    template <typename list>
    concept has_first_method = requires (list x) {
        { x.first () } -> Reference<> ;
    };
        
    template <typename list>
    concept has_rest_method = requires (list x) {
        { x.rest () } -> ImplicitlyConvertible<const list>;
    };

    template <typename L, typename = void>
    struct first_return {
        using type = void;
    };

    template <typename L>
    struct first_return<L, std::void_t<decltype (std::declval<L> ().first ())>> {
        using type = unref<decltype (std::declval<L> ().first ())>;
    };

    template <typename L>
    using first_return_type = typename first_return<L>::type;

    template <typename L, typename elem = first_return_type<L>>
    concept Sequence = (!Same<elem, void>) && requires (const L x) {
        { data::empty (x) } -> Same<bool>;
    } && has_rest_method<L> && has_first_method<L> &&
        ImplicitlyConvertible<elem, inserted<elem>>;

    // may be thrown when calling first on an empty sequence;
    struct empty_sequence_exception : std::logic_error {
        empty_sequence_exception (): std::logic_error {"first called on an empty sequence"} {}
    };
    
    namespace meta {
        
        template <typename X> struct rest {
            X operator () (const X &x) {
                return x;
            }
            
            const X* operator () (const X *x) {
                return x;
            }
        };
        
        template <has_rest_method X> struct rest<X> {
            X operator () (const X &x) {
                return x.rest ();
            }
            
            const X* operator () (const X *x) {
                return x == nullptr ? nullptr : x->rest ();
            }
        };
        
    }

    template <typename X> requires has_first_method<const X>
    decltype (auto) inline first (X &&x) {
        return x.first ();
    }

    template <typename X> requires has_first_method<X>
    const auto inline rest (const X &x) {
        return x.rest ();
    }

    template <typename X> requires has_first_method<X>
    const auto inline rest (X const *const x) {
        if (x == nullptr) return nullptr;
        return x->rest ();
    }

    template <typename list, typename element>
    requires Sequence<list> && requires (const list x, const element e) {
        { x.first () == e } -> Same<bool>;
    } bool contains (const list &x, const element &e) {
        if (data::empty (x)) return false;
        if (data::first (x) == e) return true;
        return contains (data::rest (x), e);
    }
    
    namespace functional {

        template <Sequence L> 
        std::ostream &write (std::ostream &o, L n) {
            o << "{";
            if (!data::empty (n)) {
                o << n.first ();
                L r = n.rest ();
                while (!data::empty (r)) {
                    o << ", " << r.first ();
                    r = r.rest ();
                }
            } 
            o << "}";
            return o;
        }
        
    }
    
    template <Sequence list> 
    list drop (const list &x, uint32 n) {
        return data::empty (x) || n == 0 ? x : drop (rest (x), n - 1);
    }

    template <Sequence X, Sequence Y> 
    requires std::equality_comparable_with<decltype (first (std::declval<const X> ())), decltype (first (std::declval<const Y> ()))>
    bool inline sequence_equal (const X &a, const Y &b) {
        return (void*) &a == (void*) &b ? true :
            data::size (a) != data::size (b) ? false :
                data::empty (a) ? true :
                    data::first (a) != data::first (b) ? false :
                        data::rest (a) == data::rest (b);
    }

}

#endif
