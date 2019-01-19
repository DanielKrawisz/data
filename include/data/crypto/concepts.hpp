#ifndef DATA_CRYPTO_CONCEPTS_HPP
#define DATA_CRYPTO_CONCEPTS_HPP

namespace data {
    
    namespace crypto {
    
        template <typename sk, typename pk> 
        struct keypair {
            pk operator()(const sk s) const {
                return s.to_public();
            }
        };
        
        template <typename sk, typename pk, typename msg, typename sig> 
        struct signature_scheme : public keypair<sk, pk> {
            sig sign(const sk s, const msg m) const {
                return s.sign(m);
            }
            
            bool verify(const pk p, const sig g) {
                return p.verify(g);
            }
        };
    
    }

}

#endif

