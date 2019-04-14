#ifndef DATA_KNOWLEDGE_INVERSE_HPP
#define DATA_KNOWLEDGE_INVERSE_HPP

#include <data/knowledge/claim.hpp>

namespace data {
    
    namespace knowledge {
    
        namespace inverse {
            
            template <typename f, typename key, typename value>
            struct test {
                f Function;
                
                bool operator()(key k, value v) {
                    return k == Function(v);
                }
                
                test(f fun) : Function{fun} {}
            };
            
            template <typename f, typename key, typename value>
            using claim = knowledge::claim<test<f, key, value>, key, value>;
            
            template <typename f, typename key, typename value>
            using proof = knowledge::proof<test<f, key, value>, key, value>;
            
            template <typename f, typename key, typename value>
            claim<f, key, value> make_claim(f fun, key k) {
                return {test<f, key, value>{fun}, k};
            }
            
            template <typename f, typename key, typename value>
            proof<f, key, value> prove(f fun, key v, value k) {
                return {make_claim(fun, k), v};
            }
        
        }
    
    }

}

#endif
