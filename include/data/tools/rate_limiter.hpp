// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_RATE_LIMITER_H
#define DATA_RATE_LIMITER_H

#include "circular_queue.h"

namespace data {
    namespace tools {
        struct rate_limiter {
            rate_limiter(int hits, int duration) : m_queue(hits, -1), m_duration(duration) {};
            long getTime();
            
        private:
            circular_queue m_queue;
            int m_duration;
        };
    }
}

#endif //DATA_RATE_LIMITER_H
