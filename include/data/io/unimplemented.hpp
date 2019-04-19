#ifndef DATA_PROGRAM_UNIMPLEMENTED
#define DATA_PROGRAM_UNIMPLEMENTED

#include <exception>

namespace data {
     
    namespace method { 
        
        class unimplemented : std::exception {
            
        public:
            const char* what() const noexcept final override {
                return "Unimplemented method";
            }
        };
    
    }
    
}

#endif
