// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/tools/rate_limiter.hpp"

namespace data::tools {

    milliseconds rate_limiter::get_time () {
        using namespace std::chrono;
        if (m_duration == milliseconds {0}) return milliseconds {0};
            
        milliseconds now = duration_cast<milliseconds> (system_clock::now ().time_since_epoch ());
                    
        milliseconds lastSent = m_queue.get ();
            
        if (now - lastSent < m_duration) {
            milliseconds wait_name = m_duration - (now - lastSent);
            m_queue.set (now + wait_name);
            return wait_name;
        }
            
        m_queue.set (now);

        return milliseconds {0};
    }
}

