// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_DIGEST
#define DATA_CRYPTO_DIGEST

#include <data/math/number/bounded.hpp>

namespace data::crypto {
    
    template <size_t s>
    struct digest : public math::uint_little<s, byte> {
        using math::uint_little<s, byte>::uint_little;
        digest (const math::uint_little<s, byte> &u) : math::uint_little<s, byte> {u} {}
        bool valid () const;
    };

    using digest128 = digest<16>;
    using digest160 = digest<20>;
    using digest224 = digest<28>;
    using digest256 = digest<32>;
    using digest320 = digest<40>;
    using digest384 = digest<48>;
    using digest448 = digest<56>;
    using digest512 = digest<64>;
    
    template<size_t s>
    bool inline digest<s>::valid () const {
        return *this != digest {0};
    }

    template <size_t size> 
    std::ostream inline &operator << (std::ostream &o, const digest<size> &s) {
        return o << encoding::hexidecimal::write (s);
    }
    
}

#endif

