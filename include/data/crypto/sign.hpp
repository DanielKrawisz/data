#ifndef DATA_CRYPTO_SIGN
#define DATA_CRYPTO_SIGN

namespace data {
    
    namespace crypto {
        
        // template specialize this if you have your
        // own definition of the sign for your types. 
        template <typename sk, typename msg, typename sig> 
        struct sign {            
            sig operator()(const sk s, const msg m) const {
                return s.sign(m);
            }
        };
    
    }

}

#endif
