// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_DIGEST
#define DATA_CRYPTO_DIGEST

#include <data/math/number/bounded.hpp>

namespace data::crypto {
    
    template <uint32 size>
    struct digest {
        uint<size> Digest;
        
        digest() : Digest(0) {}
        digest(uint<size> d) : Digest{d} {}
        digest(const digest& d) : Digest{d.Digest} {}
        digest(digest&& d);
        digest(bytes& b) : Digest{b} {}
        
        bool valid() const;
        
        digest& operator=(const digest&);
        bool operator==(const digest&) const;
        bool operator!=(const digest&) const;
        bool operator<(const digest&) const;
        bool operator>(const digest&) const;
        bool operator<=(const digest&) const;
        bool operator>=(const digest&) const;
    };
    
    template <uint32 size>
    inline bool digest<size>::valid() const {
        return operator!=(digest{0});
    }
    
    template <uint32 size>
    inline digest<size>& digest<size>::operator=(const digest<size>& d) {
        Digest = d.Digest;
        return *this;
    }

}

#endif

