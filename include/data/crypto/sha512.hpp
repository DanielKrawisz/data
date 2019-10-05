// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_SHA512
#define DATA_CRYPTO_SHA512

#include <data/crypto/digest.hpp>

namespace data::sha512 {
    
    const uint32 size = 64;
    
    using digest = crypto::digest<size>;
    
    const digest Zero = digest{};
    
    digest hash(const bytes&);
    digest hash(string&);
    
    template <uint32 n>
    digest hash(const std::array<byte, n>&);
    
    template <uint32 n>
    digest hash(const uint<n>&);

}

#endif
