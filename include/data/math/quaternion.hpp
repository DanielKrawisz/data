// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_HAMILTONIAN
#define DATA_MATH_HAMILTONIAN

#include <data/math/complex.hpp>

namespace data::math {

    template <typename R>
    class quaternion : public cayley_dickson<complex<R>, R> {
        using complex = math::complex<R>;
        using hamiltonian = cayley_dickson<complex, R>;
        
    public:
        quaternion() : hamiltonian{} {}
        quaternion(const R& r) : hamiltonian{r} {}
        quaternion(const complex& a, const complex& b) : hamiltonian{a, b} {}
        quaternion(R r, R i, R j, R k) : quaternion{complex{r, i}, complex{j, k}} {}
        quaternion(const hamiltonian& c) : hamiltonian{c} {}
        
        constexpr static quaternion I = {0, 1, 0, 0};
        constexpr static quaternion J = {0, 0, 1, 0};
        constexpr static quaternion K = {0, 0, 0, 1};
        
        operator hamiltonian() {
            return static_cast<hamiltonian>(*this);
        }
        
        quaternion conjugate() const {
            return hamiltonian::conjugate();
        }
        
        quaternion operator~() const {
            return conjugate();
        }
        
        quaternion operator+(const quaternion& x) const {
            return hamiltonian::operator+(x);
        }
        
        quaternion operator-() const {
            return hamiltonian::operator-();
        }
        
        quaternion operator-(const quaternion& x) const {
            return hamiltonian::operator-(x);
        }
        
        quaternion operator*(const quaternion& x) const {
            return hamiltonian::operator*(x);
        }
        
        quaternion operator/(const quaternion& x) const {
            return hamiltonian::operator/(x);
        }
        
        quaternion inverse() const {
            return hamiltonian::inverse();
        }
        
        nonnegative<R> quadrance() const {
            return hamiltonian::quadrance();
        }
    };
    
}

#endif
