// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/math/number/gmp/N.hpp>
#include <data/encoding/integer.hpp>

namespace data::math::number::gmp {
    
    N::N(const string& x) : Value{encoding::natural::valid(x) ? x : "invalid"} {}

}
