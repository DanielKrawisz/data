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
    
    // iterator types for a sequence
    // TODO remove this.
    template <typename L> struct sequence_iterator;

    template <typename L> bool operator == (const sequence_iterator<L> &, const sentinel<L>);
    template <typename L> bool operator == (const sequence_iterator<L> &, const sequence_iterator<L> &);

    template <typename L> struct sequence_iterator {

        using iterator_category = std::forward_iterator_tag;
        using value_type        = unref<decltype (first (std::declval<L> ()))>;
        using difference_type   = int;
        using pointer           = value_type *;
        using reference         = value_type &;

        const L *Sequence;
        std::remove_const_t<L> Next;
        int Index;
        
        sequence_iterator (const L &s, L n, int i) : Sequence {&s}, Next {n}, Index {i} {}
        
        // we need this constructor in order to satisfy some 
        // std concepts but it's not really good for anything. 
        sequence_iterator () : Sequence {}, Next {}, Index {0} {}
        
        sequence_iterator &operator = (const sequence_iterator &);
        
        sequence_iterator operator ++ (int);
        sequence_iterator &operator ++ ();
        
        reference operator * () const;
        pointer operator -> () const;
        
        int operator - (const sequence_iterator &i) const;
        
        sequence_iterator (const L &s) : Sequence {&s}, Next {s}, Index {0} {}
    };
    
    template <typename L>
    inline sequence_iterator<L> &sequence_iterator<L>::operator = (const sequence_iterator &i) {
        Sequence = i.Sequence;
        Next = i.Next;
        Index = i.Index;
        return *this;
    }
    
    template <typename L>
    inline sequence_iterator<L> &sequence_iterator<L>::operator ++ () { // Prefix
        if (Sequence == nullptr || data::empty (Next)) return *this;
        return *this = sequence_iterator {*Sequence, data::rest (Next), Index + 1};
    }
    
    template <typename L>
    inline sequence_iterator<L> sequence_iterator<L>::operator ++ (int) { // Postfix
        sequence_iterator n = *this;
        ++(*this);
        return n;
    }
    
    template <typename L>
    sequence_iterator<L>::reference inline sequence_iterator<L>::operator * () const {
        return data::first (Next);
    }

    template <typename L>
    sequence_iterator<L>::pointer inline sequence_iterator<L>::operator -> () const {
        return &data::first (Next);
    }

    template <typename L> bool operator == (const sequence_iterator<L> &i, const sentinel<L> j) {
        return i.Sequence == j.Structure && i.Index == data::size (*j.Structure);
    }

    template <typename L> bool operator == (const sequence_iterator<L> &i, const sequence_iterator<L> &j) {
        return i.Sequence == j.Sequence && i.Index == j.Index;
    }
    
    template <typename L>
    int inline sequence_iterator<L>::operator - (const sequence_iterator &i) const {
        if (Sequence != i.Sequence) return 0;
        return static_cast<int> (Index) - i.Index;
    }

}

#endif
