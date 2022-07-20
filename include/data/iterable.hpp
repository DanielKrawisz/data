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
    
}

#endif
