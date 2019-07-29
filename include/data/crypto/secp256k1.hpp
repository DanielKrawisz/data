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
        
        // two pubkey formats: compressed and uncompressed. 
        const uint uncompressed_pubkey_size = 65;
        const uint compressed_pubkey_size = 33;
        const uint secret_size = 32;
        
        using signature = libbitcoin::system::ec_signature;
        
        template <bool compressed_pubkey, uint32 pubkey_size> struct pubkey;
        
        using compressed_pubkey = pubkey<true, compressed_pubkey_size>;
        using uncompressed_pubkey = pubkey<false, uncompressed_pubkey_size>;
        
        template <bool compressed_pubkey, uint32 pubkey_size> 
        struct secret : public std::array<byte, secret_size> {
            using pubkey = secp256k1::pubkey<compressed_pubkey, pubkey_size>;
            
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
        
        using compressed_secret = secret<true, compressed_pubkey_size>;
        using uncompressed_secret = secret<false, uncompressed_pubkey_size>;

        template <bool compressed_pubkey, uint32 pubkey_size> 
        struct pubkey : public std::array<byte, pubkey_size> {
            using secret = secp256k1::secret<compressed_pubkey, pubkey_size>;
            
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
        
        constexpr data::math::module<compressed_pubkey, compressed_secret> is_module_compressed{};
        constexpr data::crypto::signature_scheme<compressed_secret, compressed_pubkey, const sha256::digest, signature> is_signature_scheme_compressed{};
        
        constexpr data::math::module<uncompressed_pubkey, uncompressed_secret> is_module_uncompressed{};
        constexpr data::crypto::signature_scheme<uncompressed_secret, uncompressed_pubkey, const sha256::digest, signature> is_signature_scheme_uncompressed{};
        
        namespace low {
        
            template <bool compressed_pubkey, uint32 pubkey_size>
            inline const std::array<byte, secret_size>& 
            libbitcoin(const secret<compressed_pubkey, pubkey_size>& s) {
                return static_cast<const std::array<byte, secret_size>&>(s);
            }
        
            template <bool compressed_pubkey, uint32 pubkey_size>
            inline const std::array<byte, pubkey_size>& 
            libbitcoin(const pubkey<compressed_pubkey, pubkey_size>& s) {
                return static_cast<const std::array<byte, pubkey_size>&>(s);
            }
        
            template <bool compressed_pubkey, uint32 pubkey_size>
            inline std::array<byte, secret_size>& 
            libbitcoin(secret<compressed_pubkey, pubkey_size>& s) {
                return static_cast<std::array<byte, secret_size>&>(s);
            }
        
            template <bool compressed_pubkey, uint32 pubkey_size>
            inline std::array<byte, pubkey_size>& 
            libbitcoin(pubkey<compressed_pubkey, pubkey_size>& s) {
                return static_cast<std::array<byte, pubkey_size>&>(s);
            }
            
            const libbitcoin::system::hash_digest libbitcoin(const sha256::digest& d);
        
        }
        
        template <bool compressed_pubkey, uint32 pubkey_size> 
        inline bool secret<compressed_pubkey, pubkey_size>::valid() const {
            return libbitcoin::system::verify(low::libbitcoin(*this));
        }

        template <bool compressed_pubkey, uint32 pubkey_size> 
        inline bool pubkey<compressed_pubkey, pubkey_size>::valid() const {
            return libbitcoin::system::verify(low::libbitcoin(*this));
        }
        
        template <bool compressed_pubkey, uint32 pubkey_size> 
        inline pubkey<compressed_pubkey, pubkey_size>
        secret<compressed_pubkey, pubkey_size>::to_public() const {
            pubkey x;
            libbitcoin::system::secret_to_public(low::libbitcoin(x), low::libbitcoin(*this));
            return x;
        }
        
        template <bool compressed_pubkey, uint32 pubkey_size> 
        inline bool pubkey<compressed_pubkey, pubkey_size>::verify(const sha256::digest& d, const signature& s) const {
            return libbitcoin::system::verify_signature(low::libbitcoin(*this), low::convert_to_libbitcoin(d), s);
        }
        
        template <bool compressed_pubkey, uint32 pubkey_size> 
        inline secret<compressed_pubkey, pubkey_size> 
        secret<compressed_pubkey, pubkey_size>::operator+(const secret& s) const {
            secret x = *this;
            libbitcoin::system::ec_add(low::libbitcoin(x), low::libbitcoin(s));
            return x;
        }
        
        template <bool compressed_pubkey, uint32 pubkey_size> 
        inline secret<compressed_pubkey, pubkey_size> 
        secret<compressed_pubkey, pubkey_size>::operator*(const secret& s) const {
            secret x = *this;
            libbitcoin::system::ec_multiply(low::libbitcoin(x), low::libbitcoin(s));
            return x;
        }
        
        template <bool compressed_pubkey, uint32 pubkey_size> 
        inline pubkey<compressed_pubkey, pubkey_size>
        pubkey<compressed_pubkey, pubkey_size>::operator+(const pubkey& p) const {
            pubkey x = *this;
            if (!libbitcoin::system::ec_sum(low::libbitcoin(x), {low::libbitcoin(p)})) return pubkey{};
            return x;
        }
        
        template <bool compressed_pubkey, uint32 pubkey_size> 
        inline pubkey<compressed_pubkey, pubkey_size> 
        pubkey<compressed_pubkey, pubkey_size>::operator*(const secret& s) const {
            pubkey x = *this;
            if (!libbitcoin::system::ec_multiply(low::libbitcoin(x), low::libbitcoin(s))) return pubkey{};
            return x;
        }
        
        template <bool compressed_pubkey, uint32 pubkey_size> 
        inline signature secret<compressed_pubkey, pubkey_size>::sign(const sha256::digest& d) const {
            signature sig;
            if (!libbitcoin::system::sign(sig, low::libbitcoin(*this), low::libbitcoin(d))) return signature{};
            return sig;
        }
    
    }

}

#endif
