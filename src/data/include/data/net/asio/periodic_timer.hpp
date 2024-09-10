// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_ASIO_PERIODIC_TIMER
#define DATA_NET_ASIO_PERIODIC_TIMER

#include <boost/date_time/posix_time/conversion.hpp>
#include "session.hpp"
#include <chrono>

namespace data::net::asio {
    // this timer will call a function until a stop condition is met.
    class periodic_timer : public std::enable_shared_from_this<periodic_timer> {
        deadline_timer Timer;
        boost::posix_time::time_duration Interval;
        function<void (const error_code &e)> Function;
        bool Stopped;

    public:
        // the timer starts running once the user calls io.run ().
        periodic_timer (io_context &io, boost::posix_time::time_duration interval, function<void ()> tick):
            Timer {io, boost::posix_time::from_time_t (std::chrono::system_clock::to_time_t (std::chrono::system_clock::now ())) + interval},
            Interval {interval},
            Function {[tick, self = this->shared_from_this ()] (const boost::system::error_code &e) {
                if (bool (e) || self->Stopped) return;
                tick ();
                self->Timer.expires_at (self->Timer.expires_at () + self->Interval);
                // Posts the timer event
                self->Timer.async_wait (self->Function);
            }}, Stopped {false} {
                Timer.async_wait (Function);
            }

        void stop () {
            Stopped = true;
        }
    };
}

#endif


