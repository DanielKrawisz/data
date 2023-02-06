// Copyright (c) 2022 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#include <data/io/run.hpp>
#include <data/net/async/wait_for_message.hpp>

namespace data::io {

    namespace bp = boost::process;

    // run an external command with standard in and standard out connected.
    void run (boost::asio::io_context &io, string command, error_handler err_handler, interaction i) {

        ptr<bp::async_pipe> in { new bp::async_pipe {io}};
        ptr<bp::async_pipe> out { new bp::async_pipe {io}};
        ptr<bp::async_pipe> err { new bp::async_pipe {io}};

        bp::child child {command, bp::std_out > *out, bp::std_err > *err, bp::std_in < *in};

        ptr<process> p {new process {std::move (child), net::async::message_queue<pipe, char> {in, err_handler}}};

        auto handle = i (p);

        net::async::wait_for_message<pipe, char> (out, [handle] (string_view x) -> void {
            handle->read_out (x);
        }, err_handler);

        net::async::wait_for_message<pipe, char> (err, [handle] (string_view x) -> void {
            handle->read_err (x);
        }, err_handler);

    }

}


