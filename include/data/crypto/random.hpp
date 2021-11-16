// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_RANDOM
#define DATA_CRYPTO_RANDOM

#include <data/iterable.hpp>
#include <data/cross.hpp>

namespace data::crypto {
    
    struct random {
        random& operator>>(byte& x) {
            get(&x, 1);
            return *this;
        }
        
        random& operator>>(uint16_t& x) {
            get(reinterpret_cast<byte*>(&x), 2);
            return *this;
        }
        
        random& operator>>(uint32_t& x) {
            get(reinterpret_cast<byte*>(&x), 4);
            return *this;
        }
        
        random& operator>>(uint64_t& x) {
            get(reinterpret_cast<byte*>(&x), 8);
            return *this;
        }
        
        template <typename X>
        random& operator>>(X& x) {
            get(x.data(), x.size());
            return *this;
        }
        
        // std::uniform_random_bit_generator concept. 
        using result_type = byte;
        
        static constexpr byte min() {
            return 0;
        }
        
        static constexpr byte max() {
            return 255;
        }
        
        byte operator()() {
            byte b;
            *this >> b;
            return b;
        }
        
    protected: 
        virtual void get(byte*, size_t) = 0;
    };
    
    struct entropy {
        struct fail : std::exception {
            const char* what() const noexcept override {
                return "failed to get entropy";
            }
        };
        
        virtual bytes get(size_t) = 0;
    };
    
    struct entropy_sum : entropy {
        ptr<entropy> Left;
        ptr<entropy> Right;
        
        bytes get(size_t x) override {
            bytes left = Left->get(x);
            bytes right = Right->get(x);
            return stream::write_bytes(left.size() + right.size(), left, right);
        }
    };
    
    struct user_entropy : entropy {
        std::string UserMessageAsk;
        std::string UserMessageConfirm;
        std::ostream& Cout;
        std::istream& Cin;
        
        user_entropy(std::string ask, std::string confirm, std::ostream& out, std::istream& in) : 
            UserMessageAsk{ask}, UserMessageConfirm{confirm}, Cout{out}, Cin{in} {}
        
        bytes get(size_t x) override {
            Cout << UserMessageAsk << std::endl;
            bytes b(x * 4);
            for (uint32 i = 0; i < x * 4; i++) Cin >> b[i];
            Cout << UserMessageConfirm << std::endl;
            Cin.seekg(0, std::ios::end);
            Cin.clear();
            return b;
        }
    };
    
    struct fixed_entropy : entropy {
        bytes Entropy;
        int Position;
        
        fixed_entropy(bytes_view b) : Entropy{b}, Position{0} {}
        
        bytes get(size_t x) override {
            if (x > Entropy.size() - Position) throw std::logic_error{"ran out of entropy"};
            
            bytes b(x * 4);
            for (int i = 0; i < x; i++) {
                b[i] = Entropy[i];
                Position++;
            }
            
            return b;
        }
    };

}

#endif
