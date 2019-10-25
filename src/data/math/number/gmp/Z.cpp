// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/math/number/gmp/N.hpp>
#include <data/encoding/integer.hpp>

namespace data::math::number::gmp {
    
    Z Z_read_N_gmp(const string& s) {
        Z z{};
        mpz_init(z.MPZ);
        mpz_set_str(z.MPZ, s.c_str(), 0);
        return z;
    }
    
    Z Z_read_N_data(string_view x) {
        if (encoding::decimal::valid(x)) {
            string s{x};
            return Z_read_N_gmp(s);
        } 
        
        if (encoding::hexidecimal::valid(x)) {
            if (encoding::hexidecimal::zero(x)) return Z{0};
            int first_nonzero_index = 2;
            while (true) {
                if (first_nonzero_index == x.size()) return Z{0};
                if (x[first_nonzero_index] != '0') break;
                first_nonzero_index++;
            }
            std::stringstream ss;
            ss << "0x";
            ss << x.substr(first_nonzero_index);
            return Z_read_N_gmp(ss.str());
        }
        
        return Z{}; // shouldn't really happen;
    }
    
    Z Z::read(string_view x) {
        if (!encoding::integer::valid(x)) return Z{};
        return encoding::integer::negative(x) ? Z_read_N_data(x.substr(1)) : Z_read_N_data(x);
    }
    
    void Z_write_dec(std::ostream& o, const Z& n) {
        o << std::dec << n.MPZ;
    }
    
    void Z_write_hex(std::ostream& o, const Z& n) {
        std::stringstream gmp_format_stream;
        gmp_format_stream << std::hex << n.MPZ;
        string gmp_format = gmp_format_stream.str();
        
        bool negative = n < 0;
        if (negative) o << "-";
        
        o << "0x";
        if (gmp_format.size() % 2 == (negative ? 0 : 1)) o << "0";
        o << (negative ? string_view{gmp_format}.substr(1) : gmp_format);
    }
    
}

namespace data::encoding::hexidecimal {
    
    string write(const math::number::gmp::Z& n) {
        std::stringstream ss;
        ss << std::hex << n;
        return ss.str();
    }
    
}

namespace data::encoding::decimal {
    
    string write(const math::number::gmp::Z& n) {
        std::stringstream ss;
        ss << std::dec << n;
        return ss.str();
    }
    
}

std::ostream& operator<<(std::ostream& o, const data::math::number::gmp::Z& n) {
    if (o.flags() & std::ios::hex) {
        data::math::number::gmp::Z_write_hex(o, n);
        return o;
    }
    if (o.flags() & std::ios::dec) {
        data::math::number::gmp::Z_write_dec(o, n);
        return o;
    }
    o << &n.MPZ;
    return o;
}
