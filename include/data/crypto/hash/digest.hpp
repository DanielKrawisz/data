// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_DIGEST
#define DATA_CRYPTO_DIGEST

#include <data/math/number/bounded/bounded.hpp>

namespace data::crypto {
    
    template <size_t s>
    struct digest : math::uint<endian::little, s> {
        static const uint32 size = s;
        
        using math::uint<endian::little, s>::uint;
        
        digest() : math::uint<endian::little, s>() {}
        
        digest(bytes_view b) : math::uint<endian::little, s>{0} {
            if (b.size() == s) std::copy(b.begin(), b.end(), this->begin());
        }
        
        operator bytes_view() const {
            return bytes_view(*this);
        }
        
        bool valid() const;
    };
    
    template<size_t s>
    inline bool digest<s>::valid() const {
        return operator!=(digest{0});
    }
    
}

#endif

