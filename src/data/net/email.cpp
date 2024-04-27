
// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#include <data/net/email.hpp>

// https://www.rfc-editor.org/rfc/rfc5322

namespace data::net::email {

    char CRLF[] = {13, 10};

    bool lines_lengths_exceed (string_view, size_t);

    // although not required, lines should be 78 chars or less.
    bool inline lines_satisfy_78_char_limit (string_view x) {
        return !lines_lengths_exceed (x, 78);
    }

    // break lines up so that none is longer than the given limit (not including CRLFs)
    // this may require inserting comments containing line breaks and whitespace.
    // string is only returned if the limit could be satisfied.
    maybe<message> break_lines (const message &m, uint32 limit);

    ASCII remove_folded_whitespace (const ASCII &);

    maybe<IP::address> inline address::ip_address () const {
        string_view x = ip_address (*this);
        if (x.data () == nullptr) return {};
        return remove_folded_whitespace (data::ASCII {x});
    }

    ASCII field::to_string (name n) {
        switch (n) {
            case orig_date : return ASCII {"Date"};
            case from : return ASCII {"From"};
            case subject : return ASCII {"Subject"};
            case message_id : return ASCII {"Message-ID"};
            case sender : return ASCII {"Sender"};
            case reply_to : return ASCII {"Reply-To"};
            case to : return ASCII {"To"};
            case cc : return ASCII {"Cc"};
            case bcc : return ASCII {"Bcc"};
            case in_reply_to : return ASCII {"In-Reply-To"};
            case references : return ASCII {"References"};
            case comments : return ASCII {"Comments"};
            case keywords : return ASCII {"Keywords"};
            case return_path : return ASCII {"Return-Path"};
            case received : return ASCII {"Received"};
            case resent_date : return ASCII {"Resent-Date"};
            case resent_from : return ASCII {"Resent-From"};
            case resent_sender : return ASCII {"Resent-Sender"};
            case resent_to : return ASCII {"Resent-To"};
            case resent_cc : return ASCII {"Resent-Cc"};
            case resent_bcc : return ASCII {"Resent-Bcc"};
            case resent_msg_id : return ASCII {"Resent-Message-ID"};
            default: throw exception {} << "unknown field type.";
        }
    }

    list<std::pair<string_view, string_view>> message::headers (string_view x) {
        char empty_line[] = {13, 10, 13, 10};
        list<string_view> z = split (x, std::string {empty_line});
        if (z.size () != 2) throw exception {} << "invalid message";
        list<string_view> hz = split (z[0], string_view {CRLF, 2});
        list<std::pair<string_view, string_view>> result;
        for (string_view h : hz) result <<= std::pair<string_view, string_view> {header::name (h), header::value (h)};
        return result;
    }

    string_view message::body (string_view x) {
        char empty_line[] = {13, 10, 13, 10};
        list<string_view> z = split (x, string_view {empty_line, 4});
        if (z.size () != 2) throw exception {} << "invalid message";
        return z[1];
    }

    message::message (email::headers hz, const ASCII &body) {
        std::stringstream ss;
        for (const auto &h : hz) ss << h << CRLF;
        ss << CRLF << body;
        message m {ss.str ()};

        // try to satisfy the recommended 78 character line limit.
        if (auto mm = break_lines (m, 78); bool (mm)) {
            *this = *mm;
            return;
        }

        // if not, try to satisfy the required 998 character line limit.
        if (auto mm = break_lines (m, 998); bool (mm)) {
            *this = *mm;
            return;
        }

        // if not, then this message is not valid and the user will have to
        // fix it himself.
        *this = m;
    }

    list<message::sent_info> message::sent () const {

        string_view d = date (*this);
        string_view f = from (*this);
        string_view i = message_id (*this);
        string_view s = sender (*this);
        string_view t = to (*this);
        string_view c = cc (*this);
        string_view b = bcc (*this);

        list<string_view> ddd = resent_date (*this);
        list<string_view> fff = resent_from (*this);
        list<string_view> iii = resent_message_id (*this);
        list<string_view> sss = resent_sender (*this);
        list<string_view> ttt = resent_to (*this);
        list<string_view> ccc = resent_cc (*this);
        list<string_view> bbb = resent_bcc (*this);

        if (d.data () == nullptr) throw exception {"invalid message"};
        if (f.data () == nullptr) throw exception {"invalid message"};

        if (fff.size () != ddd.size ()) throw exception {"invalid message"};

        if (i.data () == nullptr && iii.size () != 0 || i.data () != nullptr && iii.size () != fff.size ()) throw exception {"invalid message"};
        if (s.data () == nullptr && sss.size () != 0 || s.data () != nullptr && sss.size () != fff.size ()) throw exception {"invalid message"};
        if (t.data () == nullptr && ttt.size () != 0 || t.data () != nullptr && ttt.size () != fff.size ()) throw exception {"invalid message"};
        if (c.data () == nullptr && ccc.size () != 0 || c.data () != nullptr && ccc.size () != fff.size ()) throw exception {"invalid message"};
        if (b.data () == nullptr && bbb.size () != 0 || b.data () != nullptr && bbb.size () != fff.size ()) throw exception {"invalid message"};

        list<sent_info> result;
        while (fff.size () > 0) {
            sent_info si {};
            si.Date = date_time {ddd.first ()};
            ddd = ddd.rest ();
            si.From = address {fff.first ()};
            fff = fff.rest ();

            if (iii.size () > 0) {
                si.MessageID = msg_id {iii.first ()};
                iii = iii.rest ();
            }

            if (sss.size () > 0) {
                si.Sender = address {sss.first ()};
                sss = sss.rest ();
            }

            if (ttt.size () > 0) {
                si.To = for_each ([] (string_view x) -> address {
                    return address {x};
                }, split (ttt.first (), ","));
                ttt = ttt.rest ();
            }

            if (ccc.size () > 0) {
                si.Cc = for_each ([] (string_view x) -> address {
                    return address {x};
                }, split (ccc.first (), ","));
                ccc = ccc.rest ();
            }

            if (bbb.size () > 0) {
                si.Bcc = for_each ([] (string_view x) -> address {
                    return address {x};
                }, split (bbb.first (), ","));
                bbb = bbb.rest ();
            }

            result <<= si;
        }

        sent_info si {};
        si.Date = date_time {d};
        si.From = address {f};

        if (i.data () != nullptr) si.MessageID = msg_id {i};
        if (s.data () != nullptr) si.Sender = address {s};
        if (t.data () != nullptr) si.To = for_each ([] (string_view x) -> address {
                return address {x};
            }, split (t, ","));
        if (c.data () != nullptr) si.Cc = for_each ([] (string_view x) -> address {
                return address {x};
            }, split (c, ","));
        if (b.data () != nullptr) si.Bcc = for_each ([] (string_view x) -> address {
                return address {x};
            }, split (b, ","));

        result <<= si;
        return result;
    }

