#ifndef DATA_CRYPTO_SHA256_HPP
#define DATA_CRYPTO_SHA256_HPP

#include <data/types.hpp>
#include <data/slice.hpp>
#include <data/math/number/bounded.hpp>

namespace data {
    
    struct uint256 : public math::number::bounded<4> {
        byte operator[](const uint n) const {
            return math::number::bounded<4>::operator[](n / 4) >> ((n % 4) * 8);
        }
    };
    
    namespace sha256 {

        struct digest : public uint256 {
            bool valid() const;
            
            digest() {};
        };
        
        const digest zero = digest{};
        
        digest hash(const slice<byte>);
        
        inline bool digest::valid() const {
            return *this != sha256::zero;
        }
    
    }

}

#endif

