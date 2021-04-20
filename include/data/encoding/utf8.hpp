#ifndef DATA_ENCODING_UTF8_HPP
#define DATA_ENCODING_UTF8_HPP

#include <data/types.hpp>
#include <data/slice.hpp>
#include <data/encoding/unicode.hpp>

namespace data::encoding::utf8 {
    
    bytes encode(const string&);
    bytes encode(const std::wstring&);
    
    ptr<std::wstring> decode(const bytes&);
    
}

#endif
