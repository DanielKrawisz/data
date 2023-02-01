// Copyright (c) 2021 Katrina Knight
// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NETWORKING_HTTP
#define DATA_NETWORKING_HTTP

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
#include <data/net/URL.hpp>
#include <map>

namespace data::net {
    
    struct HTTP {
        using header = boost::beast::http::field;
        using method = boost::beast::http::verb;
        using status = boost::beast::http::status;
        
        struct request {
            method Method;
            net::URL URL;
            map<header, string> Headers;
            string Body;
            
            request (
                method method, 
                net::URL url,
                map<header, string> headers = {}, 
                string body = {}) : Method {method}, URL {url}, Headers {headers}, Body {body} {}
            
        };
        
        struct response {
            status Status;
            map<header, string> Headers;
            string Body;
        };
        
        // throws an exception under conditions in which no response is received.
        response operator () (const request &, int redirects = 10);
        
        // this is for when the HTTP object is your only connection to the outside world.
        HTTP ();

        // this is for when you might be connected in other ways so that you have to sync different interactions.
        HTTP (ptr<boost::asio::io_context>, ptr<boost::asio::ssl::context>);
        
        struct exception : std::exception {
            request Request;
            response Response;
            string Problem;
            exception (const request &req, const response &res, const string &p) :
                Request {req}, Response {res}, Problem {p} {}
            const char *what () const noexcept override {
                return Problem.c_str ();
            }
        };
        
        ptr<boost::asio::io_context> IOContext;
        ptr<boost::asio::ssl::context> SSLContext;
        boost::asio::ip::tcp::resolver Resolver;
 
    };
}

#endif 
