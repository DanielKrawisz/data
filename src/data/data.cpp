#include <data/data.hpp>

namespace data::exported {
    
    // get all values from a map with the given keys. 
    template <typename key, typename value, typename map>
    list<value> get_all(map m, list<key> k) {
        struct inner {
            map M;
                    
            inner(map m) : M{m} {}
                    
            list<value> operator()(key k, list<value> l) {
                value v = M[k];
                if (v == value{}) return l;
                return l + v;
            }
        };
                
        return reduce(inner{m}, k);
    }

}
