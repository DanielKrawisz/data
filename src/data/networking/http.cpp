// Copyright (c) 2021 Katrina Knight
// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/networking/http.hpp>
#include <iostream>

namespace data::networking {

    http::http() :
        ssl_ctx(boost::asio::ssl::context::tlsv12_client),
        resolver(ioc) {
            ssl_ctx.set_default_verify_paths();
            ssl_ctx.set_verify_mode(boost::asio::ssl::verify_peer);
        }
        
    string http::append_params(string path, const std::map<string, string>& params) {
        string newPath=path;
        if(params.size()>0) {
            newPath.append("?");
            for (const auto &it : params) {
                newPath.append(it.first+"="+it.second+"&");
            }
            newPath.pop_back();
        }
        return newPath;
    }
    
    string encode_form_data(const std::map<string, string>& form_data) {
        string newBody;
        if(form_data.size()>0) {
            for (const auto &it : form_data) {
                newBody.append(it.first+"="+it.second+"&");
            }
            newBody.pop_back();
        }
        return newBody;
    }
    
    string http::request(string port, method verb, string hostname, string path, const std::map<header, string> &headers, string body) {
        boost::beast::ssl_stream<boost::beast::tcp_stream> stream(ioc, ssl_ctx);
        
        // Set SNI Hostname (many hosts need this to handshake successfully)
        if(! SSL_set_tlsext_host_name(stream.native_handle(), hostname.c_str()))
        {
            boost::beast::error_code ec{static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
            throw boost::beast::system_error{ec};
        }

        auto const results = resolver.resolve(hostname.c_str(), port.c_str());

        boost::beast::get_lowest_layer(stream).connect(results);
        stream.handshake(boost::asio::ssl::stream_base::client);

        boost::beast::http::request<boost::beast::http::string_body> req(verb, path.c_str(), 11);

        req.set(header::host,hostname.c_str());
        req.set(header::user_agent, BOOST_BEAST_VERSION_STRING);

        for(const auto & header : headers) {
            req.set(header.first,header.second);
        }
        req.body() = body;
        req.prepare_payload();
        boost::beast::http::write(stream, req);

        boost::beast::flat_buffer buffer;
        boost::beast::http::response<boost::beast::http::dynamic_body> res;
        boost::beast::error_code ec;

        try {
            boost::beast::http::read(stream, buffer, res,ec);
        } catch(boost::exception& ex) {}

        return boost::beast::buffers_to_string(res.body().data());
    }

    string http::POST(string hostname, string path, const std::map<string, string> &params,
                      const std::map<header, string> &headers, const std::map<string, string> &form_data) {
        
        auto newHeaders=std::map<header, string>(headers);
        newHeaders[boost::beast::http::field::content_type]="application/x-www-form-urlencoded";
        
        return request(method::post, hostname, append_params(path, params), newHeaders, encode_form_data(form_data));
    }


}
