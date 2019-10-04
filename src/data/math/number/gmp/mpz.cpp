// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/math/number/gmp/mpz.hpp>

namespace data::math::number::gmp {
    
    __mpz_struct read_decimal(const string&) {
        throw method::unimplemented{};
    }
    
    __mpz_struct read_hexidecimal(const string&) {
        throw method::unimplemented{};
    }
    
}
