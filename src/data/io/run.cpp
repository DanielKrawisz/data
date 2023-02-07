// Copyright (c) 2022 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#include <data/io/run.hpp>
#include <data/net/async/wait_for_message.hpp>
#include <data/net/asio/socket.hpp>

namespace data::io {

    namespace bp = boost::process;

    using pipe = bp::async_pipe;

    using socket = net::asio::socket<bp::async_pipe>;

    // run an external command with standard in and standard out connected.
    void run (boost::asio::io_context &io, string command, error_handler err_handler, interaction i, close_handler close) {

        ptr<bp::async_pipe> in {new pipe {io}};
        ptr<bp::async_pipe> out {new pipe {io}};
        ptr<bp::async_pipe> err {new pipe {io}};

        ptr<bp::child> child { new bp::child (command, bp::std_out > *out, bp::std_err > *err, bp::std_in < *in) };

        ptr<socket> In { new socket {in, close}};
        ptr<socket> Out { new socket {out, close}};
        ptr<socket> Err { new socket {err, close}};

        ptr<process> p {new process {std::move (child), In, err_handler}};

        auto handle = i (p);

        net::async::wait_for_message<string_view, error_code> (Out, [handle] (string_view x) -> void {
            handle->read_out (x);
        }, err_handler);

        net::async::wait_for_message<string_view, error_code> (Err, [handle] (string_view x) -> void {
            handle->read_err (x);
        }, err_handler);

    }

}