    maybe<list<address>> message::reply_to () const {
        string_view rt = reply_to (*this);
        if (rt.data () == nullptr) return {};
        return for_each ([] (string_view x) -> address {
            return address {x};
        }, split (rt, ","));
    }

    maybe<list<msg_id>> message::in_reply_to () const {
        string_view irt = in_reply_to (*this);
        if (irt.data () == nullptr) return {};
        return for_each ([] (string_view x) -> msg_id {
            return msg_id {x};
        }, split (irt, ","));
    }

    maybe<list<msg_id>> message::references () const {
        string_view r = references (*this);
        if (r.data () == nullptr) return {};
        return for_each ([] (string_view x) -> msg_id {
            return msg_id {x};
        }, split (r, ","));
    }

    list<list<ASCII>> message::keywords () const {
        list<string_view> kkk = keywords (*this);
        if (kkk.size () == 0) return {};
        return for_each ([] (string_view k) -> list<ASCII> {
            return for_each ([] (string_view x) -> ASCII {
                return ASCII {x};
            }, split (k, ","));
        }, kkk);
    }

    message::message (const write &w) {
        incomplete_message i {};
        i = i.attach (email::date {w.Sent.Date});
        i = i.attach (email::from {w.Sent.From});
        if (bool (w.Sent.MessageID)) i = i.attach (email::message_id (*w.Sent.MessageID));
        if (bool (w.Sent.Sender)) i = i.attach (email::sender (*w.Sent.Sender));
        if (bool (w.Sent.To)) i = i.attach (email::to (*w.Sent.To));
        if (bool (w.Sent.Cc)) i = i.attach (email::cc (*w.Sent.Cc));
        if (bool (w.Sent.Bcc)) i = i.attach (email::bcc (*w.Sent.Bcc));
        if (bool (w.Subject)) i = i.attach (email::subject (*w.Subject));
        for (auto &x : w.Comments) i = i.attach (email::comments {x});
        for (auto &x : w.Keywords) i = i.attach (email::keywords {x});
        if (bool (w.ReplyTo)) i = i.attach (email::reply_to (*w.ReplyTo));
        if (bool (w.InReplyTo)) i = i.attach (email::in_reply_to (*w.InReplyTo));
        if (bool (w.References)) i = i.attach (email::references (*w.References));
        for (auto &h : w.Optional) i = i.attach (h);
        if (bool (w.Body)) i = i.attach (*w.Body);
        *this = message (i);
    }

}

#include <tao/pegtl.hpp>

namespace pegtl {

    using namespace tao::pegtl;

    struct US_ASCII : range<1, 127> {};

    struct CLRF : seq<one<13>, one<10>> {};

    // printable ASCII characters
    struct VCHAR : range<0x21, 0x7e> {};

    // whitespace
    struct WSP : sor<one<32>, one<9>> {};

    struct quoted_pair : seq<one<'\\'>, sor<VCHAR, WSP>> {};

    // folded whitespace
    struct FWS : seq<opt<star<WSP>, CLRF>, plus<WSP>> {};

    // printable ascii characters other than '(', ')', and '\'.
    struct ctext : sor<range<33, 39>, range<42, 91>, range<93, 126>> {};

    struct ccontent;

    struct comment : seq<one<'('>, star<seq<opt<FWS>, ccontent>>, opt<FWS>, one<')'>> {};

    struct ccontent : sor<ctext, quoted_pair, comment> {};

    // commented folded whitespace
    struct CFWS : sor<seq<plus<opt<FWS>, comment>, opt<FWS>>, FWS> {};

    struct atext : sor<alnum, one<'!'>, one<'#'>, one<'$'>, one<'%'>, one<'&'>, one<'\''>, one<'*'>, one<'+'>,
        one<'-'>, one<'/'>, one<'='>, one<'?'>, one<'^'>, one<'_'>, one<'`'>, one<'{'>, one<'|'>, one<'}'>, one<'~'>> {};

    struct atom : seq<opt<CFWS>, plus<atext>, opt<CFWS>> {};

    struct dot_atext : seq<plus<atext>, star<seq<one<'.'>, plus<atext>>>> {};

    struct dot_atom : seq<opt<CFWS>, dot_atext, opt<CFWS>> {};

    // printable ascii characters other than '"', and '\'.
    struct qtext : sor<one<33>, range<35, 91>, range<93, 126>> {};

