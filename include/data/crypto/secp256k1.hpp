// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_SECP256K1
#define DATA_CRYPTO_SECP256K1

#include <data/types.hpp>
#include <data/slice.hpp>
#include <data/crypto/one_way.hpp>
#include <data/math/module.hpp>
#include <data/crypto/sha256.hpp>
#include <bitcoin/system/math/elliptic_curve.hpp>

namespace data::crypto {
    namespace secp256k1 {
    
        // two pubkey formats: compressed and uncompressed. 
        const uint32 uncompressed_pubkey_size = 65;
        const uint32 compressed_pubkey_size = 33;
        const uint32 secret_size = 32;
        
        template <bool compressed_pubkey, uint32 pubkey_size> struct pubkey;
        
        using compressed_pubkey = pubkey<true, compressed_pubkey_size>;
        using uncompressed_pubkey = pubkey<false, uncompressed_pubkey_size>;
        
        using signature = libbitcoin::system::ec_signature;
        
        struct secret : public uint<secret_size> {
            
            bool valid() const;
            
            pubkey<true, compressed_pubkey_size> to_public_compressed() const;
            pubkey<false, uncompressed_pubkey_size> to_public_uncompressed() const;
            
            secret operator+(const secret&) const;
            secret operator*(const secret&) const;
            
            using uint<secret_size>::number;
            
            signature sign(const sha256::digest&) const;
        };
        
        template <bool compressed_pubkey, uint32 pubkey_size> 
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
    
    }
    
    template <> struct sign<secp256k1::signature, sha256::digest, secp256k1::secret>
        : function::abstract<secp256k1::signature, sha256::digest, const secp256k1::secret&> {
        secp256k1::signature operator()(sha256::digest m, const secp256k1::secret& s) const noexcept final override {
            return s.sign(m);
        }
    };
    
    template <> struct to_public<secp256k1::compressed_pubkey, secp256k1::secret>
        : function::abstract<secp256k1::compressed_pubkey, const secp256k1::secret&> {
        secp256k1::compressed_pubkey operator()(const secp256k1::secret& s) const noexcept final override {
            return s.to_public_compressed();
        }
    };
    
    template <> struct verify<secp256k1::signature, secp256k1::compressed_pubkey> 
        : function::abstract<bool, 
            const sha256::digest&,
            const secp256k1::signature&, 
            const secp256k1::compressed_pubkey&> {
        bool operator()(
            const sha256::digest& d, 
            const secp256k1::signature& x, 
            const secp256k1::compressed_pubkey& p) const noexcept final override {
            return p.verify(d, x);
        }
    };
    
    template <> struct to_public<secp256k1::uncompressed_pubkey, secp256k1::secret>
        : function::abstract<secp256k1::uncompressed_pubkey, const secp256k1::secret&> {
        secp256k1::uncompressed_pubkey operator()(const secp256k1::secret& s) const noexcept final override {
            return s.to_public_uncompressed();
        }
    };
    
    template <> struct verify<secp256k1::signature, secp256k1::uncompressed_pubkey> 
        : function::abstract<bool, 
            const sha256::digest&,
            const secp256k1::signature&,
            const secp256k1::uncompressed_pubkey&> {
        bool operator()(
            const sha256::digest& d, 
            const secp256k1::signature& x,
            const secp256k1::uncompressed_pubkey& p) const noexcept final override {
            return p.verify(d, x);
        }
    };
    
    namespace secp256k1 { 
    
        constexpr data::math::module<compressed_pubkey, secret> is_module_compressed{};
        constexpr data::crypto::signature_scheme<secret, compressed_pubkey, const sha256::digest, signature> 
            is_signature_scheme_compressed{};
        
        constexpr data::math::module<uncompressed_pubkey, secret> is_module_uncompressed{};
        constexpr data::crypto::signature_scheme<secret, uncompressed_pubkey, const sha256::digest, signature> 
            is_signature_scheme_uncompressed{};
        
        namespace low {
        
            template <bool compressed_pubkey, uint32 pubkey_size>
            inline const std::array<byte, pubkey_size>& 
            libbitcoin(const pubkey<compressed_pubkey, pubkey_size>& s) {
                return static_cast<const std::array<byte, pubkey_size>&>(s);
            }
        
