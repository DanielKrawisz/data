// Copyright (c) 2023 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/networking/nHTTP.hpp>

namespace data::networking::experimental {

    std::string fromRange(const UriTextRangeA & rng)
    {
        return {rng.first, rng.afterLast};
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

    template<class SyncReadStream>
    boost::beast::http::response<boost::beast::http::dynamic_body> http_request(
            SyncReadStream& stream,
            std::string hostname,
            method verb,
            string path,
            map<header, string> headers, string body, int redirects) {

        boost::beast::http::request<boost::beast::http::string_body> req(verb, path.c_str(), 11);

        req.set(header::host, hostname.c_str());
        req.set(header::user_agent, BOOST_BEAST_VERSION_STRING);

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


    bool HTTP::sslInitialized=false;

    boost::asio::ssl::context HTTP::SSLContext = boost::asio::ssl::context(boost::asio::ssl::context::tlsv12_client);

    HTTP::HTTP(boost::asio::io_context &) {
        if(!HTTP::sslInitialized) {
            HTTP::SSLContext.set_default_verify_paths();
            HTTP::SSLContext.set_verify_mode(boost::asio::ssl::verify_peer);
            HTTP::sslInitialized=true;
        }
    }

    std::shared_ptr<HTTP::base_response> HTTP::operator()(const HTTP::request & request, int redirects, bool stream) {
        if(stream) {
            std::shared_ptr<HTTP::stream_response> streamResponse = std::make_shared<HTTP::stream_response>();

            return streamResponse;
        } else {
            std::shared_ptr<HTTP::message_response> messageResponse = std::make_shared<HTTP::message_response>();


            return messageResponse;
        }
    }


}