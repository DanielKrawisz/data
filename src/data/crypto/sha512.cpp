#include <data/crypto/sha512.hpp>
#include <crypto++/sha.h>

namespace data::sha512 {
    digest hash(bytes& data) {
        digest hash;
        CryptoPP::SHA512 shaHash;
        shaHash.CalculateDigest(hash.data(),data.data(),data.size());
        return hash;
    }
    
    template <uint32 n>
    digest hash(const array<byte, n>& data) {
        digest hash;
        CryptoPP::SHA512 shaHash;
        shaHash.CalculateDigest(hash.data(), data.data(), data.size());
        return hash;
    };

}

