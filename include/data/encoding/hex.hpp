// Copyright (c) 2019 Daniel Krawisz
// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_HEX
#define DATA_ENCODING_HEX

#include <data/types.hpp>
#include <data/encoding/hex.hpp>
#include <data/encoding/invalid.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string.hpp>
#include <ctre.hpp>

namespace data::encoding::hex {
    const std::string format = "hex";
    
    constexpr auto pattern = ctll::fixed_string{"(([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*)"};
    
    inline bool valid(string_view s) {
        return ctre::match<pattern>(s);
    }
    
    std::string write(bytes_view);
    std::string write(uint64);
    std::string write(uint32);
    std::string write(uint16);
    std::string write(byte);
    
    class string {
        string_view String;
        bytes Bytes;
        bytes *ToBytes;
    public:
        
        explicit operator bytes() const {
            if (ToBytes == nullptr) throw invalid{format, String};
            return *ToBytes;
        }
        
        bool valid() const {
            return ToBytes != nullptr;
        }
        
        string(string_view);
    };
}

#endif
