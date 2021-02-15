// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/tools/circular_queue.h"
namespace data::tools {


        void CircularQueue::setValue(long val) {
            circular_queue[cur] = val;
        }

        long CircularQueue::getValue() {
            return circular_queue[cur];
        }

        void CircularQueue::next() {
            if(cur == size-1 )
                cur = 0;
            else
                cur = cur+1;
        }
    }
