#ifndef DATA_ENCODINGS_BASE58_HPP
#define DATA_ENCODINGS_BASE58_HPP

#include <data/types.hpp>
#include <data/slice.hpp>
#include <bitcoin/bitcoin/formats/base_58.hpp>

namespace data {
    
    namespace encoding {
        
        namespace base58 {            
            string write(slice<byte> x) noexcept {
                libbitcoin::encode_base58(libbitcoin::data_slice(x.begin(), x.end()));
            }
            
            vector<byte> read(const string& s) noexcept {
                std::vector<byte> out;
                if (!libbitcoin::decode_base58(out, s)) return {};
                return out;
            }
        }
        
    }
    
}

#endif
