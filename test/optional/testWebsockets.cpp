// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/net/websocket.hpp"

#include "gtest/gtest.h"
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/chrono.hpp>
#include "data/net/session.hpp"

using json = nlohmann::json;
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>

namespace data {

    const std::string websocketUrl = "ws://localhost:8765/";

    TEST (WebsocketsTest, TestWebsockets) {
        boost::asio::io_context io;
        ssl::context ctx {ssl::context::tlsv12_client};
        ctx.set_default_verify_paths ();
        data::net::websocket::open (io, data::net::URL {"ws://localhost:8765/"}, &ctx,
            [] (boost::system::error_code temp) {
                FAIL () << "Failed with errror code: " << temp;
            }, [] () {},
            [] (ptr<data::net::session<const string &>> session) -> handler<string_view> {
                return [session] (string_view x) {
                    std::cout << "Received " << x << std::endl;
                };
            });

        using clock = std::chrono::system_clock;
        clock::time_point nowp = clock::now ();
        io.run ();
    }
}

