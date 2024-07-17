// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_HAMILTONIAN
#define DATA_MATH_HAMILTONIAN

#include <data/math/complex.hpp>

namespace data::math {
    template <typename R> class quaternion;

    template <typename R> bool operator == (const quaternion<R> &, const quaternion<R> &);

    template <typename R>
    class quaternion : public cayley_dickson<R, complex<R>> {
        using complex = math::complex<R>;
        using hamiltonian = cayley_dickson<R, complex>;
        
    public:
        using hamiltonian::hamiltonian;
        quaternion (R r, R i, R j, R k) : quaternion {complex {r, i}, complex {j, k}} {}
        quaternion (const complex &x) : quaternion {complex {x}, complex {}} {}
        quaternion (hamiltonian &&c) : hamiltonian {c} {}
        
        constexpr static quaternion I = {0, 1, 0, 0};
        constexpr static quaternion J = {0, 0, 1, 0};
        constexpr static quaternion K = {0, 0, 0, 1};
        
        quaternion conjugate () const {
            return hamiltonian::conjugate ();
        }
        
        quaternion operator ~ () const {
            return hamiltonian::operator ~ ();
        }
        
        quaternion operator + (const quaternion &x) const {
            return hamiltonian::operator + (x);
        }
        
        quaternion operator - () const {
            return hamiltonian::operator - ();
        }
        
        quaternion operator - (const quaternion &x) const {
            return hamiltonian::operator - (x);
        }
        
        quaternion operator * (const quaternion &x) const {
            return hamiltonian::operator * (x);
        }
        
        quaternion operator / (const quaternion &x) const {
            return hamiltonian::operator / (x);
        }
        
        quaternion inverse () const {
            return hamiltonian::inverse ();
        }
    };
    
    template <typename R> struct conjugate<quaternion<R>> {
        quaternion<R> operator () (const quaternion<R>& x) {
            return {conjugate<cayley_dickson<R, complex<R>>> {} (x)};
        }
    };
    
    template <typename R>
    struct inverse<plus<quaternion<R>>, quaternion<R>> {
        quaternion<R> operator () (const quaternion<R> &a, const quaternion<R> &b) {
            return b - a;
        }
    };

    template <typename q>
    struct times<quaternion<q>> {
        quaternion<q> operator () (const quaternion<q> &a, const quaternion<q> &b) {
            return a * b;
        }

        nonzero<quaternion<q>> operator () (const nonzero<quaternion<q>> &a, const nonzero<quaternion<q>> &b) {
            return a * b;
        }
    };

    template <typename q>
    struct inverse<times<quaternion<q>>, quaternion<q>> : inverse<times<q>, q> {
        nonzero<quaternion<q>> operator () (const nonzero<quaternion<q>> &a, const nonzero<quaternion<q>> &b) {
            return b / a;
        }
    };
    
}

namespace data::math::linear {
    
    template <typename q> 
    struct dimensions<q, quaternion<q>> : dimensions<q, cayley_dickson<q, complex<q>>> {};

    template <typename q>
    struct dimensions<complex<q>, quaternion<q>> {
        constexpr static dimension value = 2;
    };

    template <typename q>
    struct inner<q, quaternion<q>> {
        q operator () (const quaternion<q> &a, const quaternion<q> &b);
    };
    
}

#endif
