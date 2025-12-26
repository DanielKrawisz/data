// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_CIPHER
#define DATA_CRYPTO_CIPHER

#include <data/types.hpp>

namespace data::crypto::cipher {

    enum direction {
        encryption,
        decryption
    };

    template <typename X> struct encryptor;
    template <typename Y> struct decryptor;

}

#endif

