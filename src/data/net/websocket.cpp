// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/net/websocket.hpp>

namespace data::net::websocket {

    ptr<session> open (
        io::io_context &ioc,
        const URL &url,
        asio::error_handler error_handler,
        receive_handler<session, string_view> receive_handler) {

        if (url.Protocol == protocol::WSS) throw exception {} << "secure websockets not yet implemented";

        if (url.Protocol != protocol::WS) throw exception {} << "protocol " << url.Protocol << " is not websockets";

        // These objects perform our I/O
        tcp::resolver resolver {ioc};
        ptr<insecure_stream> ws = std::make_shared<insecure_stream> (ioc);

        // Look up the domain name
        auto const results = resolver.resolve (url.Host, url.Port);

        // Make the connection on the IP address we get from a lookup
        auto ep = io::connect (ws->next_layer (), results);

        // Update the host_ string. This will provide the value of the
        // Host HTTP header during the WebSocket handshake.
        // See https://tools.ietf.org/html/rfc7230#section-5.4
        string host = url.Host + ':' + std::to_string (ep.port ());

        // Set a decorator to change the User-Agent of the handshake
        ws->set_option (beast::websocket::stream_base::decorator (
            [] (beast::websocket::request_type& req) {
                req.set (http::field::user_agent,
                    std::string (BOOST_BEAST_VERSION_STRING) +
                        " websocket-client-coro");
            }));

        // Perform the websocket handshake
        ws->handshake (host, "/");


    }

}

