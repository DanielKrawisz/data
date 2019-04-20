#ifndef DATA_CRYPTO_SHA256
#define DATA_CRYPTO_SHA256

#include <data/data.hpp>
#include <data/slice.hpp>

namespace data {
    
    namespace sha256 {

        struct digest : public uint256 {
            bool valid() const;
            
            digest() {};
            
            bool operator==(const digest& d) const {
                return uint256::operator==(static_cast<const uint256&>(d));
            }
            
            bool operator!=(const digest& d) const {
                return uint256::operator!=(static_cast<const uint256&>(d));
            }
            
            bool operator>=(const digest& d) const {
                return uint256::operator>=(static_cast<const uint256&>(d));
            }
            
            bool operator<=(const digest& d) const {
                return uint256::operator<=(static_cast<const uint256&>(d));
            }
            
            bool operator>(const digest& d) const {
                return uint256::operator>(static_cast<const uint256&>(d));
            }
            
            bool operator<(const digest& d) const {
                return uint256::operator<(static_cast<const uint256&>(d));
            }
        };
        
        const digest zero = digest{};
        
        digest hash(const slice<byte>);
        
        inline bool digest::valid() const {
            return operator!=(sha256::zero);
        }
    
    }

}

#endif
