//
// Created by nekosune on 20/08/2019.
//

#include "data/crypto/digest.hpp"
#include "data/types.hpp"
namespace data::crypto {


    template<uint32 s>
    inline bool digest<s>::valid() const {
        return operator!=(digest{0});
    }

    template<uint32 s>
    inline digest<s> &digest<s>::operator=(const digest<s> &d) {
        Digest = d.Digest;
        return *this;
    }

    template<uint32 s>
    bool digest<s>::operator==(const digest & other) const {
        return Digest == other.Digest;
    }
}