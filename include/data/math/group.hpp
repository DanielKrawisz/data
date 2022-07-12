// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_GROUP
#define DATA_MATH_GROUP

#include <data/math/associative.hpp>
#include <data/math/commutative.hpp>
#include <data/math/arithmetic.hpp>
#include <data/function.hpp>

namespace data::math {
    
    template <typename elem, typename op = plus<elem>>
    concept group = requires(){
        typename associative<op, elem>; 
        {identity<op, elem>{}()} -> std::convertible_to<elem>; 
    } && requires (const elem &a, const elem &b) {
        {op{}(a, b)} -> std::convertible_to<elem>;
        {inverse<op, elem>{}(a, b)} -> std::convertible_to<elem>; 
    };
    
    template <typename elem, typename op = elem>
    concept abelian = group<elem, op> && requires() {
        typename math::commutative<op, elem>;
    };
    
}

#endif 
