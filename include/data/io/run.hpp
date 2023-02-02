// Copyright (c) 2022 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_IO_RUN
#define DATA_IO_RUN

#include <data/net/asio/async_message_queue.hpp>
#include <boost/process.hpp>

namespace data::io {

    struct process;

    struct read_handlers {
        handler<string_view> Out;
        handler<string_view> Err;
    };

    using interaction = function<read_handlers (ptr<process>)>;

    using error_code = net::asio::error_code;
    using error_handler = net::asio::error_handler;

    void run (boost::asio::io_context &, string command, error_handler, interaction);

    using pipe = boost::process::async_pipe;

    struct process {

        boost::process::child Child;

        net::asio::async_message_queue<pipe, char> Queue;

        void send (const string &x) {
            Queue.write(x);
        }

        int close () {
            Child.terminate ();
            return Child.exit_code ();
        }

        bool closed () {
            return !Child.running ();
        }

        ~process () {
            close ();
        }

    private:
        process (boost::process::child &&child, net::asio::async_message_queue<pipe, char> q) : Child {std::move (child)}, Queue {q} {}

        friend void run (boost::asio::io_context &, string command, error_handler err_handler, interaction i);
    };
    
}

#endif

