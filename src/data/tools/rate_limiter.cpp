// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/tools/rate_limiter.hpp"

namespace data {

    millisecond rate_limiter::get_time () {
        if (m_queue.size == 0) return millisecond {0};

        std::scoped_lock lock (*mutex);

        using namespace std::chrono;
        if (m_duration == millisecond {0}) return millisecond {0};
            
        millisecond now = duration_cast<millisecond> (system_clock::now ().time_since_epoch ());
                    
        millisecond lastSent = m_queue.get ();
            
        if (now - lastSent < m_duration) {
            millisecond wait_name = m_duration - (now - lastSent);
            m_queue.set (now + wait_name);
            return wait_name;
        }

        m_queue.set (now);

        return millisecond {0};
    }
}

