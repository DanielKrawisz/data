// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include <data/encoding/digits.hpp>
#include <boost/algorithm/string.hpp>

namespace data::math::number::GMP {
    
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
        if (!encoding::integer::valid(s)) throw exception{} << "invalid number string " << s;
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
    
    std::ostream &write(std::ostream& o, const math::N& n, hex::letter_case q) {
        return write(o, math::number::N_bytes<endian::big>(n));
    }
    
    std::ostream &write(std::ostream &o, const math::Z &z, hex::letter_case q, math::number::complement n) {
        if (n != math::number::ones) {
            std::stringstream ss;
            ss << "we don't know how to do " << n << " yet.";
            throw std::invalid_argument{ss.str()};
        }
        return write(o << "0x", math::number::Z_bytes<endian::big>(z), q);
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
            encoding::hexidecimal::write(o, n, encoding::hex::lower, math::number::ones);
            return o;
        }
        if (o.flags() & std::ios::dec) {
            return Z_write_dec(o, n);
        }
        o << &n.MPZ;
        return o;
    }
    
    Z N_read_hex(string_view x) {
        if (encoding::hexidecimal::zero(x)) return N{0};
        return Z_read_hex_positive(x);
    }
    
    Z N_read(string_view x) {
        if (!encoding::natural::valid(x)) throw exception{} << "invalid number string \"" << x << "\"";
        if (encoding::hexidecimal::valid(x)) return N_read_hex(x);
        return Z_read_N_gmp(x);
    }
    
    N N::read(string_view x) {
        return N_read(x);
    }
    
    // inefficient but easier to write and more certain to be correct. 
    N read_bytes_big(bytes_view x) {
        std::stringstream format_stream;
        format_stream << "0x" << encoding::hex::write(x);
        return N{format_stream.str()};
    }
    
    N read_bytes_little(bytes_view x) {
        auto z = bytes{x};
        std::reverse(z.begin(), z.end());
        return read_bytes_big(z);
    }
    
    N read_bytes(bytes_view x, endian::order o) {
        if (o == endian::order::big) return read_bytes_big(x);
        return read_bytes_little(x);
        /* 
        N r{0};
        if (x.size() > 0) { 
            if (o == endian::order::little) {
                r += x[0];
                for (int i = 1; i < x.size(); i++) {
                    r <<= 8;
                    r += x[i];
                }
            } else {
                r += x[x.size() - 1];
                for (int i = x.size() - 2; i >= 0; i--) {
                    r <<= 0;
                    r += x[i];
                }
            }
        }
        return r;*/
    }
    
    N::N(bytes_view x, endian::order o) : Value{read_bytes(x, o).Value} {}
    
    void N_write_big(bytes& b, const N& n) {
        b = *data::encoding::hex::read(data::encoding::hexidecimal::write<encoding::hex::lower>(n).substr(2));
    }
    
    void N_write_little(bytes& b, const N& n) {
        N_write_big(b, n);
        std::reverse(b.begin(), b.end());
    }
        
    void N::write_bytes(bytes& b, endian::order o) const {
        if (o == endian::order::big) {
            N_write_big(b, *this);
            return;
        }
        
        N_write_little(b, *this);
        
        /* I didn't finish this because it was getting too confusing. But it's more efficient. 
        int last = Value.size() - 1;
        while(last >= 0 && Value[last] == 0) last--;
        if (last == -1) return bytes{};
        gmp_uint big_endian = endian::native<gmp_uint, endian::big>::from(Value[last]);
        int extra = 0;
        while (((byte*)(&big_endian))[extra] == 0) extra++;
        size_t size = sizeof(gmp_uint) * (last + 1) - extra;
        bytes b{size, ' '};
        */
    }
        
    Z::operator int64() const {
        if (*this > std::numeric_limits<int64>::max()) throw exception{"too big"};
        if (*this < std::numeric_limits<int64>::min()) throw exception{"too big"};
        return mpz_get_si(MPZ);
    } 
    
    N::operator uint64() const {
        if (__gmp_binary_greater::eval(Value.MPZ, (unsigned long int)(std::numeric_limits<uint64>::max())))
            throw exception{"too big"};
        if (*this < 0) throw exception{"too big"};
        return mpz_get_ui(Value.MPZ);
    } 

    std::ostream& operator<<(std::ostream& o, const N& n) {
        if (o.flags() & std::ios::hex) {
            encoding::hexidecimal::write(o, n);
            return o;
        }
        if (o.flags() & std::ios::dec) {
            Z_write_dec(o, n.Value);
            return o;
        }
        o << &n.Value.MPZ;
        return o;
    }

}
