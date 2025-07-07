// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ITERABLE
#define DATA_ITERABLE

#include <ranges>
#include <data/concepts.hpp>

namespace data {
    
    template <typename X, typename elem = unref<unconst<decltype (*std::declval<X> ().begin ())>>>
    concept const_iterable = std::ranges::input_range<X> && requires (const X x) {
        { *x.begin () } -> ImplicitlyConvertible<const elem>;
        { x.end () };
    };
    
    template <typename X, typename elem = decltype (*std::declval<const X> ().begin ())>
    concept iterable = const_iterable<X, elem> && std::ranges::output_range<X, elem> && requires () {
        typename X::iterator;
    };
    
    // used to represent the end of a data structure. 
    template <typename X> 
    struct sentinel {
        const X *Structure;
        sentinel (): Structure {} {}
        sentinel (const X &s) : Structure {&s} {}
    };
    
    template <typename iterator>
    struct reverse_iterator {
        iterator It;
        
        reverse_iterator (iterator it) : It {it} {}
        reverse_iterator () : It {} {}
        
        const auto &operator * () const {
            return *It;
        }
        
        reverse_iterator &operator ++ ();
        reverse_iterator &operator -- ();
        
        reverse_iterator operator ++ (int);
        reverse_iterator operator -- (int);
        
        bool operator == (const reverse_iterator i) const;
        
        int operator - (const reverse_iterator &i) const;
    };

    template <typename... its> struct product_range;
    
}

namespace std {
    template <typename elem> 
    struct iterator_traits<data::reverse_iterator<elem>> {
        using value_type = remove_const_t<elem>;
        using difference_type = int;
        using pointer = const remove_reference_t<elem> *;
        using reference = const elem &;
        using iterator_concept = input_iterator_tag;
    };
}

namespace data {
    
    template <typename iterator>
    reverse_iterator<iterator> inline &reverse_iterator<iterator>::operator ++ () {
        return {--It};
    }
    
    template <typename iterator>
    reverse_iterator<iterator> inline &reverse_iterator<iterator>::operator -- () {
        return {++It};
    }
    
    template <typename iterator>
    reverse_iterator<iterator> reverse_iterator<iterator>::operator ++ (int) {
        auto x = *this;
        --(*this);
        return x;
    }
    
    template <typename iterator>
    reverse_iterator<iterator> reverse_iterator<iterator>::operator -- (int) {
        auto x = *this;
        ++(*this);
        return x;
    }
    
    template <typename iterator>
    bool inline reverse_iterator<iterator>::operator == (const reverse_iterator i) const {
        return It == i;
    }
    
    template <typename iterator>
    int inline reverse_iterator<iterator>::operator - (const reverse_iterator &i) const {
        return -(It - i.It);
    }
    
}

#endif
