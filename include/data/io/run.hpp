// Copyright (c) 2022 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_IO_RUN
#define DATA_IO_RUN

#include <data/networking/asio/async_message_queue.hpp>
#include <boost/process.hpp>

namespace data::io {

    struct process;

    using output_handler = networking::receive_handler<process, string_view>;

    using error = networking::asio::io_error;

    ptr<process> run (boost::asio::io_context &, string command,
        output_handler std_out_handler,
        output_handler std_err_handler,
        function<void (error)> err_handler);

    using pipe = boost::process::async_pipe;

    struct process {

        boost::process::child Child;

        networking::asio::async_message_queue<pipe, char> Queue;

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
        process (boost::process::child &&child, networking::asio::async_message_queue<pipe, char> q) : Child{std::move (child)}, Queue {q} {}

        friend ptr<process> run (boost::asio::io_context &, string command,
            output_handler std_out_handler,
            output_handler std_err_handler,
            function<void (error)> err_handler);
    };
    
}

#endif

