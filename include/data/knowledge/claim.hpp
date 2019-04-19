#ifndef DATA_KNOWLEDGE_CLAIM
#define DATA_KNOWLEDGE_CLAIM

namespace data {
    
    namespace knowledge {
    
        template <typename quantifier, typename proposition, typename derivation>
        struct claim {
            quantifier Exist;
            proposition SuchThat;
            
            bool verify(derivation d) const {
                if (Exist == quantifier{} || SuchThat == proposition{}) return false;
                
                return Exist(SuchThat, d);
            }
            
            claim(quantifier f, proposition r) : SuchThat(r), Exist(f) {}
            claim() : Exist{}, SuchThat{} {}
        };
        
        template <typename quantifier, typename proposition, typename derivation>
        struct proof : public virtual claim<quantifier, proposition, derivation> {
            using parent_claim = claim<quantifier, proposition, derivation>;
            
            derivation Derivation;
            
            bool valid() const {
                return verify(Derivation);
            }
            
            proof(quantifier f, proposition r, derivation d) : parent_claim{f, r}, Derivation{d} {}
            proof() : parent_claim{}, derivation{} {}
            
            parent_claim make_claim() {
                return parent_claim{parent_claim::Exist, parent_claim::SuchThat};
            }
            
        };
    
    }

}

#endif
