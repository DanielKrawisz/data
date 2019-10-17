// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/math/number/gmp/N.hpp>
#include <data/encoding/integer.hpp>

namespace data::math::number::gmp {
    
    Z::Z(N n) {
        mpz_set(MPZ, n.Value.MPZ);
    }
    
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

}
