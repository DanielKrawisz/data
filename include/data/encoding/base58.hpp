// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_BASE58
#define DATA_ENCODING_BASE58

#include <data/types.hpp>
#include <data/slice.hpp>

namespace data::encoding::base58 {
    const std::string format = "base58";
    
    bool valid(const std::string&);
    string write(const bytes&);

    template<unsigned long size>
    string write(const std::array<byte, size>&);
    bytes read(const std::string&);
    
    class string : public std::string {
        bytes Bytes;
        bytes* ToBytes;
        
    public:
        explicit operator bytes() const {
            if (ToBytes == nullptr) throw ;
            return *ToBytes;
        }
        
        bool valid() const {
            return ToBytes != nullptr;
        }
        
        string(std::string);
        
        string(std::string * sourceString):string(*sourceString){};
        
        string(const string& s) : 
            std::string{static_cast<const std::string&>(s)}, 
            Bytes{s.Bytes}, 
            ToBytes{s.ToBytes == nullptr ? nullptr : &Bytes} {}
        
        string(string&& s) : 
            std::string{static_cast<std::string&&>(s)}, 
            Bytes{s.Bytes}, 
            ToBytes{s.ToBytes == nullptr ? nullptr : &Bytes} {}
        
        string& operator=(string&);
    };
}

#endif
