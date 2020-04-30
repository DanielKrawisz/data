// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/crypto/AES.hpp>
#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <crypto++/filters.h>

namespace data::crypto::aes {
    
    template <size_t keylen> struct aes {
        bytes encrypt(bytes_view b, const key<keylen>& k, const initialization_vector& iv) const {

            CryptoPP::AES::Encryption aesEncryption(k.data(), keylen);
            CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption( aesEncryption, iv.data() );
            
            size_t size = b.size();
            bytes cyphertext(static_cast<unsigned char>(size));
            
            CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, 
                new CryptoPP::ArraySink(cyphertext.data(), size));
            stfEncryptor.Put( b.begin(), b.size() );
            stfEncryptor.MessageEnd();
            
            return cyphertext;
        }
        
        bytes decrypt(bytes_view b, const key<keylen>& k, const initialization_vector& iv) const {
            
            CryptoPP::AES::Decryption aesDecryption(k.data(), keylen);
            CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption( aesDecryption, iv.data() );
            
            size_t size = b.size();
            bytes decryptedtext(static_cast<unsigned char>(size));

            CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, 
                new CryptoPP::ArraySink(decryptedtext.data(), size));
            stfDecryptor.Put( b.begin(), b.size() );
            stfDecryptor.MessageEnd();
            
            return decryptedtext;
        }
    };
    
    bytes encrypt(bytes_view b, const key<16>& k, const initialization_vector& iv) {
        return aes<16>{}.encrypt(b, k, iv);
    }
    
    bytes decrypt(bytes_view b, const key<16>& k, const initialization_vector& iv) {
        return aes<16>{}.decrypt(b, k, iv);
    }
    
    bytes encrypt(bytes_view b, const key<24>& k, const initialization_vector& iv) {
        return aes<24>{}.encrypt(b, k, iv);
    }
    
    bytes decrypt(bytes_view b, const key<24>& k, const initialization_vector& iv) {
        return aes<24>{}.decrypt(b, k, iv);
    }
    
    bytes encrypt(bytes_view b, const key<32>& k, const initialization_vector& iv) {
        return aes<32>{}.encrypt(b, k, iv);
    }
    
    bytes decrypt(bytes_view b, const key<32>& k, const initialization_vector& iv) {
        return aes<32>{}.decrypt(b, k, iv);
    }
    
}

