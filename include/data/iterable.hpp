// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ITERABLE
#define DATA_ITERABLE

#include <ranges>

namespace data {
    
    template <typename X, typename elem = std::remove_const_t<decltype(*std::declval<X>().begin())>> 
    concept const_iterable = std::ranges::input_range<X> && requires(const X x) {
        { *x.begin() } -> std::convertible_to<const elem>;
    };
    
    template <typename X, typename elem = decltype(*std::declval<const X>().begin())> 
    concept iterable = const_iterable<X, elem> && std::ranges::output_range<X, elem> && requires() {
        typename X::iterator;
    };
    
    template <typename iterator>
    struct reverse_iterator {
        iterator It;
        
        reverse_iterator(iterator it) : It{it} {}
        reverse_iterator() : It{} {}
        
        const auto &operator*() const {
            return *It;
        }
        
        reverse_iterator &operator++() {
            return {--It};
        }
        
        reverse_iterator &operator--() {
            return {++It};
        }
        
        reverse_iterator operator++(int) {
            auto x = *this;
            --(*this);
            return x;
        }
        
        reverse_iterator operator--(int) {
            auto x = *this;
            ++(*this);
            return x;
        }
        
        bool operator==(const reverse_iterator i) const {
            return It == i;
        }
        
        int operator-(const reverse_iterator& i) const {
            return -(It - i.It);
        }
    };
    
}

namespace std {
    template <typename elem> 
    struct iterator_traits<data::reverse_iterator<elem>> {
        using value_type = remove_const_t<elem>;
        using difference_type = int;
        using pointer = const remove_reference_t<elem>*;
        using reference = const elem&;
        using iterator_concept = input_iterator_tag;
    };
}

#endif
