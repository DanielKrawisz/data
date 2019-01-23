#ifndef DATA_ENCODINGS_HEX_HPP
#define DATA_ENCODINGS_HEX_HPP

#include <data/types.hpp>
#include <data/slice.hpp>
#include <boost/algorithm/hex.hpp>

namespace data {
    
    namespace encoding {
        
        namespace hex {            
            string write(slice<byte> x) noexcept {
                try {
                    std::string s{2 * x.size()};
                    boost::algorithm::hex(x.begin(), x.end(), s.begin());
                    return s;
                } catch (boost::algorithm::hex_decode_error&) {
                    return {};
                }
            }
            
            vector<byte> read(const string& s) noexcept {
                try {
                    std::vector<byte> v{static_cast<unsigned char>(s.size() / 2)};
                    boost::algorithm::unhex(s.begin(), s.end(), v.begin());
                    return v;
                } catch (boost::algorithm::hex_decode_error&) {
                    return {};
                }
            }
        }
        
    }
    
}

#endif
