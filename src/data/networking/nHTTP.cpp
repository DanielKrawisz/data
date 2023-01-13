// Copyright (c) 2023 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/networking/nHTTP.hpp>

namespace data::networking::experimental {

    bool HTTP::sslInitialized=false;

    boost::asio::ssl::context HTTP::SSLContext = boost::asio::ssl::context(boost::asio::ssl::context::tlsv12_client);

    HTTP::HTTP(boost::asio::io_context &) {
        if(!HTTP::sslInitialized) {
            HTTP::SSLContext.set_default_verify_paths();
            HTTP::SSLContext.set_verify_mode(boost::asio::ssl::verify_peer);
            HTTP::sslInitialized=true;
        }
    }
}