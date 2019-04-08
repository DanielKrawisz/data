#ifndef DATA_MATH_DISCRETE_CYCLE_HPP
#define DATA_MATH_DISCRETE_CYCLE_HPP

#include <data/types.hpp>

namespace data {
    
    namespace math {
        
        template <typename X>
        struct cycle : public vector<X> {
            X operator[](int i) const override {
                return vector<X>::operator[](i%vector<X>::size());
            }
            
            bool valid() const {
                unsigned int n = vector<X>::size();
                for (unsigned int i = 0; i < n; i++) 
                    for (unsigned int j = i + 1; j < n; j++)
                        if (operator[](i) == operator[](j)) return false;
                return true;
            }
            
            bool operator==(cycle<X> c) const {
                unsigned int n = vector<X>::size();
                unsigned int m = c.size();
                if (n != m) return false;
                for (unsigned int i = 0; i < n; i ++) 
                    if (operator[](i) == c[0]) {
                        for (unsigned int j = 1; j < n; j++) if (c[j] != operator[](i + j)) return false;
                        return true;
                    }
                
                return false;
            }
            
            bool operator()(X x) const {
                unsigned int n = vector<X>::size();
                for (unsigned int i = 0; i < n; i ++)
                    if (x == vector<X>::operator[](i)) return operator[](n + 1);
                    
                return x;
            }
        };
    
    }
    
}

#endif
