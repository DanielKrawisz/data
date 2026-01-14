// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/crypto/secret_share.hpp>
#include <cryptopp/ida.h>
#include <cryptopp/filters.h>

namespace data::crypto {
    
    cross<secret_share> secret_share_split (random::source &rand, const bytes &b, byte max, byte threshold) {
        using namespace CryptoPP;

        random::to_crypto_pp_RNG rng {rand};
        
        ChannelSwitch *channelSwitch = NULL;
        VectorSource source (b, false, new SecretSharing (rng, threshold, max, channelSwitch = new ChannelSwitch));
        
        cross<secret_share> shares (max);
        
        vector_member_ptrs<VectorSink> vectorSinks (max);
        
        for (int i = 0; i < max; i++) {
            vectorSinks[i].reset (new VectorSink (shares[i].Data));

            std::string channel = WordToString<word32> (i);
            shares[i].Index = i;
            channelSwitch->AddRoute (channel, *vectorSinks[i], DEFAULT_CHANNEL);
        }

        source.PumpAll ();
        
        return shares;

    }
    
    bytes secret_share_merge (cross<secret_share> shares, byte threshold) {
        using namespace CryptoPP;
        
        if (threshold == 0 || shares.size () < threshold) return {};
        
        bytes result {};
        
        SecretRecovery recovery (threshold, new VectorSink (result));

        vector_member_ptrs<VectorSource> sources (threshold);
        
        for(int i = 0; i < threshold; i++) {
            
            sources[i].reset (new VectorSource (shares[i].Data, false));
            sources[i]->Attach (new ChannelSwitch(recovery, WordToString<word32> (shares[i].Index)));
            
        }
        
        while (sources[0]->Pump (256)) for (int i = 1; i < threshold; i++) sources[i]->Pump (256);
        
        for (int i = 0; i < threshold; i++) sources[i]->PumpAll ();

        return result;
    }
    
}
 
