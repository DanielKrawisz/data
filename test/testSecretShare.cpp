// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/crypto/secret_share.hpp>
#include <data/crypto/NIST_DRBG.hpp>
#include <data/encoding/hex.hpp>
#include <data/string.hpp>
#include "gtest/gtest.h"
#include <algorithm>

namespace data {
    
    TEST(SecretShareTest, TestSecretShare) {
        ptr<crypto::entropy> entropy = std::static_pointer_cast<crypto::entropy> (std::make_shared<crypto::fixed_entropy>
            (bytes_view (bytes (string ("atehulak,rc.TjmleoTHReseSRCjt")))));
        
        crypto::NIST::DRBG random {crypto::NIST::DRBG::HMAC, {*entropy, bytes {}, 302}};
        
        for (byte total = 1; total <= 5; total++) for (byte threshold = 1; threshold <= total; threshold++) {
            
            // generate random share 
            bytes message (std::uniform_int_distribution<int> (3, 10) (random));
            random >> message;
            
            // split share 
            cross<crypto::secret_share> shares = crypto::secret_share_split (*random.Random, message, total, threshold);
            
            for (byte to_take = threshold; to_take <= std::min (static_cast<byte> (threshold + 2), total); to_take++) {
                
                std::shuffle (shares.begin (), shares.end (), random);
                
                bytes merged = crypto::secret_share_merge (shares, threshold);
                
                EXPECT_EQ (merged, message);
            }
            
        }
        
    }
    
    
}