    struct qcontent : sor<qtext, quoted_pair> {};

    struct quoted_string : seq<opt<CFWS>, one<'"'>, star<opt<FWS>, qcontent>, opt<FWS>, one<'"'>, opt<CFWS>> {};

    struct word : sor<atom, quoted_string> {};

    struct phrase : plus<word> {};

    struct unstructured : seq<star<seq<FWS, VCHAR>>, star<WSP>> {};

    struct local_part : sor<quoted_string, dot_atom> {};

    // printable characters other than '[', ']', or '\'
    struct dtext : sor<range<33, 90>, range<94, 126>> {};

    struct ip_address : seq<star<opt<FWS>, dtext>, opt<FWS>> {};

    struct domain_literal : seq<opt<CFWS>, one<'['>, ip_address, one<']'>, opt<CFWS>> {};

    struct domain_name : dot_atom {};

    struct domain : sor<domain_name, domain_literal> {};

    struct addr_spec : seq<local_part, one<'@'>, domain> {};

    struct addr_spec_whole : seq<bof, addr_spec, eof> {};

    struct angle_addr : seq<opt<CFWS>, one<'<'>, addr_spec, one<'>'>, opt<CFWS>> {};

    struct display_name : phrase {};

    struct name_addr : seq<opt<display_name>, angle_addr> {};

    struct mailbox : sor<name_addr, addr_spec> {};

    struct mailbox_whole : seq<bof, mailbox, eof> {};

    struct mailbox_list : seq<mailbox, star<one<','>, mailbox>> {};

    struct group_list : sor<mailbox_list, CFWS> {};

    struct group : seq<display_name, one<':'>, opt<group_list>, one<';'>, opt<CFWS>> {};

    struct address : sor<mailbox, group> {};

    struct address_whole : seq<bof, address, eof> {};

    struct address_list : seq<address, star<one<','>, address>> {};

    struct hour : seq<digit, digit> {};

    struct minute : seq<digit, digit> {};

    struct second : seq<digit, digit> {};

    struct time_of_day : seq<hour, one<':'>, minute, opt<seq<one<':'>, second>>> {};

    struct zone : seq<FWS, sor<one<'+'>, one<'-'>>, seq<digit, digit, digit, digit>> {};

    struct time : seq<time_of_day, zone> {};

    struct day : seq<opt<FWS>, plus<seq<digit, digit>>, FWS> {};

    struct month : sor<string<'J', 'a', 'n'>, string<'F', 'e', 'b'>, string<'M', 'a', 'r'>, string<'A', 'p', 'r'>,
        string<'M', 'a', 'y'>, string<'J', 'u', 'n'>, string<'J', 'u', 'l'>, string<'A', 'u', 'g'>,
        string<'S', 'e', 'p'>, string<'O', 'c', 't'>, string<'N', 'o', 'v'>, string<'D', 'e', 'c'>> {};

    struct year : seq<FWS, digit, digit, digit, digit, FWS> {};

    struct date : seq<day, month, year> {};

    struct day_name : sor<string<'M', 'o', 'n'>, string<'T', 'u', 'e'>, string<'W', 'e', 'd'>,
        string<'T', 'h', 'u'>, string<'F', 'r', 'i'>, string<'S', 'a', 't'>, string<'S', 'u', 'n'>> {};

    struct day_of_week : seq<FWS, day_name> {};

    struct date_time : seq<opt<day_of_week, one<','>>, date, time, CFWS> {};

    struct date_time_whole : seq<bof, date_time, eof> {};

    struct header_orig_date : seq<string<'D', 'a', 't', 'e', ':'>, date_time> {};

    //updated in rfc6854
    // struct from : seq<string<'F', 'r', 'o', 'm', ':'>, mailbox_list> {};
    struct header_from : seq<string<'F', 'r', 'o', 'm', ':'>, sor<mailbox_list, address_list>> {};

    // struct sender : seq<string<'S', 'e', 'n', 'd', 'e', 'r', ':'>, mailbox> {};
    struct header_sender : seq<string<'S', 'e', 'n', 'd', 'e', 'r', ':'>, address> {};

    struct header_reply_to : seq<string<'R', 'e', 'p', 'l', 'y', '-', 'T', 'o', ':'>, address_list> {};

    struct header_to : seq<string<'T', 'o', ':'>, address_list> {};

    struct header_cc : seq<string<'C', 'c', ':'>, address_list> {};

    struct header_bcc : seq<string<'B', 'c', 'c', ':'>, sor<address_list, CFWS>> {};

    struct no_fold_literal : seq<one<'['>, dtext, one<']'>> {};

    struct msg_id : seq<CFWS, one<'<'>, dot_atext, one<'@'>, sor<dot_atext, no_fold_literal>, one<'>'>, CFWS> {};

    struct msg_id_whole : seq<bof, msg_id, eof> {};

    struct header_message_id : seq<string<'M', 'e', 's', 's', 'a', 'g', 'e', '-', 'I', 'D', ':'>, msg_id> {};

    struct header_in_reply_to : seq<string<'I', 'n', '-', 'R', 'e', 'p', 'l', 'y', '-', 'T', 'o', ':'>, plus<msg_id>> {};

    struct header_references : seq<string<'R', 'e', 'f', 'e', 'r', 'e', 'n', 'c', 'e', 's', ':'>, plus<msg_id>> {};

    struct header_subject : seq<string<'S', 'u', 'b', 'j', 'e', 'c', 't', ':'>, unstructured> {};

    struct header_comments : seq<string<'C', 'o', 'm', 'm', 'e', 'n', 't', 's', ':'>, unstructured> {};

