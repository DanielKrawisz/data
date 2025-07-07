// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_GROUP
#define DATA_MATH_GROUP

#include <data/concepts.hpp>
#include <data/arithmetic.hpp>
#include <data/math/algebra.hpp>

namespace data::math {
    
    template <typename elem, typename op = plus<elem>>
    concept group = requires () {
        {identity<op, elem> {} ()} -> ImplicitlyConvertible<elem>;
    } && requires (const elem &a, const elem &b) {
        {op {} (a, b)} -> ImplicitlyConvertible<elem>;
        {inverse<op, elem> {} (a, b)} -> ImplicitlyConvertible<elem>;
    };
    
}

#endif 
