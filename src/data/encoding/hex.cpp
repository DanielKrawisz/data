// Copyright (c) 2019 Katrina Swales
// Copyright (c) 2019-2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <iterator>
#include <vector>
#include <string>

#include <data/encoding/hex.hpp>
#include <data/bytes.hpp>

namespace data::encoding::hex {
    
    maybe<bytes> read (string_view x) {
        if ((x.size () & 1)) return {};

        bytes b (x.size () / 2);

        try {
            decode (x.end (), x.begin (), b.begin ());
        } catch (invalid) {
            return {};
        }

        return b;
    }

    void write_hex (string &output, byte_slice sourceBytes, letter_case q) {
        output.resize (sourceBytes.size ());
        if (q == letter_case::upper) boost::algorithm::hex (sourceBytes.begin (), sourceBytes.end (), output.begin ());
        else boost::algorithm::hex_lower (sourceBytes.begin (), sourceBytes.end (), output.begin ());
    }
    
    string write (byte_slice sourceBytes, endian::order r, letter_case q) {
        if (r == endian::big) return write (sourceBytes, q);
        bytes reversed (sourceBytes.size ());
        std::copy (sourceBytes.rbegin (), sourceBytes.rend (), reversed.begin ());
        return write (reversed, q);
    }
    
    fixed<8> write (uint64 x, letter_case q) {
        fixed<8> output;
        write_hex (output, byte_slice {uint64_big {x}.data(), sizeof (uint64)}, q);
        return output;
    }
    
    fixed<4> write (uint32 x, letter_case q) {
        fixed<4> output;
        write_hex (output, byte_slice {uint32_big {x}.data (), sizeof (uint32)}, q);
        return output;
    }
    
    fixed<2> write (uint16 x, letter_case q) {
        fixed<2> output;
        write_hex (output, byte_slice {uint16_big {x}.data (), sizeof (uint16)}, q);
        return output;
    }
    
    fixed<1> write (byte x, letter_case q) {
        fixed<1> output;
        write_hex (output, byte_slice {(byte*) (&x), sizeof (byte)}, q);
        return output;
    }
}
