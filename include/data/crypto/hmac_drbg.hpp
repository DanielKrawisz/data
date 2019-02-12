#ifndef DATA_CRYPTO_HMAC_DRBG_HPP
#define DATA_CRYPTO_HMAC_DRBG_HPP

#include <cryptopp/drbg.h>
#include <data/types.hpp>

namespace data {
    
    namespace crypto {
    
        struct hmac_drbg {
            CryptoPP::HMAC_DRBG<> HmacDrbg; 
            
            hmac_drbg(vector<byte>& seed, vector<byte>& nonce) : HmacDrbg{seed.data(), seed.size(), nonce.data(), nonce.size()} {}
            
            hmac_drbg& operator>>(byte& x) {
                HmacDrbg.GenerateBlock(&x, 1);
                return *this;
            }
            
            hmac_drbg& operator>>(uint16_t& x) {
                HmacDrbg.GenerateBlock(reinterpret_cast<byte*>(&x), 2);
                return *this;
            }
            
            hmac_drbg& operator>>(uint32_t& x) {
                HmacDrbg.GenerateBlock(reinterpret_cast<byte*>(&x), 4);
                return *this;
            }
            
            hmac_drbg& operator>>(uint64_t& x) {
                HmacDrbg.GenerateBlock(reinterpret_cast<byte*>(&x), 8);
                return *this;
            }
            
            hmac_drbg& operator>>(std::vector<byte>& x) {
                HmacDrbg.GenerateBlock(x.data(), x.size());
                return *this;
            }
            
            template <typename X>
            hmac_drbg& operator>>(X& x) {
                do operator>>(static_cast<std::vector<byte>&>(x)); 
                while (!x.valid());
                return *this;
            }
            
        };
    
    }

}

#endif
