#include <data/crypto/ripemd160.hpp>
#include <crypto++/ripemd.h>

namespace data::ripemd160 {
    digest hash(bytes& data) {
        std::array<byte, size> hash;
        CryptoPP::RIPEMD160 rhash;
        rhash.CalculateDigest(hash.data(), data.data(), data.size());
        return uint<size>{hash};
    }
    
    template <uint32 n>
    digest hash(const std::array<byte, n>& data) {
        std::array<byte, size> hash;
        CryptoPP::RIPEMD160 rhash;
        rhash.CalculateDigest(hash.data(), data.data(), data.size());
        return uint<size>{hash};
    };

}

