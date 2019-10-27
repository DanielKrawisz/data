#include <data/crypto/sha256.hpp>
#include <crypto++/sha.h>
#include <data/io/unimplemented.hpp>

namespace data::sha256 {

    digest hash(const bytes_view data) {
        std::array<byte, CryptoPP::SHA256::DIGESTSIZE> hash;
        CryptoPP::SHA256{}.CalculateDigest(hash.data(), data.begin(), data.size());
        return uint<CryptoPP::SHA256::DIGESTSIZE>{hash};
    }


}