    struct header_keywords : seq<string<'K', 'e', 'y', 'w', 'o', 'r', 'd', 's', ':'>, seq<phrase, opt<one<','>, phrase>>> {};

    struct header_resent_date : seq<string<'R', 'e', 's', 'e', 'n', 't', '-', 'D', 'a', 't', 'e', ':'>, date_time> {};

    struct header_resent_from : seq<string<'R', 'e', 's', 'e', 'n', 't', '-', 'F', 'r', 'o', 'm', ':'>, address_list> {};

    struct header_resent_sender : seq<string<'R', 'e', 's', 'e', 'n', 't', '-', 'S', 'e', 'n', 'd', 'e', 'r', ':'>, address> {};

    struct header_resent_to : seq<string<'R', 'e', 's', 'e', 'n', 't', '-', 'T', 'o', ':'>, address_list> {};

    struct header_resent_cc : seq<string<'R', 'e', 's', 'e', 'n', 't', '-', 'C', 'c', ':'>, address_list> {};

    struct header_resent_bcc : seq<string<'R', 'e', 's', 'e', 'n', 't', '-', 'B', 'c', 'c', ':'>, sor<address_list, CFWS>> {};

    struct header_resent_message_id :
        seq<string<'R', 'e', 's', 'e', 'n', 't', '-', 'M', 'e', 's', 's', 'a', 'g', 'e', '-', 'I', 'D', ':'>, msg_id> {};

    struct path : sor<angle_addr, seq<CFWS, one<'<'>, CFWS, one<'>'>, CFWS>> {};

    struct header_return_path : seq<string<'R', 'e', 't', 'u', 'r', 'n', '-', 'P', 'a', 't', 'h', ':'>, path> {};

    struct received_token : sor<word, angle_addr, addr_spec, domain> {};

    struct header_received : seq<string<'R', 'e', 'c', 'e', 'i', 'v', 'e', 'd', ':'>, star<received_token>, one<';'>, date_time> {};

    struct trace : seq<opt<header_return_path>, plus<header_received>> {};

    // printable ascii characters other than ':'.
    struct ftext : sor<range<33, 57>, range<5, 126>> {};

    struct field_name : plus<ftext> {};

    struct field_name_whole : seq<bof, field_name, eof> {};

    struct field : seq<field_name, one<':'>, unstructured> {};

    struct fields : plus<seq<field, CLRF>> {};

    struct optional_field : minus<field,
        sor<header_from, header_orig_date, header_subject, header_sender, header_reply_to, header_to, header_cc, header_bcc,
            header_message_id, header_in_reply_to, header_references, header_comments, header_keywords, header_resent_date,
            header_resent_from, header_resent_sender, header_resent_to, header_resent_cc, header_resent_bcc,
            header_resent_message_id, header_return_path, header_received>> {};

    struct header : sor<header_from, header_orig_date, header_subject, header_sender, header_reply_to, header_to, header_cc, header_bcc,
            header_message_id, header_in_reply_to, header_references, header_comments, header_keywords, header_resent_date,
            header_resent_from, header_resent_sender, header_resent_to, header_resent_cc, header_resent_bcc,
            header_resent_message_id, header_return_path, header_received, optional_field> {};

    struct header_whole : seq<bof, header, eof> {};

    struct resent_headers : sor<header_resent_date, header_resent_from, header_resent_sender, header_resent_to,
        header_resent_cc, header_resent_bcc, header_resent_message_id> {};

    struct headers : seq<star<trace, star<resent_headers>, star<optional_field>>,
        star<sor<header_subject, header_from, header_orig_date, header_sender, header_reply_to, header_to, header_cc, header_bcc,
            header_message_id, header_in_reply_to, header_references, header_comments, header_keywords, optional_field>>> {};

    // an email message is divided into lines by CLRF
    // in the body, CL and RF may not appear on their own in the body.
    // this naturally also happens in header lines.
    // These lines must not be more than 998 characters long.
    struct line : star<minus<US_ASCII, sor<one<13>, one<10>>>> {};

    struct body : star<seq<line, CLRF>, line> {};

    struct message : seq<headers, CLRF, opt<body>> {};

    struct message_whole : seq<bof, message, eof> {};

    // TODO
    struct message_obsolete : message {};

    struct message_obsolete_whole : seq<bof, message_obsolete, eof> {};

}

namespace data::net::email {

    bool address::valid (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_address");
        return tao::pegtl::parse<pegtl::address_whole> (in);
    }

    namespace {

        template <typename Rule> struct read_display_name : pegtl::nothing<Rule> {};

        template <> struct read_display_name<pegtl::display_name> {
            template <typename Input>
            static void apply (const Input &in, string_view &name) {
                name = string_view {&*in.begin (), static_cast<size_t> (in.end () - in.begin ())};
            }
        };

        template <typename Rule> struct read_local_part : pegtl::nothing<Rule> {};

        template <> struct read_local_part<pegtl::local_part> {
            template <typename Input>
            static void apply (const Input &in, string_view &lp) {
                lp = string_view {&*in.begin (), static_cast<size_t> (in.end () - in.begin ())};
            }
        };

        template <typename Rule> struct read_domain : pegtl::nothing<Rule> {};

        template <> struct read_domain<pegtl::domain> {
            template <typename Input>
            static void apply (const Input &in, string_view &d) {
                d = string_view {&*in.begin (), static_cast<size_t> (in.end () - in.begin ())};
            }
        };

        template <typename Rule> struct read_domain_name : pegtl::nothing<Rule> {};

