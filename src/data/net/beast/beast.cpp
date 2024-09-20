
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
        header Header;
        ASCII operator () (const ASCII &o, const ASCII &n) const {
            throw data::exception {} << "HTTP response " << Response << " contains duplicate header " << Header;
        }
    };

    // note: it is possible for a header to be unknown by boost::beast. In that case it gets deleted. Kind of dumb.
    HTTP::response to (const response &res) {

        map<header, ASCII> response_headers {};

        for (const auto &field : res) if (field.name () != header::unknown) response_headers = response_headers.insert
            (field.name (), ASCII {std::string {field.value ()}}, header_already_exists {res, field.name ()});

        return HTTP::response {res.base ().result (), response_headers, res.body ()};
    }

}


