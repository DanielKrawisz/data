// Copyright (c) 2021-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_ASIO_STREAM
#define DATA_NET_ASIO_STREAM

#include <data/net/stream.hpp>
#include <data/bytes.hpp>
#include <data/string.hpp>
#include <boost/asio.hpp>

namespace data::net::asio {
    using namespace boost::asio;
    using error = boost::system::error_code;

    template <typename const_buffer_sequence>
    concept ConstBufferSequence = std::copy_constructible<const_buffer_sequence> && std::destructible<const_buffer_sequence> &&
        requires (const const_buffer_sequence x) {
            { buffer_sequence_begin (x) };
            { buffer_sequence_end (x) };
        };

    template <typename async_write_stream, typename const_buffer_sequence = const_buffer>
    concept AsyncWriteStream = ConstBufferSequence<const_buffer_sequence> && requires (async_write_stream stream) {
        { stream.get_executor () };
    } && requires (async_write_stream stream, const_buffer_sequence bf) {
        { stream.async_write_some (bf, use_awaitable) } -> Same<awaitable<size_t>>;
    };

    template <typename async_read_stream, typename mutable_buffer_sequence>
    concept AsyncReadStream = requires (async_read_stream stream, mutable_buffer_sequence m) {
        { stream.async_read_some (buffer (m), use_awaitable) } -> Same<awaitable<size_t>>;
    };

    template <AsyncWriteStream X, typename bytes, typename byte_slice> requires AsyncReadStream<X, bytes> class stream;

    template <AsyncWriteStream X> using byte_stream = stream<X, bytes, byte_slice>;
    template <AsyncWriteStream X> using char_stream = stream<X, string, string_view>;

    struct exception : data::exception {
        error Error;
        exception (error ec): Error {ec} {
            *this << "Operation failed because: " << ec;
        }
    };

    template <AsyncWriteStream X, typename bytes, typename byte_slice> requires AsyncReadStream<X, bytes>
    class stream final : public net::stream<bytes, byte_slice> {

        ptr<X> Stream;

        close_handler OnClose;

        bool Closed;

        constexpr static const int buffer_size = 4096;
        bytes Buffer;

    public:
        stream (ptr<X> socket, close_handler on_close) :
            Stream {socket}, OnClose {on_close}, Closed {!Stream->is_open ()}, Buffer (buffer_size) {}

        awaitable<void> send (byte_slice x) final override {
            error ec;
            std::size_t n = co_await async_write (
                *Stream,
                buffer (x.data (), x.size ()),
                asio::redirect_error (asio::use_awaitable, ec)
            );

            if (ec) {
                throw exception {ec};
                close ();
            }
        }

        void close () final override {
            if (Closed) return;
            Closed = true;
            Stream->close ();
            OnClose ();
        }

        bool closed () final override {
            bool closed = !Stream->is_open ();
            if (closed && !Closed) {
                Closed = true;
                OnClose ();
            }
            return closed;
        }

        awaitable<bytes> receive () final override {
            error ec;
            // if there is no error, bytes_transferred should always be greater than zero.
            // if bytes_read == buffer_size, we cannot know if there are more bytes on their
            // way. Reconstructing a complete message has to come at a higher level.
            size_t bytes_read = co_await Stream->async_read_some (buffer (Buffer.data (), Buffer.size ()), redirect_error (use_awaitable, ec));

            if (ec) {
                close ();
                throw exception {ec};
            }

            bytes b (bytes_read);
            std::copy (Buffer.begin (), Buffer.begin () + bytes_read, b.begin ());
            co_return b;
        }

        ~stream () {
            close ();
        }
    };

}

#endif

