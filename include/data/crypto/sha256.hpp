// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_SHA256
#define DATA_CRYPTO_SHA256

#include <data/data.hpp>
#include <data/slice.hpp>

namespace data {
    
    namespace sha256 {

        struct digest : public uint256 {
            using uint256::uint256;
            
            digest(uint32_t n) : uint256{n} {}
            digest() : digest(0) {}
            digest(const digest&);
            digest(digest&&);
            
            bool valid() const;
            bool operator==(const digest& d) const;
            bool operator!=(const digest& d) const;
            bool operator>=(const digest& d) const;
            bool operator<=(const digest& d) const;
            bool operator>(const digest& d) const;
            bool operator<(const digest& d) const;
            
            digest& operator=(const digest& d);
        };
        
        const digest Zero = digest{};

        digest hash(const bytes&);
        
        template <uint32_t n>
        digest hash(const array<byte, n>&);
        
        inline bool digest::valid() const {
            return operator!=(sha256::Zero);
        }
            
        inline bool digest::operator==(const digest& d) const {
            return uint256::operator==(static_cast<const uint256&>(d));
        }
        
        inline bool digest::operator!=(const digest& d) const {
            return uint256::operator!=(static_cast<const uint256&>(d));
        }
        
        inline bool digest::operator>=(const digest& d) const {
            return uint256::operator>=(static_cast<const uint256&>(d));
        }
        
        inline bool digest::operator<=(const digest& d) const {
            return uint256::operator<=(static_cast<const uint256&>(d));
        }
        
        inline bool digest::operator>(const digest& d) const {
            return uint256::operator>(static_cast<const uint256&>(d));
        }
        
        inline bool digest::operator<(const digest& d) const {
            return uint256::operator<(static_cast<const uint256&>(d));
        }
        
        inline digest& digest::operator=(const digest& d) {
            uint256::operator=(static_cast<const uint256&>(d));
            return *this;
        }
    
    }

}

#endif
