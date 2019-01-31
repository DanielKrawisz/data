#ifndef DATA_FOLD_HPP
#define DATA_FOLD_HPP

#include "list.hpp"

namespace data
{

    template <typename f, typename x, typename l>
    inline x fold(f fun, x init, l ls) {
        static list::is_list<l> is_list{};
        if (list::empty(ls)) return init;
        return fold(fun, fun(init, list::first(ls)), list::rest(ls));
    }
    
    template <typename f, typename x, typename l>
    inline x reduce(f fun, l ls) {
        static list::is_list<l> is_list{};
        if (list::empty(ls)) return x{};
        return fun(list::first(ls), reduce(fun, list::rest(ls)));
    }

}

#endif
