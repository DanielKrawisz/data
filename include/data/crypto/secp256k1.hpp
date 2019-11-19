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
        const size_t uncompressed_pubkey_size = 65;
        const size_t compressed_pubkey_size = 33;
        const size_t secret_size = 32;
        
        template <bool compressed_pubkey, uint32 pubkey_size> struct pubkey;
        
        using compressed_pubkey = pubkey<true, compressed_pubkey_size>;
        using uncompressed_pubkey = pubkey<false, uncompressed_pubkey_size>;
        
        using signature = bytes;
        
        struct secret {
            uint<secret_size> Value;
            
            bool valid() const;
            
            pubkey<true, compressed_pubkey_size> to_public_compressed() const;
            pubkey<false, uncompressed_pubkey_size> to_public_uncompressed() const;
            
            secret operator+(const secret&) const;
            secret operator*(const secret&) const;
            bool operator==(const secret&) const;
            
            signature sign(const sha256::digest&) const;
            
            operator bytes_view() const {
                return bytes_view(Value);
            }
        };
        
        template <bool compressed_pubkey, uint32 pubkey_size> 
        struct pubkey {
            std::array<byte, pubkey_size> Value;
            
            byte& operator[](int x) {
                return Value[x];
            }
            
            const byte& operator[](int x) const {
                return Value[x];
            }
            
            bool valid() const;
            
            pubkey operator+(const pubkey&) const;
            
            pubkey operator*(const secret&) const;
            
            pubkey() : Value{} {}
            pubkey(std::array<byte, pubkey_size> a) : Value{a} {}
            
            pubkey& operator=(const pubkey& p) {
                Value = p.Value;
                return *this;
            }
            
            bool operator==(const pubkey& p) const {
                return Value == p.Value;
            }
            
            bool verify(const sha256::digest&, const signature&) const;
            
            operator bytes_view() const {
                return bytes_view{Value.data(), Value.size()};
            }
        };
        
        constexpr data::math::module<compressed_pubkey, secret> is_module_compressed{};
        constexpr data::crypto::signature_scheme<secret, compressed_pubkey, const sha256::digest, signature> 
            is_signature_scheme_compressed{};
        
        constexpr data::math::module<uncompressed_pubkey, secret> is_module_uncompressed{};
        constexpr data::crypto::signature_scheme<secret, uncompressed_pubkey, const sha256::digest, signature> 
            is_signature_scheme_uncompressed{};
    
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
        
        namespace low {
            
            template <typename A, typename B> struct compression;
            
            template <typename A> struct compression<A, A> {
                const A& operator()(const A& a){
                    return a;
                }
            };

            template <> struct compression<compressed_pubkey, uncompressed_pubkey> {
                compressed_pubkey P;
                compressed_pubkey& operator()(const uncompressed_pubkey& p) {
                    libbitcoin::system::compress(P.Value, p.Value);
                    return P;
                }
            };
            
            template <> struct compression<uncompressed_pubkey, compressed_pubkey> {
                uncompressed_pubkey P;
                uncompressed_pubkey& operator()(const compressed_pubkey& p) {
                    libbitcoin::system::decompress(P.Value, p.Value);
                    return P;
                }
            };
            
            const libbitcoin::system::hash_digest libbitcoin(const sha256::digest& d);
        
        }
        
        inline bool secret::valid() const {
            return libbitcoin::system::verify(Value.Array);
        }
        
        template <bool compressed_pubkey, uint32 pubkey_size> 
        inline bool pubkey<compressed_pubkey, pubkey_size>::valid() const {
            return libbitcoin::system::verify(Value);
        }
        
        inline secp256k1::pubkey<true, compressed_pubkey_size>
        secret::to_public_compressed() const {
            secp256k1::pubkey<true, compressed_pubkey_size> x;
            libbitcoin::system::secret_to_public(x.Value, Value.Array);
            return x;
        }
        
        inline secp256k1::pubkey<false, uncompressed_pubkey_size>
        secret::to_public_uncompressed() const {
            secp256k1::pubkey<false, uncompressed_pubkey_size> x;
            libbitcoin::system::secret_to_public(x.Value, Value.Array);
            return x;
        }
        
        template <bool compressed_pubkey, uint32 pubkey_size> 
        inline bool pubkey<compressed_pubkey, pubkey_size>::verify(const sha256::digest& d, const signature& s) const {
            std::array<byte, 64> sig;
            std::copy(s.begin(), s.end(), sig.begin());
            return libbitcoin::system::verify_signature(Value, d.Digest.Array, sig);
        }
        
        inline secret secret::operator+(const secret& s) const {
            secret x = *this;
            libbitcoin::system::ec_add(x.Value.Array, s.Value.Array);
            return x;
        }
        
        inline secret secret::operator*(const secret& s) const {
            secret x = *this;
            libbitcoin::system::ec_multiply(x.Value.Array, s.Value.Array);
            return x;
        }

        inline bool secret::operator==(const secret &s) const {
            return Value == s.Value;
        }
        
        template <bool compressed_pubkey, uint32 pubkey_size> 
        inline pubkey<compressed_pubkey, pubkey_size>
        pubkey<compressed_pubkey, pubkey_size>::operator+(const pubkey& p) const {
            secp256k1::compressed_pubkey x = low::compression<secp256k1::compressed_pubkey, pubkey<compressed_pubkey, pubkey_size>>{}(*this);
            low::compression<secp256k1::compressed_pubkey, pubkey<compressed_pubkey, pubkey_size>> l{};
            if (!libbitcoin::system::ec_sum(x.Value, {l(p).Value})) return pubkey{};
            return low::compression<pubkey<compressed_pubkey, pubkey_size>, secp256k1::compressed_pubkey>{}(x);
        }
        
        template <bool compressed_pubkey, uint32 pubkey_size> 
        inline pubkey<compressed_pubkey, pubkey_size> 
        pubkey<compressed_pubkey, pubkey_size>::operator*(const secret& s) const {
            pubkey x = *this;
            if (!libbitcoin::system::ec_multiply(x.Value, s.Value.Array)) return pubkey{};
            return x;
        }
        
        inline signature secret::sign(const sha256::digest& d) const {
            std::array<byte, 64> s;
            if (!libbitcoin::system::sign(s, Value.Array, d.Digest.Array)) return signature{};
            signature sig{};
            std::copy(s.begin(), s.end(), sig.begin());
            return sig;
        }



    }

}

#endif
