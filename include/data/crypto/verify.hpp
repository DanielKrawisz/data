#ifndef DATA_CRYPTO_VERIFY
#define DATA_CRYPTO_VERIFY

namespace data {
    
    namespace crypto {
        
        // template specialize this if you have your
        // own definition of verify for your types. 
        template <typename pk, typename sig> 
        struct verify {            
            bool operator()(const pk p, const sig g) const {
                return p.verify(g);
            }
        };
    
    }

}

#endif
