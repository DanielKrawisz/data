// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/encoding/base58.hpp>
#include <data/math/number/gmp/gmp.hpp>
#include <data/math/number/bytes/N.hpp>

namespace data::encoding::base58 {
    
    std::string write(const bytes_view b) {
        return write<math::number::gmp::N>(math::number::gmp::N(math::number::N_bytes<endian::big>(b)));
    }
    
    view::view(string_view s) : string_view{s}, Bytes{}, ToBytes{nullptr} {
        if (base58::valid(s)) {
            Bytes = bytes_view(math::number::N_bytes<endian::big>(read<math::number::gmp::N>(s)));
            ToBytes = &Bytes;
        }
    }

}
