// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CIRCULAR_QUEUE_H
#define DATA_CIRCULAR_QUEUE_H

#include <ostream>
#include <cstddef>
#include <iterator>
#include <vector>

namespace data::tools {
    struct circular_queue {
        // Initialize front and rear
        int cur;
        
        // Circular Queue
        int size;
        std::vector<long> circularQueue;

        explicit circular_queue (int sz, long init_value = 0) {
            cur = 0;
            size = sz;
            circularQueue.resize(sz);
            std::fill(circularQueue.begin(), circularQueue.end(), init_value);
        }
        
        void setValue(long val);
        void next();
        long getValue();
    };
}

#endif //DATA_CIRCULAR_QUEUE_H
