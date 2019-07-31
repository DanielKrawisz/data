// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_SHA512
#define DATA_CRYPTO_SHA512

#include <data/math/number/bounded.hpp>
#include <data/slice.hpp>

namespace data {
    
    namespace sha512 {

        struct digest : public uint512 {
            
            digest(uint32_t n) : uint512{n} {}
            digest() : digest(0) {}
            digest(const digest& d) : uint512{d} {}
            digest(digest&& d) : uint512{static_cast<uint512&&>(d)} {};
            digest(const std::array<byte, 64>& a) : uint512{a} {}
            digest(std::array<byte, 64>&& a) : uint512{a} {}
            digest(bytes& b) : uint512{b} {}
            
            bool valid() const;
            
            bool operator!=(const digest& d) const;
            bool operator==(const digest& d) const;
            bool operator>=(const digest& d) const;
            bool operator<=(const digest& d) const;
            bool operator>(const digest& d) const;
            bool operator<(const digest& d) const;
            
            using uint512::operator[];
            
            digest& operator=(const digest& d);
            
        };
        
        const digest Zero = digest{0};
        
        digest hash(bytes&);
        
        template <uint32 n>
        digest hash(const std::array<byte, n>&);
        
        inline bool digest::valid() const {
            return operator!=(sha512::Zero);
        }
        
        inline bool digest::operator!=(const digest& d) const {
            return uint512::operator!=(static_cast<const uint512&>(d));
        }
            
        inline bool digest::operator==(const digest& d) const {
            return uint512::operator==(static_cast<const uint512&>(d));
        }
        
        inline bool digest::operator>=(const digest& d) const {
            return uint512::operator>=(static_cast<const uint512&>(d));
        }
        
        inline bool digest::operator<=(const digest& d) const {
            return uint512::operator<=(static_cast<const uint512&>(d));
        }
        
        inline bool digest::operator>(const digest& d) const {
            return uint512::operator>(static_cast<const uint512&>(d));
        }
        
        inline bool digest::operator<(const digest& d) const {
            return uint512::operator<(static_cast<const uint512&>(d));
        }
        
        inline digest& digest::operator=(const digest& d) {
            uint512::operator=(static_cast<const uint512&>(d));
            return *this;
        }
    
    }

}

#endif
