// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_DIGEST
#define DATA_CRYPTO_DIGEST

#include <data/math/number/bounded/bounded.hpp>

namespace data::crypto::hash {
    
    template <size_t s>
    struct digest : math::uint_little<s> {
        
        using math::uint_little<s>::uint_little;
        
        digest () : math::uint_little<s> () {}
        
        digest (bytes_view b) : math::uint_little<s> {0} {
            if (b.size () == s) std::copy (b.begin (), b.end (), this->begin ());
        }
        
        bool valid () const;
    };
    
    template<size_t s>
    bool inline digest<s>::valid () const {
        return *this != digest {0};
    }

    template <size_t size> 
    std::ostream inline &operator << (std::ostream &o, const digest<size> &s) {
        return o << "digest{" << static_cast<math::uint_little<size>> (s) << "}";
    }
    
}

#endif

