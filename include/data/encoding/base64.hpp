// Copyright (c) 2019-2020 Daniel Krawisz
// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_BASE64
#define DATA_ENCODING_BASE64

#include <boost/regex.hpp>
#include <data/encoding/invalid.hpp>
#include <data/iterable.hpp>
#include <data/math/division.hpp>

namespace data::encoding::base64 {      
    const std::string Format{"base64"};
    
    inline std::string characters() {
        static std::string Characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        return Characters;
    }
    
    constexpr static char Pad = '=';
    static const boost::regex pattern{"^(?:[A-Za-z0-9+/]{4})*(?:[A-Za-z0-9+/]{2}==|[A-Za-z0-9+/]{3}=|[A-Za-z0-9+/]{4})$"};

    inline bool valid(string_view s) {
        return boost::regex_match(s.data(),pattern);
    }
    
    std::string write(bytes_view, endian::order);
    
    std::string write(bytes_view);
    std::string write(uint64);
    std::string write(uint32);
    std::string write(uint16);
    std::string write(byte);
    
    class view : public string_view {
        bytes Bytes;
        bytes *ToBytes;
        
    public:
        explicit operator bytes_view() const {
            if (ToBytes == nullptr) throw invalid{Format, *this};
            return bytes_view(*ToBytes);
        }
        
        bool valid() const noexcept {
            return ToBytes != nullptr;
        }
        
        view(string_view);
    };
}

#endif

