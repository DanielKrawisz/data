#ifndef DATA_CRYPTO_SHA512
#define DATA_CRYPTO_SHA512

#include <data/data.hpp>
#include <data/slice.hpp>

namespace data {
    
    namespace sha512 {

        struct digest : public math::number::bounded<8> {
            
            digest(uint32_t n) : math::number::bounded<8>{n} {}
            digest() : digest(0) {}
            
            bool valid() const;
            
            bool operator!=(const digest& d) const;
            bool operator==(const digest& d) const;
            
        };
        
        const digest Zero = digest{0};
        
        digest hash(const slice<byte>);
        
        inline bool digest::valid() const {
            return operator!=(sha512::Zero);
        }
        
        inline bool digest::operator!=(const digest& d) const {
            return math::number::bounded<8>::operator!=(static_cast<const math::number::bounded<8>&>(d));
        }
            
        inline bool digest::operator==(const digest& d) const {
            return math::number::bounded<8>::operator==(static_cast<const math::number::bounded<8>&>(d));
        }
    
    }

}

#endif
