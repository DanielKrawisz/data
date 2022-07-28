// Copyright (c) 2019 Daniel Krawisz
// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_INTEGER
#define DATA_ENCODING_INTEGER

#include <ctre.hpp>

#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string.hpp>

#include <data/types.hpp>
#include <data/encoding/invalid.hpp>
#include <data/math/number/bytes/bytes.hpp>

namespace data::encoding {
    
    namespace decimal {
        static constexpr ctll::fixed_string pattern{"0|([1-9][0-9]*)"};
        
        std::string inline characters() {
            return "0123456789";
        }
        
        char inline digit(char x) {
            return x < '0' || x > '9' ? -1 : x - '0';
        }
        
        bool inline valid(string_view s) {
            return ctre::match<pattern>(s);
        } 
        
        bool inline nonzero(string_view s) {
            return valid(s) && s[0] != '0';
        }
        
        uint32 inline digits(string_view s) {
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
    
    namespace signed_decimal {
        
        static constexpr ctll::fixed_string pattern{"0|(-?[1-9][0-9]*)"};
        
        bool inline valid(string_view s) {
            return ctre::match<pattern>(s);
        }
        
        template <endian::order r> 
        ptr<math::Z_bytes<r>> read(string_view s);
    
        template <endian::order r> 
        std::ostream inline &write(std::ostream &o, const math::number::Z_bytes<r> &z) {
            if (math::number::is_negative(z)) o << '-';
            return decimal::write(o, data::abs(z));
        }
    
        template <endian::order r> 
        string inline write(const math::number::Z_bytes<r> &z) {
            std::stringstream ss;
            write(ss, z);
            return ss.str();
        }
    }
    
    namespace hexidecimal {
        static constexpr ctll::fixed_string pattern{"0x((([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*))"};
        static constexpr ctll::fixed_string zero_pattern{"0x(00)*"};
        
        bool inline valid(string_view s) {
            return ctre::match<pattern>(s);
        } 
        
        bool inline zero(string_view s) {
            return ctre::match<zero_pattern>(s);
        }
        
        bool inline nonzero(string_view s) {
            return valid(s) && !ctre::match<zero_pattern>(s);
        }
        
        uint32 inline digits(string_view s) {
            return valid(s) ? s.size() - 2 : 0;
        }
        
        char inline digit(char x) {
            if (x >= '0' && x <= '9') return x - '0';
            if (x >= 'A' && x <= 'F') return x - 'A' + 10;
            if (x >= 'a' && x <= 'f') return x - 'a' + 10;
            return -1;
        }
        
        hex::letter_case read_case(string_view s);
    
        template <endian::order r> 
        ptr<oriented<r, byte>> read(string_view s) {
            if (!valid(s)) return nullptr;
            
            ptr<oriented<r, byte>> n = std::make_shared<oriented<r, byte>>();
            n->resize((s.size() - 2) / 2);
            boost::algorithm::unhex(s.begin() + 2, s.end(), n->words().rbegin());
            
            return n;
        }
        
        template <endian::order r> 
        std::ostream inline &write(std::ostream &o, const oriented<r, byte> &d, hex::letter_case q) {
            o << "0x"; 
            return encoding::hex::write(o, d.words().reverse(), q);
        }
        
        template <endian::order r> 
        string write(const oriented<r, byte> &z, hex::letter_case q) {
            std::stringstream ss;
            write(ss, z, q);
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
        
        bool inline valid(string_view s) {
            return ctre::match<pattern>(s);
        } 
        
        bool inline zero(string_view s) {
            return ctre::match<zero_pattern>(s);
        }
        
        bool inline nonzero(string_view s) {
            return valid(s) && ! ctre::match<zero_pattern>(s);
        }
        
        uint32 inline digits(string_view s) {
            return std::max(decimal::digits(s), hexidecimal::digits(s));
        }
        
        template <endian::order r> ptr<math::N_bytes<r>> read(string_view s);
        
    }
    
    namespace integer {
        static constexpr ctll::fixed_string pattern{"0|(-?[1-9][0-9]*)|(0x((([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*)))"};
        
        static constexpr ctll::fixed_string zero_pattern {"0|0x(00)*"};
        
        static constexpr ctll::fixed_string negative_pattern{"(-(0*[1-9][0-9]*))|0x(([8-9a-f][0-9a-f]([0-9a-f][0-9a-f])*)|([8-9A-F][0-9A-F]([0-9A-F][0-9A-F])*))"};
        
        bool inline valid(string_view s) {
            return ctre::match<pattern>(s);
        } 
        
        bool inline negative(string_view s) {
            return ctre::match<negative_pattern>(s);
        }
        
        bool inline zero(string_view s) {
            return ctre::match<zero_pattern>(s);
        }
        
        bool inline nonzero(string_view s) {
            return valid(s) && ! ctre::match<zero_pattern>(s);
        }
        
        uint32 inline digits(string_view s) {
            return negative(s) ? natural::digits(s.substr(1, s.size() - 1)) : natural::digits(s);
        }
        
        template <endian::order r> 
        ptr<math::number::Z_bytes<r>> read(string_view s) {
            if (!valid(s)) return nullptr;
            
            if (hexidecimal::valid(s)) {
                auto z = hexidecimal::read<r>(s);
                if (z == nullptr) return nullptr;
                auto x = std::make_shared<math::number::Z_bytes<r>>();
                x->resize(z->size());
                std::copy(z->begin(), z->end(), x->begin());
                return x;
            }
            
            if (negative(s)) {
                auto z = decimal::read<r>(s.substr(1));
                if (z == nullptr) return nullptr;
                return std::make_shared<math::number::Z_bytes<r>>(math::number::Z_bytes<r>(-*z));
            }
            
            auto z = decimal::read<r>(s);
            if (z == nullptr) return nullptr;
            return std::make_shared<math::number::Z_bytes<r>>(math::number::Z_bytes<r>(*z));
        }
        
    };
    
}

#endif

