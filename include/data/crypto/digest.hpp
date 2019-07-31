// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_DIGEST
#define DATA_CRYPTO_DIGEST

#include <data/math/number/bounded.hpp>

namespace data::crypto {
    
    template <uint32 size>
    struct digest : math::number::bounded::uint<size> {
        using uint = math::number::bounded::uint<size>;
        using uint::uint;
        
        digest() : digest(0) {}
        digest(const digest& d) : uint{d} {}
        digest(digest&& d) : uint{static_cast<uint&&>(d)} {};
        digest(bytes& b) : uint{b} {}
        
        bool valid() const;
        
        digest& operator=(const digest& d);
    };
    
    template <uint32 size>
    inline bool digest<size>::valid() const {
        return operator!=(digest{0});
    }
    
    template <uint32 size>
    inline digest<size>& digest<size>::operator=(const digest<size>& d) {
        uint160::operator=(static_cast<const uint160&>(d));
        return *this;
    }

}

#endif

