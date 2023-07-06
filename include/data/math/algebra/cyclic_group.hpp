// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ALGEBRA_CYCLIC_GROUP
#define DATA_MATH_ALGEBRA_CYCLIC_GROUP

#include <data/valid.hpp>

#include <data/math/number/modular.hpp>
#include <data/math/group.hpp>
#include <data/math/commutative.hpp>
#include <data/math/associative.hpp>

namespace data::math::algebra {
    
    template <typename N, auto & natural> using cyclic_group = number::modular<N, natural>;
    
}

#endif
