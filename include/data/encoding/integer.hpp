// Copyright (c) 2019 Daniel Krawisz
// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_INTEGER
#define DATA_ENCODING_INTEGER

#include <ctre.hpp>

#include <data/types.hpp>
#include <data/encoding/hex.hpp>
#include <data/encoding/invalid.hpp>
#include <data/encoding/endian/endian.hpp>
#include <data/math/division.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string.hpp>

namespace data::math::number {
    
    template <endian::order r> struct N_bytes;
    template <endian::order r> struct Z_bytes;
    
}

namespace data::math {
    
    template <endian::order r> using N_bytes = number::N_bytes<r>;
    template <endian::order r> using Z_bytes = number::Z_bytes<r>;
    
}

namespace data {
    
    using N_bytes_little = math::N_bytes<endian::order::little>;
    using N_bytes_big = math::N_bytes<endian::order::big>;
    
    using Z_bytes_little = math::Z_bytes<endian::order::little>;
    using Z_bytes_big = math::Z_bytes<endian::order::big>;
    
}

namespace data::encoding {
    
    namespace decimal {
        static constexpr ctll::fixed_string pattern{"0|([1-9][0-9]*)"};
        
        inline std::string characters() {
            return "0123456789";
        }
        
        inline char digit(char x) {
            return x < '0' || x > '9' ? -1 : x - '0';
        }
        
        inline bool valid(string_view s) {
            return ctre::match<pattern>(s);
        } 
        
        inline bool nonzero(string_view s) {
            return valid(s) && s[0] != '0';
        }
        
        inline uint32 digits(string_view s) {
            return valid(s) ? s.size() : 0;
        }
        
        template <endian::order r> ptr<math::N_bytes<r>> read(string_view s);
        
        template <typename range> 
        std::ostream &write(std::ostream& o, range r);
        
        template <typename range> 
        std::string inline write(range r) {
            std::stringstream ss;
            write(ss, r);
            return ss.str();
        }
        
        struct N : string {
            N();
            N(const string&);
            N(uint64);
            
            bool valid() const {
                return decimal::valid(*this);
            }
            
            bool operator<=(const N&) const;
            bool operator>=(const N&) const;
            bool operator<(const N&) const;
            bool operator>(const N&) const;
        
            N& operator++();
            N& operator--();
            
            N operator++(int);
            N operator--(int);
            
            N operator+(const N&) const;
            N operator-(const N&) const;
            N operator*(const N&) const;
            
            N operator<<(int) const;
            N operator>>(int) const;
            
            N& operator+=(const N&);
            N& operator-=(const N&);
            N& operator*=(const N&);
            
            N& operator<<=(int);
            N& operator>>=(int);
            
            math::division<N, uint64> divide(uint64) const;
        };
        
    }
    
    namespace hexidecimal {
        static constexpr ctll::fixed_string pattern{"0x((([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*))"};
        static constexpr ctll::fixed_string zero_pattern{"0x(00)*"};
        
        inline bool valid(string_view s) {
            return ctre::match<pattern>(s);
        } 
        
        inline bool zero(string_view s) {
            return ctre::match<zero_pattern>(s);
        }
        
        inline bool nonzero(string_view s) {
            return valid(s) && !ctre::match<zero_pattern>(s);
        }
        
        inline uint32 digits(string_view s) {
            return valid(s) ? s.size() - 2 : 0;
        }
        
        inline char digit(char x) {
            if (x >= '0' && x <= '9') return x - '0';
            if (x >= 'A' && x <= 'F') return x - 'A' + 10;
            if (x >= 'a' && x <= 'f') return x - 'a' + 10;
            return -1;
        }
        
        template <endian::order r> ptr<math::N_bytes<r>> read(string_view s);
        
        template <typename range> 
        std::ostream &write(std::ostream& o, range r, hex::letter_case q = hex::upper) {
            return hex::write(o << "0x", r, q);
        }
        
