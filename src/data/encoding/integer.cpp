// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/encoding/integer.hpp>
#include <data/encoding/hex.hpp>
#include <data/math/number/gmp/gmp.hpp>
#include <data/math/number/bytes/N.hpp>
#include <data/math/number/bytes/Z.hpp>
#include <data/io/unimplemented.hpp>
#include <algorithm>

namespace data::encoding {
    
    namespace decimal {
        
        bytes read(string_view s, endian::order r) {
            if (!valid(s)) throw std::invalid_argument{"not decimal"};
            math::number::N_bytes<endian::big> n{math::number::N{s}};
            if (r == endian::little) std::reverse(n.begin(), n.end());
            return static_cast<bytes>(n);
        }
        
    };
    
    namespace hexidecimal {
        
        std::ostream& write(std::ostream& o, bytes_view b, endian::order r, hex::letter_case q) {
            o << "0x";
            if (r == endian::order::big) for (int i = 0; i < b.size(); i++) o << hex::write(b[i], q);
            else for(int i = b.size() - 1; i >= 0; i--) o << hex::write(b[i], q);
            return o;
        }
        
        bytes read(string_view s, endian::order r) {
            if (!valid(s)) throw std::invalid_argument{"not hexidecimal"};
            bytes b = bytes_view(hex::view{s.substr(2)});
            if (r == endian::big) return b;
            std::reverse(b.begin(), b.end());
            return b;
        }
        
    };
    
    namespace natural {
        
        bytes read(string_view s, endian::order r) {
            if (!valid(s)) throw std::invalid_argument{"not a natural number"};
            if (hexidecimal::valid(s)) return hexidecimal::read(s, r);
            return decimal::read(s, r);
        }
        
    };
    
    namespace integer {
        
        bytes read(string_view s, endian::order r) {
            //std::cout << "reading in integer string \"" << s << "\"" << std::endl;
            if (!valid(s)) throw std::invalid_argument{"not an integer"};
            if (hexidecimal::valid(s)) return hexidecimal::read(s, r);
            if (negative(s)) {
                //std::cout << "reading in negative decimal integer string \"" << s << "\"" << std::endl;
                math::number::Z z{s};
                //std::cout << "reading in Z " << z << std::endl;
                math::number::Z_bytes<endian::little> n{z};
                //std::cout << "reading in Z_bytes " << n << std::endl;
                if (r == endian::big) std::reverse(n.begin(), n.end());
                //std::cout << "reading in negative decimal integer string ; about to return" << std::endl;
                return bytes(bytes_view(n));
            }
            return decimal::read(s, r);
        }
        
    };
    
}


