#ifndef DATA_FOLD_HPP
#define DATA_FOLD_HPP

#include "data.hpp"

namespace data {

    template <typename f, typename x, typename l>
    inline x fold(f fun, x init, l ls) {
        if (empty(ls)) return init;
        return fold(fun, fun(init, first(ls)), rest(ls));
    }
    
    template <typename f, typename x, typename l>
    inline x reduce(f fun, l ls) {
        if (empty(ls)) return x{};
        return fun(first(ls), reduce(fun, rest(ls)));
    }

}

#endif
