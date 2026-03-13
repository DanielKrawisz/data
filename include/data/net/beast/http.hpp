#ifndef DATA_NET_HTTP_BEAST_HTTP
#define DATA_NET_HTTP_BEAST_HTTP

// Copyright (c) 2021 Katrina Knight
// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/beast.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>

#include <data/net/HTTP.hpp>

// we use this for HTTP.
namespace data::net::HTTP::beast {
    using namespace boost::beast;            // from <boost/beast.hpp>
    namespace net = boost::asio;            // from <boost/asio.hpp>
    using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
    namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>

    using request = http::request<http::string_body>;
    using response = http::response<http::string_body>;

    // convert to beast format
    request to (const HTTP::request &r);

    response to (const HTTP::response &r);

    HTTP::response from (const response &res);

    HTTP::request from (const request &);
}

#endif
