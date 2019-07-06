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
                char* errorString;
            public:
                invalid(std::string* str);
                ~invalid();
                const char* what() const noexcept final override {

                    return errorString;
                }
            };
            
            class string : std::string{

                bytes* ToBytes;
            public:
                operator std::string() {
                    if (this == nullptr) throw ;
                    return *this;
                }
                
                operator bytes() const {
                    if (ToBytes == nullptr) throw ;
                    return *ToBytes;
                }
                
                bool valid() const {
                    return ToBytes != nullptr;
                }
                
            private:                                                           
                std::string String;
                bytes Bytes;
            public:
                string(std::string);

                string(std::string * sourceString):string(*sourceString){};
                
                string(string& sourceString):string((std::string*)&sourceString){};
                string(string&& sourceString):std::string(static_cast<std::string&&>(sourceString)){};
                
                string& operator=(string&);
            };
        }
        
    }
    
}

#endif
