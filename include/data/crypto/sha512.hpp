// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_SHA512
#define DATA_CRYPTO_SHA512

#include <data/crypto/digest.hpp>

namespace data::sha512 {
    
    const uint32 size = 64;
    
    using digest = crypto::digest<size>;
    
    const digest Zero = digest{};
    
    digest hash(const bytes_view);
    
    inline digest hash(const string& s) {
        return hash(bytes_view{(byte*)(const_cast<string&>(s).data()), s.size()});
    }

    template <size_t n>
    digest hash(const std::array<byte, n>& data){
        return hash(bytes_view(data.data(), data.size()));
    };
    
    template <size_t n>
    digest hash(const uint<n>& data){
        return hash(data.Array);
    }
    
    template <typename A>
    inline digest double_hash(A a) {
        return hash<size>(hash(a).Digest);
    }

}

#endif
