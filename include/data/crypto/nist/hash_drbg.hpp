#ifndef DATA_CRYPTO_NIST_HASH_DRBG_HPP
#define DATA_CRYPTO_NIST_HASH_DRBG_HPP

#include <data/crypto/pp/CryptoPP.hpp>

namespace data {
    
    namespace crypto {
        
        namespace nist {
            
            namespace drbg {
                
                struct hash : private pp::Hash_DRBG<pp::SHA256, 256/8, 440/8> {
                    static const unsigned int SEEDLENGTH = 2*440/8;
            
                    struct state {
                        sha256::digest Hash;
                        std::array<byte, SEEDLENGTH> Seed;
                    };
                    
                    state get_state();
                    
                    hash(state);
                    
                    bool next(byte* b, size_t len);
                
                };
            
            }
            
        }
    
    }

}

#endif
