#ifndef DATA_CRYPTO_ONE_WAY
#define DATA_CRYPTO_ONE_WAY

#include <data/function.hpp>

namespace data {
    
    namespace crypto {
        
        // one_way asserts that a non-invertable
        // function exists which takes s to p. 
        template <typename f, typename s, typename p>
        struct one_way : function::definition<f, s, p> {};
        
    }

}

#endif
