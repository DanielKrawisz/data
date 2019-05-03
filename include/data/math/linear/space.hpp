#ifndef DATA_MATH_LINEAR_SPACE
#define DATA_MATH_LINEAR_SPACE

#include <data/math/module.hpp>
#include <data/math/field.hpp>

namespace data {
    
    namespace math {
        
        namespace linear {
            
            template <typename vector, typename field>
            struct space {
                static const module<vector, field> r1{};
                static const math::field<field> r2{};
                    
                vector times(vector v, field s) const {
                    return v * s;
                }
                    
                vector plus(vector a, vector b) const {
                    return a + b;
                }
                    
                vector zero() const {
                    return 0;
                }
            };
            
        }
    
    }
    
}

#endif
