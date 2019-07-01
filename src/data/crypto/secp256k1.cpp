#include <data/crypto/secp256k1.hpp>

namespace data {
    
    namespace secp256k1 {
        
        namespace low {
        
            inline const ::libbitcoin::system::ec_secret& libbitcoin(const secret& s) {
                return static_cast<const std::array<byte, secret_size>&>(s);
            }
        
            inline const ::libbitcoin::system::ec_compressed& libbitcoin(const pubkey& s) {
                return static_cast<const std::array<byte, pubkey_size>&>(s);
            }
        
            inline ::libbitcoin::system::ec_secret& libbitcoin(secret& s) {
                return static_cast<std::array<byte, secret_size>&>(s);
            }
        
            inline ::libbitcoin::system::ec_compressed& libbitcoin(pubkey& s) {
                return static_cast<std::array<byte, pubkey_size>&>(s);
            }
            
            const libbitcoin::system::hash_digest convert_to_libbitcoin(const sha256::digest& d);
        
        }
        
        bool secret::valid() const {
            return libbitcoin::system::verify(low::libbitcoin(*this));
        }

        bool pubkey::valid() const {
            return libbitcoin::system::verify(low::libbitcoin(*this));
        }
        
        inline pubkey secret::to_public() const {
            pubkey x;
            libbitcoin::system::secret_to_public(low::libbitcoin(x), low::libbitcoin(*this));
            return x;
        }

        inline secret secret::operator+(const secret& s) const {
            secret x = *this;
            libbitcoin::system::ec_add(low::libbitcoin(x), low::libbitcoin(s));
            return x;
        }
        
        inline secret secret::operator*(const secret& s) const {
            secret x = *this;
            libbitcoin::system::ec_multiply(low::libbitcoin(x), low::libbitcoin(s));
            return x;
        }
        
        inline pubkey pubkey::operator+(const pubkey& p) const {
            pubkey x = *this;
            if (!libbitcoin::system::ec_sum(low::libbitcoin(x), {low::libbitcoin(p)})) return pubkey{};
            return x;
        }
        
        inline pubkey pubkey::operator*(const secret& s) const {
            pubkey x = *this;
            if (!libbitcoin::system::ec_multiply(low::libbitcoin(x), low::libbitcoin(s))) return pubkey{};
            return x;
        }
        
        libbitcoin::system::ec_signature secret::sign(const sha256::digest& d) const {
            libbitcoin::system::ec_signature sig;;
            if (!libbitcoin::system::sign(sig, low::libbitcoin(*this), low::convert_to_libbitcoin(d))) return libbitcoin::system::ec_signature{};
            return sig;
        }

        bool pubkey::verify(const sha256::digest& d, const libbitcoin::system::ec_signature& s) const {
            return libbitcoin::system::verify_signature(low::libbitcoin(*this), low::convert_to_libbitcoin(d), s);
        }
    
    }

}
