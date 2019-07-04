// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODINGS_HEX
#define DATA_ENCODINGS_HEX

#include <data/types.hpp>
#include <data/slice.hpp>
#include <boost/algorithm/hex.hpp>
namespace data {
    
    namespace encoding {
        
        namespace hex {
            bool valid(const std::string&);
            string write(const bytes&);
            bytes read(const std::string&);
            
            class invalid : std::exception {
                std::string* String;
                const char* errorString;
            public:
                invalid(std::string* str) {
                    String=str;
                    if (String == nullptr) errorString="Invalid hex string";
                    std::string output=std::string{"Invalid hex string: "} + *String;
                    errorString==output.c_str();
                }
                const char* what() const noexcept final override {

                    return errorString;
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
                
                operator bytes() const {
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
