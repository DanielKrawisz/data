
#include <data/io/exception.hpp>
#include <data/net/HTTP_server.hpp>

namespace data::net::HTTP::beast {
    bool is_websocket_upgrade (const request &req) {
        return req.method () == http::verb::get &&
            req[http::field::connection].find ("Upgrade") != boost::beast::string_view::npos &&
            req[http::field::upgrade] == "websocket";
    }

    awaitable<void> write_websocket_decline (asio::ip::tcp::socket &socket) {
        http::response<http::string_body> res {http::status::upgrade_required, 11};
        res.set(http::field::content_type, "text/plain");
        res.body() = "WebSocket upgrade not supported.";
        res.prepare_payload();

        co_await beast::http::async_write (socket, res, asio::use_awaitable);
    }

}

namespace data::net::HTTP {
    server::server (exec ex, net::IP::TCP::endpoint ep, request_handler handler):
        Acceptor {ex}, Handler {std::move (handler)} {

        asio::error ec;
        asio::ip::tcp::endpoint endpoint (ep);

        // Open the acceptor
        Acceptor.open (endpoint.protocol (), ec);
        if (ec) throw data::exception {} << "Error opening acceptor: " << ec.message ();

        // Bind to the endpoint
        Acceptor.bind (endpoint, ec);
        if (ec) throw data::exception {} << "Error binding acceptor: " << ec.message ();

        // Start listening for connections
        Acceptor.listen (asio::socket_base::max_listen_connections, ec);
        if (ec) throw data::exception {} << "Error listening for connections: " << ec.message ();
    }

    awaitable<bool> server::accept () {
        if (!Acceptor.is_open ()) co_return false;

        try {
            asio::ip::tcp::socket socket = co_await Acceptor.async_accept (asio::use_awaitable);
            // Spawn a new session to handle the connection
            ptr<session> sess = std::make_shared<session> (this->Sessions, std::move (socket), this->Handler);
            this->Sessions.add (sess);

            sess->Self = sess;

            auto ex = co_await asio::this_coro::executor;

            spawn (ex, [sess] () -> awaitable<void> {
                co_await sess->respond ();
                sess->Self = nullptr;
                co_return;
            });

            // yield if single-threaded
            co_await asio::post (ex, asio::use_awaitable);
        } catch (const boost::system::system_error &e) {
            if (e.code () == boost::asio::error::operation_aborted) {
                co_return false;  // The acceptor was closed—time to shut down
            } else throw e;
        }

        co_return true;
    }

    awaitable<void> server::session::respond () {
        while (true) {
            beast::flat_buffer buff;
            beast::request req;
            try {
                co_await beast::http::async_read (Socket, buff, req, asio::use_awaitable);
            } catch (const boost::system::system_error &e) {
                // Normal: client closed connection after request
                if (e.code () == beast::http::error::end_of_stream ||
                    e.code () == boost::asio::error::eof) {
                    // Remote side closed connection
                    Sessions.remove (Self);
                    co_return;
                } else if (e.code () == boost::asio::error::operation_aborted ||
                    e.code () == boost::asio::error::bad_descriptor) {
                    // You cancelled this (e.g. during shutdown)
                    co_return;
                } else throw e;
            }

            co_await handle_request (req);

            if (!req.keep_alive ()) break;
        }
    }

    void server::sessions::remove_all () {
        std::lock_guard<std::mutex> lock (Mtx);
        auto x = Sessions.begin ();
        while (x != Sessions.end ()) {
            (*x)->close ();
            x = Sessions.erase (x);
        }
    }

    void server::sessions::add (ptr<session> x) {
        std::lock_guard<std::mutex> lock (Mtx);
        Sessions.insert (x);
    }

    void server::sessions::remove (ptr<session> x) {
        std::lock_guard<std::mutex> lock (Mtx);
        x->close ();
        Sessions.erase (x);
    }
}
