// Copyright (c) 2022 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_IO_RUN
#define DATA_IO_RUN

#include <data/net/asio/session.hpp>
#include <data/net/async/message_queue.hpp>
#include <boost/process.hpp>

namespace data::io {

    using error_code = net::asio::error_code;
    using error_handler = net::asio::error_handler;

    using close_handler = handler<int>;

    class process;

    struct handlers {
        virtual ~handlers () {}
        virtual void read_out (string_view) = 0;
        virtual void read_err (string_view) = 0;
    };

    using interaction = function<ptr<handlers> (ptr<process>)>;

    // run an external commannd.
    void run (boost::asio::io_context &, string command, error_handler, interaction, close_handler);

    class process : public net::async::message_queue<const string &, error_code> {
        ptr<boost::process::child> Child;
        process (ptr<boost::process::child> cx, ptr<net::async::write_stream<const string &, error_code>> stream, error_handler errors):
            net::async::message_queue<const string &, error_code> {stream, errors}, Child {cx} {}

        friend void run (boost::asio::io_context &, string command, error_handler, interaction, close_handler);
    };

}

#endif

