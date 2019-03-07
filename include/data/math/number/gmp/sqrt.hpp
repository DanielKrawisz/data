#ifndef DATA_MATH_NUMBER_GMP_SQRT_HPP
#define DATA_MATH_NUMBER_GMP_SQRT_HPP

#include <data/math/number/gmp/N.hpp>
#include <data/math/number/gmp/Q.hpp>
#include <data/math/number/gmp/R.hpp>

namespace data {
    
    namespace math {
        
        number::gmp::R sqrt(number::gmp::N n);
        
        number::gmp::R sqrt(number::gmp::Q n);
        
        number::gmp::R sqrt(number::gmp::R n);
    }

}

#endif
