// Copyright (c) 2023 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NHTTP_HPP
#define DATA_NHTTP_HPP

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <uriparser/Uri.h>
#include <data/tools.hpp>
#include <data/cross.hpp>
#include <data/networking/URL.hpp>
#include <map>
#include <utility>

namespace data::networking::experimental {
    using header = boost::beast::http::field;
    using method = boost::beast::http::verb;
    using status = boost::beast::http::status;
    class HTTP {


        struct request {
            method Method;
            string Port;
            string Host;
            string Path;
            map<header, string> Headers;
            string Body;
            bool Secure;

            request(
                    method method,
                    string port,
                    string host,
                    string path,
                    bool secure=false,
                    map<header, string> headers = {},
                    string body = {}) : Method{method}, Port{std::move(port)}, Host{std::move(host)}, Path{std::move(path)}, Secure{secure}, Headers{std::move(headers)}, Body{std::move(body)} {}

            request(method method, const URL& url, map<header, string> headers = {}, bool secure=false, string body = {}) :
                    Method{method}, Port{url.Port}, Host{url.Host}, Path{url.Path},Secure{secure}, Headers{std::move(headers)}, Body{std::move(body)} {}
        };


        struct base_response {
           bool connected;
        };

        struct stream_response  : public base_response {

        };

        struct message_response : public base_response {
            status Status;
            map<header, string> Headers;
            string Body;
        };
        std::shared_ptr<base_response> operator()(const request & request, int redirects = 10, bool stream=false);


        HTTP(boost::asio::io_context &);
        //=SSLContext(boost::asio::ssl::context::tlsv12_client)

    private:
        static boost::asio::ssl::context SSLContext;
        static bool sslInitialized;

    };
}
#endif //DATA_NHTTP_HPP
