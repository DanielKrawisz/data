// Copyright (c) 2019-2021 Daniel Krawisz
// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_HEX
#define DATA_ENCODING_HEX

#include <boost/algorithm/hex.hpp>
#include <data/encoding/invalid.hpp>
#include <data/iterable.hpp>
#include <boost/regex.hpp>

namespace data::encoding::hex {
    const std::string Format{"hex"};
    
    enum letter_case {
        upper, 
        lower
    };
    
    inline std::string characters_lower() {
        return "0123456789abcdef";
    }
    
    inline std::string characters_upper() {
        return "0123456789ABCDEF";
    }

    static const boost::regex pattern{"^(([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*)$"};
    inline bool valid(string_view s) {
        return boost::regex_match(s.data(), pattern);
    }
    
    ptr<bytes> read(string_view);
    
    struct string : std::string {
        string() : std::string{} {}
        string(const std::string& x) : std::string{x} {}
        string(size_t n) : std::string(2 * n, '0') {}
        
        bool valid() const {
            return hex::valid(*this);
        }
        
        explicit operator bytes() const;
    };
    
    string write(bytes_view, letter_case = upper);
    string write(bytes_view, endian::order, letter_case = upper);
    
    template <size_t n>
    struct fixed : string {
        using string::string;
        fixed(const std::string& x) : string{x} {}
        fixed() : string(n) {}
        
        bool valid() const {
            return string::valid() && string::size() == 2 * n;
        }
    };
    
    fixed<8> write(uint64, letter_case = upper);
    fixed<4> write(uint32, letter_case = upper);
    fixed<2> write(uint16, letter_case = upper);
    fixed<1> write(byte, letter_case = upper);
    
    template <endian::order o, size_t x>
    fixed<x> write(endian::arithmetic<o, false, x>, letter_case = upper);
    
    template <endian::order o, size_t x>
    fixed<x> write(endian::arithmetic<o, false, x> n, letter_case q) {
        fixed<x> output;
        if (q == upper) boost::algorithm::hex(n.begin(), n.end(), output.begin());
        else boost::algorithm::hex_lower(n.begin(), n.end(), output.begin());
        return output;
    }
    
}

#endif
