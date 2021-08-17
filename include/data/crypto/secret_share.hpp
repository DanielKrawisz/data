// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_SECRET_SHARE
#define DATA_CRYPTO_SECRET_SHARE

#include <data/crypto/NIST_DRBG.hpp>
#include <data/tools.hpp>
#include <cryptopp/drbg.h>

namespace data::crypto {
    
    struct secret_share {
        uint32 Index;
        bytes Data;
    };
    
    cross<secret_share> secret_share_split(CryptoPP::RandomNumberGenerator&, const bytes& message, uint32 max, uint32 threshold);
    
    bytes secret_share_merge(cross<secret_share>, uint32 threshold);
    
}

#endif
