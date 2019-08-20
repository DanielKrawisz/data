#include <data/encoding/base58.hpp>
#include <bitcoin/system/formats/base_58.hpp>

namespace data {
    
    namespace encoding {
        
        namespace base58 {            
            string write(vector<byte> x) {
                libbitcoin::system::encode_base58(libbitcoin::system::data_slice(x));

            }
            
            vector<byte> read(const string& s) {
                std::vector<byte> out;
                if (!libbitcoin::system::decode_base58(out, s)) return {};
                return out;
            }
        }
        
    }
    
}
