#include <data/crypto/sha256.hpp>
#include <crypto++/sha.h>

namespace data::crypto::sha256 {

    digest hash(const bytes_view data) {
        uint<size> hash{};
        CryptoPP::SHA256{}.CalculateDigest(hash.data(), data.begin(), data.size());
        return hash;
    }

}
