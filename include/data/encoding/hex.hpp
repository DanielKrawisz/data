// Copyright (c) 2019-2022 Daniel Krawisz
// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <ranges>

#include <ctre.hpp>

#include <data/encoding/invalid.hpp>
#include <data/maybe.hpp>
#include <data/array.hpp>
#include <data/encoding/endian.hpp>

namespace data {
    template <std::integral word> struct bytestring;

    using bytes = bytestring<byte>;
}

namespace data::encoding::hex {
    const std::string Format {"hex"};
    
    enum class letter_case {
        unknown, 
        upper, 
        lower
    };
    
    const std::string inline &characters_lower () {
        static std::string Lower {"0123456789abcdef"};
        return Lower;
    }
    
    const std::string inline &characters_upper () {
        static std::string Upper {"0123456789ABCDEF"};
        return Upper;
    }
    
    const std::string inline &characters (letter_case c) {
        static std::string None {};
        if (c == letter_case::upper) return characters_upper ();
        if (c == letter_case::lower) return characters_lower ();
        return None;
    }
    
    static constexpr auto pattern = ctll::fixed_string {"(([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*)"};

    constexpr bool inline valid (string_view s) {
        return ctre::match<pattern> (s);
    }
    
    maybe<bytes> read (string_view);
    
    // A hex-encoded string
    struct string : std::string {
        string () : std::string {} {}
        explicit string (const char *x) : std::string {x} {}
        explicit string (const std::string &x) : std::string {x} {}
        string (size_t n) : std::string (2 * n, '0') {}
        
        bool valid () const {
            return hex::valid (*this);
        }

        template <std::integral word> operator bytestring<word> () const;
    };
    
    template <std::ranges::range range> 
    std::ostream &write (std::ostream &o, range r, letter_case q = letter_case::lower) {
        return o << write (r, q);
    }

    constexpr char to_char (unsigned x, letter_case q) {
        return x < 10
            ? char ('0' + x)
            : char ((q == letter_case::lower ? 'a' : 'A') + x - 10);
    }

    template <std::input_iterator iti, std::sentinel_for<iti> sen, typename ito>
    constexpr void encode (iti it, sen end, ito out, letter_case q = letter_case::lower) {
        using word_type = unconst<unref<decltype (*it)>>;
        constexpr size_t word_size = sizeof (word_type);

        while (it != end) {
            word_type value = *it++;

            for (int i = word_size * 8 - 4; i >= 0; i -= 4) {
                *out++ = to_char (static_cast<unsigned> ((value >> i) & word_type {0xf}), q);
            }
        }
    }
    
    template <std::ranges::range range> 
    string inline write (range r, letter_case q = letter_case::lower) {
        string output ((r.end () - r.begin ()) * sizeof (decltype (*r.begin ())));
        encode (r.begin (), r.end (), output.begin (), q);
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

        operator byte_array<n> () const;
    };
    
    // write numbers as fixed size hex strings.
    fixed<8> write (uint64, letter_case = letter_case::upper);
    fixed<4> write (uint32, letter_case = letter_case::upper);
    fixed<2> write (uint16, letter_case = letter_case::upper);
    fixed<1> write (byte, letter_case = letter_case::upper);
    
    template <endian::order o, size_t x>
    fixed<x> write (endian::integral<false, o, x>, letter_case = letter_case::upper);
    
    template <endian::order o, size_t x>
    fixed<x> write (endian::integral<false, o, x> n, letter_case q) {
        fixed<x> output;
        encode (n.begin (), n.end (), output.begin (), q);
        return output;
    }

    struct invalid : exception::base<invalid> {
        invalid (): exception::base<invalid> {"invalid hex"} {}
    };

    constexpr uint8_t from_char (char c) {
        if (c >= '0' && c <= '9') return static_cast<uint8_t>(c - '0');
        if (c >= 'A' && c <= 'F') return static_cast<uint8_t>(c - 'A' + 10);
        if (c >= 'a' && c <= 'f') return static_cast<uint8_t>(c - 'a' + 10);
        throw invalid {} << "; bad character " << c << " (" << (int64 (c)) << ")";
    }

    template <std::input_iterator iti, std::sentinel_for<iti> sen, typename ito>
    constexpr void decode (iti it, sen end, ito out) {
        using word_type = unconst<unref<decltype (*out)>>;
        constexpr const size_t word_size = sizeof (word_type);
        while (it != end) {
            *out = 0;

            for (int i = word_size * 8 - 4; i > 0; i -= 4) {
                *out += word_type (from_char (*it)) << i;
                it++;
                // hex string length must be even, so we don't expect the end here.
                if (it == end) throw invalid {} << "; invalid length ";
            }

            *out += from_char (*it);
            it++;
            out++;
        }
    }

    template <size_t n> fixed<n>::operator byte_array<n> () const {
        if (!this->valid ()) throw invalid {} << ": " << *this;
        byte_array<n> T;
        encoding::hex::decode (this->begin (), this->end (), T.data ());
        return T;
    }
    
}

namespace data {
    
    using hex_case = encoding::hex::letter_case;
    using hex_string = encoding::hex::string;

    template <std::integral word, size_t size>
    std::ostream inline &operator << (std::ostream &o, const bytes_array<word, size> &s) {
        return o << "\"" << encoding::hex::write (slice<const word> (s)) << "\"";
    }
    
}
