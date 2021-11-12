// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_DIGEST
#define DATA_CRYPTO_DIGEST

#include "data/types.hpp"
#include <data/math/number/bounded/bounded.hpp>

namespace data::crypto {
    
    template <size_t s>
    struct digest : uint<s> {
        static const uint32 size = s;
        
        using uint<s>::uint;
        
        digest() : uint<s>() {}
        
        digest(bytes_view b) : uint<s>{0} {
            if (b.size() == s) std::copy(b.begin(), b.end(), uint<s>::begin());
        }
        
        bool valid() const;
        
        digest& operator=(const digest&);
    };
    
    template<size_t s>
    inline bool digest<s>::valid() const {
        return operator!=(digest{0});
    }
    
    template<size_t s>
    inline digest<s>& digest<s>::operator=(const digest<s>& d) {
        uint<s>::operator=(d);
        return *this;
    }
    
}

#endif

