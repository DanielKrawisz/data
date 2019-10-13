// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/math/number/gmp/N.hpp>
#include <data/encoding/integer.hpp>

namespace data::math::number::gmp {
    
    N::N(const string& x) : Value{encoding::natural::valid(x) ? x : "invalid"} {}
    
    string write_hex(const N n) {
        std::stringstream ss;
        ss << std::hex << &n.Value.MPZ;
        return ss.str();
    }
    
    string write_dec(const N n) {
        std::stringstream ss;
        ss << std::dec << &n.Value.MPZ;
        return ss.str();
    }
}
