// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_RIPEMD160
#define DATA_CRYPTO_RIPEMD160

#include <data/math/number/bounded.hpp>

namespace data {
    
    namespace ripemd160 {

        struct digest : public uint160 {
            
            digest(uint32_t n) : uint160{n} {}
            digest() : digest(0) {}
            digest(const digest& d) : uint160{d} {}
            digest(digest&& d) : uint160{static_cast<uint160&&>(d)} {};
            digest(const std::array<byte, 20>& a) : uint160{a} {}
            digest(std::array<byte, 20>&& a) : uint160{a} {}
            digest(bytes& b) : uint160{b} {}
            
            bool valid() const;
            bool operator==(const digest& d) const;
            bool operator!=(const digest& d) const;
            bool operator>=(const digest& d) const;
            bool operator<=(const digest& d) const;
            bool operator>(const digest& d) const;
            bool operator<(const digest& d) const;
            
            using uint160::operator[];
            
            digest& operator=(const digest& d);
        };
        
        const digest Zero = digest{};
        
        digest hash(bytes&);
        
        template <uint32_t n>
        digest hash(const array<byte, n>&);
        
        inline bool digest::valid() const {
            return operator!=(ripemd160::Zero);
        }
            
        inline bool digest::operator==(const digest& d) const {
            return uint160::operator==(static_cast<const uint160&>(d));
        }
        
        inline bool digest::operator!=(const digest& d) const {
            return uint160::operator!=(static_cast<const uint160&>(d));
        }
        
        inline bool digest::operator>=(const digest& d) const {
            return uint160::operator>=(static_cast<const uint160&>(d));
        }
        
        inline bool digest::operator<=(const digest& d) const {
            return uint160::operator<=(static_cast<const uint160&>(d));
        }
        
        inline bool digest::operator>(const digest& d) const {
            return uint160::operator>(static_cast<const uint160&>(d));
        }
        
        inline bool digest::operator<(const digest& d) const {
            return uint160::operator<(static_cast<const uint160&>(d));
        }
        
        inline digest& digest::operator=(const digest& d) {
            uint160::operator=(static_cast<const uint160&>(d));
            return *this;
        }
    
    }

}

#endif

