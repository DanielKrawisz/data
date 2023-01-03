// Copyright (c) 2023 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_NETWORKING_SERIALIZED
#define DATA_NETWORKING_SERIALIZED

#include <data/networking/session.hpp>

namespace data::networking {

    template <typename word, typename in, typename out = in>
    struct serialized : parser<word, in>,
        virtual from<const in &>,
        virtual to<const out &>,
        virtual from<std::basic_string_view<word>>,
        virtual to<const std::basic_string<word> &> {

        using to<const std::basic_string<word> &>::send;
        using from<const in &>::receive;

        virtual std::basic_string<word> serialize(const out &m);

        void send(const out &m) final override;

        virtual ~serialized() {}

        virtual void receive(std::basic_string_view<word> b) final override;

        void parsed(const in &m) final override;

    };

    template <typename word, typename in, typename out>
    void inline serialized<word, in, out>::send (const out &m) {
        send (std::basic_string_view<word> (serialize (m)));
    }

    template <typename word, typename in, typename out>
    void inline serialized<word, in, out>::receive (std::basic_string_view<word> b) {
        this->write (b.data (), b.size ());
    }

    template <typename word, typename in, typename out>
    void inline serialized<word, in, out>::parsed (const in &m) {
        receive (m);
    }

}

#endif


