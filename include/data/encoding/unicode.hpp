#ifndef DATA_ENCODING_UNICODE
#define DATA_ENCODING_UNICODE

#include <data/string.hpp>

namespace data::encoding::UTF8 {
    struct string;
}

namespace data::encoding::unicode {
    struct string;
}

namespace data {
    using UTF8 = encoding::UTF8::string;
    using unicode = encoding::unicode::string;
}

namespace data::encoding::UTF8 {

    struct string;

    string encode (const data::unicode &);

    ptr<data::unicode> decode (const string &);

    struct string : data::string {
        using data::string::string;
        string (const data::unicode &u) : data::string {UTF8::encode (u)} {}
    };

}

namespace data::encoding::unicode {

    struct string : std::u32string {
        using std::u32string::u32string;
        string (const std::u32string &x) : std::u32string {x} {}
        string (std::u32string &&x) : std::u32string {x} {}
        string (const data::UTF8 &u) : std::u32string {*UTF8::decode (u)} {}
    };

    std::ostream inline &operator << (std::ostream &o, const string &x) {
        return o << UTF8::encode (x) << std::endl;
    }

}


#endif
