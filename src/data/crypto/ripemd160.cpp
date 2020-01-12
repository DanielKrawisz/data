#include <data/crypto/ripemd160.hpp>
#include <crypto++/ripemd.h>

namespace data::ripemd160 {

    digest hash(const bytes_view data) {
        uint<size> hash{};
        CryptoPP::RIPEMD160{}.CalculateDigest(hash.data(), data.begin(), data.size());
        return hash;
    }


}

