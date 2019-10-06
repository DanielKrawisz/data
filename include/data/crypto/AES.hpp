// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_AES
#define DATA_CRYPTO_AES

#include "data/types.hpp"
#include <data/math/number/bounded.hpp>

namespace data::crypto::aes {
    bytes encrypt(bytes, uint<128>);
    bytes decrypt(bytes, uint<128>);
    bytes encrypt(bytes, uint<192>);
    bytes decrypt(bytes, uint<192>);
    bytes encrypt(bytes, uint<256>);
    bytes decrypt(bytes, uint<256>);
}

#endif

