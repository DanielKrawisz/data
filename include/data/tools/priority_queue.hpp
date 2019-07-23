// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_PRIORITY_QUEUE
#define DATA_TOOLS_PRIORITY_QUEUE

#include <data/types.hpp>
    
namespace data {

    template <typename e, typename n>
    struct priority_queue {
        
        struct priority {
            e Entry;
            n Priority;
        };
        
        priority_queue();
        
        bool empty() const;
            
        priority first() const;

        priority_queue rest() const;
        
        priority_queue insert(priority p) const;
        
        priority_queue insert(e elem, n prior) const {
            return insert({elem, prior});
        }

    };
    

}

#endif
