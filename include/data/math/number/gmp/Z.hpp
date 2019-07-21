// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_Z
#define DATA_MATH_NUMBER_GMP_Z

#include <data/math/number/gmp/N.hpp>

namespace data {
    
    namespace math {
    
        namespace number {
            
            namespace gmp {
                
                struct Z final : public mpz {
                    Z() : mpz() {}
                    
                    Z(uint n) {
                        if (n <= std::numeric_limits<uint>::max()) mpz_init_set_ui(&MPZ, n);
                        throw 0;
                    }
                    
                    Z(const N& n) : mpz(n.MPZ) {}
                    
                    Z(const Z& z) : mpz(z.MPZ) {}
                    
                    Z(Z&& z) {
                        swap(MPZ, z.MPZ);
                    }
                    
                    Z& operator=(Z& z) {
                        mpz_set(&MPZ, &z.MPZ);
                        return *this;
                    }
                    
                    bool operator==(const Z&) const;
                    
                    bool operator<(const Z&) const;
                    
                    bool operator>(const Z&) const;
                    
                    bool operator<=(const Z&) const;
                    
                    bool operator>=(const Z&) const;
                    
                    Z operator+(const Z&) const;
                    
                    Z& operator+=(const Z&);
                    
                    Z operator*(const Z&) const;
                    
                    Z& operator*=(const Z&);
                    
                    Z operator^(uint) const;
                    
                    Z& operator^=(uint);
                    
                    math::number::division<Z> divide(const Z&) const;
                    
                    bool operator|(const Z& z) const {
                        return divide(z).Quotient == 0;
                    }
                    
                    Z operator/(const Z& z) const {
                        return divide(z).Quotient;
                    }
                    
                    Z operator%(const Z& z) const {
                        return divide(z).Remainder;
                    }
                    
                    Z& operator/=(const Z& z) {
                        Z q = operator/(z);
                        return operator=(q);
                    }
                    
                    Z& operator%=(const Z& z) {
                        Z r = operator%(z);
                        return operator=(r);
                    }
                    
                    N abs() const;
                    
                    friend struct Q;
                
                    constexpr static math::number::natural<Z> is_integer{};
                };
                
            }
            
        }
    
        inline number::gmp::N abs(number::gmp::Z& z) {
            return z.abs();
        }
        
        number::gmp::N square(number::gmp::Z& z) {
            return z * z;
        }
    }

}

#endif
