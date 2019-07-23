#include <data/crypto/ripemd160.hpp>
#include <crypto++/ripemd.h>

namespace data::ripemd160 {
    digest hash(bytes& data) {
        digest hash;
        CryptoPP::RIPEMD160 rhash;
        rhash.CalculateDigest(hash.data(),data.data(),data.size());
        return hash;
    }
    
    template <uint32 n>
    digest hash(const array<byte, n>& data) {
        digest hash;
        CryptoPP::RIPEMD160 rhash;
        rhash.CalculateDigest(hash.data(), data.data(), data.size());
        return hash;
    };

}

