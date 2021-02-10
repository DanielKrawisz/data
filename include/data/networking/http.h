// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>

#ifndef DATA_HTTP_H
#define DATA_HTTP_H
namespace data {
    namespace networking {
        class Http {
        public:
            Http();
            std::string POST(std::string hostname,std::string path="/",const std::map<std::string,std::string>& params={},const std::map<boost::beast::http::field,std::string>& headers = {},std::string port="https");
            std::string GET(std::string hostname,std::string path="/",const std::map<boost::beast::http::field,std::string>& headers = {},std::string port="https");
            std::string GET(std::string hostname,std::string path,std::map<std::string,std::string>& params,const std::map<boost::beast::http::field,std::string>& headers = {},std::string port="https");
        private:
            boost::asio::io_context ioc;
            boost::asio::ssl::context ssl_ctx;
            boost::asio::ip::tcp::resolver resolver;
        };
    }
}
#endif //DATA_HTTP_H
