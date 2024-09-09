// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_RATE_LIMITER_H
#define DATA_RATE_LIMITER_H

#include "circular_queue.h"

namespace data::tools {
    // Do you have an operation that cannot be repeated too quicky? 
    // for example, a free API that is rate-limited? Use this to 
    // limit your requests to the required time interval. 
    struct rate_limiter {
        // the rate limiter will not allow more actions than 'hits' per 'duration'. 
        rate_limiter(int hits, int duration) : m_queue(hits, -1), m_duration(duration) {};
        
        // how much time we need to wait until an action can be taken?
        // this function assumes an action will be taken after this 
        // time has passed and adds an action to the queue internally.
        long getTime();
        
        // use this to make an unlimited rate limiter that does nothing. 
        rate_limiter() : m_queue(0), m_duration(0) {};
    
    private:
        
        circular_queue m_queue;
        int m_duration;
    };
}

#endif //DATA_RATE_LIMITER_H
