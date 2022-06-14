// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include <data/math/number/bytes.hpp>
#include <data/math/number/gmp/N.hpp>
#include <boost/algorithm/string.hpp>

namespace data::math::number::GMP {
    
    N N::read(string_view x) {
        if (!encoding::natural::valid(x)) throw std::invalid_argument{string{"invalid number string"} + string{x}};
        if (encoding::hexidecimal::valid(x)) return N(N_bytes<endian::little>::read(x));
        return encoding::read_base<N>(x, 10, encoding::decimal::digit);
    }
        
    Z Z::read(string_view x) {
        if (!encoding::integer::valid(x)) throw std::invalid_argument{string{"invalid number string"} + string{x}};
        if (encoding::hexidecimal::valid(x)) return Z(Z_bytes<endian::little, ones>::read(x));
        if (encoding::decimal::valid(x)) return encoding::read_base<Z>(x, 10, encoding::decimal::digit);
        return -encoding::read_base<Z>(x.substr(1), 10, encoding::decimal::digit);
    }
    
    Z::Z(const N& n) : Z{n.Value} {}
    
    Z Z_read_N_gmp(string_view s) {
        Z z{};
        //mpz_init(z.MPZ);
        mpz_set_str(z.MPZ, std::string{s}.c_str(), 0);
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
    
    Z Z_read_hex_positive(string_view x) {
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
    
    Z Z_read_hex(string_view x) {
        if (encoding::hexidecimal::zero(x)) return Z{0};
        if (encoding::integer::negative(x)) {
            std::stringstream ss;
            ss << "0x01";
            for (int i = 0; i < x.size() - 2; i += 2) ss << "00";
            return Z{N::read(x)} - Z_read_hex_positive(ss.str());
        };
        return Z_read_hex_positive(x);
    }
    
    Z Z::read(string_view s) {
        if (!encoding::integer::valid(s)) throw std::invalid_argument{std::string{"invalid number string "} + std::string{s}};
        if (encoding::hexidecimal::valid(s)) return Z_read_hex(s);
        return encoding::integer::negative(s) ? -Z_read_N_gmp(s.substr(1)) : Z_read_N_gmp(s);
    }
    
    std::ostream& Z_write_dec(std::ostream& o, const Z& n) {
        if (n == 0) return o << "0";
        if (n < 0) return Z_write_dec(o << "-", -n);
        return o << encoding::write_base<N>(abs<Z>{}(n), encoding::decimal::characters());
    }
    
    std::ostream& N_write_dec(std::ostream& o, const N& n) {
        if (n == 0) return o << "0";
        return o << encoding::write_base<N>(n, encoding::decimal::characters());
    }
    
}

namespace data::encoding::decimal {
    
    std::ostream &write(std::ostream& o, const math::N &n) {
        return N_write_dec(o, n);
    }
    
}

namespace data::encoding::hexidecimal {
    
    std::ostream &write(std::ostream &o, const math::Z &n) {
        return write(o, Z_bytes_big(n));
    }
    
    std::ostream &write(std::ostream& o, const math::N& n) {
        return write(o, N_bytes_big(n));
    }
    
}

namespace data::encoding::signed_decimal {
    
    std::ostream &write(std::ostream &o, const math::Z& n) {
        return Z_write_dec(o, n);
    }
    
}

namespace data::math::number::GMP {
        
    std::ostream& operator<<(std::ostream& o, const Z& n) {
        if (o.flags() & std::ios::hex) {
            encoding::hexidecimal::write(o, n);
            return o;
        }
        if (o.flags() & std::ios::dec) {
            return Z_write_dec(o, n);
        }
        o << &n.MPZ;
        return o;
    }
    
    Z N_read_hex(string_view x) {
        if (encoding::hexidecimal::zero(x)) return Z{0};
        return Z_read_hex_positive(x);
    }
    
    Z N_read(string_view x) {
        if (!encoding::natural::valid(x)) throw std::invalid_argument{std::string{"invalid number string "} + std::string{x}};
        if (encoding::hexidecimal::valid(x)) return N_read_hex(x);
        return Z_read_N_gmp(x);
    }
    
    N read_bytes(bytes_view x, endian::order o) {
        N r{0};
        if (x.size() > 0) { 
            if (o == endian::order::big) {
                r += x[0];
                for (int i = 1; i < x.size(); i++) {
                    r <<= 8;
                    r += x[i];
                }
            } else {
                r += x[x.size() - 1];
                for (int i = int(x.size()) - 2; i >= 0; i--) {
                    r <<= 8;
                    r += x[i];
                }
            }
        }
        return r;
    }
    
    N::N(bytes_view x, endian::order o) : Value{read_bytes(x, o).Value} {}
        
    Z::operator int64() const {
        if (*this > std::numeric_limits<int64>::max()) throw std::logic_error{"too big"};
        if (*this < std::numeric_limits<int64>::min()) throw std::logic_error{"too big"};
        return mpz_get_si(MPZ);
    } 
    
    N::operator uint64() const {
        if (__gmp_binary_greater::eval(Value.MPZ, (unsigned long int)(std::numeric_limits<uint64>::max())))
            throw std::logic_error{"too big"};
        if (is_negative(*this)) throw std::logic_error{"too big"};
        return mpz_get_ui(MPZ);
    } 

}
