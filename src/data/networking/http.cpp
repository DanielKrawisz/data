// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/networking/http.h>
#include <iostream>
namespace data::networking {

        Http::Http() :
        ssl_ctx(boost::asio::ssl::context::tlsv12_client),
        resolver(ioc)
        {
            ssl_ctx.set_default_verify_paths();
            ssl_ctx.set_verify_mode(boost::asio::ssl::verify_peer);
        }

    std::string
    Http::POST(std::string hostname,  const std::string path, const std::map<std::string, std::string>& params,
               const std::map<boost::beast::http::field,std::string> &headers,std::string port) {
        boost::beast::ssl_stream<boost::beast::tcp_stream> stream(ioc, ssl_ctx);

        // Set SNI Hostname (many hosts need this to handshake successfully)
        if(! SSL_set_tlsext_host_name(stream.native_handle(), hostname.c_str()))
        {
            boost::beast::error_code ec{static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
            throw boost::beast::system_error{ec};
        }
        auto const results = resolver.resolve(hostname.c_str(),port.c_str());
        boost::beast::get_lowest_layer(stream).connect(results);
        stream.handshake(boost::asio::ssl::stream_base::client);
        boost::beast::http::request<boost::beast::http::string_body> req(boost::beast::http::verb::post,path.c_str(),10);
        req.set(boost::beast::http::field::host,hostname.c_str());
        req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        for(const auto & header : headers) {
            req.set(header.first,header.second);
        }
        return std::string();
    }

    std::string Http::GET(std::string hostname, std::string path,
                          const std::map<boost::beast::http::field, std::string> &headers,std::string port) {
        boost::beast::ssl_stream<boost::beast::tcp_stream> stream(ioc, ssl_ctx);

        // Set SNI Hostname (many hosts need this to handshake successfully)
        if(! SSL_set_tlsext_host_name(stream.native_handle(), hostname.c_str()))
        {
            boost::beast::error_code ec{static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
            throw boost::beast::system_error{ec};
        }
        auto const results = resolver.resolve(hostname.c_str(),port.c_str());
        boost::beast::get_lowest_layer(stream).connect(results);
        stream.handshake(boost::asio::ssl::stream_base::client);
        boost::beast::http::request<boost::beast::http::string_body> req(boost::beast::http::verb::get,path.c_str(),10);
        req.set(boost::beast::http::field::host,hostname.c_str());
        req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        for(const auto & header : headers) {
            req.set(header.first,header.second);
        }
        boost::beast::http::write(stream,req);
        boost::beast::flat_buffer buffer;
        boost::beast::http::response<boost::beast::http::dynamic_body> res;
        boost::beast::http::read(stream,buffer,res);
        return boost::beast::buffers_to_string(res.body().data());
    }

    std::string Http::GET(std::string hostname, std::string path, std::map<std::string, std::string> &params,
                          const std::map<boost::beast::http::field, std::string> &headers, std::string port) {
            std::string newPath=path;
            if(params.size()>0) {
                newPath.append("?");
                for (const auto &it : params) {
                    newPath.append(it.first+"="+it.second+"&");
                }
                newPath.pop_back();
            }
        return GET(hostname,newPath,headers,port);
    }
}