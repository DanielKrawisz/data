// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_SECP256K1
#define DATA_CRYPTO_SECP256K1

#include <data/types.hpp>
#include <data/slice.hpp>
#include <data/crypto/signature_scheme.hpp>
#include <data/math/module.hpp>
#include <data/crypto/sha256.hpp>
#include <bitcoin/system/math/elliptic_curve.hpp>

namespace data {
    
    namespace secp256k1 {

        const uint pubkey_size = 33;
        const uint secret_size = 32;
        
        struct pubkey;
        
        using signature = libbitcoin::system::ec_signature;
        
        struct secret : public std::array<byte, secret_size> {
            using std::array<byte, secret_size>::operator[];
            bool valid() const;
            
            pubkey to_public() const;
            
            secret operator+(const secret&) const;
            secret operator*(const secret&) const;
            
            secret() : std::array<byte, secret_size>{} {}
            secret(std::array<byte, secret_size> a) : std::array<byte, secret_size>(a) {}
            
            secret& operator=(const secret& s) {
                static_cast<std::array<byte, secret_size>&>(*this) = static_cast<const std::array<byte, secret_size>&>(s);
                return *this;
            }
            
            signature sign(const sha256::digest&) const;
        };

        struct pubkey : public std::array<byte, pubkey_size> {
            using std::array<byte, pubkey_size>::operator[];
            bool valid() const;
            
            pubkey operator+(const pubkey&) const;
            
            pubkey operator*(const secret&) const;
            
            pubkey() : std::array<byte, pubkey_size>{} {}
            pubkey(std::array<byte, pubkey_size> a) : std::array<byte, pubkey_size>(a) {}
            
            pubkey& operator=(const pubkey& p) {
                static_cast<std::array<byte, pubkey_size>&>(*this) = static_cast<const std::array<byte, pubkey_size>&>(p);
                return *this;
            }
            
            bool verify(const sha256::digest&, const signature&) const;
        };
        
        constexpr data::math::module<pubkey, secret> is_module{};
        constexpr data::crypto::signature_scheme<secret, pubkey, const sha256::digest, libbitcoin::system::ec_signature> is_signature_scheme{};
    
    }

}

#endif
