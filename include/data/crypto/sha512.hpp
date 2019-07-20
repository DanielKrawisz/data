// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_SHA512
#define DATA_CRYPTO_SHA512

#include <data/data.hpp>
#include <data/slice.hpp>

namespace data {
    
    namespace sha512 {

        struct digest : public uint512 {
            using uint512::uint512;
            digest(uint32_t n) : uint512{n} {}
            digest(const digest&);
            digest(digest&&);
            
            bool valid() const;
            
            bool operator!=(const digest& d) const;
            bool operator==(const digest& d) const;
            bool operator>=(const digest& d) const;
            bool operator<=(const digest& d) const;
            bool operator>(const digest& d) const;
            bool operator<(const digest& d) const;
            
            byte& operator[](uint32 i);
            const byte& operator[](uint32 i) const;
            
            digest& operator=(const digest& d);
            
        };
        
        const digest Zero = digest{0};
        
        digest hash(const slice<byte>);
        digest hash(const bytes&);
        
        template <uint32_t n>
        digest hash(const array<byte, n>&);
        
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
        
        inline byte& digest::operator[](uint32 i) {
            return uint512::parent::operator[](i);
        }
        
        inline const byte& digest::operator[](uint32 i) const {
            return uint512::parent::operator[](i);
        }
    
    }

}

#endif
