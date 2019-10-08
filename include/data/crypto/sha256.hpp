// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_SHA256
#define DATA_CRYPTO_SHA256

#include <data/slice.hpp>
#include <data/crypto/digest.hpp>
#include <crypto++/sha.h>

namespace data::sha256 {
    
    const size_t size = 32;
    
    using digest = crypto::digest<size>;
    
    const digest Zero = digest{};
    
    digest hash(const slice<byte>&);
    
    inline digest hash(const bytes& b) {
        return hash(slice<byte>::make(b));
    }
    
    inline digest hash(const string& s) {
        return hash(slice<byte>{(byte*)(const_cast<string&>(s).data()), s.size()});
    }

    template <size_t n>
    digest hash(const std::array<byte, n>& data){
        return hash(slice<byte>::make(data));
    };
    
    template <size_t n>
    digest hash(const uint<n>& data){
        return hash(slice<byte>::make(data.Array));
    }
    
    template <typename A>
    inline digest double_hash(A a) {
        return hash<32>(hash(a).Digest);
    }

}

#endif
