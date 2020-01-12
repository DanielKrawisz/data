// Copyright (c) 2019 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_PROGRAM_UNIMPLEMENTED
#define DATA_PROGRAM_UNIMPLEMENTED

#include <exception>

namespace data {
     
    namespace method { 
        
        struct unimplemented : std::logic_error {
            unimplemented(std::string func) : std::logic_error(func + " not yet implemented") { };
        };
    
    }
    
}

#endif
