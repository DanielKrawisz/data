// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/net/websocket.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace data::net::websocket {

    namespace beast = boost::beast;             // from <boost/beast.hpp>
    namespace http = beast::http;               // from <boost/beast/http.hpp>
    namespace io = boost::asio;                 // from <boost/asio.hpp>
    using tcp = boost::asio::ip::tcp;           // from <boost/asio/ip/tcp.hpp>
    using insecure_stream = beast::websocket::stream<tcp::socket>;

    // TODO implement async::read_stream and async::write_stream for websocket sessions.
    // it should work just like in net/asio/socket.hpp


    void open (
        asio::io_context &ioc,
        const URL &url,
        asio::error_handler error_handler,
        interaction<string_view, const string &> interact,
        close_handler closed) {

        // TODO fill this in correctly
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