        template <> struct read_domain_name<pegtl::domain_name> {
            template <typename Input>
            static void apply (const Input &in, string_view &d) {
                d = string_view {&*in.begin (), static_cast<size_t> (in.end () - in.begin ())};
            }
        };

        template <typename Rule> struct read_ip_address : pegtl::nothing<Rule> {};

        template <> struct read_ip_address<pegtl::ip_address> {
            template <typename Input>
            static void apply (const Input &in, string_view &ip) {
                ip = string_view {&*in.begin (), static_cast<size_t> (in.end () - in.begin ())};
            }
        };

        template <typename Rule> struct read_addr_spec : pegtl::nothing<Rule> {};

        template <> struct read_addr_spec<pegtl::addr_spec> {
            template <typename Input>
            static void apply (const Input &in, string_view &ad) {
                ad = string_view {&*in.begin (), static_cast<size_t> (in.end () - in.begin ())};
            }
        };
    }

    string_view address::display_name (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        string_view name {};
        tao::pegtl::parse<pegtl::mailbox_whole, read_display_name> (in, name);
        return name;
    }

    string_view address::local_part (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        string_view lp {};
        tao::pegtl::parse<pegtl::mailbox_whole, read_local_part> (in, lp);
        return lp;
    }

    string_view address::domain (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        string_view d {};
        tao::pegtl::parse<pegtl::mailbox_whole, read_domain> (in, d);
        return d;
    }

    string_view address::domain_name (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        string_view d {};
        tao::pegtl::parse<pegtl::mailbox_whole, read_domain_name> (in, d);
        return d;
    }

    string_view address::ip_address (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        string_view ip {};
        tao::pegtl::parse<pegtl::mailbox_whole, read_ip_address> (in, ip);
        return ip;
    }

    string_view address::addr_spec (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        string_view ad {};
        tao::pegtl::parse<pegtl::mailbox_whole, read_domain> (in, ad);
        return ad;
    }

    bool mailbox::valid (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_address");
        return tao::pegtl::parse<pegtl::mailbox_whole> (in);
    }

    bool field::valid (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_field");
        return tao::pegtl::parse<pegtl::field_name_whole> (in);
    }

    bool header::valid (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_header");
        return tao::pegtl::parse<pegtl::header_whole> (in);
    }

    bool date_time::valid (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_date");
        return tao::pegtl::parse<pegtl::date_time_whole> (in);
    }

    bool msg_id::valid (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_id");
        return tao::pegtl::parse<pegtl::msg_id_whole> (in);
    }

    namespace {

        struct header_counter {
            uint32 orig_date {0};
            uint32 message_id {0};
            uint32 from {0};
            uint32 sender {0};
            uint32 reply_to {0};
            uint32 to {0};
            uint32 cc {0};
            uint32 bcc {0};
            uint32 in_reply_to {0};
            uint32 references {0};
            uint32 subject {0};
            uint32 resent_date {0};
            uint32 resent_from {0};
            uint32 resent_sender {0};
            uint32 resent_to {0};
            uint32 resent_cc {0};
            uint32 resent_bcc {0};
            uint32 resent_message_id {0};
            uint32 trace {0};
        };

        template <typename Rule> struct count_headers : pegtl::nothing<Rule> {};

        template <> struct count_headers<pegtl::header_from> {
            template <typename Input>
            static void apply (const Input &in, header_counter &h) {
                h.from++;
            }
        };

        template <> struct count_headers<pegtl::header_orig_date> {
            template <typename Input>
            static void apply (const Input &in, header_counter &h) {
                h.orig_date++;
            }
        };

        template <> struct count_headers<pegtl::header_message_id> {
            template <typename Input>
            static void apply (const Input &in, header_counter &h) {
                h.message_id++;
            }
        };

        template <> struct count_headers<pegtl::header_sender> {
            template <typename Input>
            static void apply (const Input &in, header_counter &h) {
                h.sender++;
            }
        };

        template <> struct count_headers<pegtl::header_reply_to> {
            template <typename Input>
            static void apply (const Input &in, header_counter &h) {
                h.reply_to++;
            }
        };

        template <> struct count_headers<pegtl::header_to> {
            template <typename Input>
            static void apply (const Input &in, header_counter &h) {
                h.to++;
            }
        };

        template <> struct count_headers<pegtl::header_cc> {
            template <typename Input>
            static void apply (const Input &in, header_counter &h) {
                h.cc++;
            }
        };

        template <> struct count_headers<pegtl::header_bcc> {
            template <typename Input>
            static void apply (const Input &in, header_counter &h) {
                h.bcc++;
            }
        };

        template <> struct count_headers<pegtl::header_in_reply_to> {
            template <typename Input>
            static void apply (const Input &in, header_counter &h) {
                h.in_reply_to++;
            }
        };

        template <> struct count_headers<pegtl::header_references> {
            template <typename Input>
            static void apply (const Input &in, header_counter &h) {
                h.references++;
            }
        };

        template <> struct count_headers<pegtl::header_subject> {
            template <typename Input>
            static void apply (const Input &in, header_counter &h) {
                h.subject++;
            }
        };

        template <> struct count_headers<pegtl::header_resent_date> {
            template <typename Input>
            static void apply (const Input &in, header_counter &h) {
                h.resent_date++;
            }
        };

        template <> struct count_headers<pegtl::header_resent_from> {
            template <typename Input>
            static void apply (const Input &in, header_counter &h) {
                h.resent_from++;
            }
        };

        template <> struct count_headers<pegtl::header_resent_sender> {
            template <typename Input>
            static void apply (const Input &in, header_counter &h) {
                h.resent_sender++;
            }
        };

        template <> struct count_headers<pegtl::header_resent_to> {
            template <typename Input>
            static void apply (const Input &in, header_counter &h) {
                h.resent_to++;
            }
        };

