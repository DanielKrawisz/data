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

        function<std::basic_string<word> (const out &)> Generator;
        ptr<session<const std::basic_string<word> &>> Session;

    };

    template <typename word, typename out, typename in = out> struct open_serialized_session {
        parser<word, out> Parser;
        function<void (parse_error)> ParseErrorHandler;
        open<session<const std::basic_string<word> &>, std::basic_string_view<word>> Open;
        function<std::basic_string<word> (const out &)> Generator;

        ptr<session<out>> operator () (receive_handler<session<out>, in> receiver) {
            // the high level out session. It does not know how to send messages yet.
            ptr<serialized_session<word, out>> high_level = std::make_shared<serialized_session<word, out>> (Generator);

            ptr<session<const std::basic_string<word> &>> low_level = Open ([
                high_level, receiver,
                parser = std::bind (Parser, std::placeholders::_1, ParseErrorHandler)
            ] (
                ptr<session<const std::basic_string<word> &>> low_level) -> std::function<void (std::basic_string_view<word>
            )> {

                high_level->Session = low_level;

                auto in_handler = receiver (std::static_pointer_cast<session<out>> (high_level));

                auto parse = parser (in_handler);

                return [parse] (std::basic_string_view<word> i) -> void {
                    parse->write (i.data (), i.size ());
                };
            });

            if (low_level == nullptr) return nullptr;

            return std::static_pointer_cast<session<out>> (high_level);
        }
    };

}

#endif


