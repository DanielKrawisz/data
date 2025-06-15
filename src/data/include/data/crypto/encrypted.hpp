// Copyright (c) 2020-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_ENCRYPTED
#define DATA_CRYPTO_ENCRYPTED

#include <data/iterable.hpp>
#include <data/stream.hpp>
#include <data/crypto/hash.hpp>

namespace data::crypto {
    
    template <size_t key_size>
    using symmetric_key = byte_array<key_size>;
    
    template <size_t block_size>
    using initialization_vector = byte_array<block_size>;
    
    template <size_t block_size, size_t key_size>
    using encryption = function<bytes (const initialization_vector<block_size> &, const symmetric_key<key_size> &, byte_slice)>;
    
    template <size_t block_size, size_t key_size>
    using decryption = function<bytes (const initialization_vector<block_size> &, const symmetric_key<key_size> &, byte_slice)>;
    
    template <size_t size> 
    struct retriever {
        virtual symmetric_key<size> retrieve () = 0;
    };

    template <size_t block_size>
    struct encrypted {
        bytes Data;
        initialization_vector<block_size> &IV;
        
        bool operator == (const encrypted &e) const {
            return Data == e.Data && IV == e.IV;
        }
        
        bool operator != (const encrypted &e) const {
            return !operator == (e);
        }
    };
    
    template <size_t block_size, size_t key_size>
    inline encrypted<block_size> encrypt
        (byte_slice b, encryption<block_size, key_size> e, const symmetric_key<key_size> &k, const initialization_vector<block_size> &iv) {
        return {e (write_bytes (12 + b.size (), uint64_big {0}, uint64_big {b.size ()}, b, uint64_big {0}), k, iv), iv};
    }
    
    struct decrypted;
    
    template <size_t block_size, size_t key_size>
    decrypted decrypt (const encrypted<block_size> &e, decryption<block_size, key_size> d, const symmetric_key<key_size> &k);

    struct decrypted : bytes {
        struct fail : std::exception {
            const char* what () const noexcept override {
                return "decryption failure";
            }
        };
        
        decrypted (decrypted &&) = default;
        
    private:
        decrypted (const size_t size) : bytes (size) {}
        decrypted (const decrypted &) = delete;
        
        template <size_t block_size, size_t key_size>
        friend decrypted decrypt (const encrypted<block_size> &e, decryption<block_size, key_size> d, const symmetric_key<key_size> &k);
    };
    
    template <size_t block_size, size_t key_size>
    decrypted decrypt (const encrypted<block_size> &e, decryption<block_size, key_size> d, const symmetric_key<key_size> &k) {
        bytes x = d (e.Data, k, e.IV);
        iterator_reader r (x.begin (), x.end ());
        uint64_big check_start;
        r >> check_start;
        if (check_start != 0) throw decrypted::fail {};
        uint64_big len;
        r >> len;
        if (len > e.Data.size ()) throw decrypted::fail {};
        decrypted data (len);
        r >> data;
        uint64_big check_end;
        r >> check_end;
        if (check_end != 0) throw decrypted::fail {};
        return data;
    }
    
    template <size_t block_size, size_t key_size>
    struct locked : encrypted<block_size> {
        ptr<retriever<key_size>> Retriever;
        crypto::decryption<block_size, key_size> Decrypt;
        decrypted decrypt () const {
            return crypto::decrypt (*this, Decrypt, Retriever->retrieve ());
        };
        
        locked (const bytes &b,
            initialization_vector<block_size> iv,
            crypto::decryption<block_size, key_size> d,
            ptr<retriever<key_size>> r) : encrypted<block_size> {b, iv}, Retriever {r}, Decrypt {d} {}
    };
    
    template <size_t key_size>
    struct trivial_passphrase : retriever<key_size> {
        symmetric_key<key_size> Key;
        
        symmetric_key<key_size> retrieve () override {
            return Key;
        }
    };
    
    // A key retriever that prints a message to the user and then reads a passphrase. 
    struct user_passphrase : retriever<32> {
        std::string UserMessage;
        std::ostream &Cout;
        std::istream &Cin;
        
        user_passphrase (std::string user_message, std::ostream &out, std::istream &in) :
            UserMessage {user_message}, Cout {out}, Cin {in} {}
        
        symmetric_key<32> retrieve () override {
            Cout << UserMessage;
            std::string line;
            std::getline (Cin, line);
            symmetric_key<32> x;
            auto d = SHA2_256 (byte_slice {(byte *) line.data (), line.size ()});
            std::copy (d.begin (), d.end (), x.begin ());
            return x;
        }
    };
    
    // a key retriever that caches the key for a certain amount of time. 
    template <size_t size>
    struct cached_passphrase : retriever<size> {
        symmetric_key<size> retrieve () override;
    };
    
}

#endif

