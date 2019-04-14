#ifndef DATA_PROOF_INTUITIONISTIC
#define DATA_PROOF_INTUITIONISTIC

#include <truth/logic/intuitionistic.hpp>

#include <truth/statement/operation.hpp>
#include <data/constant.hpp>
#include <data/operate.hpp>

namespace truth {
    
    template <typename x, typename y> struct Or<x, y> {
        virtual x left() const = 0;
        virtual y right() const = 0;
        virtual bool is_left() const = 0;
    };
    
    template <typename x, typename y> 
    struct Implies<x, Implies<y, x>&> {
        
        struct constant : public Implies<y, x>, public data::constant<y>{};
        
        Implies<y, x>& operator()(x a) const {
            return *new constant{a};
        }
    };
    
    template <typename x, typename y, typename z> 
    struct Implies<Implies<x, Implies<y, z>&>&, Implies<Implies<x, y>&, Implies<x, z>&>&> {
        
        struct operation :
            public Implies<x, z>, 
            public data::operate<Implies<x, Implies<y, z>&>&, Implies<x, y>&> {};
            
        struct application : public Implies<Implies<x, y>&, Implies<x, z>&> {
            Implies<x, Implies<y, z>&>& Function;
            
            Implies<x, z>& operator()(Implies<x, y>& exp) const {
                return *new operation{Function, exp};
            }
        };
        
        Implies<Implies<x, y>&, Implies<x, z>&>& operator()(Implies<x, Implies<y, z>&>& exp) const {
            return *new application{exp};
        }
    };
    
    template <typename x, typename y> 
    struct Implies<And<x, y>&, x> {                
        x operator()(And<x, y> a) const {
            return a.Left;
        }
    };
    
    template <typename x, typename y> 
    struct Implies<And<x, y>&, y> {                
        y operator()(And<x, y> a) const {
            return a.Right;
        }
    };
    
    template <typename x, typename y> 
    struct Implies<x, Implies<y, And<x, y>>&> {
        struct incomplete_and : public Implies<y, And<x, y>> {
            x Left;
            
            And<x, y> operator()(y b) {
                return And<x, y>{Left, b};
            }
        };
        
        Implies<y, And<x, y>>& operator()(x a) const {
            return *new incomplete_and{a};
        }
    };
    
    class illogical : std::exception {
        string What;
        
    public:
        illogical(string w) : What{w} {}
        
        const char* what() const noexcept final override {
            return const_cast<std::string>(What).c_str();
        }
    };
    
    template <typename x, typename y> struct left_or : public Or<x, y> {
        x Left;
        virtual x left() const final override {
            return Left;
        }
        
        virtual y right() const final override {
            throw illogical("wrong side");
        }
        
        bool is_left() const final override (
            return true;
        );
    };

    template <typename x, typename y> struct right_or : public Or<x, y> {
        y Right;
        virtual x left() const final override {
            throw illogical("wrong side");
        }
        
        virtual y right() const final override {
            return Right;
        }
        
        bool is_left() const final override (
            return true;
        );
    };
    
    template <typename x, typename y> 
    struct Implies<x, Or<x, y>&> {                
        Or<x, y>& operator()(x a) const {
            return left_or<x, y>{a};
        }
    };
    
    template <typename x, typename y> 
    struct Implies<y, Or<x, y>&> {                
        Or<x, y>& operator()(y b) const {
            return right_or<x, y>{b};
        }
    };
    
    template <typename x>
    struct Implies<contradiction, x> {
        x operator()(contradiction c) {
            return illogical("contradictory");
        }
    };
    
    namespace statement {
        template <typename x, typename y>
        struct writer<logic::intuitionistic::Implies<x, y>> {
            static string write() {
                return statement::write<operation<operand::Implies, x, y>>();
            }
        };
        
        template <typename x, typename y>
        struct writer<logic::intuitionistic::And<x, y>> {
            static string write() {
                return statement::write<operation<operand::And, x, y>>();
            }
        };
        
        template <typename x, typename y>
        struct writer<logic::intuitionistic::Or<x, y>> {
            static string write() {
                return statement::write<operation<operand::Or, x, y>>();
            }
        };
    }
}

#endif
