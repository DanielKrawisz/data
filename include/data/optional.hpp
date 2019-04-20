#ifndef DATA_OPTIONAL
#define DATA_OPTIONAL

namespace data {

    template <typename X>
    struct optional {
        bool Exists;
        X Value;
        
        optional() : Exists(false), Value{} {}
        optional(X x) : Exists(true), Value(x) {}
        
        optional(const optional<X>& o) : Exists(o.Exists), Value(o.Value) {}
        
        bool valid() const {
            return (!Exists) || valid(Value);
        }
        
        const bool operator==(const optional<X>& o) const {
            return (Exists && o.Exists && Value == o.Value) || !(Exists || o.Exists);
        }
        
        optional& operator=(const optional<X>& o) {
            Exists = o.Exists;
            Value = o.Value;
            return *this;
        }
    };

}

#endif
