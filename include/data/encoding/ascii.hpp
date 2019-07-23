#ifndef DATA_ENCODING_ASCII_HPP
#define DATA_ENCODING_ASCII_HPP

#include <string>
#include <data/types.hpp>
#include <boost/convert.hpp>

namespace data::encoding::ascii {
    const byte max = 0x7f;

    using character = byte;

    inline bool valid(character b) {
        return b <= max && b>=0;
    }

    inline bool valid(char c) {
        return c <= max && c>=0;
    }


    class string {
        std::string const* innerString= nullptr;

    public:
        string(std::string const* input);
        inline bool isValid() {
            return innerString!= nullptr;
        }

        explicit operator std::string() {
            return *innerString ;
        }

        explicit operator bytes();

    };
}

#endif 
