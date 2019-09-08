//
// Created by nekosune on 04/07/19.
//

#include <data/encoding/hex.hpp>
#include <data/encoding/invalid.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string.hpp>
#include <iterator>
#include <iostream>
#include <vector>
#include <string>

namespace  data {
    namespace encoding {
        namespace hex {

            string::string(std::string sourceString):std::string(sourceString) {
                std::vector<uint8_t> *tmp = new std::vector<uint8_t>();
                try {
                    boost::algorithm::unhex(std::string::c_str(), std::back_inserter(*tmp));
                }
                catch(boost::algorithm::hex_decode_error exception)
                {
                    throw invalid(format, *this);
                }
                ToBytes=tmp;
            }

            bool valid(const std::string &sourceString) {

                if(sourceString.size() %2 ==1)
                    return false;
                std::string upper=boost::to_upper_copy(sourceString);
                for(int i=0;i<upper.size();i++)
                {
                    if(upper[i]<48  || upper[i] > 70)
                        return false;
                    if(upper[i] > 57 && upper[i] < 65)
                        return false;
                }
                return true;
            }

            data::string write(const bytes & sourceBytes) {
                std::string output;
                boost::algorithm::hex(sourceBytes.begin(),sourceBytes.end(),std::back_inserter(output));
                return data::string(output);
            }

            template<uint32 size>
            string write(const std::array<byte, size>& a) {
                std::string output;
                boost::algorithm::hex(a.begin(), a.end(),std::back_inserter(output));
                return data::string(output);
            }
        }
    }
}
