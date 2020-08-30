// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ITERABLE
#define DATA_ITERABLE

#include <data/sequence.hpp>
#include <data/indexed.hpp>
#include <data/slice.hpp>
#include <data/encoding/endian/endian.hpp>
#include <data/valid.hpp>

namespace data {
    
    namespace meta {
        
        template <typename X, typename it>
        class is_iterable {
            template <typename T, typename i> static auto test(int) -> typename 
                std::enable_if<std::is_same<decltype(std::declval<T>().begin()), i>::value && 
                    std::is_same<decltype(std::declval<T>().end()), i>::value, yes>::type;
            template <typename, typename> static no test(...);
        public:
            static constexpr bool value = std::is_same<decltype(test<X, it>(0)), yes>::value;
        };
        
        template <typename X, typename it>
        class is_const_iterable {
            template <typename T, typename i> static auto test(int) -> typename 
                std::enable_if<std::is_same<decltype(std::declval<const T>().begin()), i>::value && 
                    std::is_same<decltype(std::declval<const T>().end()), i>::value, yes>::type;
            template <typename, typename> static no test(...);
        public:
            static constexpr bool value = std::is_same<decltype(test<X, it>(0)), yes>::value;
        };
    }
    
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
        
        template <typename X, typename e> requires indexed<X, e>
        struct array : iterable<X> {};
    }
    
}

#endif