        template <> struct count_headers<pegtl::header_resent_cc> {
            template <typename Input>
            static void apply (const Input &in, header_counter &h) {
                h.resent_cc++;
            }
        };

        template <> struct count_headers<pegtl::header_resent_bcc> {
            template <typename Input>
            static void apply (const Input &in, header_counter &h) {
                h.resent_bcc++;
            }
        };

        template <> struct count_headers<pegtl::header_resent_message_id> {
            template <typename Input>
            static void apply (const Input &in, header_counter &h) {
                h.resent_message_id++;
            }
        };

        template <> struct count_headers<pegtl::trace> {
            template <typename Input>
            static void apply (const Input &in, header_counter &h) {
                h.trace++;
            }
        };

        struct line_is_too_big {};

        template <typename Rule> struct check_line_limit : pegtl::nothing<Rule> {};

        template <> struct check_line_limit<pegtl::line> {
            template <typename Input>
            static void apply (const Input &in, size_t max_size) {
                if (static_cast<size_t> (in.end () - in.begin ()) > max_size) throw line_is_too_big {};
            }
        };

        template <typename Rule> struct find_excessive_lines : pegtl::nothing<Rule> {};

        template <> struct find_excessive_lines<pegtl::line> {
            template <typename Input>
            static void apply (const Input &in, size_t max_size, list<string_view> &x) {
                if (static_cast<size_t> (in.end () - in.begin ()) > max_size)
                    x <<= string_view {&*in.begin (), static_cast<size_t> (in.end () - in.begin ())};
            }
        };
    }

    bool lines_lengths_exceed (string_view x, size_t size) {
        tao::pegtl::memory_input<> in (x, "email_message");
        try {
            tao::pegtl::parse<pegtl::line, check_line_limit> (in, size);
        } catch (line_is_too_big) {
            return true;
        }

        return false;
    }

    bool message::valid (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        header_counter c {};
        return tao::pegtl::parse<pegtl::message_whole, count_headers> (in, c) &&
            // check that every line is not greater than 998 characters.
            !lines_lengths_exceed (x, 998) &&
            // there are also rules about how often certain headers may appear.
            // there must be one orig_date, and one from field.
            c.orig_date == 1 && c.from == 1 && c.message_id == 1 &&
            c.sender <= 1 && c.reply_to <= 1 && c.to <= 1 && c.cc <= 1 && c.bcc <= 1 &&
            c.in_reply_to <= 1 && c.references <= 1 && c.subject <= 1 &&
            // there must be one of to, cc, or bcc.
            (c.to == 1 || c.cc == 1 || c.bcc == 1) &&
            // there must be a sender field with multi-addresses.
            mailbox {from (x)}.valid () == (c.sender == 0) ||
            // resent fields must be present in a block preceeding the other fields.
            c.resent_from == c.resent_date &&
            c.resent_sender == c.resent_from * c.sender &&
            c.resent_to == c.resent_to * c.to &&
            c.resent_cc == c.resent_cc * c.cc &&
            c.resent_bcc == c.resent_bcc * c.bcc &&
            c.resent_message_id == c.resent_message_id * c.message_id;
    }

    bool message::standard (string_view x) {
        return valid (x) && !lines_lengths_exceed (x, 78) && bool (message_id (x).data () != nullptr);
    }

    struct insert {
        string This;
        size_t At;

        bool valid () const {
            return This != "";
        }
    };

    insert insert_CLRF_before_whitespace (string_view line, uint32 limit) {
        for (int i = limit; i >= 0; i--) if (line[i] == 32 || line[i] == 9) return insert {string {CRLF}, static_cast<size_t> (i)};
        return insert {"", 0};
    }

    // break lines up so that none is longer than the given limit (not including CRLFs)
    // this may require inserting comments containing line breaks and whitespace.
    // string is only returned if the limit could be satisfied.
    maybe<message> inline break_lines (const message &m, uint32 limit) {
        tao::pegtl::memory_input<> in (m, "email_message");
        list<string_view> big_lines;
        tao::pegtl::parse<pegtl::message_whole, find_excessive_lines> (in, limit, big_lines);
        if (data::empty (big_lines)) return {m};

        // let's look at the lines.
        auto lines = message::lines (m);

        list<insert> insertions;

        size_t begin_pos = 0;
        for (string_view line : lines) {
            if (line.size () > limit) {
                if (insert i = insert_CLRF_before_whitespace (line, limit); i.valid ()) {
                    i.At += begin_pos;
                    insertions <<= i;
                    // TODO it is possible that we could try other things to insert a line break.
                    // for example, there may be places where we could insert a comment that
                    // contains whitespace, which means it can have a line break.
                } else return {};
            }
            begin_pos += line.size () + 2;
        }

        begin_pos = 0;
        std::stringstream ss;
        for (const insert &i : insertions) {
            ss << string_view {m.data () + begin_pos, i.At};
            ss << CRLF;
            begin_pos += i.At;
        }

        ss << string_view {m.data () + begin_pos, m.size () - begin_pos};
        return {message {ss.str ()}};
    }

    namespace {
        template <typename Rule> struct read_header_from : pegtl::nothing<Rule> {};

        template <> struct read_header_from<pegtl::header_from> {
            template <typename Input>
            static void apply (const Input &in, string_view &name) {
                name = string_view {&*in.begin () + 5, static_cast<size_t> (in.end () - in.begin () - 5)};
            }
        };

        template <typename Rule> struct read_header_date : pegtl::nothing<Rule> {};

