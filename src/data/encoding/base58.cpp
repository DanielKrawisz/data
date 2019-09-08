#include <data/encoding/base58.hpp>
#include <bitcoin/system/formats/base_58.hpp>
#include <data/encoding/invalid.hpp>

namespace data {
    
    namespace encoding {
        
        namespace base58 {
            
            vector<byte> read(const string& s) {
                std::vector<byte> out;
                if (!libbitcoin::system::decode_base58(out, s)) return {};
                return out;
            }

            string::string(std::string str) {
                auto *tmp=new std::vector<uint8_t>() ;
                if(!libbitcoin::system::decode_base58(*tmp,str))
                    throw invalid(format, *this);
                ToBytes=tmp;
            }

            bool valid(const std::string &str) {
                return libbitcoin::system::is_base58(str);
            }


            data::string write(const bytes & sourceBytes) {
                return libbitcoin::system::encode_base58(sourceBytes);
            }

        }
        
    }
    
}
