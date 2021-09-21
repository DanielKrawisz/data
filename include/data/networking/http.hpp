// Copyright (c) 2021 Katrina Knight
// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_HTTP
#define DATA_HTTP

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>

#include <data/tools.hpp>
#include <map>

namespace data::networking {
    struct http {
        using header = boost::beast::http::field;
        using method = boost::beast::http::verb;
        
        http();
        
        static string append_params(string path, const std::map<string, string>& params);
        
        string request(
            string port, 
            method verb, 
            string hostname, 
            string path = "/", 
            const std::map<header, string>& headers = {}, 
            string body = "");
        
        string request(
            method verb, 
            string hostname, 
            string path = "/", 
            const std::map<header, string>& headers = {}, 
            string body = "") {
            return request("https", verb, hostname, path, headers, body);
        }
        
        string GET(
            string hostname, 
            string path, 
            const std::map<string, string>& params = {},
            const std::map<header, string>& headers = {}, 
            string body = "") {
            return request(method::get, hostname, append_params(path, params), headers, body);
        }
        
        string POST(
            string hostname, 
            string path="/", 
            const std::map<string, string>& params = {}, 
            const std::map<header, string>& headers = {},
            string body = "") {
            return request(method::post, hostname, append_params(path, params), headers, body);
        }
        
        // post data from a form. 
        string POST(
            string hostname,
            string path="/",
            const std::map<string, string>& params={},
            const std::map<header, string>& headers={},
            const std::map<string, string>& form_data={});
        
    private:
        boost::asio::io_context ioc;
        boost::asio::ssl::context ssl_ctx;
        boost::asio::ip::tcp::resolver resolver;
 
    };
}

#endif 
