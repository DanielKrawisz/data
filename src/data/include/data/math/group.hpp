// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_GROUP
#define DATA_MATH_GROUP

#include <data/concepts.hpp>
#include <data/math/arithmetic.hpp>
#include <data/math/algebra.hpp>
#include <data/function.hpp>

namespace data::math {
    
    template <typename elem, typename op = plus<elem>>
    concept group = requires () {
        {identity<op, elem> {} ()} -> implicitly_convertible_to<elem>;
    } && requires (const elem &a, const elem &b) {
        {op {} (a, b)} -> implicitly_convertible_to<elem>;
        {inverse<op, elem> {} (a, b)} -> implicitly_convertible_to<elem>;
    };
    
}

#endif 
