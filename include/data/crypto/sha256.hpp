// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_SHA256
#define DATA_CRYPTO_SHA256

#include <data/crypto/digest.hpp>
#include <crypto++/sha.h>

namespace data::sha256 {
    
    const uint32 size = 32;
    
    using digest = crypto::digest<size>;
    
    const digest Zero = digest{};
    
    digest hash(bytes&);
    digest hash(slice<byte>);

    template <uint32 n>
    digest hash(const std::array<byte, n>& data){
        std::array<byte, size> hash;
        CryptoPP::SHA256 shaHash;
        shaHash.CalculateDigest(hash.data(), data.data(), data.size());
        return uint<size>{hash};
    };
    
    template <uint32 n>
    digest hash(const uint<n>&);

}

#endif
