// Copyright (c) 2019-2020 Daniel Krawisz
// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_BASE64
#define DATA_ENCODING_BASE64

#include <ctre.hpp>

#include <data/encoding/invalid.hpp>
#include <data/cross.hpp>
#include <data/math/division.hpp>

namespace data::encoding::base64 {      
    const std::string Format{"base64"};
    
    inline std::string characters() {
        static std::string Characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        return Characters;
    }
    
    constexpr static char Pad = '=';
    
    static constexpr auto pattern = ctll::fixed_string{"(?:[A-Za-z0-9+/]{4})*(?:[A-Za-z0-9+/]{2}==|[A-Za-z0-9+/]{3}=|[A-Za-z0-9+/]{4})"};

    inline bool valid(string_view s) {
        return ctre::match<pattern>(s);
    }
    
    ptr<bytes> read(string_view);
    
    struct string : std::string {
        using std::string::string;
        string(const std::string& x) : std::string{x} {}
        
        bool valid() const noexcept {
            return base64::valid(*this);
        }
        
        explicit operator bytes() const {
            ptr<bytes> b = read(*this);
            if (b == nullptr) throw invalid{Format, *this};
            return *b;
        }
    };
    
    string write(bytes_view, endian::order);
    
    string write(bytes_view);
    string write(uint64);
    string write(uint32);
    string write(uint16);
    string write(byte);
}

#endif

