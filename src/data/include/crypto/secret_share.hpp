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
        byte Index;
        bytes Data;
    };
    
    cross<secret_share> secret_share_split (CryptoPP::RandomNumberGenerator &, const bytes &message, byte max, byte threshold);
    
    bytes secret_share_merge (cross<secret_share>, byte threshold);
    
}

#endif
