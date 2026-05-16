// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_LINEAR_SPACE
#define DATA_MATH_LINEAR_SPACE

#include <data/cross.hpp>
#include <data/array.hpp>
#include <data/arithmetic.hpp>
#include <data/math/module.hpp>
#include <data/math/field.hpp>
#include <data/math/infinite.hpp>

namespace data::math::linear {
    
    template <typename F, typename V>
    concept space = algebraic_module<F, V> && field<F>;
    
    using dimension = unsigned_limit<size_t>;
    
    template <typename F, typename V> requires space<F, V> struct dimensions;
    
    template <typename F> 
    struct dimensions<F, F> {
        constexpr static dimension value = 1;
    };
    
    template <typename F, size_t size> 
    struct dimensions<F, array<F, size>> {
        constexpr static dimension value = size;
    };
    
    template <typename F, size_t size, size_t... sizes> 
    struct dimensions<F, array<F, size, sizes...>> {
        constexpr static dimension value = dimensions<F, array<F, sizes...>>::value * size;
    };
    
    template <typename F, typename V1, typename V2> struct contract;
}

namespace data::math::def {
    
    template <math::field F, size_t... sizes>
    struct inverse<plus<array<F, sizes...>>, array<F, sizes...>> {
        array<F, sizes...> operator () (const array<F, sizes...> &a, const array<F, sizes...> &b) {
            return b - a;
        }
    };
    
}

#endif
