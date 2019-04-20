#ifndef DATA_CRYPTO_KEYPAIR
#define DATA_CRYPTO_KEYPAIR

#include "one_way.hpp"
#include "to_public.hpp"

namespace data {
    
    namespace crypto {
    
        // keypair is an assertion that to_public
        // is a non-invertable function between
        // sk and pk. 
        template <typename sk, typename pk> 
        struct keypair : one_way<to_public<sk, pk>, sk, pk> {};
    
    }

}

#endif
