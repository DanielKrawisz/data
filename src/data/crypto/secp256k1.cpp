#include <data/crypto/secp256k1.hpp>

namespace data {
    
    namespace secp256k1 {
        
        namespace low {
        
            inline const ::libbitcoin::ec_secret& libbitcoin(const secret& s) {
                return static_cast<const std::array<byte, secret_size>&>(s);
            }
        
            inline const ::libbitcoin::ec_compressed& libbitcoin(const pubkey& s) {
                return static_cast<const std::array<byte, secret_size>&>(s);
            }
        
            inline ::libbitcoin::ec_secret& libbitcoin(secret& s) {
                return static_cast<std::array<byte, secret_size>&>(s);
            }
        
            inline ::libbitcoin::ec_compressed& libbitcoin(pubkey& s) {
                return static_cast<std::array<byte, secret_size>&>(s);
            }
        
        }
        
        bool secret::valid() const {
            libbitcoin::verify(low::libbitcoin(*this));
        }

        bool pubkey::valid() const {
            libbitcoin::verify(low::libbitcoin(*this));
        }
        
        inline pubkey secret::to_public() const {
            pubkey x;
            libbitcoin::secret_to_public(low::libbitcoin(x), low::libbitcoin(*this));
            return x;
        }

        inline secret secret::operator+(const secret& s) const {
            secret x = *this;
            libbitcoin::ec_add(low::libbitcoin(x), low::libbitcoin(s));
            return x;
        }
        
        inline secret secret::operator*(const secret& s) const {
            secret x = *this;
            libbitcoin::ec_multiply(low::libbitcoin(x), low::libbitcoin(s));
            return x;
        }
        
        inline pubkey pubkey::operator+(const pubkey& p) const {
            pubkey x = *this;
            if (!libbitcoin::ec_sum(low::libbitcoin(x), {low::libbitcoin(p)})) return pubkey{};
            return x;
        }
        
        inline pubkey pubkey::operator*(const secret& s) const {
            pubkey x = *this;
            if (!libbitcoin::ec_multiply(low::libbitcoin(x), low::libbitcoin(s))) return pubkey{};
            return x;
        }
        
        libbitcoin::ec_signature secret::sign(const libbitcoin::hash_digest& h) const {
            libbitcoin::ec_signature sig;
            if (!libbitcoin::sign(sig, low::libbitcoin(*this), h)) return libbitcoin::ec_signature{};
            return sig;
        }

        bool pubkey::verify(const libbitcoin::hash_digest& d, const libbitcoin::ec_signature& s) const {
            return libbitcoin::verify_signature(low::libbitcoin(*this), d, s);
        }
    
    }

}
