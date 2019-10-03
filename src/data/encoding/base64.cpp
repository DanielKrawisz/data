// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include <data/encoding/base64.hpp>
#include <data/encoding/invalid.hpp>

#include <bitcoin/system/formats/base_58.hpp>
#include <bitcoin/system/formats/base_64.hpp>

namespace data::encoding::base64 {

    bytes read(const std::string & s) {
        libbitcoin::system::data_chunk out;
        if (!libbitcoin::system::decode_base64(out, s)) return {};
        return out;
    }

    string::string(std::string str) {
        libbitcoin::system::data_chunk out;
        if(!libbitcoin::system::decode_base64(out,str))
            throw invalid(format, *this);
        Bytes=out;
        ToBytes=&Bytes;
    }

    bool valid(const std::string &str) {
        libbitcoin::system::data_chunk out;
        return libbitcoin::system::decode_base58(out,str);
    }


    data::string write(const bytes & sourceBytes) {
        return libbitcoin::system::encode_base58(sourceBytes);
    }



}


