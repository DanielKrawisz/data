// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/encoding/integer.hpp>
#include <data/encoding/hex.hpp>
#include <data/encoding/digits.hpp>
#include <data/math/number/gmp/gmp.hpp>
#include <data/math/number/bytes/N.hpp>
#include <data/math/number/bytes/Z.hpp>
#include <data/io/unimplemented.hpp>
#include <algorithm>

namespace data::encoding {
    
    namespace decimal {
        
        using nat = math::N;
        
        ptr<bytes> read(string_view s, endian::order r) {
            if (!valid(s)) return nullptr;
            math::number::N_bytes<endian::big> n{nat{s}};
            if (r == endian::little) std::reverse(n.begin(), n.end());
            return std::make_shared<bytes>(static_cast<bytes>(n));
        }
        
        template <typename N>
        std::string write_decimal(const N& n) {
            static std::string Characters = characters();
            if (n == 0) return "0";
            return write_base<N>(n, Characters);
        }
        
        string::string(uint64 x) : string{write_decimal(nat{x})} {}
        
        inline nat read_num(const string& n) {
            return read_base<nat>(n, 10, &digit);
        }
        
        string string::operator+(const string& n) const {
            return string{write_decimal(read_num(*this) + read_num(n))};
        }
        
        string string::operator-(const string& n) const {
            return string{write_decimal(read_num(*this) - read_num(n))};
        }
        
        string string::operator*(const string& n) const {
            return string{write_decimal(read_num(*this) * read_num(n))};
        }
        
        string string::operator<<(int i) const {
            return string{write_decimal(read_num(*this) << i)};
        }
        
        string string::operator>>(int i) const {
            return string{write_decimal(read_num(*this) >> i)};
        }
        
        math::division<string, uint64> string::divide(uint64 x) const {
            if (x == 0) throw math::division_by_zero{};
            // it is important to have this optimization. 
            if (x == 10) {
                int last = string::size() - 1;
                return math::division<string, uint64>{
                    string{string::substr(0, last)}, 
                    static_cast<uint64>(digit(string::operator[](last)))};
            }
            
            math::division<nat> div = read_num(*this).divide(nat{x});
            return math::division<string, uint64>{string{write_decimal(div.Quotient)}, uint64(div.Remainder)};
        }
    
    }
    
    namespace hexidecimal {
        
        template <typename N>
        std::string write_hexidecimal(const N& n) {
            static std::string Characters = hex::characters_lower();
            if (n == 0) return "0x";
            std::string p = write_base<N>(n, Characters);
            if ((p.size() % 2) == 1) return std::string{"0x0"} + p;
            return std::string{"0x"} + p;
        }
        
    }
    
}


