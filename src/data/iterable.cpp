#include <data/iterable.hpp>
#include <data/encoding/hex.hpp>

std::ostream& operator<<(std::ostream& o, const data::bytes_view s) {
    return o << data::encoding::hex::write(s); 
}

