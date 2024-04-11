
#include <data/net/beast/beast.hpp>

namespace data::net::HTTP::beast {

    // convert to beast format
    request from (const HTTP::request &r) {
        request req (r.Method, r.target ().c_str (), 11);

        req.set (HTTP::header::host, r.URL.domain_name ()->c_str ());
        req.set (HTTP::header::user_agent, r.UserAgent);

        for (const auto &header: r.Headers) req.set (header.Key, header.Value);

        req.body () = r.Body;
        req.prepare_payload ();
        return req;
    }

    struct header_already_exists {
        response Response;
        map<header, ASCII> operator () (map<header, ASCII> m, const header &h, const ASCII &o, const ASCII &n) const {
            throw data::exception {} << "HTTP response " << Response << " contains duplicate header " << h;
        }
    };

    // note: it is possible for a header to be known by boost::beast. In that case it gets deleted. Kind of dumb.
    HTTP::response to (const response &res) {

        map<header, ASCII> response_headers {};

        for (const auto &field : res) if (field.name () != header::unknown) response_headers = response_headers.insert
            (field.name (), ASCII {std::string {field.value ()}}, header_already_exists {res});

        return HTTP::response {res.base ().result (), response_headers, res.body ()};
    }

}


