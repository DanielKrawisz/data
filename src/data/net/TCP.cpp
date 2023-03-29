// Copyright (c) 2021-2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/net/TCP.hpp>
#include <data/net/async/message_queue.hpp>
#include <iostream>

namespace data::net::IP {
    
    namespace TCP {

        using socket = asio::socket<asio::ip::tcp::socket>;

        ptr<socket> connect (asio::io_context &io, const endpoint &p, asio::error_handler on_error, close_handler on_close) {
            ptr<asio::ip::tcp::socket> x {new asio::ip::tcp::socket (io)};
            asio::error_code error;
            x->connect (asio::ip::tcp::endpoint (p), error);

            if (error) throw exception {error};
            return ptr<socket> {new socket {x, on_error, on_close}};
        }
        
        void open (
            asio::io_context &context,
            endpoint end,
            asio::error_handler on_error,
            close_handler on_close,
            interaction<string_view, const string &> interact) {

            ptr<socket> ss = connect (context, end, on_error, on_close);

            ptr<session<const string &>> zz {static_cast<session<const string &> *>
                (new async::message_queue<const string &> {
                    std::static_pointer_cast<async::write_stream<const string &>> (ss)})};

            async::wait_for_message<string_view> (std::static_pointer_cast<async::read_stream<string_view>> (ss),
                [xx = interact (zz)] (string_view z) -> void {
                    return xx (z);
                });
        }

        void server::accept () {
            Socket.emplace (IO);

            Acceptor.async_accept (*Socket, [self = shared_from_this ()] (asio::error_code error) {
                ptr<asio::ip::tcp::socket> t {new asio::ip::tcp::socket {std::move (*self->Socket)}};

                auto on_error = [] (const asio::error_code &) -> void {};
                auto on_close = [] () -> void {};

                ptr<socket> ss {new socket {t, on_error, on_close}};

                ptr<session<const string &>> zz {static_cast<session<const string &> *>
                (new async::message_queue<const string &> {
                    std::static_pointer_cast<async::write_stream<const string &>> (ss)})};

                async::wait_for_message<string_view>
                (std::static_pointer_cast<async::read_stream<string_view>> (ss),
                    [xx = self->Interact (zz)] (string_view z) -> void {
                        return xx (z);
                    });

                self->accept ();
            });
        }
    }
}
