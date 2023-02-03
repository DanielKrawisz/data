// Copyright (c) 2023 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_NET_SERIALIZED
#define DATA_NET_SERIALIZED

#include <data/net/session.hpp>
#include <data/parser.hpp>
#include <data/function.hpp>

namespace data::net {

    template <typename word, typename message_out, typename message_in = message_out>
    void open_serialized_session (
        open<in<std::basic_string_view<word>>, out<const std::basic_string<word> &>> Open,
        function<std::basic_string<word> (message_out)> Generator,
        parser<word, message_in> Parser,
        handler<parse_error> ParseErrorHandler,
        interaction<in<message_in>, out<message_out>> Interaction) {

        struct out final : net::out<message_out> {

            void send (message_out o) final override {
                Out->send (Generator (o));
            };

            bool closed () final override {
                return Out->closed ();
            };

            void close () final override {
                return Out->close ();
            };

            out (function<std::basic_string<word> (const message_out &)> g) : Generator {g}, Out {} {}

            // write as a string.
            function<std::basic_string<word> (const message_out &)> Generator;

            // the low-level session.
            ptr<net::out<const std::basic_string<word> &>> Out;

        };

        // the high level out session. It does not know how to send messages yet.
        ptr<out> high_level = std::make_shared<out> (Generator);

        // Bind the parse error handler to the parser. Now we have a parser that just takes one argument
        function<ptr<writer<word>> (handler<const message_out &>)> parser = curry (Parser, ParseErrorHandler);

        struct in : net::in<std::basic_string_view<word>> {
            ptr<net::in<message_in>> In;
            ptr<writer<word>> Parse;

            in (ptr<net::in<message_in>> i, ptr<writer<word>> w): In {i}, Parse {w} {}

            void receive (std::basic_string_view<word> i) final override {
                Parse->write (i.data (), i.size ());
            }

            void close () final override {
                In->close ();
            }
        };

        // Open the low-level connection.
        Open (
            // the receive handler.
            [high_level, parser, Interaction]
            (ptr<net::out<const std::basic_string<word> &>> low_level) -> ptr<net::in<std::basic_string_view<word>>> {
                // add the low level sesion to the high level session.
                high_level->Out = low_level;

                auto In = Interaction (std::static_pointer_cast<net::out<message_out>> (high_level));

                return std::static_pointer_cast<net::in<std::basic_string_view<word>>>
                    (ptr<in> {new in {In, parser ([In] (message_in i) -> void {
                        In->receive (i);
                    })}});
            });
    }

}

#endif


