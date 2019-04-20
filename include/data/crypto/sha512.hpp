#ifndef DATA_CRYPTO_SHA512
#define DATA_CRYPTO_SHA512

#include <data/data.hpp>
#include <data/slice.hpp>

namespace data {
    
    namespace sha512 {

        struct digest : public math::number::bounded<8> {
            bool valid() const;
            
            digest() {};
            
            bool operator==(const digest& d) const {
                return math::number::bounded<8>::operator==(static_cast<const math::number::bounded<8>&>(d));
            }
            
            bool operator!=(const digest& d) const {
                return math::number::bounded<8>::operator!=(static_cast<const math::number::bounded<8>&>(d));
            }
            
        };
        
        const digest zero = digest{};
        
        digest hash(const slice<byte>);
        
        inline bool digest::valid() const {
            return operator!=(sha512::zero);
        }
    
    }

}

#endif
