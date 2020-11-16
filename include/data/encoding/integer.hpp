// Copyright (c) 2019 Daniel Krawisz
// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_INTEGER
#define DATA_ENCODING_INTEGER

#include <data/types.hpp>
#include <data/encoding/hex.hpp>
#include <data/encoding/invalid.hpp>
#include <data/encoding/endian.hpp>
#include <data/math/division.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

namespace data::encoding {
    namespace decimal {
        static const boost::regex pattern{"^0|([1-9][0-9]*)$"};
        inline std::string characters() {
            return "0123456789";
        }
        
        inline char digit(char x) {
            return x < '0' || x > '9' ? -1 : x - '0';
        }
        
        inline bool valid(string_view s) {
            return boost::regex_match(s.data(),pattern);
        } 
        
        inline bool nonzero(string_view s) {
            return valid(s) && s[0] != '0';
        }
        
        inline uint32 digits(string_view s) {
            return valid(s) ? s.size() : 0;
        }
        
        bytes read(string_view s, endian::order r);
        
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
    };
    
    namespace hexidecimal {
        static const boost::regex pattern{"0x((([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*))"};
        static const boost::regex zero_pattern{"^0x(00)*$"};
        inline bool valid(string_view s) {
            return boost::regex_match(s.data(),pattern);
        } 
        
        inline bool zero(string_view s) {
            return boost::regex_match(s.data(),zero_pattern);
        }
        
        inline bool nonzero(string_view s) {
            return valid(s) && !boost::regex_match(s.data(),zero_pattern);
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
        
        bytes read(string_view s, endian::order r);
        
        std::ostream& write(std::ostream& o, bytes_view b, endian::order r, hex::letter_case q = hex::upper);
        
        inline std::string write(bytes_view b, endian::order r, hex::letter_case q = hex::upper) {
            std::stringstream ss;
            write(ss, b, r, q);
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
    };
    
    namespace natural {
        static const boost::regex pattern{"^0|([1-9][0-9]*)|(0x((([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*)))$"};

        static const boost::regex zero_pattern{"^0|0x(00)*$"};
        
        inline bool valid(string_view s) {
            return boost::regex_match(s.data(),pattern);
        } 
        
        inline bool zero(string_view s) {
            return boost::regex_match(s.data(),zero_pattern);
        }
        
        inline bool nonzero(string_view s) {
            return valid(s) && ! boost::regex_match(s.data(),zero_pattern);
        }
        
        inline uint32 digits(string_view s) {
            return std::max(decimal::digits(s), hexidecimal::digits(s));
        }
        
        bytes read(string_view s, endian::order r);
    };
    
    namespace integer {
        static const boost::regex pattern{"^0|(-?[1-9][0-9]*)|(0x((([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*)))$"};
        static const boost::regex zero_pattern {"^0|0x(00)*$"};
        static const boost::regex negative_pattern{"^(-(0*[1-9][0-9]*))|0x(([8-9a-f][0-9a-f]([0-9a-f][0-9a-f])*)|([8-9A-F][0-9A-F]([0-9A-F][0-9A-F])*))"};
        
        inline bool valid(string_view s) {
            return boost::regex_match(s.data(),pattern);
        } 
        
        inline bool negative(string_view s) {
            return boost::regex_match(s.data(),negative_pattern);
        }
        
        inline bool zero(string_view s) {
            return boost::regex_match(s.data(),zero_pattern);
        }
        
        inline bool nonzero(string_view s) {
            return valid(s) && ! boost::regex_match(s.data(),zero_pattern);
        }
        
        inline uint32 digits(string_view s) {
            return negative(s) ? natural::digits(s.substr(1, s.size() - 1)) : natural::digits(s);
        }
        
        bytes read(string_view s, endian::order r);
    };
    
}

#endif

