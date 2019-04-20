#ifndef DATA_MATH_NUMBER_BOUNDED
#define DATA_MATH_NUMBER_BOUNDED

#include <data/types.hpp>
#include <data/math/group.hpp>
#include <data/math/sign.hpp>

namespace data {
    
    namespace math {
    
        namespace number {
            
            template <uint32_t size>
            struct bounded : public std::array<uint64_t, size> {  
                byte operator[](const uint n) const {
                    return std::array<uint64_t, size>::operator[](n / 4) >> ((n % 4) * 8);
                }
                
                bounded() : std::array<uint64_t, size>() {}
                bounded(uint64_t x) : bounded() {
                    std::array<uint64_t, size>::operator[](size - 1) = x;
                }
                
                bounded(std::array<byte, 4 * size> b);
                
                static bounded zero() {
                    return bounded{};
                }

                bool operator<(const bounded& d) const {
                    for (uint32_t i = 0; i < size; i++) {
                        if (std::array<uint64_t, size>::operator[](i) < d[i]) return true;
                        if (std::array<uint64_t, size>::operator[](i) > d[i]) return false;
                    }
                    
                    return false;
                }
 
                bool operator<=(const bounded& d) const {
                    for (uint32_t i = 0; i < size; i++) {
                        if (std::array<uint64_t, size>::operator[](i) < d[i]) return true;
                        if (std::array<uint64_t, size>::operator[](i) > d[i]) return false;
                    }
                    
                    return true;
                }
                    
                bool operator>(const bounded& d) const {
                    return d <= *this;
                }
                    
                bool operator>=(const bounded& d) const {
                    return d < *this;
                }
                
                bool operator==(const bounded& d) const {
                    return std::array<uint64_t, size>::operator==(static_cast<const std::array<uint64_t, size>&>(d));
                }
                
                bool operator!=(const bounded& d) const {
                    return std::array<uint64_t, size>::operator!=(static_cast<const std::array<uint64_t, size>&>(d));
                }
            };
            
        }
    
    }

}

#endif
