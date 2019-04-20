#ifndef DATA_CRYPTO_TO_PUBLIC
#define DATA_CRYPTO_TO_PUBLIC

namespace data {
    
    namespace crypto {
        
        // template specialize this if you have your
        // own definition of to_public for your types. 
        template <typename sk, typename pk>
        struct to_public {
            pk operator()(const sk s) const {
                return s.to_public();
            }
        };
    
    }

}

#endif