        template <> struct read_header_date<pegtl::header_orig_date> {
            template <typename Input>
            static void apply (const Input &in, string_view &name) {
                name = string_view {&*in.begin () + 5, static_cast<size_t> (in.end () - in.begin () - 5)};
            }
        };

        template <typename Rule> struct read_header_message_id : pegtl::nothing<Rule> {};

        template <> struct read_header_message_id<pegtl::header_message_id> {
            template <typename Input>
            static void apply (const Input &in, string_view &name) {
                name = string_view {&*in.begin () + 11, static_cast<size_t> (in.end () - in.begin () - 11)};
            }
        };

        template <typename Rule> struct read_header_subject : pegtl::nothing<Rule> {};

        template <> struct read_header_subject<pegtl::header_subject> {
            template <typename Input>
            static void apply (const Input &in, string_view &name) {
                name = string_view {&*in.begin () + 8, static_cast<size_t> (in.end () - in.begin () - 8)};
            }
        };

        template <typename Rule> struct read_header_sender : pegtl::nothing<Rule> {};

        template <> struct read_header_sender<pegtl::header_sender> {
            template <typename Input>
            static void apply (const Input &in, string_view &name) {
                name = string_view {&*in.begin () + 7, static_cast<size_t> (in.end () - in.begin () - 7)};
            }
        };

        template <typename Rule> struct read_header_in_reply_to : pegtl::nothing<Rule> {};

        template <> struct read_header_in_reply_to<pegtl::header_in_reply_to> {
            template <typename Input>
            static void apply (const Input &in, string_view &name) {
                name = string_view {&*in.begin () + 12, static_cast<size_t> (in.end () - in.begin () - 12)};
            }
        };

        template <typename Rule> struct read_header_reply_to : pegtl::nothing<Rule> {};

        template <> struct read_header_reply_to<pegtl::header_reply_to> {
            template <typename Input>
            static void apply (const Input &in, string_view &name) {
                name = string_view {&*in.begin () + 9, static_cast<size_t> (in.end () - in.begin () - 9)};
            }
        };

        template <typename Rule> struct read_header_to : pegtl::nothing<Rule> {};

        template <> struct read_header_to<pegtl::header_to> {
            template <typename Input>
            static void apply (const Input &in, string_view &name) {
                name = string_view {&*in.begin () + 3, static_cast<size_t> (in.end () - in.begin () - 3)};
            }
        };

        template <typename Rule> struct read_header_cc : pegtl::nothing<Rule> {};

        template <> struct read_header_cc<pegtl::header_cc> {
            template <typename Input>
            static void apply (const Input &in, string_view &name) {
                name = string_view {&*in.begin () + 3, static_cast<size_t> (in.end () - in.begin () - 3)};
            }
        };

        template <typename Rule> struct read_header_bcc : pegtl::nothing<Rule> {};

        template <> struct read_header_bcc<pegtl::header_bcc> {
            template <typename Input>
            static void apply (const Input &in, string_view &name) {
                name = string_view {&*in.begin () + 4, static_cast<size_t> (in.end () - in.begin () - 4)};
            }
        };

        template <typename Rule> struct read_header_references : pegtl::nothing<Rule> {};

        template <> struct read_header_references<pegtl::header_references> {
            template <typename Input>
            static void apply (const Input &in, string_view &name) {
                name = string_view {&*in.begin () + 11, static_cast<size_t> (in.end () - in.begin () - 11)};
            }
        };

        template <typename Rule> struct read_header_comments : pegtl::nothing<Rule> {};

        template <> struct read_header_comments<pegtl::header_comments> {
            template <typename Input>
            static void apply (const Input &in, list<string_view> &xx) {
                xx <<= string_view {&*in.begin () + 9, static_cast<size_t> (in.end () - in.begin () - 9)};
            }
        };

        template <typename Rule> struct read_header_keywords : pegtl::nothing<Rule> {};

        template <> struct read_header_keywords<pegtl::header_keywords> {
            template <typename Input>
            static void apply (const Input &in, list<string_view> &xx) {
                xx <<= string_view {&*in.begin () + 9, static_cast<size_t> (in.end () - in.begin () - 9)};
            }
        };

        template <typename Rule> struct read_header_return_path : pegtl::nothing<Rule> {};

        template <> struct read_header_return_path<pegtl::header_return_path> {
            template <typename Input>
            static void apply (const Input &in, list<string_view> &xx) {
                xx <<= string_view {&*in.begin () + 12, static_cast<size_t> (in.end () - in.begin () - 12)};
            }
        };

        template <typename Rule> struct read_header_received : pegtl::nothing<Rule> {};

        template <> struct read_header_received<pegtl::header_received> {
            template <typename Input>
            static void apply (const Input &in, list<string_view> &xx) {
                xx <<= string_view {&*in.begin () + 9, static_cast<size_t> (in.end () - in.begin () - 9)};
            }
        };
        template <typename Rule> struct read_header_resent_from : pegtl::nothing<Rule> {};

        template <> struct read_header_resent_from<pegtl::header_resent_from> {
            template <typename Input>
            static void apply (const Input &in, list<string_view> &xx) {
                xx <<= string_view {&*in.begin () + 9, static_cast<size_t> (in.end () - in.begin () - 9)};
            }
        };

        template <typename Rule> struct read_header_resent_date : pegtl::nothing<Rule> {};

        template <> struct read_header_resent_date<pegtl::header_resent_date> {
            template <typename Input>
            static void apply (const Input &in, list<string_view> &xx) {
                xx <<= string_view {&*in.begin () + 9, static_cast<size_t> (in.end () - in.begin () - 9)};
            }
        };

        template <typename Rule> struct read_header_resent_message_id : pegtl::nothing<Rule> {};

