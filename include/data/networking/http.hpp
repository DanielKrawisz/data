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
#include <uriparser/Uri.h>
#include <data/tools.hpp>
#include <map>

namespace data::networking {

    std::string fromRange(const UriTextRangeA & rng);
    std::string fromList(UriPathSegmentA * xs, const std::string & delim);
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
            string body = "",
            int redirects=10);
        
        string request(
            method verb, 
            string hostname, 
            string path = "/",
            string port="https",
            const std::map<header, string>& headers = {}, 
            string body = "",
            int redirects=10) {
            return request(port, verb, hostname, path, headers, body,redirects);
        }
        
        string GET(
            string hostname, 
            string path,
            string port="https",
            const std::map<string, string>& params = {},
            const std::map<header, string>& headers = {}, 
            string body = "",
            int redirects=10) {
            return request(method::get, hostname,append_params(path, params),port , headers, body,redirects);
        }
        
        string POST(
            string hostname, 
            string path="/",
            string port="https",
            const std::map<string, string>& params = {}, 
            const std::map<header, string>& headers = {},
            string body = "",
            int redirects=10) {
            return request(method::post, hostname, append_params(path, params),port, headers, body,redirects);
        }
        
        // post data from a form. 
        string POST(
            string hostname,
            string path="/",
            string port="https",
            const std::map<string, string>& params={},
            const std::map<header, string>& headers={},
            const std::map<string, string>& form_data={},
            int redirects=10);
        
    private:
        boost::asio::io_context ioc;
        boost::asio::ssl::context ssl_ctx;
        boost::asio::ip::tcp::resolver resolver;
 
    };
}

#endif 
