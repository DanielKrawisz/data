// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_DIGEST
#define DATA_CRYPTO_DIGEST
#include "data/types.hpp"
#include <data/math/number/bounded.hpp>

namespace data::crypto {
    
    template <size_t s>
    struct digest {
        uint<s> Digest;
        static const uint32 size = s;
        
        digest() : Digest(0) {}
        digest(uint<s> d) : Digest{d} {}
        digest(uint32 d) : Digest{d} {}
        digest(const digest& d) : Digest{d.Digest} {}
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


    template<size_t s>
    inline bool digest<s>::valid() const {
        return operator!=(digest{0});
    }

    template<size_t s>
    inline digest<s>& digest<s>::operator=(const digest<s>& d) {
        Digest = d.Digest;
        return *this;
    }

    template<size_t s>
    inline bool digest<s>::operator==(const digest& other) const {
        return Digest == other.Digest;
    }

    template<size_t s>
    inline bool digest<s>::operator!=(const digest& other) const {
        return Digest!=other.Digest;
    }

    template<size_t s>
    inline bool digest<s>::operator<(const digest& other) const {
        return Digest < other.Digest;
    }

    template<size_t s>
    inline bool digest<s>::operator>(const digest& other ) const {
        return Digest > other.Digest;
    }

}

#endif

