#ifndef DATA_CRYPTO_SIGNATURE_SCHEME
#define DATA_CRYPTO_SIGNATURE_SCHEME

#include "keypair.hpp"
#include "sign.hpp"
#include "verify.hpp"

namespace data {
    
    namespace crypto {
        
        template <typename sk, typename pk, typename msg, typename sig> 
        struct signature_scheme : public keypair<sk, pk> {
            sig sign(const sk s, const msg m) const {
                return crypto::sign<sk, msg, sig>{}(s, m);
            }
            
            bool verify(const pk p, const sig g) const {
                return crypto::verify<pk, sig>{}(p, g);
            }
        };
    
    }

}

#endif
