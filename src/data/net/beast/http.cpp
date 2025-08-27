
#include <data/net/beast/http.hpp>

namespace data::net::HTTP::beast {

    template <typename msg> dispatch<header, ASCII> read_headers (const msg &m) {
        dispatch<header, ASCII> x;
        for (const auto &field : m)
            x <<= entry<header, ASCII> {header {field.name_string ()}, ASCII {field.value ()}};
        return x;
    }

    template <typename msg> void write_headers (msg &m, dispatch<header, ASCII> x) {
        for (const auto &[h, v]: x) m.insert (h, v);
    }

    // convert to beast format
    request to (const HTTP::request &r) {
        request req (r.Method, r.Target.c_str (), 11);

        write_headers (req, r.Headers);

        req.body () = string (r.Body);
        req.prepare_payload ();
        return req;
    }

    // note: it is possible for a header to be unknown by boost::beast. In that case it gets deleted. Kind of dumb.
    HTTP::response from (const response &res) {
        return HTTP::response {res.base ().result (), read_headers (res), bytes (data::string (res.body ()))};
    }

    HTTP::request from (const request &req) {
        return HTTP::request {req.method (), data::net::target {req.target ()}, read_headers (req), bytes (string (req.body ()))};
    }

    response to (const HTTP::response &r) {
        response res {r.Status.Status, 11};

        write_headers (res, r.Headers);

        res.body () = string (r.Body);
        res.prepare_payload ();
        return res;
    }

}


