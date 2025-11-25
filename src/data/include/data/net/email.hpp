
// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_EMAIL
#define DATA_NET_EMAIL

#include <data/stream.hpp>
#include <data/net/URL.hpp>
#include <data/lift.hpp>

// https://www.rfc-editor.org/rfc/rfc5322
// https://www.rfc-editor.org/rfc/rfc6854

namespace data::net::email {

    struct address;
    struct mailbox;
    struct message;
    struct header;

    writer<byte> &operator << (writer<byte> &, const address &);
    writer<byte> &operator << (writer<byte> &, const mailbox &);
    writer<byte> &operator << (writer<byte> &, const message &);
    writer<byte> &operator << (writer<byte> &, const header &);

    reader<byte> &operator >> (reader<byte> &, const address &);
    reader<byte> &operator >> (reader<byte> &, const mailbox &);
    reader<byte> &operator >> (reader<byte> &, const message &);
    reader<byte> &operator >> (reader<byte> &, const header &);

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

        static ASCII to_string (name);

        using ASCII::ASCII;
        field (name n) : field {to_string (n)} {}
        field (const ASCII &x) : ASCII {x} {}

        static bool valid (string_view);
        bool valid () const;
    };

    struct header : ASCII {
        using ASCII::ASCII;
        header (const ASCII &x) : ASCII {x} {};
        header (const field &f, const ASCII &v);

        static string_view name (string_view);
        static string_view value (string_view);

        field name () const;
        ASCII value () const;

        static bool valid (string_view);
        bool valid () const;
    };

    using headers = list<header>;

    struct date_time : ASCII {
        using ASCII::ASCII;
        date_time (const ASCII &);

        static bool valid (string_view);
        bool valid () const;
    };

    // a msg_id looks similar to an email address but is less restrictive.
    struct msg_id : ASCII {
        using ASCII::ASCII;
        msg_id (const ASCII &);

        static bool valid (string_view);
        bool valid () const;
    };

    struct message : ASCII {
        using ASCII::ASCII;
        message (const ASCII &x): ASCII {x} {}

        // an email message is divided into lines divided by CRLF.
        static list<string_view> lines (const ASCII &x);

        static list<std::pair<string_view, string_view>> headers (string_view);

        // body is optional
        static string_view body (string_view);

        email::headers headers () const;

        ASCII body () const;

        static bool valid (string_view);
        bool valid () const;

        // lines are not more than 78 chars.
        // message_id field is included.
        static bool standard (string_view);
        bool standard () const;

        message (email::headers, const ASCII &body);

        // header values having to do with information on
        // the sender and receiver.
        struct sent_info {
            // these are the only two required fields.
            date_time Date;
            address From;
            maybe<address> Sender;

            maybe<msg_id> MessageID;

            // one of these fields will be present.
            maybe<list<address>> To;
            maybe<list<address>> Cc;
            maybe<list<address>> Bcc;
        };

        // more than one indicates a resend, most recent first.
        list<sent_info> sent () const;

        maybe<ASCII> subject () const;
        maybe<list<address>> reply_to () const;

        maybe<list<msg_id>> in_reply_to () const;
        maybe<list<msg_id>> references () const;

        list<ASCII> comments () const;
        list<list<ASCII>> keywords () const;

        // put a list of headers here followed by the body.
        template <typename... P>
        explicit message (const header &, P... p);

        // for writing a message before you send it.
        struct write {
            sent_info Sent;

            maybe<ASCII> Subject;
            list<ASCII> Comments;
            list<list<ASCII>> Keywords;

            maybe<list<address>> ReplyTo;
            maybe<list<msg_id>> InReplyTo;
            maybe<list<msg_id>> References;

            list<header> Optional;

            maybe<ASCII> Body;
        };

        message (const write &);

        // construct headers for a reply message.
        write reply () const;

        write read (string_view) const;
        write read () const;

        // add appropriate resend fields.
        message resend () const;

        struct trace {
            mailbox ReturnPath;
            string ReceivedToken;
            date_time ReceivedTime;
        };

        list<trace> traces () const;

        message add_trace (const trace &);

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
        static list<string_view> return_path (string_view);
        static list<string_view> received (string_view);
        static list<string_view> resent_date (string_view);
        static list<string_view> resent_from (string_view);
        static list<string_view> resent_message_id (string_view);
        static list<string_view> resent_sender (string_view);
        static list<string_view> resent_to (string_view);
        static list<string_view> resent_cc (string_view);
        static list<string_view> resent_bcc (string_view);
    };

    struct date : header {
        date (const date_time &d): header {field::orig_date, d} {}
    };

    struct from : header {
        from (list<address>);
    };

    struct sender : header {
        sender (const address &a): header {field::sender, a} {}
    };

    struct reply_to : header {
        reply_to (list<address>);
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
        subject (const ASCII &x) : header {field::subject, x} {}
    };

    struct comments : header {
        comments (const ASCII &c) : header {field::comments, c} {}
    };

    struct keywords : header {
        keywords (list<ASCII>);
    };

    struct return_path : header {
        return_path (const mailbox &m) : header {field::return_path, m} {}
    };

    struct received : header {
        received (const string &token, const date_time &);
    };

    struct resent_date : header {
        resent_date (const date_time &d): header {field::orig_date, d} {}
    };

    struct resent_from : header {
        resent_from (list<address>);
    };

    struct resent_sender : header {
        resent_sender (list<address>);
    };

    struct resent_to : header {
        resent_to (list<address>);
    };

    struct resent_cc : header {
        resent_cc (list<address>);
    };

    struct resent_bcc : header {
        resent_bcc (list<address>);
    };

    struct resent_message_id : header {
        resent_message_id (const msg_id &d): header {field::message_id, d} {}
    };

    namespace {
        struct incomplete_message {

            headers Headers;
            ASCII Body;

            incomplete_message attach (const header &h) {
                return incomplete_message {Headers << h, Body};
            }

            incomplete_message attach (const ASCII &b) {
                return incomplete_message {Headers, b};
            }

            template <typename... P>
            incomplete_message attach (const header &h, P... p) {
                return attach (h).attach (p...);
            }

            operator message () const {
                return message {Headers, Body};
            }
        };

    }

    template <typename... P>
    inline message::message (const header &h, P... p) {
        *this = message (incomplete_message {}.attach (h, p...));
    }

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

    bool inline date_time::valid () const {
        return valid (*this);
    }

    bool inline msg_id::valid () const {
        return valid (*this);
    }

    bool inline message::valid () const {
        return valid (*this);
    }

    bool inline message::standard () const {
        return standard (*this);
    }

    writer<byte> inline &operator << (writer<byte> &b, const address &a) {
        b.write ((byte const *) (a.data ()), a.size ());
        return b;
    }

    writer<byte> inline &operator << (writer<byte> &b, const mailbox &m) {
        b.write ((byte const *) (m.data ()), m.size ());
        return b;
    }

    writer<byte> inline &operator << (writer<byte> &b, const message &m) {
        b.write ((byte const *) (m.data ()), m.size ());
        return b;
    }

    writer<byte> inline &operator << (writer<byte> &b, const header &h) {
        b.write ((byte const *) (h.data ()), h.size ());
        return b;
    }

    inline address::address (const ASCII &local, const net::domain_name &domain): ASCII {local + "@" + domain} {}

    inline address::address (const ASCII &display, const ASCII &local, const net::domain_name &domain):
        ASCII {display + " <" + local + "@" + domain + ">"} {}

    inline address::address (const ASCII &local, const IP::address &addr): ASCII {local + "@[" + addr + "]"} {}

    inline address::address (const ASCII &display, const ASCII &local, const IP::address &addr):
        ASCII {display + " <" + local + "@[" + addr + "]>"} {}

    inline header::header (const field &f, const ASCII &v) : ASCII {f + ":" + v} {}

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
        if (x.data () == nullptr) throw exception {} << "invalid mailbox";
        return data::ASCII {x};
    }

    field inline header::name () const {
        return field {header::name (*this)};
    }

    ASCII inline header::value () const {
        return data::ASCII {header::value (*this)};
    }

    list<string_view> inline message::lines (const ASCII &x) {
        char CRLF[] = {13, 10};
        return split (x, std::string {CRLF});
    }

    string_view inline header::name (string_view x) {
        auto z = split (x, ":");
        if (z.size () < 2) throw exception {} << "invalid header";
        return z[1];
    }

    string_view inline header::value (string_view x) {
        auto z = split (x, ":");
        if (z.size () < 2) throw exception {} << "invalid header";
        return string_view {x.data () + z[1].size () + 1, x.size () - z[1].size () - 1};
    }

    email::headers inline message::headers () const {
        return lift ([] (std::pair<string_view, string_view> x) -> email::header {
            return email::header {email::field (x.first), ASCII (x.second)};
        }, headers (*this));
    }

    ASCII inline message::body () const {
        return ASCII {body (*this)};
    }

    list<ASCII> inline message::comments () const {
        return lift ([] (string_view x) -> ASCII {
            return ASCII {x};
        }, comments (*this));
    }

    maybe<ASCII> inline message::subject () const {
        string_view x = message::subject (*this);
        if (x.data () == nullptr) return {};
        return {data::ASCII {x}};
    }

    inline from::from (list<address> x) : header {field::from, string_join (riffle (x, ", "))} {}

    inline reply_to::reply_to (list<address> x) : header {field::reply_to, string_join (riffle (x, ", "))} {}

    inline to::to (list<address> x) : header {field::to, string_join (riffle (x, ", "))} {}

    inline cc::cc (list<address> x) : header {field::cc, string_join (riffle (x, ", "))} {}

    inline bcc::bcc (list<address> x) : header {field::bcc, string_join (riffle (x, ", "))} {}

    inline in_reply_to::in_reply_to (list<msg_id> x) : header {field::in_reply_to, string_join (riffle (x, ", "))} {}

    inline references::references (list<msg_id> x) : header {field::references, string_join (riffle (x, ", "))} {}

    inline keywords::keywords (list<ASCII> x): header {field::keywords, string_join (riffle (x, ", "))} {}

    inline resent_from::resent_from (list<address> x) : header {field::resent_from, string_join (riffle (x, ", "))} {}

    inline resent_sender::resent_sender (list<address> x) : header {field::resent_sender, string_join (riffle (x, ", "))} {}

    inline resent_to::resent_to (list<address> x) : header {field::resent_to, string_join (riffle (x, ", "))} {}

    inline resent_cc::resent_cc (list<address> x) : header {field::resent_cc, string_join (riffle (x, ", "))} {}

    inline resent_bcc::resent_bcc (list<address> x) : header {field::resent_bcc, string_join (riffle (x, ", "))} {}

    inline received::received (const string &token, const date_time &time): header {field::received, token + "; " + time} {}
}


#endif
