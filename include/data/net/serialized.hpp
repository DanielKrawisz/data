// Copyright (c) 2023 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_NET_SERIALIZED
#define DATA_NET_SERIALIZED

#include <data/net/session.hpp>
#include <data/parser.hpp>
#include <functional>

namespace data::net {

    template <typename word, typename out> struct serialized_session final : session<out> {

        void send (out o) final override {
            Session->send (Generator (o));
        };

        bool closed () final override {
            return Session->closed ();
        };

        void close () final override {
            return Session->close ();
        };

        serialized_session (function<std::basic_string<word> (const out &)> g) : Generator {g}, Session {} {}

        // write as a string.
        function<std::basic_string<word> (const out &)> Generator;

        // the low-level session.
        ptr<session<const std::basic_string<word> &>> Session;

    };

    template <typename word, typename out, typename in = out>
    void open_serialized_session (
        open<session<const std::basic_string<word> &>, std::basic_string_view<word>> Open,
        function<std::basic_string<word> (const out &)> Generator,
        parser<word, out> Parser,
        handler<parse_error> ParseErrorHandler,
        interaction<in, session<out>> Interaction) {

        // the high level out session. It does not know how to send messages yet.
        ptr<serialized_session<word, out>> high_level = std::make_shared<serialized_session<word, out>> (Generator);

        // Bind the parse error handler to the parser. Now we have a parser that just takes one argument
        function<ptr<writer<word>> (handler<const out &>)> parser = std::bind (Parser, std::placeholders::_1, ParseErrorHandler);

        // Open the low-level connection.
        Open (
            // the receive handler.
            [high_level, parser, Interaction]
            (ptr<session<const std::basic_string<word> &>> low_level) -> handler<std::basic_string_view<word>> {
                // add the low level sesion to the high level session.
                high_level->Session = low_level;

                return [parse = parser (Interaction (std::static_pointer_cast<session<out>> (high_level)))]
                (std::basic_string_view<word> i) -> void {
                    parse->write (i.data (), i.size ());
                };
            });
    }

}

#endif


