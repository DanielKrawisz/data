// Copyright (c) 2019 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_PLUS
#define DATA_PLUS

#include "types.hpp"

namespace data {
    
    template <typename X>
    struct plus {
        X operator()(X a, X b) {
            return a + b;
        }
    };

}

#endif
