// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_OCTONIAN
#define DATA_MATH_OCTONIAN

#include <data/math/hamiltonian.hpp>

namespace data::math {

    template <typename R>
    class octonion : public cayley_dickson<Hamiltonian<R>, R> {
        using com = complex<R>;
        using ham = Hamiltonian<R>;
        using oct = cayley_dickson<ham, R>;
    public:
        
        octonion() : ham{} {}
        octonion(const R& r) : ham{r} {}
        octonion(const ham& a, const ham& b) : oct{a, b} {}
        octonion(const com& a, const com& b, const com& c, const com& d) : 
            octonion{ham{a, b}, ham{c, d}} {}
        octonion(R r, R i, R j, R k, R l, R m, R n, R o) : 
            octonion{com{r, i}, com{j, k}, com{l, m}, com{n, o}} {}
        octonion(const oct& o) : oct{o} {}
        
        constexpr static octonion E0 = {1, 0, 0, 0, 0, 0, 0, 0};
        constexpr static octonion E1 = {0, 1, 0, 0, 0, 0, 0, 0};
        constexpr static octonion E2 = {0, 0, 1, 0, 0, 0, 0, 0};
        constexpr static octonion E3 = {0, 0, 0, 1, 0, 0, 0, 0};
        constexpr static octonion E4 = {0, 0, 0, 0, 1, 0, 0, 0};
        constexpr static octonion E5 = {0, 0, 0, 0, 0, 1, 0, 0};
        constexpr static octonion E6 = {0, 0, 0, 0, 0, 0, 1, 0};
        constexpr static octonion E7 = {0, 0, 0, 0, 0, 0, 0, 1};
        
        operator oct() {
            return static_cast<oct>(*this);
        }
        
        octonion conjugate() const {
            return oct::conjugate();
        }
        
        octonion operator~() const {
            return conjugate();
        }
        
        octonion operator+(const octonion& x) const {
            return oct::operator+(x);
        }
        
        octonion operator-() const {
            return oct::operator-();
        }
        
        octonion operator-(const octonion& x) const {
            return oct::operator-(x);
        }
        
        octonion operator*(octonion x) const {
            return oct::operator*(x);
        }
        
        octonion inverse() const {
            return oct::inverse();
        }
        
        octonion operator/(octonion x) const {
            return oct::operator/(x);
        }
    };
    
}

#endif
