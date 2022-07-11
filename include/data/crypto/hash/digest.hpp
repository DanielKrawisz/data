// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_DIGEST
#define DATA_CRYPTO_DIGEST

#include <data/math/number/bounded/bounded.hpp>

namespace data::crypto {
    
    template <size_t s>
    struct digest : math::uint_little<s> {
        static const uint32 size = s;
        
        using math::uint_little<s>::uint_little;
        
        digest() : math::uint_little<s>() {}
        
        digest(bytes_view b) : math::uint_little<s>{0} {
            if (b.size() == s) std::copy(b.begin(), b.end(), this->begin());
        }
        
        // I don't think this should be necessary. 
        operator bytes_view() const {
            return bytes_view(*this);
        }
        
        bool valid() const {
            return *this != 0;
        }
        
    };
    
}

#endif

