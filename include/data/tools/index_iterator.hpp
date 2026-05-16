// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_INDEX_ITERATOR
#define DATA_TOOLS_INDEX_ITERATOR

#include <data/types.hpp>

namespace data {
        
    template <typename slice, typename X>
    struct index_iterator {
        slice Slice;
        uint32 Index;
        
        index_iterator &operator = (index_iterator i) {
            Slice = i.Slice;
            Index = i.Index;
            return *this;
        }
        
        index_iterator operator ++ (int) { // Postfix
            index_iterator n = *this;
            operator = (index_iterator {Slice, Index + 1});
            return n;
        }
        
        index_iterator &operator ++ () { // Prefix
            return operator = (index_iterator {Slice, Index + 1});
        }
        
        X operator * () {
            return Slice[Index];
        }
        
        const X &operator * () const {
            return Slice[Index];
        }

        const X *operator -> () const {
            return &Slice[Index];
        }
        
        bool operator == (const index_iterator i) const {
            return Slice == i.Slice && Index == i.Index;
        }
        
    };

}

#endif
