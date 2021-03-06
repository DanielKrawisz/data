#include <data/crypto/sha512.hpp>
#include <crypto++/sha.h>

namespace data::sha512 {

    digest hash(const bytes_view data) {
        uint<size> hash{};
        CryptoPP::SHA512{}.CalculateDigest(hash.data(),data.begin(),data.size());
        return hash;
    }

}

