#ifndef DATA_ENCODING_ASCII_HPP
#define DATA_ENCODING_ASCII_HPP

#include <string>
#include <abstractions/bytestring.hpp>

namespace abstractions {
    
    namespace encoding {
        
        namespace ascii {
            const byte max = 0x7f;
            
            using character = byte;
            
            inline bool valid(character b) {
                return b <= max;
            }
            
            inline bool valid(char c) {
                return c <= max;
            }
            
            struct string : public vector<character> {
                bool valid() const {
                    for (character x : *this) if (!ascii::valid(x)) return false;
                    
                    return true;
                }
                
                string() {}
                string(N n) : vector<character>(n) {}
                string(std::string s) : vector<character>(s.size()) {
                    for (int i = 0; i < s.size(); i++) this->at(i) = s[i];
                }
                string(const string& s) : vector<character>(static_cast<vector<character>>(s)) {}
                
                bool operator==(const string& s) {
                    return static_cast<vector<character>>(*this) == static_cast<const vector<character>&>(s);
                }
            };
            
            string read(const bytestring x) {
                string str(x.size());
                
                for (bytestring::iterator i = x.begin(); i != x.end(); i++) {
                    byte b = *i;
                    if (!valid(b)) return {}; 
                }
                
                return str;
            }
            
            vector<byte> write(string str) {
                std::vector<byte> x(str.size());
                
                auto str_itr = str.begin();
                for (auto i = x.begin(); i != x.end(); i++) {
                    if (str_itr == str.end()) {
                        *i = 0;
                        continue;
                    }
                    
                    char c = *str_itr;
                    if (!valid(c)) return {};
                    *i = c;
                }
                
                return x;
            }

        }

    }

}

#endif 
