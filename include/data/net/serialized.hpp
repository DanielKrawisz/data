// Copyright (c) 2023 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_NET_SERIALIZED
#define DATA_NET_SERIALIZED

#include <data/net/session.hpp>
#include <data/parser.hpp>
#include <data/function.hpp>
#include <iostream>

namespace data::net {

    // an implementation of session that takes a message type that can be serialized into a string.
    template <typename message_out, typename word = char>
    struct serialized_session final : session<message_out> {

        void send (message_out o) final override {
            Out->send (Generator (o));
        };

        bool closed () final override {
            return Out->closed ();
        };

        void close () final override {
            return Out->close ();
        };

        serialized_session (function<std::basic_string<word> (const message_out &)> g) : Generator {g}, Out {} {}

        // write as a string.
        function<std::basic_string<word> (const message_out &)> Generator;

        // the low-level session.
        ptr<net::session<const std::basic_string<word> &>> Out;

    };

    // open a serialized session with a parser, meaning that the serialized message type
    // can be read as a stream to determine where a message begins ends. We would use
    // this with something like a TCP connection.
    template <typename message_out, typename message_in = message_out, typename word = char>
    open<message_in, message_out> serialized_session_parsed (
        open<std::basic_string_view<word>, const std::basic_string<word> &> Open,
        function<std::basic_string<word> (message_out)> Generator,
        function<ptr<writer<word>> (handler<const message_out &>)> Parser) {

        return [Open, Generator, Parser]
            (close_handler on_close, interaction<message_in, message_out> interact) -> void {

                Open (on_close,
                    // the receive handler.
                    [interact, Generator, Parser]
                    (ptr<session<const std::basic_string<word> &>> low_level) -> handler<std::basic_string_view<word>> {

                        // the high level out session. It does not know how to send messages yet.
                        ptr<serialized_session<message_out, char>> high_level =
                            std::make_shared<serialized_session<message_out, char>> (Generator);

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

    // open a serialized session with a reader, meaning that string that is received is known
    // to be a complete message and we just read it into the deserialized type. We would use
    // this with a websockets connection or something like that.
    template <typename message_out, typename message_in = message_out, typename word = char>
    open<message_in, message_out> serialized_session_read (
        open<std::basic_string_view<word>, const std::basic_string<word> &> Open,
        function<std::basic_string<word> (message_out)> Generator,
        function<message_in (std::basic_string_view<word>)> Reader) {

        return [Open, Generator, Reader]
            (close_handler on_close, interaction<message_in, message_out> interact) -> void {

                Open (on_close,
                    // the receive handler.
                    [interact, Generator, Reader]
                    (ptr<session<const std::basic_string<word> &>> low_level) -> handler<std::basic_string_view<word>> {

                        // the high level out session. It does not know how to send messages yet.
                        ptr<serialized_session<message_out, char>> high_level =
                            std::make_shared<serialized_session<message_out, char>> (Generator);

                        // add the low level sesion to the high level session.
                        high_level->Out = low_level;

                        return [In = interact (std::static_pointer_cast<session<message_out>> (high_level)), Reader]
                            (std::basic_string_view<word> x) {
                                std::cout << "meep meep meep ... " << x << std::endl;
                                auto j = Reader (x);
                                std::cout << "moop moop moop" << std::endl;
                                std::cout << "read message " << j << std::endl;
                                In (j);
                            };

                    });
            };
    }


}

#endif


