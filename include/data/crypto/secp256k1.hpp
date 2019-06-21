// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_SECP256K1
#define DATA_CRYPTO_SECP256K1

#include <data/types.hpp>
#include <data/slice.hpp>
#include <data/crypto/signature_scheme.hpp>
#include <data/math/module.hpp>
#include <bitcoin/system/math/elliptic_curve.hpp>

namespace data {
    
    namespace secp256k1 {

        const uint pubkey_size = 33;
        const uint secret_size = 32;
        
        struct pubkey;
        
        struct secret : std::array<byte, secret_size> {
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
            
            libbitcoin::system::ec_signature sign(const libbitcoin::system::hash_digest&) const;
        };

        struct pubkey : public std::array<byte, pubkey_size> {
            bool valid() const;
            
            pubkey operator+(const pubkey&) const;
            
            pubkey operator*(const secret&) const;
            
            pubkey() : std::array<byte, pubkey_size>{} {}
            pubkey(std::array<byte, pubkey_size> a) : std::array<byte, pubkey_size>(a) {}
            
            pubkey& operator=(const pubkey& p) {
                static_cast<std::array<byte, pubkey_size>&>(*this) = static_cast<const std::array<byte, pubkey_size>&>(p);
                return *this;
            }
            
            bool verify(const libbitcoin::system::hash_digest&, const libbitcoin::system::ec_signature&) const;
        };
        
        constexpr data::math::module<pubkey, secret> is_module{};
        constexpr data::crypto::signature_scheme<secret, pubkey, libbitcoin::system::hash_digest, libbitcoin::system::ec_signature> is_signature_scheme{};
    
    }

}

#endif
