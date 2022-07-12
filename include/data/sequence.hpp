// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_SEQUENCE
#define DATA_SEQUENCE

#include <type_traits>
#include <data/empty.hpp>
#include <data/function.hpp>
#include <data/math/ordered.hpp>
#include <data/iterable.hpp>

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
    
    template <sequence L> requires ordered<element_of<L>>
    bool sorted(const L &x) {
        return size(x) < 2 ? true : first(x) <= first(rest(x)) && sorted(rest(x));
    }
    
}

template <data::sequence X, data::sequence Y> requires std::equality_comparable_with<data::element_of<X>, data::element_of<Y>> 
bool inline operator==(const X &a, const Y &b) {
    return (void*)&a == (void*)&b ? true : 
        data::empty(a) && data::empty(b) ? true : 
            data::empty(a) || data::empty(b) ? false : 
                data::first(a) != data::first(b) ? false : 
                    data::rest(a) == data::rest(b);
}

namespace data {
    
    // iterator types for a sequence     
    template <typename L> 
    struct sequence_iterator {
        const L *Sequence;
        L Next;
        int Index;
        
        sequence_iterator(const L &s, L n, int i) : Sequence{&s}, Next{n}, Index{i} {}
        
        // we need this constructor in order to satisfy some 
        // std concepts but it's not really good for anything. 
        sequence_iterator() : Sequence{}, Next{}, Index{0} {}
        
        sequence_iterator &operator=(const sequence_iterator&);
        
        sequence_iterator operator++(int);
        sequence_iterator &operator++();
        
        const element_of<L> &operator*() const;
        
        bool operator==(const sentinel<L> i) const;
        bool operator!=(const sentinel<L> i) const;
        bool operator==(const sequence_iterator i) const;
        
        int operator-(const sequence_iterator& i) const;
        
        sequence_iterator(const L &s) : Sequence{&s}, Next{s}, Index{0} {}
    };
}

namespace std {
    template <typename list> 
    struct iterator_traits<data::sequence_iterator<list>> {
        using value_type = remove_const_t<data::element_of<list>>;
        using difference_type = int;
        using pointer = const remove_reference_t<data::element_of<list>>*;
        using reference = const data::element_of<list>&;
        using iterator_concept = input_iterator_tag;
    };
}

namespace data {
    
    template <sequence L> 
    inline sequence_iterator<L>& sequence_iterator<L>::operator=(const sequence_iterator& i) {
        Sequence = i.Sequence;
        Next = i.Next;
        Index = i.Index;
        return *this;
    }
    
    template <sequence L> 
    inline sequence_iterator<L>& sequence_iterator<L>::operator++() { // Prefix
        if (Sequence == nullptr || data::empty(Next)) return *this;
        return *this = sequence_iterator{*Sequence, data::rest(Next), Index + 1};
    }
    
    template <sequence L> 
    inline sequence_iterator<L> sequence_iterator<L>::operator++(int) { // Postfix
        sequence_iterator n = *this;
        ++(*this);
        return n;
    }
    
    template <sequence L> 
    const element_of<L> inline &sequence_iterator<L>::operator*() const {
        return data::first(Next);
    }
    
    template <sequence L> 
    bool inline sequence_iterator<L>::operator==(const sequence_iterator i) const {
        return Sequence == i.Sequence && Index == i.Index;
    }
    
    template <sequence L> 
    bool inline sequence_iterator<L>::operator==(const sentinel<L> i) const {
        return Sequence == i.Structure && Index == data::size(*Sequence);
    }
    
    template <sequence L> 
    bool inline sequence_iterator<L>::operator!=(const sentinel<L> i) const {
        return !(*this == i);
    }
    
    template <sequence L> 
    int inline sequence_iterator<L>::operator-(const sequence_iterator& i) const {
        if (Sequence != i.Sequence) return 0;
        return static_cast<int>(Index) - i.Index;
    }

}

#endif
