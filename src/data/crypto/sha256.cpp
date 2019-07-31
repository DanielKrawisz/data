#include <data/crypto/sha256.hpp>
#include <crypto++/sha.h>

namespace data::sha256 {
    digest hash(bytes& data) {
        std::array<byte, 32> hash;
        CryptoPP::SHA256 shaHash;
        shaHash.CalculateDigest(hash.data(),data.data(),data.size());
        return hash;
    }
    
    template <uint32 n>
    digest hash(const std::array<byte, n>& data) {
        std::array<byte, 32> hash;
        CryptoPP::SHA256 shaHash;
        shaHash.CalculateDigest(hash.data(), data.data(), data.size());
        return hash;
    };

}
