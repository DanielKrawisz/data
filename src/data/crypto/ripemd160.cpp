#include <data/crypto/ripemd160.hpp>
#include <crypto++/ripemd.h>

namespace data::ripemd160 {

    digest hash(const bytes_view data) {
        static CryptoPP::RIPEMD160 rhash;
        std::array<byte, size> hash;
        rhash.CalculateDigest(hash.data(),data.begin(),data.size());
        return uint<size>{hash};
    }


}

