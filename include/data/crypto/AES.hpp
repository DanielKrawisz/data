// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_AES
#define DATA_CRYPTO_AES

#include "data/types.hpp"
#include <data/math/number/bounded.hpp>

namespace data::crypto::aes {
    bytes encrypt(const slice<byte>, uint<16>);
    bytes decrypt(const slice<byte>, uint<16>);
    bytes encrypt(const slice<byte>, uint<24>);
    bytes decrypt(const slice<byte>, uint<24>);
    bytes encrypt(const slice<byte>, uint<32>);
    bytes decrypt(const slice<byte>, uint<32>);
    
    template <size_t keylen> bytes encrypt(const string, uint<keylen>);
}

#endif

