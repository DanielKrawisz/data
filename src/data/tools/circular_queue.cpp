// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/tools/circular_queue.h"
namespace data::tools {


        void circular_queue::setValue(long val) {
            circularQueue[cur] = val;
        }

        long circular_queue::getValue() {
            return circularQueue[cur];
        }

        void circular_queue::next() {
            if(cur == size-1 )
                cur = 0;
            else
                cur = cur+1;
        }
    }
