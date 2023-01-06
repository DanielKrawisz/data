// Copyright (c) 2022 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#include <data/io/run.hpp>
#include <data/networking/asio/async_wait_for_message.hpp>

namespace data::io {

    namespace bp = boost::process;

    // run an external command with standard in and standard out connected.
    ptr<process> run (boost::asio::io_context &io, string command,
        output_handler std_out_handler,
        output_handler std_err_handler,
        function<void (error)> err_handler) {

        ptr<bp::async_pipe> in { new bp::async_pipe {io}};
        ptr<bp::async_pipe> out { new bp::async_pipe {io}};
        ptr<bp::async_pipe> err { new bp::async_pipe {io}};

        bp::child child{command, bp::std_out > *out, bp::std_err > *err, bp::std_in < *in};

        ptr<process> p {new process {std::move (child), networking::asio::async_message_queue<pipe, char> {in, err_handler}}};

        networking::asio::async_wait_for_message<pipe, char> (out, std_out_handler (p), err_handler);
        networking::asio::async_wait_for_message<pipe, char> (err, std_err_handler (p), err_handler);

        return p;
    }

}


