// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CIRCULAR_QUEUE_H
#define DATA_CIRCULAR_QUEUE_H

#include <ostream>
#include <cstddef>
#include <iterator>

namespace data::tools {
        class circular_queue {
        public:
            // Initialize front and rear
           int cur;

            // Circular Queue
            int size;
            long *circularQueue;

            explicit circular_queue (int sz, long init_value=0) {
                cur = 0;
                size = sz;
                circularQueue = new long[sz];
                std::fill(circularQueue,circularQueue+sz,init_value);
            }
            void setValue(long val);
            void next();
            long getValue();
        };
    }
#endif //DATA_CIRCULAR_QUEUE_H
