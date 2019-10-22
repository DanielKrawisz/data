#include <data/crypto/sha512.hpp>
#include <crypto++/sha.h>

namespace data::sha512 {

    digest hash(const bytes_view data) {
        static CryptoPP::SHA512 shaHash;
        std::array<byte, size> hash;
        shaHash.CalculateDigest(hash.data(),data.begin(),data.size());
        return uint<size>{hash};
    }

}