        template <> struct read_header_resent_message_id<pegtl::header_resent_message_id> {
            template <typename Input>
            static void apply (const Input &in, list<string_view> &xx) {
                xx <<= string_view {&*in.begin () + 9, static_cast<size_t> (in.end () - in.begin () - 9)};
            }
        };

        template <typename Rule> struct read_header_resent_sender : pegtl::nothing<Rule> {};

        template <> struct read_header_resent_sender<pegtl::header_resent_sender> {
            template <typename Input>
            static void apply (const Input &in, list<string_view> &xx) {
                xx <<= string_view {&*in.begin () + 9, static_cast<size_t> (in.end () - in.begin () - 9)};
            }
        };

        template <typename Rule> struct read_header_resent_to : pegtl::nothing<Rule> {};

        template <> struct read_header_resent_to<pegtl::header_resent_to> {
            template <typename Input>
            static void apply (const Input &in, list<string_view> &xx) {
                xx <<= string_view {&*in.begin () + 9, static_cast<size_t> (in.end () - in.begin () - 9)};
            }
        };

        template <typename Rule> struct read_header_resent_cc : pegtl::nothing<Rule> {};

        template <> struct read_header_resent_cc<pegtl::header_resent_cc> {
            template <typename Input>
            static void apply (const Input &in, list<string_view> &xx) {
                xx <<= string_view {&*in.begin () + 9, static_cast<size_t> (in.end () - in.begin () - 9)};
            }
        };

        template <typename Rule> struct read_header_resent_bcc : pegtl::nothing<Rule> {};

        template <> struct read_header_resent_bcc<pegtl::header_resent_bcc> {
            template <typename Input>
            static void apply (const Input &in, list<string_view> &xx) {
                xx <<= string_view {&*in.begin () + 9, static_cast<size_t> (in.end () - in.begin () - 9)};
            }
        };

    }

    string_view message::from (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        string_view value {};
        tao::pegtl::parse<pegtl::message_whole, read_header_from> (in, value);
        return value;
    }

    string_view message::date (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        string_view value {};
        tao::pegtl::parse<pegtl::message_whole, read_header_date> (in, value);
        return value;
    }

    string_view message::message_id (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        string_view value {};
        tao::pegtl::parse<pegtl::message_whole, read_header_message_id> (in, value);
        return value;
    }

    string_view message::subject (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        string_view value {};
        tao::pegtl::parse<pegtl::message_whole, read_header_subject> (in, value);
        return value;
    }

    string_view message::sender (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        string_view value {};
        tao::pegtl::parse<pegtl::message_whole, read_header_sender> (in, value);
        return value;
    }

    string_view message::reply_to (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        string_view value {};
        tao::pegtl::parse<pegtl::message_whole, read_header_reply_to> (in, value);
        return value;
    }

    string_view message::to (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        string_view value {};
        tao::pegtl::parse<pegtl::message_whole, read_header_to> (in, value);
        return value;
    }

    string_view message::cc (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        string_view value {};
        tao::pegtl::parse<pegtl::message_whole, read_header_cc> (in, value);
        return value;
    }

    string_view message::bcc (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        string_view value {};
        tao::pegtl::parse<pegtl::message_whole, read_header_bcc> (in, value);
        return value;
    }

    string_view message::in_reply_to (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        string_view value {};
        tao::pegtl::parse<pegtl::message_whole, read_header_in_reply_to> (in, value);
        return value;
    }

    string_view message::references (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        string_view value {};
        tao::pegtl::parse<pegtl::message_whole, read_header_references> (in, value);
        return value;
    }

    list<string_view> message::comments (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        list<string_view> value {};
        tao::pegtl::parse<pegtl::message_whole, read_header_comments> (in, value);
        return value;
    }

    list<string_view> message::keywords (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        list<string_view> value {};
        tao::pegtl::parse<pegtl::message_whole, read_header_keywords> (in, value);
        return value;
    }

    list<string_view> message::return_path (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        list<string_view> value {};
        tao::pegtl::parse<pegtl::message_whole, read_header_return_path> (in, value);
        return value;
    }

    list<string_view> message::received (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        list<string_view> value {};
        tao::pegtl::parse<pegtl::message_whole, read_header_received> (in, value);
        return value;
    }

    list<string_view> message::resent_date (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        list<string_view> value {};
        tao::pegtl::parse<pegtl::message_whole, read_header_resent_date> (in, value);
        return value;
    }

    list<string_view> message::resent_from (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        list<string_view> value {};
        tao::pegtl::parse<pegtl::message_whole, read_header_resent_from> (in, value);
        return value;
    }

    list<string_view> message::resent_message_id (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        list<string_view> value {};
        tao::pegtl::parse<pegtl::message_whole, read_header_resent_message_id> (in, value);
        return value;
    }

    list<string_view> message::resent_sender (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        list<string_view> value {};
        tao::pegtl::parse<pegtl::message_whole, read_header_resent_sender> (in, value);
        return value;
    }

    list<string_view> message::resent_to (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        list<string_view> value {};
        tao::pegtl::parse<pegtl::message_whole, read_header_resent_to> (in, value);
        return value;
    }

    list<string_view> message::resent_cc (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        list<string_view> value {};
        tao::pegtl::parse<pegtl::message_whole, read_header_resent_cc> (in, value);
        return value;
    }

    list<string_view> message::resent_bcc (string_view x) {
        tao::pegtl::memory_input<> in (x, "email_message");
        list<string_view> value {};
        tao::pegtl::parse<pegtl::message_whole, read_header_resent_bcc> (in, value);
        return value;
    }

}
