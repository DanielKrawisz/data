// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/encoding/hex.hpp>
#include <data/encoding/invalid.hpp>
#include <data/encoding/endian.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string.hpp>
#include <iterator>
#include <iostream>
#include <vector>
#include <string>

namespace  data::encoding::hex {
    
    string::string(string_view sourceString) : String{sourceString}, Bytes{(sourceString.size() + 1) / 2}, ToBytes{nullptr} {
        try {
            boost::algorithm::unhex(String.begin(), String.end(), Bytes.begin());
        }
        catch(boost::algorithm::hex_decode_error exception)
        {
            return;
        }
        ToBytes=&Bytes;
    }
    
    std::string write(bytes_view sourceBytes) {
        std::string output;
        output.resize(sourceBytes.size() * 2);
        boost::algorithm::hex(sourceBytes.begin(), sourceBytes.end(), output.begin());
        return output;
    }
    
    std::string write(uint64 x) {
        auto o = endian::ordered<uint64, endian::big>{x};
        return write(bytes_view{(byte*)(&o.Value), sizeof(uint64)});
    }
    
    std::string write(uint32 x) {
        auto o = endian::ordered<uint32, endian::big>{x};
        return write(bytes_view{(byte*)(&o.Value), sizeof(uint32)});
    }
    
    std::string write(uint16 x) {
        auto o = endian::ordered<uint16, endian::big>{x};
        return write(bytes_view{(byte*)(&o.Value), sizeof(uint16)});
    }
    
    std::string write(byte x) {
        return write(bytes_view{(byte*)(&x), sizeof(byte)});
    }
}
