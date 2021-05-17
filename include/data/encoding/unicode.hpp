#ifndef DATA_UNICODE
#define DATA_UNICODE

#include <data/cross.hpp>

namespace data::encoding::unicode {
    
    bytes utf8_encode(const string&);
    bytes utf8_encode(const std::u32string&);
    
    ptr<std::u32string> utf8_decode(const bytes&);
    
}

#endif
