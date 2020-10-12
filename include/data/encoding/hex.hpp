// Copyright (c) 2019-2020 Daniel Krawisz
// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_HEX
#define DATA_ENCODING_HEX

#include <data/encoding/invalid.hpp>
#include <data/iterable.hpp>
#include <ctre.hpp>

namespace data::encoding::hex {
    const std::string Format{"hex"};
    
    enum letter_case {
        upper, 
        lower
    };
    
    inline std::string characters_lower() {
        return "0123456789abcdef";
    }
    
    inline std::string characters_upper() {
        return "0123456789ABCDEF";
    }
    
    constexpr auto pattern = ctll::fixed_string{"^(([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*)$"};
    
    inline bool valid(string_view s) {
        return ctre::match<pattern>(s);
    }
    
    struct string : std::string {
        using std::string::string;
        
        bool valid() const {
            return hex::valid(*this);
        }
        
        explicit operator bytes() const;
    };
    
    string write(bytes_view, endian::order, letter_case = upper);
    
    string write(bytes_view, letter_case = upper);
    string write(uint64, letter_case = upper);
    string write(uint32, letter_case = upper);
    string write(uint16, letter_case = upper);
    string write(byte, letter_case = upper);
    
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
