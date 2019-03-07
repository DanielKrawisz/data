#ifndef DATA_MATH_NONNEGATIVE_HPP
#define DATA_MATH_NONNEGATIVE_HPP

namespace data {
    
    namespace math {
    
        template <typename R>
        struct nonnegative {
            R Number;
            
            nonnegative(R n) : Number{n} {}
            
            bool valid() {
                return Number >= 0;
            }
            
            operator R() const {
                return Number;
            }
            
            nonnegative<R> operator+(nonnegative<R> n) const {
                return Number + n.Number;
            }
            
            nonnegative<R> operator*(nonnegative<R> n) const {
                return Number * n.Number;
            }
            
            R operator+(R n) const {
                return Number + n.Number;
            }
            
            R operator*(R n) const {
                return Number * n.Number;
            }
            
            R operator-() const {
                return -Number;
            }
            
            R operator-(R n) const {
                return Number - n.Number;
            }
            
            bool operator>(R n) const {
                return Number > n.Number;
            }
            
            bool operator>=(R n) const {
                return Number >= n.Number;
            }
            
            bool operator<(R n) const {
                return Number < n.Number;
            }
            
            bool operator<=(R n) const {
                return Number <= n.Number;
            }
        };
    
    }

}

#endif
