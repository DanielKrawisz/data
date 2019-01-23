#ifndef DATA_CRYPTO_SHA256_HPP
#define DATA_CRYPTO_SHA256_HPP

#include <data/types.hpp>
#include <data/slice.hpp>

namespace data {
    
    namespace sha256 {

        const uint size = 32;

        struct digest : public std::array<byte, size> {
            bool valid() const;
            
            digest();
            digest(std::array<byte, sha256::size> a) : std::array<byte, sha256::size>(a) {}
        };
        
        const digest zero = digest{};
        
        digest hash(const slice<byte>);
        
        inline bool digest::valid() const {
            return *this != zero;
        }
        
        inline digest::digest() : std::array<byte, sha256::size>() {}
    
    }

}

#endif

