// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/net/URL.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
#include <iostream>

namespace data::net {
    namespace {
        string write_params (list<entry<string, string>> params) {
            string path;

            if (params.size () > 0) {
                path.append ("?");
                for (const auto &it : params) {
                    path.append (it.Key + "=" + it.Value + "&");
                }

                path.pop_back ();
            }

            return path;
        }
    }

    string URL::path () const {
        std::stringstream ss;
        ss << Path << write_params (Params);
        return string {ss.str ()};
    }
    
    URL::operator string () const {
        std::stringstream ss;
        ss << Protocol << "://" << Host << (Port == "" ? string {""} : string {":"} + Port) << Path << write_params (Params);
        return string {ss.str ()};
    }

    protocol::operator name () const {
        string self = *this;

        boost::to_upper (self);

        if (self == "FTP") return FTP;
        if (self == "HTTP") return HTTP;
        if (self == "HTTPS") return HTTPS;
        if (self == "WS") return WS;
        if (self == "WSS") return WSS;
        return invalid;
    }

    string protocol::to_string (name n) {
        if (n == FTP) return "FTP";
        if (n == HTTP) return "HTTP";
        if (n == HTTPS) return "HTTPS";
        if (n == WS) return "WS";
        if (n == WSS) return "WSS";
        return "";
    }

}
