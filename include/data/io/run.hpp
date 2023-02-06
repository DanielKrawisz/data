// Copyright (c) 2022 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_IO_RUN
#define DATA_IO_RUN

#include <data/net/asio/session.hpp>
#include <data/net/async/message_queue.hpp>
#include <boost/process.hpp>

namespace data::io {

    struct in {
        virtual ~in () {}
        virtual void read_out (string_view) = 0;
        virtual void read_err (string_view) = 0;
        virtual void close (int return_code) = 0;
    };

    struct process;

    using interaction = net::interaction<in, process>;

    using error_code = net::asio::error_code;
    using error_handler = net::asio::error_handler;

    void run (boost::asio::io_context &, string command, error_handler, interaction);

    using pipe = boost::process::async_pipe;

    struct process : net::out<const string &> {

        boost::process::child Child;

        net::asio::async_message_queue<pipe, char> Queue;

        void send (const string &x) final override {
            Queue.write(x);
        }

        void close () final override {
            method::unimplemented {"process::close"};
        }

        bool closed () final override {
            return !Child.running ();
        }

        ~process () {
            close ();
        }

    private:
        process (boost::process::child &&child, net::async::message_queue<const string &, char> q) : Child {std::move (child)}, Queue {q} {}

        friend void run (boost::asio::io_context &, string command, error_handler err_handler, interaction i);
    };
    
}

#endif

