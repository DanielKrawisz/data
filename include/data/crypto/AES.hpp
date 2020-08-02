// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_AES
#define DATA_CRYPTO_AES

#include "data/iterable.hpp"

namespace data::crypto {
    template <size_t size> using key = array<byte, size>;
}

// AES in cypher block chaining (CBC) mode. 
namespace data::crypto::aes {
    using initialization_vector = array<byte, 32>;
    
    bytes encrypt(bytes_view, const key<16>&, const initialization_vector&);
    bytes decrypt(bytes_view, const key<16>&, const initialization_vector&);
    bytes encrypt(bytes_view, const key<24>&, const initialization_vector&);
    bytes decrypt(bytes_view, const key<24>&, const initialization_vector&);
    bytes encrypt(bytes_view, const key<32>&, const initialization_vector&);
    bytes decrypt(bytes_view, const key<32>&, const initialization_vector&);
}

#endif

