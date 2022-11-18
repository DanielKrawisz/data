// Copyright (c) 2019-2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_IO_UNIMPLEMENTED
#define DATA_IO_UNIMPLEMENTED

#include <data/io/exception.hpp>

namespace data::method { 
    
    struct unimplemented : data::exception {
        unimplemented(std::string func) : data::exception{} {
            *this << "method " << func << " not yet implemented";
        };
    };
    
}

#endif
