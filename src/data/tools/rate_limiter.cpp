// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include "data/tools/rate_limiter.hpp"
#include <chrono>
namespace data {
    namespace tools {

        long rate_limiter::getTime() {
            long now = std::chrono::duration_cast<std::chrono::seconds>(
                    std::chrono::system_clock::now().time_since_epoch()).count();
                    
            long lastSent = m_queue.getValue();
            
            if(lastSent == -1) {
                m_queue.setValue(now);
                m_queue.next();
                return 0;
            }
            
            if(now - lastSent < m_duration) {
                long wait_name = m_duration - (now-lastSent);
                m_queue.setValue(now + wait_name);
                m_queue.next();
                return wait_name;
            }
            
            m_queue.setValue(now);
            m_queue.next();
            
            return 0;
        }
    }
}

