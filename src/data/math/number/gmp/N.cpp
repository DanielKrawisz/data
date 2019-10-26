// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/math/number/gmp/N.hpp>
#include <data/encoding/integer.hpp>

namespace data::math::number::gmp {
    
    Z::Z(const N& n) : Z{n.Value} {}
    
    N::N(string_view x) : Value{encoding::integer::negative(x) ? Z{} : Z::read(x)} {}
    
    void N_write_dec(std::ostream& o, const N& n) {
        o << std::dec << n.Value.MPZ;
    }
    
    void N_write_hex(std::ostream& o, const N& n) {
        std::stringstream gmp_format_stream;
        gmp_format_stream << std::hex << n.Value.MPZ;
        string gmp_format = gmp_format_stream.str();
        
        o << "0x";
        if (gmp_format.size() % 2 == 1) o << "0";
        o << gmp_format;
    }
    
}

std::ostream& operator<<(std::ostream& o, const data::math::number::gmp::N& n) {
    if (o.flags() & std::ios::hex) {
        data::math::number::gmp::N_write_hex(o, n);
        return o;
    }
    if (o.flags() & std::ios::dec) {
        data::math::number::gmp::N_write_dec(o, n);
        return o;
    }
    o << &n.Value.MPZ;
    return o;
}

namespace data::encoding::hexidecimal { 
    
    string write(const math::number::gmp::N& n) {
        std::stringstream ss;
        ss << std::hex << n;
        return ss.str();
    }
    
}

namespace data::encoding::decimal {
    
    string write(const math::number::gmp::N& n) {
        std::stringstream ss;
        ss << std::dec << n;
        return ss.str();
    }
    
}
