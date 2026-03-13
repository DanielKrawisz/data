// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_MERGE_SORT
#define DATA_TOOLS_MERGE_SORT

#include <data/list/linked.hpp>

namespace data::sort {
    
    template <typename X, typename L>
    class merge : public L {
        static L build (const X &x) {
            return size (x) == 0 ? L {} : build (rest (x)) << L {first (x)};
        }
        
        static merge round (merge m) {
            return size (m) < 2 ? m : round (rest (rest (m))) << list::merge (first (m), first (rest (m)));
        }
    
        merge () : L {} {}
    
    public:
        merge (const X &init) : L {build (init)} {}
        X operator * () const {
            size_t s = size (*this);
            return s == 0 ? X {} : s == 1 ? first (*this) : *round (*this);
        }
    };

}

namespace data {
    
    template <typename X> X merge_sort (const X &a) {
        if (data::size (a) < 2) return a;
        return *sort::merge<X, list::linked<X>> {a};
    }
    
}

#endif
