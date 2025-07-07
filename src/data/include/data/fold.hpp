// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_FOLD
#define DATA_FOLD

#include <data/sequence.hpp>

namespace data {

    template <typename x, typename f, Sequence l>
    x inline fold (f fun, x init, l ls) {
        if (data::empty (ls)) return init;
        return fold (fun, fun (init, data::first (ls)), data::rest (ls));
    }
    
    template <typename f, typename x, typename num>
    x inline nest (f fun, x init, num rounds) {
        if (rounds == 0) return init;
        return nest (fun, fun (init), rounds - 1);
    }
    
    template <typename x, Sequence l>
    x inline reduce (function<x (const x&, const x&)> fun, l ls) {
        if (data::empty (ls)) return x {};
        return fun (data::first (ls), reduce<x> (fun, data::rest (ls)));
    }

}

#endif
