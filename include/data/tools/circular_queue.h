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
    template <std::totally_ordered T>
    struct circular_queue {
        // Initialize front and rear
        int cur;
        
        // Circular Queue
        size_t size;
        std::vector<T> circularQueue;

        explicit circular_queue (size_t sz, T init_value = T {0}) {
            cur = 0;
            size = sz;
            circularQueue.resize (sz);
            std::fill (circularQueue.begin (), circularQueue.end (), init_value);
        }
        
        void set (const T &val);
        const T &get ();
    };

    template <std::totally_ordered T>
    void circular_queue<T>::set (const T &val) {
        circularQueue[cur] = val;
        cur = (cur + 1) % size;
    }

    template <std::totally_ordered T>
    const T &circular_queue<T>::get () {
        T &x = circularQueue[cur];
        return x;
    }

}

#endif //DATA_CIRCULAR_QUEUE_H
