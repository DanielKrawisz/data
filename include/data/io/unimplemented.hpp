// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

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
