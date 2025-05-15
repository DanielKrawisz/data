
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
