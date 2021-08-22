// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/crypto/secret_share.hpp>
#include <data/crypto/NIST_DRBG.hpp>
#include <data/encoding/hex.hpp>
#include "gtest/gtest.h"
#include <algorithm>

namespace data {
    
    TEST(SecretShareTest, TestSecretShare) {
        ptr<crypto::entropy> entropy = std::static_pointer_cast<crypto::entropy>(std::make_shared<crypto::fixed_entropy>(
            bytes_view(bytes::from_string("atehulak,rc.TjmleoTHReseSRCjt"))));
        
        crypto::nist::drbg random{crypto::nist::drbg::HMAC_DRBG, entropy, bytes{}, 302};
        
        for (byte total = 1; total <= 5; total++) for (byte threshold = 1; threshold <= total; threshold++) {
            
            // generate random share 
            bytes message(std::uniform_int_distribution<int>(3, 10)(random));
            random >> message;
            
            // split share 
            cross<crypto::secret_share> shares = crypto::secret_share_split(*random.Random, message, total, threshold);
            
            for (byte to_take = threshold; to_take <= std::min(static_cast<byte>(threshold + 2), total); to_take++) {
                
                std::shuffle(shares.begin(), shares.end(), []() {
                    std::mt19937::result_type seeds[std::mt19937::state_size];
                    std::random_device device;
                    std::uniform_int_distribution<typename std::mt19937::result_type> dist;
                    std::generate(std::begin(seeds), std::end(seeds), [&] { return dist(device); });
                    std::seed_seq seq(std::begin(seeds), std::end(seeds));
                    return std::mt19937(seq);
                }());
                
                bytes merged = crypto::secret_share_merge(shares, threshold);
                
                EXPECT_EQ(merged, message);
            }
            
        }
        
    }
    
    
}

