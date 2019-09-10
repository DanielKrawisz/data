#include <data/crypto/ripemd160.hpp>
#include <crypto++/ripemd.h>

namespace data::ripemd160 {
    digest hash(bytes& data) {
        std::array<byte, size> hash;
        CryptoPP::RIPEMD160 rhash;
        rhash.CalculateDigest(hash.data(), data.data(), data.size());
        return uint<size>{hash};
    }


}

