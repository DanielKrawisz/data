// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_ENCRYPTED
#define DATA_CRYPTO_ENCRYPTED

#include <data/iterable.hpp>

// AES in cypher block chaining (CBC) mode. 
namespace data::crypto {
    
    template <size_t size> using symmetric_key = array<byte, size>;
    using initialization_vector = array<byte, 32>;
    
    template <size_t size>
    using encrypt = bytes (*)(bytes_view, const symmetric_key<16>&, const initialization_vector&);
    
    struct decrypted : bytes {
        decrypted(size_t size) : bytes(size) {}
        decrypted(const decrypted&) = delete;
        decrypted(decrypted&&) = default;
    };
    
    template <size_t size>
    using decrypt = decrypted (*)(bytes_view, const symmetric_key<16>&, const initialization_vector&);
    
    template <size_t size> 
    struct retriever {
        virtual symmetric_key<size> retrieve() = 0;
    };
    
    template <size_t size>
    struct encrypted : bytes {
        ptr<retriever<size>> Retriever;
        initialization_vector IV;
        crypto::decrypt<size> Decrypt;
        decrypted decrypt() {
            return Decrypt(*this, Retriever->retrieve(), IV);
        };
        
        encrypted(const bytes& b, 
            ptr<retriever<size>> r, 
            initialization_vector iv, 
            crypto::decrypt<size> d) : bytes{b}, Retriever{r}, IV{iv}, Decrypt{d} {}
    };
    
}

#endif

