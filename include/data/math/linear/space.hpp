// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_LINEAR_SPACE
#define DATA_MATH_LINEAR_SPACE

#include <data/math/module.hpp>
#include <data/math/field.hpp>

namespace data::interface::linear {
    
    template <typename vector, typename field>
    struct space : module<field, vector>, interface::field<field> {};
    
}

#endif
