#ifndef DATA_META_EQUAL_HPP
#define DATA_META_EQUAL_HPP

namespace data {
    
    namespace meta {
        
        // type that can only be constructed if two types are equal.
        template <typename x, typename y> struct Equal;
        
        template <typename x> struct Equal<x, x>{};
    }

}

#endif

        
