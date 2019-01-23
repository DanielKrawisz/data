#ifndef DATA_ENCODING_UTF8_HPP
#define DATA_ENCODING_UTF8_HPP

#include <data/types.hpp>
#include <data/slice.hpp>
#include <data/encoding/unicode.hpp>

namespace data {
    
    namespace encoding {
        
        namespace utf8 {
            using string = vector<byte>;
            
            string write(unicode);
            
            unicode read(string str);
            
        }
        
    }
    
}

#endif
