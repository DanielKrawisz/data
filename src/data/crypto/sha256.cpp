#include <data/crypto/sha256.hpp>
#include <crypto++/sha.h>
#include <data/slice.hpp>
#include <data/io/unimplemented.hpp>

namespace data::sha256 {

    digest hash(const data::slice<byte>& data) {
        std::array<byte, size> hash;
        CryptoPP::SHA256 shaHash;
        shaHash.CalculateDigest(hash.data(),data.begin(),data.size());
        return uint<size>{hash};
    }


}
