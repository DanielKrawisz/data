// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/encoding/base58.hpp>
#include <bitcoin/system/formats/base_58.hpp>
#include <data/encoding/invalid.hpp>

namespace data::encoding::base58 {

    string::string(std::string str) {
        auto tmp = std::vector<uint8_t>() ;
        if(!libbitcoin::system::decode_base58(tmp, str))
            throw invalid(format, *this);
        std::copy(tmp.begin(), tmp.end(), Bytes.begin());
        ToBytes = &Bytes;
    }
    
    data::string write(const bytes & sourceBytes) {
        return libbitcoin::system::encode_base58(sourceBytes);
    }

}
