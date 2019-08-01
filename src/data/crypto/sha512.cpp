#include <data/crypto/sha512.hpp>
#include <crypto++/sha.h>

namespace data::sha512 {
    digest hash(bytes& data) {
        std::array<byte, size> hash;
        CryptoPP::SHA512 shaHash;
        shaHash.CalculateDigest(hash.data(),data.data(),data.size());
        return uint<size>{hash};
    }
    
    template <uint32 n>
    digest hash(const std::array<byte, n>& data) {
        std::array<byte, size> hash;
        CryptoPP::SHA512 shaHash;
        shaHash.CalculateDigest(hash.data(), data.data(), data.size());
        return uint<size>{hash};
    };

}

