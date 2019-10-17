// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/encoding/hex.hpp>
#include <data/encoding/invalid.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string.hpp>
#include <iterator>
#include <iostream>
#include <vector>
#include <string>

namespace  data::encoding::hex {
    
    string::string(string_view sourceString) : String{sourceString} {
        try {
            boost::algorithm::unhex(String.begin(), String.end(), std::back_inserter(Bytes));
        }
        catch(boost::algorithm::hex_decode_error exception)
        {
            return;
        }
        ToBytes=&Bytes;
    }
    
    std::string write(bytes_view sourceBytes) {
        std::string output;
        boost::algorithm::hex(sourceBytes.begin(),sourceBytes.end(),std::back_inserter(output));
        return data::string(output);
    }
}
