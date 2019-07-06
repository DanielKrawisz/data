#include <data/crypto/sha256.hpp>
#include <crypto++/sha.h>

namespace data {
    
    namespace sha256 {
        digest hash(const bytes& data) {
            digest hash;
            CryptoPP::SHA256 shaHash;
            shaHash.CalculateDigest(hash.data(),data.data(),data.size());
            return hash;
        }
    
    }

}
