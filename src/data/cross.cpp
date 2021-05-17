#include <data/cross.hpp>
#include <data/encoding/hex.hpp>

namespace data {
    std::ostream& operator<<(std::ostream& o, const bytes& s) {
        return o << "\"" << encoding::hex::write(s) << "\""; 
    }
}
