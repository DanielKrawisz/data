// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/crypto/AES.hpp>
#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <crypto++/filters.h>

namespace data::crypto::aes {
    
    template <size_t keylen> struct aes {
        bytes encrypt(const slice<byte> b, uint<keylen> k) const {
            byte iv[ CryptoPP::AES::BLOCKSIZE ];
            memset( iv, 0x00, CryptoPP::AES::BLOCKSIZE );
            CryptoPP::AES::Encryption aesEncryption(k.Array.data(), keylen);
            CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption( aesEncryption, iv );
            
            size_t size = b.size();
            bytes cyphertext{static_cast<unsigned char>(size)};
            
            CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, 
                new CryptoPP::ArraySink(cyphertext.data(), size));
            stfEncryptor.Put( b.begin(), b.size() );
            stfEncryptor.MessageEnd();
            
            return cyphertext;
        }
        
        bytes decrypt(const slice<byte> b, uint<keylen> k) const {
            byte iv[ CryptoPP::AES::BLOCKSIZE ];
            memset( iv, 0x00, CryptoPP::AES::BLOCKSIZE );
            
            CryptoPP::AES::Decryption aesDecryption(k.Array.data(), keylen);
            CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption( aesDecryption, iv );
            
            size_t size = b.size();
            bytes decryptedtext{static_cast<unsigned char>(size)};

            CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, 
                new CryptoPP::ArraySink(decryptedtext.data(), size));
            stfDecryptor.Put( b.begin(), b.size() );
            stfDecryptor.MessageEnd();
            
            return decryptedtext;
        }
    };
    
    bytes encrypt(const slice<byte> b, uint<16> k) {
        return aes<16>{}.encrypt(b, k);
    }
    
    bytes decrypt(const slice<byte> b, uint<16> k) {
        return aes<16>{}.decrypt(b, k);
    }
    
    bytes encrypt(const slice<byte> b, uint<24> k) {
        return aes<24>{}.encrypt(b, k);
    }
    
    bytes decrypt(const slice<byte> b, uint<24> k) {
        return aes<24>{}.decrypt(b, k);
    }
    
    bytes encrypt(const slice<byte> b, uint<32> k) {
        return aes<32>{}.encrypt(b, k);
    }
    
    bytes decrypt(const slice<byte> b, uint<32> k) {
        return aes<32>{}.decrypt(b, k);
    }
}

