// Copyright (c) 2019 Katrina Swales
// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <iterator>
#include <iostream>
#include <vector>
#include <string>

#include <data/encoding/hex.hpp>
#include <data/encoding/endian.hpp>
#include <boost/algorithm/hex.hpp>

namespace data::encoding::hex {
    
    string::string(string_view sourceString) : String{sourceString}, Bytes((sourceString.size() + 1) / 2), ToBytes{nullptr} {
        try {
            boost::algorithm::unhex(String.begin(), String.end(), Bytes.begin());
        }
        catch(boost::algorithm::hex_decode_error exception)
        {
            return;
        }
        ToBytes=&Bytes;
    }
    
    std::string write(bytes_view sourceBytes, letter_case q) {
        std::string output;
        output.resize(sourceBytes.size() * 2);
        if (q == upper) boost::algorithm::hex(sourceBytes.begin(), sourceBytes.end(), output.begin());
        else boost::algorithm::hex_lower(sourceBytes.begin(), sourceBytes.end(), output.begin());
        return output;
    }
    
    std::string write(bytes_view sourceBytes, endian::order r, letter_case q) {
        if (r == endian::big) return write(sourceBytes, q);
        bytes reversed(sourceBytes.size());
        std::copy(sourceBytes.rbegin(), sourceBytes.rend(), reversed.begin());
        return write(reversed, q);
    }
    
    std::string write(uint64 x, letter_case q) {
        return write(bytes_view{uint64_big{x}.data(), sizeof(uint64)}, q);
    }
    
    std::string write(uint32 x, letter_case q) {
        return write(bytes_view{uint32_big{x}.data(), sizeof(uint32)}, q);
    }
    
    std::string write(uint16 x, letter_case q) {
        return write(bytes_view{uint16_big{x}.data(), sizeof(uint16)}, q);
    }
    
    std::string write(byte x, letter_case q) {
        return write(bytes_view{(byte*)(&x), sizeof(byte)}, q);
    }
}