        template <typename range> 
        std::string inline write(range r, hex::letter_case q = hex::upper) {
            std::stringstream ss;
            hexidecimal::write(ss, r, q);
            return ss.str();
        }
        
        struct N : string {
            
            N();
            N(const string&);
            N(uint64);
            
            bool valid() const {
                return hexidecimal::valid(*this);
            }
            
            bool operator==(const N&) const;
            bool operator!=(const N&) const;
            
            bool operator<=(const N&) const;
            bool operator>=(const N&) const;
            bool operator<(const N&) const;
            bool operator>(const N&) const;
        
            N& operator++();
            N& operator--();
            
            N operator++(int);
            N operator--(int);
            
            N operator+(const N&) const;
            N operator-(const N&) const;
            N operator*(const N&) const;
            
            N operator<<(int) const;
            N operator>>(int) const;
            
            N& operator+=(const N&);
            N& operator-=(const N&);
            N& operator*=(const N&);
            
            N& operator<<=(int);
            N& operator>>=(int);
            
            math::division<N, uint64> divide(uint64) const;
            
        };
        
    }
    
    namespace natural {
        static constexpr ctll::fixed_string pattern{"0|([1-9][0-9]*)|(0x((([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*)))"};

        static constexpr ctll::fixed_string zero_pattern{"0|0x(00)*"};
        
        inline bool valid(string_view s) {
            return ctre::match<pattern>(s);
        } 
        
        inline bool zero(string_view s) {
            return ctre::match<zero_pattern>(s);
        }
        
        inline bool nonzero(string_view s) {
            return valid(s) && ! ctre::match<zero_pattern>(s);
        }
        
        inline uint32 digits(string_view s) {
            return std::max(decimal::digits(s), hexidecimal::digits(s));
        }
        
        template <endian::order r> ptr<math::N_bytes<r>> read(string_view s) {
            if (!valid(s)) return nullptr;
            if (hexidecimal::valid(s)) return hexidecimal::read<r>(s);
            return decimal::read<r>(s);
        }
        
    }
    
    namespace integer {
        static constexpr ctll::fixed_string pattern{"0|(-?[1-9][0-9]*)|(0x((([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*)))"};
        
        static constexpr ctll::fixed_string zero_pattern {"0|0x(00)*"};
        
        static constexpr ctll::fixed_string negative_pattern{"(-(0*[1-9][0-9]*))|0x(([8-9a-f][0-9a-f]([0-9a-f][0-9a-f])*)|([8-9A-F][0-9A-F]([0-9A-F][0-9A-F])*))"};
        
        inline bool valid(string_view s) {
            return ctre::match<pattern>(s);
        } 
        
        inline bool negative(string_view s) {
            return ctre::match<negative_pattern>(s);
        }
        
        inline bool zero(string_view s) {
            return ctre::match<zero_pattern>(s);
        }
        
        inline bool nonzero(string_view s) {
            return valid(s) && ! ctre::match<zero_pattern>(s);
        }
        
        inline uint32 digits(string_view s) {
            return negative(s) ? natural::digits(s.substr(1, s.size() - 1)) : natural::digits(s);
        }
        
        template <endian::order r> ptr<math::Z_bytes<r>> read(string_view s) {
            if (!valid(s)) return nullptr;
            
            struct read_integer {
                ptr<math::Z_bytes<r>> operator()(ptr<math::N_bytes<r>> n) {
                    ptr<math::Z_bytes<r>> z = std::make_shared<math::Z_bytes<r>>();
                    z->resize(n->size());
                    std::copy(n->begin(), n->end(), z->begin());
                    return z;
                }
            };
            
            if (hexidecimal::valid(s)) return read_integer{}(hexidecimal::read<r>(s));
            
            if (negative(s)) {
                ptr<math::Z_bytes<r>> z = read_integer{}(decimal::read<r>(s.substr(1)));
                *z = -*z;
                return z;
            }
            
            return read_integer{}(decimal::read<r>(s));
        }
        
    };
    
}

#endif

