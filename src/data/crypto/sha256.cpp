#include <data/crypto/sha256.hpp>
//#include "sha256.h"

namespace data {
    
    namespace sha256 {

        digest hash(const slice<byte> data) {
            digest hash;
            SHA256_(data.data(), data.size(), hash.data());
            return hash;
        }
    
    }

}
