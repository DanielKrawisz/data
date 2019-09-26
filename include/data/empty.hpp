// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_EMPTY
#define DATA_EMPTY

#include "types.hpp"

namespace data {

    template <typename X>
    inline bool empty(X x) {
        return x.empty();
    }

}

#endif
