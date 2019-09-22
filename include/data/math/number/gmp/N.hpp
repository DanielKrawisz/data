// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_N
#define DATA_MATH_NUMBER_GMP_N

#include <data/math/number/natural.hpp>
#include <data/math/number/gmp/mpz.hpp>
#include <limits>

namespace data {
    
    namespace math {
    
        namespace number {
            
            namespace gmp {
                
                struct Z;
                struct Q;
                
                struct N final : public mpz {
                    N() : mpz() {}
                    
                    N(uint32 n) {
                        if (n <= std::numeric_limits<uint32>::max()) mpz_init_set_ui(&MPZ, n);
                        throw 0; // TODO
                    }
                    
                    N(const N& n) : mpz(n.MPZ) {}
                    
                    N(N&& n) {
                        swap(MPZ, n.MPZ);
                    }
                    
                    N& operator=(const N& n) {
                        mpz_set(&MPZ, &n.MPZ);
                        return *this;
                    }
                    
                    N(const mpz& n) : mpz{n} {}
                    
                    N(mpz&& n) : mpz{n} {}
                    
                    bool valid() {
                        return sign::positive == sign();
                    }
                    
                    bool operator==(const N&) const;
                    
                    bool operator<(const N&) const;
                    
                    bool operator>(const N&) const;
                    
                    bool operator<=(const N&) const;
                    
                    bool operator>=(const N&) const;
                    
                    N successor() const;
                    
                    N operator+(const N&) const;
                    
                    N operator+(gmp_uint) const;
                    
                    N& operator+=(const N&);
                    
                    N& operator+=(gmp_uint);
                    
                    N operator*(const N&) const;
                    
                    N operator*(gmp_uint) const;
                    
                    N& operator*=(const N&);
                    
                    N& operator*=(gmp_uint);
                    
                    N operator^(gmp_uint) const;
                    
                    N& operator^=(gmp_uint);
                    
                    math::number::division<N> divide(const N&) const;
                    
                    bool operator|(const N& n) const {
                        return divide(n).Quotient == 0;
                    }
                    
                    N operator/(const N& n) const {
                        return divide(n).Quotient;
                    }
                    
                    N operator%(const N& n) const {
                        return divide(n).Remainder;
                    }
                    
                    N& operator/=(const N& n) {
                        N q = operator/(n);
                        return operator=(q);
                    }
                    
                    N& operator%=(const N& n) {
                        N r = operator%(n);
                        return operator=(r);
                    }
                    
                    friend struct Z;
                    friend struct Q;
                
                    constexpr static math::number::natural<N> is_natural{};
                };
                
            }
            
        }
    
        inline number::gmp::N abs(number::gmp::N n) {
            return n;
        }
        
        inline number::gmp::N square(number::gmp::N n) {
            return n * n;
        }
        
        inline number::gmp::N norm(number::gmp::N n) {
            return n;
        }
    }

}

#endif
