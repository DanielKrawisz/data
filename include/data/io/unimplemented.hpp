#ifndef DATA_PROGRAM_UNIMPLEMENTED
#define DATA_PROGRAM_UNIMPLEMENTED

#include <exception>

namespace data {
     
    namespace method { 
        
        class unimplemented : std::logic_error {
            
        public:
            unimplemented() : std::logic_error("Function not yet implemented") { };
        };
    
    }
    
}

#endif
