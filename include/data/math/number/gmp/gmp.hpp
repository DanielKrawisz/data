// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP
#define DATA_MATH_NUMBER_GMP

#include <data/math/number/gmp/N.hpp>

// Include this file if you want GMP to be used as the
// standard representation of integers and naturals.
namespace data::math::number {
    
    using N = math::number::gmp::N;
    using Z = math::number::gmp::Z;
    
} 

#endif 
