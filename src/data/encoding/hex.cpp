//
// Created by nekosune on 04/07/19.
//

#include "data/encoding/hex.hpp"

#include <iterator>
#include <iostream>
#include <vector>
#include <string>

namespace  data {
    namespace encoding {
        namespace hex {

            string::string(std::string sourceString) {
                ToString=&sourceString;
                std::vector<uint8_t> *tmp = new std::vector<uint8_t>();
                try {
                    boost::algorithm::unhex(ToString->c_str(), std::back_inserter(*tmp));
                }
                catch(boost::algorithm::hex_decode_error exception)
                {
                    throw invalid(ToString);
                }
                ToBytes=tmp;
            }
        }
    }
}
