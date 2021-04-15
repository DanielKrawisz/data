//
// Created by nekosune on 05/07/19.
//

#include <data/encoding/ascii.hpp>

namespace data::encoding::ascii{

    string::operator bytes() const {
        return valid() ? bytes(slice<byte>((byte*)std::string::data(), std::string::size())) : bytes();
    }


}

