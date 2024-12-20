#ifndef DATA_NET_HTTP_BEAST
#define DATA_NET_HTTP_BEAST

// Copyright (c) 2021 Katrina Knight
// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/beast.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>

#include <data/net/HTTP.hpp>

namespace data::net::HTTP::beast {
    using namespace boost::beast;            // from <boost/beast.hpp>
    namespace net = boost::asio;            // from <boost/asio.hpp>
    using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
    namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>

    using request = boost::beast::http::request<boost::beast::http::string_body>;
    using response = boost::beast::http::response<boost::beast::http::string_body>;

    // convert to beast format
    request from (const HTTP::request &r);

    response from (const HTTP::response &r);

    // note: it is possible for a header to be known by boost::beast. In that case it gets deleted. Kind of dumb.
    HTTP::response to (const response &res);

    HTTP::request to (const request &);
}

#endif
