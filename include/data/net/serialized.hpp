// Copyright (c) 2023 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_NET_SERIALIZED
#define DATA_NET_SERIALIZED

#include <data/string.hpp>
#include <data/net/stream.hpp>
#include <data/parser.hpp>
#include <data/function.hpp>
#include <data/bytes.hpp>

namespace data::net {

    // open a serialized stream with a parser, meaning that the serialized message type
    // can be read as a stream to determine where a message begins ends. We would use
    // this with something like a TCP connection.
    template <typename in, typename out = const in &>
    open<in, out> serialized_stream_parsed (
        open<bytes, byte_slice> Open,
        function<bytes (out)> Generator,
        function<ptr<writer<byte>> (handler<out>)> Parser);

    // open a serialized stream with a reader, meaning that string that is received is known
    // to be a complete message and we just read it into the deserialized type. We would use
    // this with a websockets connection or something like that.
    template <typename in, typename out = const in &>
    open<in, out> serialized_stream_read (
        open<bytes, byte_slice> Open,
        function<bytes (out)> Generator,
        function<in (byte_slice)> Reader);

    // same thing but with strings.
    template <typename in, typename out = const in &>
    open<in, out> serialized_stream_parsed (
        open<std::string, string_view> Open,
        function<std::string (out)> Generator,
        function<ptr<writer<char>> (handler<out>)> Parser);

    template <typename in, typename out = const in &>
    open<in, out> serialized_stream_read (
        open<std::string, string_view> Open,
        function<std::string (out)> Generator,
        function<in (string_view)> Reader);

    // an implementation of session that takes a message type that can be serialized into a string.
    template <typename out>
    struct serialized_out_stream final : out_stream<out> {

        awaitable<bool> send (out o) final override {
            co_return co_await Out->send (Generator (o));
        };

        void close () final override {
            return Out->close ();
        };

        serialized_out_stream (function<bytes (const out &)> g, ptr<out_stream<byte_slice>> o) : Generator {g}, Out {o} {}

    private:
        // the low-level session.
        ptr<out_stream<byte_slice>> Out;

        // write as a string.
        function<bytes (const out &)> Generator;

    };

    // open a serialized stream with a parser, meaning that the serialized message type
    // can be read as a stream to determine where a message begins ends. We would use
    // this with something like a TCP connection.
    template <typename in, typename out>
    open<in, out> serialized_stream_parsed (
        open<bytes, byte_slice> Open,
        function<bytes (out)> Generator,
        function<ptr<writer<byte>> (handler<out>)> Parser) {
        // returning a function that opens a lower level channel and attaches
        // the parser and generator to it.
        open<in, out> open_serialized = [Open, Generator, Parser]
            (close_handler on_close, interaction<in, out> interact) -> awaitable<ptr<out_stream<out>>> {

                // the high level out stream.
                ptr<serialized_out_stream<out>> high_level_out;

                interaction<bytes, byte_slice> low_level_interaction =
                    [&high_level_out, interact, Generator, Parser]
                    (ptr<out_stream<byte_slice>> low_level_out) -> handler<const bytes &> {

                        // Here we actually create the high level out stream.
                        // this won't happen until high_level_out actually gets called, however.
                        high_level_out = std::make_shared<serialized_out_stream<out>> (Generator, low_level_out);

                        // return the in handler.
                        return [
                            Parse = Parser (
                                [In = interact (std::static_pointer_cast<out_stream<out>> (high_level_out))]
                                (in i) {
                                    In (i);
                                })] (const bytes &i) -> void {
                                Parse->write (i.data (), i.size ());
                            };

                    };

                // open the low level channel. We get a low level out stream
                // but we don't use it because it gets set in here.
                co_await Open (on_close, low_level_interaction);

                // We have to assume that low_level_interaction
                // will get called so that high_level_out will be set.
                // TODO throw if high_level_out is nullptr.
                co_return static_pointer_cast<out_stream<out>> (high_level_out);
            };
        return open_serialized;
    }

    // open a serialized stream with a reader, meaning that string that is received is known
    // to be a complete message and we just read it into the deserialized type. We would use
    // this with a websockets connection or something like that.
    template <typename in, typename out>
    open<in, out> serialized_stream_read (
        open<bytes, byte_slice> Open,
        function<bytes (out)> Generator,
        function<in (byte_slice)> Reader) {
        // returning a function that opens a lower level channel and attaches
        // the reader and the generator to it.
        open<in, out> open_serialized = [Open, Generator, Reader]
            (close_handler on_close, interaction<in, out> interact) -> awaitable<ptr<out_stream<out>>> {

                // the high level out stream.
                ptr<serialized_out_stream<out>> high_level_out;

                interaction<bytes, byte_slice> low_level_interaction = [&high_level_out, interact, Generator, Reader]
                    (ptr<out_stream<byte_slice>> low_level_out) -> handler<const bytes &> {
                        // Here we actually create the high level out stream.
                        // this won't happen until high_level_out actually gets called, however.
                        high_level_out = std::make_shared<serialized_out_stream<out>> (Generator, low_level_out);

                        return [In = interact (std::static_pointer_cast<out_stream<out>> (high_level_out)), Reader]
                            (const bytes &x) {
                                In (Reader (x));
                            };

                    };

                // open the low level channel. We get a low level out stream
                // but we don't use it because it gets set in here.
                co_await Open (on_close, low_level_interaction);

                // We have to assume that low_level_interaction
                // will get called so that high_level_out will be set.
                co_return static_pointer_cast<out_stream<out>> (high_level_out);
            };
        return open_serialized;
    }


}

#endif