            template <bool compressed_pubkey, uint32 pubkey_size>
            inline std::array<byte, pubkey_size>& 
            libbitcoin(pubkey<compressed_pubkey, pubkey_size>& s) {
                return static_cast<std::array<byte, pubkey_size>&>(s);
            }
            
            template <typename A, typename B> struct compression;
            
            template <typename A> struct compression<A, A> {
                const A& operator()(const A& a){
                    return a;
                }
            };
            
            template <> struct compression<compressed_pubkey, uncompressed_pubkey> {
                compressed_pubkey P;
                compressed_pubkey& operator()(const uncompressed_pubkey& p) {
                    libbitcoin::system::compress(P, p);
                    return P;
                }
            };
            
            template <> struct compression<uncompressed_pubkey, compressed_pubkey> {
                uncompressed_pubkey P;
                uncompressed_pubkey& operator()(const compressed_pubkey& p) {
                    libbitcoin::system::decompress(P, p);
                    return P;
                }
            };
            
            const libbitcoin::system::hash_digest libbitcoin(const sha256::digest& d);
        
        }
        
        inline bool secret::valid() const {
            return libbitcoin::system::verify(low::libbitcoin(*this));
        }
        
        template <bool compressed_pubkey, uint32 pubkey_size> 
        inline bool pubkey<compressed_pubkey, pubkey_size>::valid() const {
            return libbitcoin::system::verify(low::libbitcoin(*this));
        }
        
        inline secp256k1::pubkey<true, compressed_pubkey_size>
        secret::to_public_compressed() const {
            secp256k1::pubkey<true, compressed_pubkey_size> x;
            libbitcoin::system::secret_to_public(low::libbitcoin(x), low::libbitcoin(*this));
            return x;
        }
        
        inline secp256k1::pubkey<false, uncompressed_pubkey_size>
        secret::to_public_uncompressed() const {
            secp256k1::pubkey<false, uncompressed_pubkey_size> x;
            libbitcoin::system::secret_to_public(low::libbitcoin(x), low::libbitcoin(*this));
            return x;
        }
        
        template <bool compressed_pubkey, uint32 pubkey_size> 
        inline bool pubkey<compressed_pubkey, pubkey_size>::verify(const sha256::digest& d, const signature& s) const {
            return libbitcoin::system::verify_signature(low::libbitcoin(*this), low::libbitcoin(d), s);
        }
        
        inline secret secret::operator+(const secret& s) const {
            secret x = *this;
            libbitcoin::system::ec_add(x.Array, s.Array);
            return x;
        }
        
        inline secret secret::operator*(const secret& s) const {
            secret x = *this;
            libbitcoin::system::ec_multiply(x.Array, s.Array);
            return x;
        }
        
        template <bool compressed_pubkey, uint32 pubkey_size> 
        inline pubkey<compressed_pubkey, pubkey_size>
        pubkey<compressed_pubkey, pubkey_size>::operator+(const pubkey& p) const {
            secp256k1::compressed_pubkey x = low::compression<secp256k1::compressed_pubkey, pubkey<compressed_pubkey, pubkey_size>>{}(*this);
            low::compression<secp256k1::compressed_pubkey, pubkey<compressed_pubkey, pubkey_size>> l{};
            if (!libbitcoin::system::ec_sum(low::libbitcoin(x), {l(low::libbitcoin(p))})) return pubkey{};
            return low::compression<pubkey<compressed_pubkey, pubkey_size>, secp256k1::compressed_pubkey>{}(x);
        }
        
        template <bool compressed_pubkey, uint32 pubkey_size> 
        inline pubkey<compressed_pubkey, pubkey_size> 
        pubkey<compressed_pubkey, pubkey_size>::operator*(const secret& s) const {
            pubkey x = *this;
            if (!libbitcoin::system::ec_multiply(low::libbitcoin(x), low::libbitcoin(s))) return pubkey{};
            return x;
        }
        
        inline signature secret::sign(const sha256::digest& d) const {
            signature sig;
            if (!libbitcoin::system::sign(sig, low::libbitcoin(*this), low::libbitcoin(d))) return signature{};
            return sig;
        }
    
    }

}

#endif
