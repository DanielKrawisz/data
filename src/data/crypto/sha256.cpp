#include <data/crypto/sha256.hpp>
#include <crypto++/sha.h>
#include <data/slice.hpp>

namespace data::sha256 {
    digest hash(bytes& data) {
        std::array<byte, size> hash;
        CryptoPP::SHA256 shaHash;
        shaHash.CalculateDigest(hash.data(),data.data(),data.size());
        return uint<size>{hash};
    }

    digest hash(data::slice<byte>& data) {
        std::array<byte, size> hash;
        CryptoPP::SHA256 shaHash;
        shaHash.CalculateDigest(hash.data(),data.begin(),data.size());
        return uint<size>{hash};
    }


}
