// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_HAMILTONIAN
#define DATA_MATH_HAMILTONIAN

#include <data/math/complex.hpp>

namespace data::math {

    template <typename R>
    class Hamiltonian : public cayley_dickson<complex<R>, R> {
        using complex = math::complex<R>;
        using hamiltonian = cayley_dickson<complex, R>;
        
    public:
        Hamiltonian() : hamiltonian{} {}
        Hamiltonian(const R& r) : hamiltonian{r} {}
        Hamiltonian(const complex& a, const complex& b) : hamiltonian{a, b} {}
        Hamiltonian(R r, R i, R j, R k) : Hamiltonian{complex{r, i}, complex{j, k}} {}
        Hamiltonian(const hamiltonian& c) : hamiltonian{c} {}
        
        constexpr static Hamiltonian I = {0, 1, 0, 0};
        constexpr static Hamiltonian J = {0, 0, 1, 0};
        constexpr static Hamiltonian K = {0, 0, 0, 1};
        
        operator hamiltonian() {
            return static_cast<hamiltonian>(*this);
        }
        
        Hamiltonian conjugate() const {
            return hamiltonian::conjugate();
        }
        
        Hamiltonian operator~() const {
            return conjugate();
        }
        
        Hamiltonian operator+(const Hamiltonian& x) const {
            return hamiltonian::operator+(x);
        }
        
        Hamiltonian operator-() const {
            return hamiltonian::operator-();
        }
        
        Hamiltonian operator-(const Hamiltonian& x) const {
            return hamiltonian::operator-(x);
        }
        
        Hamiltonian operator*(Hamiltonian x) const {
            return hamiltonian::operator*(x);
        }
        
        Hamiltonian inverse() const {
            return hamiltonian::inverse();
        }
        
        Hamiltonian operator/(Hamiltonian x) const {
            return hamiltonian::operator/(x);
        }
    };
    
}

#endif
