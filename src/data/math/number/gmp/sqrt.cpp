
#include <data/math/number/gmp/sqrt.hpp>

namespace data::math::number::gmp {
        
    N sqrt(const N& n) {
        return root(n, 2);
    }
        
    N root(const N& n, uint32 p) {
        if (p == 0) return N{};
        if (p == 1 || n == N{0} || n == N{1}) return n;
        N p_root{};
        if (0 == mpz_root(p_root.Value.MPZ, n.Value.MPZ, p)) return N{};
        return p_root;
    }

}


