// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_AES
#define DATA_CRYPTO_AES

#include "encrypted.hpp"

// AES in cypher block chaining (CBC) mode. 
namespace data::crypto::aes {
    
    bytes encrypt(bytes_view, const symmetric_key<16>&, const initialization_vector&);
    bytes decrypt(bytes_view, const symmetric_key<16>&, const initialization_vector&);
    bytes encrypt(bytes_view, const symmetric_key<24>&, const initialization_vector&);
    bytes decrypt(bytes_view, const symmetric_key<24>&, const initialization_vector&);
    bytes encrypt(bytes_view, const symmetric_key<32>&, const initialization_vector&);
    bytes decrypt(bytes_view, const symmetric_key<32>&, const initialization_vector&);
}

#endif

