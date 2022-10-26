// Copyright (c) 2021 Katrina Knight
// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/networking/HTTP.hpp>
#include <data/networking/REST.hpp>

namespace data::networking {
    namespace {
        
        std::string fromRange(const UriTextRangeA & rng)
        {
            return std::string(rng.first, rng.afterLast);
        }

        std::string fromList(UriPathSegmentA * xs, const std::string & delim)
        {
            UriPathSegmentStructA * head(xs);
            std::string accum;

            while (head)
            {
                accum += delim + fromRange(head->text);
                head = head->next;
            }

            return accum;
        }
        
        string write_params(map<string, string> params) {
            string path;
            
            if(params.size()>0) {
                path.append("?");
                for (const auto &it : params) {
                    path.append(it.Key + "=" + it.Value + "&");
                }
                path.pop_back();
            }
            
            return path;
        }

        template<class SyncReadStream>
        boost::beast::http::response<boost::beast::http::dynamic_body> http_request(
            SyncReadStream& stream, 
            std::string hostname, 
            HTTP::method verb, 
            string path,
            map<HTTP::header, string> headers, string body, int redirects) {
            
            boost::beast::http::request<boost::beast::http::string_body> req(verb, path.c_str(), 11);

            req.set(HTTP::header::host, hostname.c_str());
            req.set(HTTP::header::user_agent, BOOST_BEAST_VERSION_STRING);

            for (const auto &header: headers) req.set(header.Key, header.Value);
            
            req.body() = body;
            req.prepare_payload();
            
            boost::beast::http::write(stream, req);

            boost::beast::flat_buffer buffer;
            boost::beast::http::response<boost::beast::http::dynamic_body> res;
            boost::beast::error_code ec;

            try {
                boost::beast::http::read(stream, buffer, res, ec);
            } catch (boost::exception &ex) {}
            return res;
            
        }
        
        string encode_form_data(map<string, string> form_data) {
            string newBody;
            
            if(form_data.size()>0) {
                for (const auto &it : form_data) {
                    newBody.append(it.Key+"="+it.Value+"&");
                }
                newBody.pop_back();
            }
            
            return newBody;
        }
        
    }

    HTTP::HTTP(boost::asio::io_context &ioc) : IOContext{ioc}, 
        SSLContext(boost::asio::ssl::context::tlsv12_client),
        Resolver(IOContext) {
            SSLContext.set_default_verify_paths();
            SSLContext.set_verify_mode(boost::asio::ssl::verify_peer);
        }
    
    HTTP::response HTTP::operator()(const request &req, int redirects) {
        
        if(redirects <= 0) throw std::logic_error{"too many redirects"};
        
        auto hostname = req.Host.c_str();
        auto port = req.Port.c_str();
        
        bool https = req.Port == "https";
        
        boost::beast::http::response<boost::beast::http::dynamic_body> res;
        if(https) {
            boost::beast::ssl_stream<boost::beast::tcp_stream> stream(IOContext, SSLContext);
            
            // Set SNI Hostname (many hosts need this to handshake successfully)
            if (!SSL_set_tlsext_host_name(stream.native_handle(), hostname)) {
                boost::beast::error_code ec{static_cast<int>(::ERR_get_error()),
                                            boost::asio::error::get_ssl_category()};
                throw boost::beast::system_error{ec};
            }

            auto const results = Resolver.resolve(hostname, port);

            boost::beast::get_lowest_layer(stream).connect(results);
            stream.handshake(boost::asio::ssl::stream_base::client);

            res = http_request(stream, req.Host, req.Method, req.Path, req.Headers, req.Body, redirects);
        } else {
            boost::beast::tcp_stream stream(IOContext);
            auto const results = Resolver.resolve(hostname, port);
            stream.connect(results);
            res = http_request(stream, req.Host, req.Method, req.Path, req.Headers, req.Body, redirects);
        }
        
        if (static_cast<unsigned int>(res.base().result()) >= 300 && static_cast<unsigned int>(res.base().result()) < 400) {
            std::string loc = res.base()["Location"].to_string();
            if (!loc.empty()) {
                UriUriA uri;
                const char **errorPos;
                if (uriParseSingleUriA(&uri, loc.c_str(), errorPos)) 
                    throw std::logic_error{"could not read redirect url"};
                
                return (*this)(HTTP::request{req.Method, fromRange(uri.portText), fromRange(uri.hostText),
                    fromList(uri.pathHead, "/") + fromRange(uri.fragment), req.Headers, req.Body}, redirects - 1);
            }
        }
        
        map<HTTP::header, string> response_headers{};
        for (const auto &field : res) response_headers = 
            data::insert(response_headers, data::entry<HTTP::header, string>{field.name(), std::string{field.value()}}); 
        return response{res.base().result(), response_headers, boost::beast::buffers_to_string(res.body().data())};
    }

    HTTP::request REST::POST(string path, map<string, string> params) const {
        return HTTP::request(HTTP::method::post, Port, Host, path, 
            {{boost::beast::http::field::content_type, "application/x-www-form-urlencoded"}}, 
              encode_form_data(params));
    }
    
    URL::operator string() const {
        std::stringstream ss;
        ss << Port << "://" << Host << Path << write_params(Params);
        return ss.str();
    }

}
