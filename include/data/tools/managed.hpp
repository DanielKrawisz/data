// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MANAGED
#define DATA_MANAGED

#include <data/types.hpp>

namespace data::managed {
    
    template <typename X>
    class pointer {
        ptr<X> Pointer;
        X* Value;
    public:
        const X& operator*() const {
            return *Value;
        }
        
        X& operator*() {
            return *Value;
        }
        
        const X& value() const {
            return *Value;
        }
        
        X& value() {
            return *Value;
        }
        
        pointer() : Pointer{nullptr}, Value{nullptr} {}
        pointer(X* p) : Pointer{p}, Value{&*Pointer} {}
        pointer(const X x) : pointer{new X{x}} {}
        pointer(const pointer& m) : Pointer{m.Pointer}, Value{m.Value} {}
        pointer(pointer&& m) : Pointer{m.Pointer}, Value{m.Data} {
            m.Pointer = {};
        }
        
        pointer& operator=(const pointer& m) {
            Pointer = m.Pointer;
            Value = m.Value;
            return *this;
        }
        
        bool operator==(const pointer& p) const {
            if (Value == p.Value) return true;
            return *Value == *p.Value;
        }
        
        bool operator!=(const pointer& p) const {
            return !operator==(p);
        }
        
    protected:
        X* data() {
            return Value;
        }
    };
    
    template <typename X, typename x, typename iter, typename const_iter>
    struct indexed : public pointer<X> {        
        x operator[](uint32 n) {
            return pointer<X>::value()[n];
        }
        
        const x operator[](uint32 n) const {
            return pointer<X>::value()[n];
        }
        
        iter begin() {
            return pointer<X>::value().begin();
        }
        
        const_iter begin() const {
            return pointer<X>::value().begin();
        }
        
        iter end() {
            return pointer<X>::value().end();
        }
        
        const_iter end() const {
            return pointer<X>::value().end();
        }
        
        indexed() : pointer<X>{} {}
        indexed(X* p) : pointer<X>{p} {}
        indexed(const X& d) : pointer<X>{d} {}
        indexed(const indexed& m) : pointer<X>{static_cast<const pointer<X>&>(m)} {}
        indexed(indexed&& m) : pointer<X>{static_cast<pointer<X>&&>(m)} {}
        
        indexed& operator=(const indexed& m) {
            pointer<X>::operator=(static_cast<const pointer<X>&>(m));
            return *this;
        }
        
        bool operator==(const indexed& p) const {
            return pointer<X>::operator==(static_cast<const pointer<X>&>(p));
        }
        
        bool operator!=(const indexed& p) const {
            return pointer<X>::operator!=(static_cast<const pointer<X>&>(p));
        }
    };
    
    template <typename x, uint32 size>
    using array = indexed<std::array<x, size>, x,
        typename std::array<x, size>::iterator,
        typename std::array<x, size>::const_iterator>;
    
    template <uint32 size>
    using bytes = array<byte, size>;
    
}

#endif

