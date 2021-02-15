// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CIRCULAR_QUEUE_H
#define DATA_CIRCULAR_QUEUE_H

#include <ostream>
#include <cstddef>
#include <iterator>

namespace data::tools {
        class CircularQueue {
        public:
            // Initialize front and rear
           int cur;

            // Circular Queue
            int size;
            long *circular_queue;

            explicit CircularQueue (int sz,long init_value=0) {
                cur = 0;
                size = sz;
                circular_queue = new long[sz];
                std::fill(circular_queue,circular_queue+sz,init_value);
            }
            void setValue(long val);
            void next();
            long getValue();
        };
    }
#endif //DATA_CIRCULAR_QUEUE_H
