// Copyright (c) 2019-2021 Daniel Krawisz
// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_HEX
#define DATA_ENCODING_HEX

#include <ranges>

#include <ctre.hpp>

#include <boost/algorithm/hex.hpp>

#include <data/encoding/invalid.hpp>
#include <data/cross.hpp>
#include <data/maybe.hpp>

namespace data::encoding::hex {
    const std::string Format {"hex"};
    
    enum class letter_case {
        upper, 
        lower
    };
    
    std::string inline characters_lower () {
        return "0123456789abcdef";
    }
    
    std::string inline characters_upper () {
        return "0123456789ABCDEF";
    }
    
    std::string inline characters (letter_case x) {
        return x == letter_case::upper ? characters_upper () : characters_lower ();
    }
    
    static constexpr auto pattern = ctll::fixed_string {"(([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*)"};

    bool inline valid (string_view s) {
        return ctre::match<pattern> (s);
    }
    
    maybe<bytes> read (string_view);
    
    // A hex-encoded string
    struct string : std::string {
        string () : std::string {} {}
        string (const std::string &x) : std::string {x} {}
        string (size_t n) : std::string (2 * n, '0') {}
        
        bool valid () const {
            return hex::valid (*this);
        }
        
        explicit operator bytes () const;
    };
    
    template <std::ranges::range range> 
    std::ostream &write (std::ostream &o, range r, letter_case q = letter_case::lower) {
        return o << write (r, q);
    }
    
    template <std::ranges::range range> 
    string write (range r, letter_case q = letter_case::lower) {
        string output ((r.end () - r.begin ()));
        if (q == letter_case::upper) boost::algorithm::hex (r.begin (), r.end (), output.begin ());
        else boost::algorithm::hex_lower (r.begin (), r.end (), output.begin ());
        return output;
    }
    
    // a fixed-size hex string.
    template <size_t n> struct fixed : string {
        using string::string;
        fixed (const std::string &x) : string {x} {}
        fixed () : string (n) {}
        
        bool valid () const {
            return string::valid () && string::size () == 2 * n;
        }
    };
    
    // write numbers as fixed size hex strings.
    fixed<8> write (uint64, letter_case = letter_case::upper);
    fixed<4> write (uint32, letter_case = letter_case::upper);
    fixed<2> write (uint16, letter_case = letter_case::upper);
    fixed<1> write (byte, letter_case = letter_case::upper);
    
    template <endian::order o, size_t x>
    fixed<x> write (endian::arithmetic<false, o, x>, letter_case = letter_case::upper);
    
    template <endian::order o, size_t x>
    fixed<x> write (endian::arithmetic<false, o, x> n, letter_case q) {
        fixed<x> output;
        if (q == letter_case::upper) boost::algorithm::hex (n.begin (), n.end (), output.begin ());
        else boost::algorithm::hex_lower (n.begin (), n.end (), output.begin ());
        return output;
    }
    
}

namespace data {
    
    using hex_case = encoding::hex::letter_case;
    
    std::ostream inline &operator << (std::ostream &o, const bytes &s) {
        return o << "\"" << encoding::hex::write (s) << "\"";
    }
    
}

#endif
