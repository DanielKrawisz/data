// Copyright (c) 2018-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_ONE_WAY
#define DATA_CRYPTO_ONE_WAY

#include <data/function.hpp>

namespace data::crypto {
    
    // one_way asserts that a non-invertable
    // function exists which takes s to p. 
    template <typename f, typename p, typename s>
    concept one_way = std::regular_invocable<f, s> && requires (f fun, s secret) {
        {fun(secret)} -> std::same_as<p>;
    };
    
    // keypair is an assertion that to_public
    // is a non-invertable function between
    // sk and pk. 
    template <typename sk, typename pk> 
    concept keypair = requires (sk s) {
        {to_public(s)} -> std::same_as<pk>;
    };
    
    template <typename sk, typename pk, typename message, typename signature> 
    concept signature_scheme = keypair<sk, pk> && requires (message m, sk k) {
        {sign(m, k)} -> std::same_as<signature>;
    } && requires (message m, signature x, pk k) {
        {verify(m, x, k)} -> std::same_as<bool>;
    };
    
    template <typename sk, typename pk, typename message, typename encrypted> 
    concept encryption_scheme = keypair<sk, pk> && requires (message m, pk k) {
        {encrypt(m, k)} -> std::same_as<encrypted>;
    } && requires (encrypted m, sk k) {
        {decrypt(m, k)} -> std::same_as<message>;
    };
    
}

#endif
