// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/net/HTTP_client.hpp>

namespace data::net::HTTP {

    response client_blocking::operator () (const request &r) {
        auto wait = Rate.getTime ();
        if (wait != 0) {
            asio::io_context io {};
            asio::steady_timer {io, asio::chrono::seconds (wait)}.wait ();
        }
        return HTTP::call (r, SSL.get ());
    }

    void client_async::operator () (handler<const response &> handle, const request &req) {
        Queue = Queue << std::pair<request, handler<const response &>> {req, handle};

        if (!Writing) {
            Writing = true;
            send_next_request ();
        }
    }

    void client_async::send_next_request_now () {
        auto next = data::first (Queue);
        Queue = data::rest (Queue);
        HTTP::call (*IO, [] (const HTTP::error &e) {
            // TODO get a better error function
            std::cout << " HTTP error: " << e.ErrorCode << "; " << e.What << std::endl;
        }, next.second, next.first, SSL.get ());
        if (data::empty (Queue)) Writing = false;
        else send_next_request ();
    }

    void client_async::send_next_request () {

        auto wait = Rate.getTime ();
        if (wait != 0) asio::steady_timer {*IO, asio::chrono::seconds (wait)}.async_wait (
            [self = this->shared_from_this ()] (asio::error_code err) {
                if (err) throw data::exception {} << "something went wrong with the timer " << err;
                self->send_next_request_now ();
            }
        );
        else send_next_request_now ();
    }

}

