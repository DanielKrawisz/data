// Copyright (c) 2023 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_NET_SERIALIZED
#define DATA_NET_SERIALIZED

#include <data/net/session.hpp>
#include <data/parser.hpp>
#include <data/function.hpp>

namespace data::net {

    template <typename message_out, typename message_in = message_out, typename word = char>
    open<message_in, message_out> serialized_session (
        open<std::basic_string_view<word>, const std::basic_string<word> &> Open,
        function<std::basic_string<word> (message_out)> Generator,
        function<ptr<writer<word>> (handler<const message_out &>)> Parser) {

        struct out final : session<message_out> {

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
            ptr<net::session<const std::basic_string<word> &>> Out;

        };

        return [Open, Generator, Parser]
            (close_handler on_close, interaction<message_in, message_out> interact) -> void {

                Open (on_close,
                    // the receive handler.
                    [interact, Generator, Parser]
                    (ptr<session<const std::basic_string<word> &>> low_level) -> handler<std::basic_string_view<word>> {

                        // the high level out session. It does not know how to send messages yet.
                        ptr<out> high_level = std::make_shared<out> (Generator);

                        // add the low level sesion to the high level session.
                        high_level->Out = low_level;

                        return [Parse = Parser (
                            [In = interact (std::static_pointer_cast<session<message_out>> (high_level))]
                            (message_in i) -> void {
                                In (i);
                            })] (std::basic_string_view<word> i) -> void {
                                Parse->write (i.data (), i.size ());
                            };

                    });
            };
    }

}

#endif


