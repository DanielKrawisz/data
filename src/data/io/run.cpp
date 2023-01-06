// Copyright (c) 2022 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#include <boost/process.hpp>
#include <data/io/run.hpp>
#include <data/networking/asio/async_message_queue.hpp>
#include <data/networking/asio/async_wait_for_message.hpp>

namespace data::io {
    namespace bp = boost::process;

    struct program_session final : session, std::enable_shared_from_this<program_session> {

        bp::async_pipe In;
        bp::async_pipe Out;
        bp::async_pipe Err;

        bp::child Child;

        ~program_session () {
            close ();
        }

        std::function<void (string_view)> Receive;
        std::function<void (string_view)> Error;

        void receive (string_view x) final override {
            if (!Receive (x)) Child.terminate ();
        }

        void error (string_view x) final override {
            if (!Error (x)) Child.terminate ();
        }

        program_session (boost::asio::io_context &i, string command, std::function<void (string_view)> out, std::function<void (string_view)> err):
            In {i}, Out {i}, Err {i}, Child{command, bp::std_out > Out, bp::std_err > Err, bp::std_in < In} {

            io::async_read(Out, boost::asio::buffer(buf), [](const boost::system::error_code &ec, std::size_t size) {
                throw 0;
            });

            io::async_read(Err, boost::asio::buffer(buf), [](const boost::system::error_code &ec, std::size_t size) {
                throw 0;
            });
        }

        int close () final override {
            Child.terminate ();
            return Child.exit_code ();
        }

        bool closed () final override {
            return !chlid.running ();
        }

        void send (string_view x) final override {
            x >> In;
        }
    };

    // run an external command with standard in and standard out connected.
    ptr<session> run (io::io_context &cc, string command, std::function<void (string_view)> out, std::function<void (string_view)> err) {
        return std::static_pointer_cast<session> (ptr<program_session> (new program_session {cc, command, out, err}));
    }

}


