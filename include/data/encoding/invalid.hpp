// Copyright (c) 2019 Daniel Krawisz
// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_INVALID
#define DATA_ENCODING_INVALID

#include <exception>
#include <string>

namespace data::encoding {
    class invalid : std::exception {
        string_view String;
        std::string Format;
    public:
        invalid(std::string format, string_view str) : String{str}, Format{format} {}
        const char* what() const noexcept final override {
            return (std::string{"Invalid "} + Format + " string: " + std::string{String}).c_str();
        }
    };
}

#endif
