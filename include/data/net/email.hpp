
// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_EMAIL
#define DATA_NET_EMAIL

#include <data/stream.hpp>
#include <data/net/URL.hpp>

// https://www.rfc-editor.org/rfc/rfc5322
// https://www.rfc-editor.org/rfc/rfc6854

namespace data::net::email {

    struct address;
    struct mailbox;
    struct message;
    struct header;

    bytes_writer &operator << (bytes_writer &, const address &);
    bytes_writer &operator << (bytes_writer &, const mailbox &);
    bytes_writer &operator << (bytes_writer &, const message &);
    bytes_writer &operator << (bytes_writer &, const header &);

    bytes_reader &operator >> (bytes_reader &, const address &);
    bytes_reader &operator >> (bytes_reader &, const mailbox &);
    bytes_reader &operator >> (bytes_reader &, const message &);
    bytes_reader &operator >> (bytes_reader &, const header &);

    struct address : ASCII {
        using ASCII::ASCII;
        address (const ASCII &x) : ASCII {x} {}

        address (const ASCII &local, const net::domain_name &domain);
        address (const ASCII &local, const IP::address &);
        address (const ASCII &display, const ASCII &local, const net::domain_name &domain);
        address (const ASCII &display, const ASCII &local, const IP::address &);

        static bool valid (string_view);
        bool valid () const;

        static string_view display_name (string_view);
        static string_view local_part (string_view);
        static string_view domain (string_view);
        static string_view domain_name (string_view);
        static string_view ip_address (string_view);
        static string_view addr_spec (string_view);

        // addr_spec is what most people think of as an email address.
        // you know, like "m@q.com". According to the RFC, an address
        // can include a display name or could be a group address.
        maybe<address> addr_spec () const;

        maybe<ASCII> local_part () const;
        maybe<ASCII> domain () const;

        maybe<ASCII> display_name () const;
        maybe<net::domain_name> domain_name () const;
        maybe<IP::address> ip_address () const;
    };

    // a mailbox is more like what people think of as an email address.
    // it is an address for an individual person, whereas address can
    // also include group addresses.
    struct mailbox : address {
        using address::address;

        static bool valid (string_view);
        bool valid () const;

        mailbox addr_spec () const;

        ASCII local_part () const;
        ASCII domain () const;
    };

    struct field : ASCII {
        enum name {
            optional,
            from,
            sender,
            reply_to,
            to,
            cc,
            bcc,
            message_id,
            in_reply_to,
            references,
            subject,
            comments,
            keywords,
            return_path,
            received,
            resent_date,
            resent_from,
            resent_sender,
            resent_to,
            resent_cc,
            resent_bcc,
            resent_msg_id,
            orig_date
        };

        using ASCII::ASCII;
        field (name);
        field (const ASCII &x) : ASCII {x} {}

        static bool valid (string_view);
        bool valid () const;
    };

    struct header : ASCII {
        using ASCII::ASCII;
        header (const ASCII &x) : ASCII {x} {};
        header (const field &f, const ASCII &v);

        field name () const;
        ASCII value () const;

        static bool valid (string_view);
        bool valid () const;
    };

    using headers = list<header>;

    struct date_time : ASCII {
        using ASCII::ASCII;
        date_time (const ASCII &);
    };

    struct msg_id : ASCII {
        using ASCII::ASCII;
        msg_id (const ASCII &);
    };

    struct message : ASCII {
        using ASCII::ASCII;
        message (const ASCII &);

        // an email message is divided into lines divided by CRLF.
        static list<string_view> lines (const ASCII &x);

        static list<std::pair<string_view, string_view>> headers (string_view);

        // body is optional
        static bytes_view body (string_view);

        email::headers headers () const;

        ASCII body () const;

        static bool valid (string_view);
        bool valid () const;

        // obsolete versions of the email format are accepted but not considered valid.
        static bool obsolete (string_view);
        bool obsolete () const;

        // lines are not more than 78 chars.
        // message_id field is included.
        // resent fields are correct if included.
        static bool standard (string_view);
        bool standard () const;

        message (email::headers, const ASCII &body);

        // put a list of headers here followed by the body.
        template <typename... P>
        message (P... p);

        static string_view date (string_view);
        static string_view from (string_view);
        static string_view message_id (string_view);
        static string_view subject (string_view);
        static string_view sender (string_view);
        static string_view reply_to (string_view);
        static string_view to (string_view);
        static string_view cc (string_view);
        static string_view bcc (string_view);
        static string_view in_reply_to (string_view);
        static string_view references (string_view);
        static list<string_view> comments (string_view);
        static list<string_view> keywords (string_view);

        // these are the only two required fields.
        date_time date () const;
        address from () const;

