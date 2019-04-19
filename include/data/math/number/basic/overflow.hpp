#ifndef DATA_MATH_NUMBER_BASIC_OVERFLOW
#define DATA_MATH_NUMBER_BASIC_OVERFLOW

namespace data {
    
    namespace math {
    
        namespace number {
            
            namespace basic {
                
                class overflow : std::exception {
                    
                public:
                    overflow() {}
                    const char* what() const noexcept final override {
                        return "overflow";
                    }
                    
                    template <typename T>
                    friend struct impossible;
                };
                
            }
            
        }
    
    }

}

#endif
