#ifndef DATA_ENCODING_BASE58_HPP
#define DATA_ENCODING_BASE58_HPP

#include <data/types.hpp>
#include <data/slice.hpp>

namespace data {
    
    namespace encoding {
        
        namespace base58 {            
            string write(vector<byte> x);
            
            vector<byte> read(const string& s);
        }
        
    }
    
}

#endif
