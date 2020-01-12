//
// Created by nekosune on 05/07/19.
//

#include <data/encoding/ascii.hpp>

namespace data::encoding::ascii{
    string::string(std::string const*input) {
        for(char c: *input)
        {
            if(!ascii::valid(c))
                return;
        }
        innerString=input;
    }

    string::operator bytes() {
        return innerString!= nullptr ? bytes(slice<byte>((byte*)innerString->data(), innerString->size())) : bytes();
    }


}

