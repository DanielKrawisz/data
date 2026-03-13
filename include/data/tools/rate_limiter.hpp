// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_RATE_LIMITER_H
#define DATA_RATE_LIMITER_H

#include <data/types.hpp>
#include <data/async.hpp>
#include "circular_queue.h"

namespace data::tools {
    // Do you have an operation that cannot be repeated too quicky? 
    // for example, a free API that is rate-limited? Use this to 
    // limit your requests to the required time interval. 
    struct rate_limiter {
        // the rate limiter will not allow more actions than 'hits' per 'duration'. 

        rate_limiter (size_t hits, milliseconds duration) :
            m_queue (hits), m_duration (duration), mutex {std::make_shared<std::mutex> ()} {};
        
        // how much time we need to wait until an action can be taken?
        milliseconds get_time ();
        
        // use this to make an unlimited rate limiter that does nothing. 
        rate_limiter () : m_queue (0), m_duration (0), mutex {} {};
    
    private:
        circular_queue<milliseconds> m_queue;
        milliseconds m_duration;
        ptr<std::mutex> mutex;
    };
}

#endif //DATA_RATE_LIMITER_H
