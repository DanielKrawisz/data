#ifndef DATA_CRYPTO_ONE_WAY
#define DATA_CRYPTO_ONE_WAY

#include <data/function.hpp>

namespace data::crypto {
    
    // one_way asserts that a non-invertable
    // function exists which takes s to p. 
    template <typename f, typename p, typename s>
    struct one_way : function::definition<f, p, s> {};
    
    // template specialize this if you have your
    // own definition of to_public for your types. 
    template <typename pk, typename sk>
    struct to_public : function::abstract<pk, sk> {
        to_public() = delete;
        
        pk operator()(sk s) const noexcept final override;
    };
    
    // keypair is an assertion that to_public
    // is a non-invertable function between
    // sk and pk. 
    template <typename sk, typename pk> 
    struct keypair : one_way<to_public<sk, pk>, sk, pk> {
        pk to_public(sk s) const {
            return crypto::to_public<pk, sk>(s);
        }
    };
        
    // template specialize this if you have your
    // own definition of the sign for your types. 
    template <typename sig, typename msg, typename sk> 
    struct sign : function::abstract<sig, msg, sk> {
        sign() = delete;
        
        sig operator()(msg m, sk s) const final override;
    };
    
    // template specialize this if you have your
    // own definition of verify for your types. 
    template <typename sig, typename pk> 
    struct verify : function::abstract<bool, sig, pk>{
        verify() = delete;
        
        bool operator()(sig g, pk p) const;
    };
        
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

#endif
