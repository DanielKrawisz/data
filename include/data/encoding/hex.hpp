// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODINGS_HEX
#define DATA_ENCODINGS_HEX

#include <data/types.hpp>
#include <data/slice.hpp>

namespace data {
    
    namespace encoding {
        
        namespace hex {
            bool valid(const std::string&);
            string write(const bytes&);
            
            class invalid : std::exception {
                std::string* String;
                
            public:
                const char* what() const noexcept final override {
                    if (String == nullptr) return "Invalid hex string";
                    return (std::string{"Invalid hex string: "} + *String).c_str();
                }
            };
            
            class string {
                std::string* ToString;
                bytes* ToBytes;
            public:
                operator std::string() {
                    if (ToString == nullptr) throw ;
                    return *ToString;
                }
                
                operator bytes() {
                    if (ToBytes == nullptr) throw ;
                    return *ToBytes;
                }
                
                bool valid() const {
                    return ToString != nullptr && ToBytes != nullptr;
                }
                
            private:                                                           
                std::string String;
                bytes Bytes;
            public:
                string(std::string);
                string(slice<byte>);
                
                string(std::string*);
                string(slice<byte>*);
                
                string(string&);
                string(string&&);
                
                string& operator=(string&);
            };
        }
        
    }
    
}

#endif
