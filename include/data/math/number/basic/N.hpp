#ifndef DATA_MATH_NUMBER_BASIC_N
#define DATA_MATH_NUMBER_BASIC_N

#include <data/math/number/natural.hpp>
#include <limits>
#include "overflow.hpp"

namespace data {
    
    namespace math {
    
        namespace number {
            
            namespace basic {
                
                struct Z;
                struct Q;
                
                // Implementation of naturals using basic types. 
                // It checks for overflow and throws exceptions. 
                struct N final {
                    constexpr static uint64_t Max = std::numeric_limits<uint64_t>::max();
                    
                    uint64_t Number;
                    
                    N() : Number{0} {}
                    
                    N(uint n) : Number{n} {}
                    
                    N(const N& n) : Number{n.Number} {}
                    
                    N& operator=(N n) {
                        Number = n.Number;
                        return *this;
                    }
                    
                    bool operator==(const N n) const {
                        return Number == n.Number;
                    }
                    
                    bool operator<(const N n) const {
                        return Number < n.Number;
                    }
                    
                    bool operator>(const N n) const {
                        return Number > n.Number;
                    }
                    
                    bool operator<=(const N n) const {
                        return Number <= n.Number;
                    }
                    
                    bool operator>=(const N n) const {
                        return Number >= n.Number;
                    }
                    
                    N successor() const {
                        if (Number == Max) throw overflow{};
                        return N{Number + 1};
                    }
                    
                    N operator+(const N n) const {
                        if (Max - Number < n.Number) throw overflow{};
                        return N{Number + n.Number};
                    }
                    
                    N& operator+=(const N n) {
                        Number += operator+(n).Number;
                        return *this;
                    }
                    
                    N operator*(const N n) const {
                        if (Number == 0 || n.Number == 0) return 0;
                        
                        return Number * n.Number;
                    };
                    
                    N& operator*=(const N n) {
                        Number *= operator*(n).Number;
                        return *this;
                    };
                    
                    math::number::division<N> divide(const N n) const {
                        return {Number / n.Number, Number % n.Number};
                    }
                    
                    bool operator|(const N n) const {
                        return divide(n).Quotient == 0;
                    }
                    
                    N operator/(const N n) const {
                        return divide(n).Quotient;
                    }
                    
                    N operator%(const N n) const {
                        return divide(n).Remainder;
                    }
                    
                    N& operator/=(const N n) {
                        N q = operator/(n);
                        return operator=(q);
                    }
                    
                    N& operator%=(const N n) {
                        N r = operator%(n);
                        return operator=(r);
                    }
                    
                    friend struct Z;
                    friend struct Q;
                };
                
                constexpr static math::number::natural<N> is_natural{};
                
            }
            
        }
    
        inline number::basic::N abs(number::basic::N n) {
            return n;
        }
        
        inline number::basic::N square(number::basic::N n) {
            return n * n;
        }
        
        inline number::basic::N norm(number::basic::N n) {
            return n;
        }
    }

}

#endif
