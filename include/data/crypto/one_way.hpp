// Copyright (c) 2018-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_ONE_WAY
#define DATA_CRYPTO_ONE_WAY

#include <data/function.hpp>

namespace data::crypto {
    
    enum class security : byte {
        none = 0,
        weak = 1,
        depricated = 2,
        modern = 3,
        encrypted = 4,
        integrity = 8,
        authenticated = 16
    };
    
    // keypair is an assertion that to_public
    // is a non-invertable function between
    // sk and pk. 
    template <typename sk, typename pk> 
    concept keypair = requires (sk s) {
        { to_public (s) } -> Same<pk>;
    };
    
    template <typename sk, typename pk, typename message, typename signature> 
    concept signature_scheme = keypair<sk, pk> && requires (message m, sk k) {
        { sign (m, k) } -> Same<signature>;
    } && requires (message m, signature x, pk k) {
        { verify (m, x, k) } -> Same<bool>;
    };
    
    template <typename sk, typename pk, typename message, typename encrypted> 
    concept encryption_scheme = keypair<sk, pk> && requires (message m, pk k) {
        { encrypt (m, k) } -> Same<encrypted>;
    } && requires (encrypted m, sk k) {
        {decrypt (m, k) } -> Same<message>;
    };
    
}

#endif
