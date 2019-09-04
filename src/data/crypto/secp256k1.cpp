// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/crypto/secp256k1.hpp"
namespace data::crypto::secp256k1 {
    namespace low
    {
        const libbitcoin::system::hash_digest libbitcoin(const sha256::digest &d) {
            libbitcoin::system::hash_digest tmp{};
            std::copy_n(d.Digest.begin(),libbitcoin::system::hash_size,tmp.begin());
            return tmp;
        }
    }
}