        maybe<msg_id> message_id () const;

        maybe<ASCII> subject () const;

        maybe<address> sender () const;
        maybe<list<address>> reply_to () const;

        maybe<list<address>> to () const;
        maybe<list<address>> cc () const;
        maybe<list<address>> bcc () const;

        maybe<list<msg_id>> in_reply_to () const;
        maybe<list<msg_id>> references () const;

        list<ASCII> comments () const;
        list<list<ASCII>> keywords () const;

        // construct headers for a reply message.
        email::headers reply () const;

        // add appropriate resend fields.
        message resend () const;
    };

    struct date : header {
        date (const date_time &d): header {field::orig_date, d} {}
    };

    struct from : header {
        from (list<address>);
    };

    struct sender : header {
        sender (list<address>);
    };

    struct reply_to : header {
        reply_to (const address &ad): header {field::reply_to, ad} {}
    };

    struct to : header {
        to (list<address>);
    };

    struct cc : header {
        cc (list<address>);
    };

    struct bcc : header {
        bcc (list<address>);
    };

    struct message_id : header {
        message_id (const msg_id &d): header {field::message_id, d} {}
    };

    struct in_reply_to : header {
        in_reply_to (list<msg_id>);
    };

    struct references : header {
        references (list<msg_id>);
    };

    struct subject : header {
        subject (const ASCII &);
    };

    struct comment : header {
        comment (const ASCII &);
    };

    struct keywords : header {
        keywords (list<ASCII>);
    };

    bool inline address::valid () const {
        return valid (*this);
    }

    bool inline mailbox::valid () const {
        return valid (*this);
    }

    bool inline field::valid () const {
        return valid (*this);
    }

    bool inline header::valid () const {
        return valid (*this);
    }

    bool inline message::valid () const {
        return valid (*this);
    }

    bool inline message::obsolete () const {
        return obsolete (*this);
    }

    bool inline message::standard () const {
        return standard (*this);
    }

    bytes_writer inline &operator << (bytes_writer &b, const address &a) {
        b.write ((byte const *) (a.data ()), a.size ());
        return b;
    }

    bytes_writer inline &operator << (bytes_writer &b, const mailbox &m) {
        b.write ((byte const *) (m.data ()), m.size ());
        return b;
    }

    bytes_writer inline &operator << (bytes_writer &b, const message &m) {
        b.write ((byte const *) (m.data ()), m.size ());
        return b;
    }

    bytes_writer inline &operator << (bytes_writer &b, const header &h) {
        b.write ((byte const *) (h.data ()), h.size ());
        return b;
    }

    inline address::address (const ASCII &local, const net::domain_name &domain): ASCII {local + "@" + domain} {}

    inline address::address (const ASCII &display, const ASCII &local, const net::domain_name &domain):
        ASCII {display + " <" + local + "@" + domain + ">"} {}

    inline address::address (const ASCII &local, const IP::address &addr): ASCII {local + "@[" + addr + "]"} {}

    inline address::address (const ASCII &display, const ASCII &local, const IP::address &addr):
        ASCII {display + " <" + local + "@[" + addr + "]>"} {}

    maybe<ASCII> inline address::local_part () const {
        string_view x = local_part (*this);
        if (x.data () == nullptr) return {};
        return data::ASCII {x};
    }

    maybe<ASCII> inline address::domain () const {
        string_view x = domain (*this);
        if (x.data () == nullptr) return {};
        return data::ASCII {x};
    }

    maybe<ASCII> inline address::display_name () const {
        string_view x = display_name (*this);
        if (x.data () == nullptr) return {};
        return data::ASCII {x};
    }

    maybe<net::domain_name> inline address::domain_name () const {
        string_view x = domain_name (*this);
        if (x.data () == nullptr) return {};
        return data::ASCII {x};
    }

    maybe<address> inline address::addr_spec () const {
        string_view x = addr_spec (*this);
        if (x.data () == nullptr) return {};
        return data::ASCII {x};
    }

    mailbox inline mailbox::addr_spec () const {
        string_view x = address::addr_spec (*this);
        if (x.data () == nullptr) throw exception {} << "invalid mailbox";
        return data::ASCII {x};
    }

    ASCII inline mailbox::local_part () const {
        string_view x = address::local_part (*this);
        if (x.data () == nullptr) throw exception {} << "invalid mailbox";
        return data::ASCII {x};
    }

    ASCII inline mailbox::domain () const {
        string_view x = address::domain (*this);
        if (x.data () == nullptr) return {};
        return data::ASCII {x};
    }

    list<string_view> inline message::lines (const ASCII &x) {
        char CRLF[] = {13, 10};
        return split (x, std::string {CRLF});
    }
}


#endif
