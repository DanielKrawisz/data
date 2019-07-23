//
// Created by nekosune on 05/07/19.
//

#include <data/encoding/ascii.hpp>
namespace data::encoding::ascii{
    string::string(std::string const*input) {
        for(char c: *input)
        {
            if(!valid(c))
                return;
        }
        innerString=input;
    }

    string::operator bytes() {
        if(innerString!= nullptr) {
            bytes retVal(innerString->data(), innerString->data() + innerString->size());

            return retVal;
        }
        return bytes();
    }


}

