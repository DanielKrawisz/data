// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_ASIO_ERROR_SESSION
#define DATA_NET_ASIO_ERROR_SESSION

#include <data/net/session.hpp>
#include <boost/asio.hpp>

namespace data::net::asio {
    using namespace boost::asio;
    using error_code = boost::system::error_code;

    using error_handler = handler<error_code>;

    // a function type that would open a new session.
    template <typename in, typename out>
    using open = function<void (error_handler, close_handler, interaction<in, out>)>;

    using write_token = function<void (error_code, size_t)>;

    template <typename ConstBufferSequence>
    concept const_buffer_sequence = std::copy_constructible<ConstBufferSequence> && std::destructible<ConstBufferSequence> &&
        requires (const ConstBufferSequence x) {
            { buffer_sequence_begin (x) };
            { buffer_sequence_end (x) };
        };

    template <typename AsyncWriteStream, typename ConstBufferSequence = const_buffer>
    concept async_write_stream = const_buffer_sequence<ConstBufferSequence> && requires (AsyncWriteStream stream) {
        { stream.get_executor () };
    } && requires (AsyncWriteStream stream, ConstBufferSequence bf, write_token t) {
        { stream.async_write_some (bf, t) };
    };

}

#endif

