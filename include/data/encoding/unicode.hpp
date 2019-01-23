 #ifndef DATA_UNICODE_HPP
#define DATA_UNICODE_HPP

#include <string>
#include <data/types.hpp>

namespace data {
    
    struct unicode : public std::vector<uint32_t> {
        unicode();
        
        unicode(string s): std::vector<uint32_t>(s.size()) {
            for (int i = 0; i < s.size(); i++) this->at(i) = s[i];
        }
        
        unicode(std::vector<uint32_t> v): std::vector<uint32_t>(v) {}
    };
    
}

#endif
