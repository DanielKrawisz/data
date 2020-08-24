// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_ENCRYPTED
#define DATA_CRYPTO_ENCRYPTED

#include <data/iterable.hpp>
#include <data/stream.hpp>
#include <data/crypto/sha256.hpp>

// AES in cypher block chaining (CBC) mode. 
namespace data::crypto {
    
    template <size_t size> 
    using symmetric_key = array<byte, size>;
    
    using initialization_vector = array<byte, 32>;
    
    template <size_t size>
    using encryption = bytes (*)(bytes_view, const symmetric_key<size>&, const initialization_vector&);
    
    template <size_t size>
    using decryption = bytes (*)(bytes_view, const symmetric_key<size>&, const initialization_vector&);
    
    template <size_t size> 
    struct retriever {
        virtual symmetric_key<size> retrieve() = 0;
    };
    
    struct encrypted {
        bytes Data;
        initialization_vector& IV;
    };
    
    template <size_t size>
    inline encrypted encrypt(bytes_view b, encryption<size> e, const symmetric_key<size>& k, const initialization_vector& iv) {
        return {e(stream::write_bytes(8 + b.size(), uint64_big{0}, uint64_big{b.size()}, b), k, iv), iv};
    }
    
    struct decrypted;
    
    template <size_t size>
    decrypted decrypt(const encrypted& e, decryption<size> d, const symmetric_key<size>& k);
    
    struct decrypted {
        bytes Data;
        bool Valid;
        decrypted() : Data{}, Valid{false} {}
        
        decrypted(decrypted&&) = default;
        
    private:
        decrypted(const bytes& d) : Data{d}, Valid{true} {} 
        decrypted(const decrypted&) = delete;
        
        template <size_t size>
        friend decrypted decrypt(const encrypted& e, decryption<size> d, const symmetric_key<size>& k);
    };
    
    template <size_t size>
    decrypted decrypt(const encrypted& e, decryption<size> d, const symmetric_key<size>& k) {
        bytes x = d(e.Data, k, e.IV);
        reader<bytes::iterator> r(x.begin(), x.end());
        uint64_big check;
        r >> check;
        if (check != 0) return decrypted{};
        uint64_big len;
        r >> len;
        bytes data(len);
        r >> data;
        return decrypted{data};
    }
    
    template <size_t size>
    struct locked : encrypted {
        ptr<retriever<size>> Retriever;
        crypto::decryption<size> Decrypt;
        decrypted decrypt() const {
            return crypto::decrypt(*this, Decrypt, Retriever->retrieve());
        };
        
        locked(const bytes& b, 
            initialization_vector iv, 
            crypto::decryption<size> d, 
            ptr<retriever<size>> r) : encrypted{b, iv}, Retriever{r}, Decrypt{d} {}
    };
    
    // A key retriever that prints a message to the user and then reads a passphrase. 
    struct read_passphrase : retriever<32> {
        std::string UserMessage;
        std::ostream& Cout;
        std::istream& Cin;
        
        symmetric_key<32> retrieve() override {
            Cout << UserMessage;
            std::string line;
            std::getline(Cin, line);
            symmetric_key<32> x;
            auto d = sha256::hash(line);
            std::copy(d.begin(), d.end(), x.begin());
            return x;
        }
    };
    
}

#endif

