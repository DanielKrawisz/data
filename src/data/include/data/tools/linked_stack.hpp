// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_LINKED_STACK
#define DATA_TOOLS_LINKED_STACK

#include <ostream>
#include <data/functional/stack.hpp>
    
namespace data {

    template <Copyable elem> class linked_stack;
    
    template <typename elem> bool empty (const linked_stack<elem> &x);
    template <typename elem> size_t size (const linked_stack<elem> &x);

    // stack two stacks together.
    template <typename elem> linked_stack<elem> operator + (linked_stack<elem>, linked_stack<elem>);

    // if elem has a << operator, then we can print the whole stack
    template <typename elem> requires requires (std::ostream &o, const elem &e) {
        { o << e } -> Same<std::ostream &>;
    } std::ostream inline &operator << (std::ostream &o, const linked_stack<elem> &x);

    template <typename elem> linked_stack<elem> values (const linked_stack<elem> &x);

    template <Copyable elem>
    class linked_stack {
        
        using node = functional::stack_node<elem, linked_stack>;
        using next = ptr<node>;
        
        next Next;
        linked_stack (next n);
        
    public:
        constexpr linked_stack ();
        explicit linked_stack (inserted<elem> e, const linked_stack &l);
        explicit linked_stack (inserted<elem> e);
        
        linked_stack (std::initializer_list<wrapped<elem>> init): linked_stack {} {
            for (int i = init.size () - 1; i >= 0; i--) *this = prepend (*(init.begin () + i));
        }
        
        // if the list is empty, then this function
        // will dereference a nullptr. It is your
        // responsibility to check. 
        const elem &first () const;
        
        elem &first ();

        // if elem is a reference, then const elem & and elem & are both simply elem
    
        bool empty () const;
        
        linked_stack rest () const;
        
        bool valid () const;
        
        bool contains (elem x) const;
        
        size_t size () const;
        
        linked_stack operator >> (inserted<elem> x) const;
        linked_stack &operator >>= (inserted<elem> x);
        
        linked_stack prepend (inserted<elem> x) const;
        
        template <typename X, typename Y, typename ... P>
        linked_stack prepend (X x, Y y, P ... p) const;
        
        linked_stack operator ^ (linked_stack l) const;
        
        linked_stack from (uint32 n) const;
        
        elem &operator [] (size_t n);
        const elem &operator [] (size_t n) const;

        template <Sequence X> requires std::equality_comparable_with<elem, decltype (std::declval<X> ().first ())>
        bool operator == (const X &x) const;

        // automatic conversions
        template <typename X> requires ImplicitlyConvertible<elem, X>
        operator linked_stack<X> () const;

        // explicit conversions
        template <typename X> requires ExplicitlyConvertible<elem, X>
        explicit operator linked_stack<X> () const;

        template <typename L, typename V>
        struct it {

            using value_type        = unref<V>;
            using difference_type   = int;
            using pointer           = value_type *;
            using reference         = value_type &;
            using iterator_category = std::forward_iterator_tag;

            bool      operator == (const it &i) const;

            reference operator *  () const;
            pointer   operator -> () const;
            it       &operator ++ ();    // pre-increment
            it        operator ++ (int); // post-increment

            it (): Stack {nullptr}, Next {nullptr} {}
            it (L *st, next n): Stack {st}, Next {n} {}

        private:
            L *Stack;
            next Next;
        };

        using iterator = it<linked_stack<elem>, elem>;
        
        using const_iterator = it<const linked_stack<elem>, const elem>;

        iterator begin ();
        iterator end ();
        
        const_iterator begin () const;
        const_iterator end () const;
        
    };

    template <typename elem> linked_stack<elem> operator + (linked_stack<elem> a, linked_stack<elem> b) {
        return join (a, b);
    }

    template <typename elem> bool inline empty (const linked_stack<elem> &x) {
        return x.empty ();
    }

    template <typename elem> linked_stack<elem> inline values (const linked_stack<elem> &x) {
        return x;
    }

    template <typename elem> size_t inline size (const linked_stack<elem> &x) {
        return x.size ();
    }
    
    template <typename elem> requires requires (std::ostream &o, const elem &e) {
        { o << e } -> Same<std::ostream &>;
    } std::ostream inline &operator << (std::ostream &o, const linked_stack<elem> &x) {
        return functional::write (o << "stack ", x);
    }
    
    template <Copyable elem>
    inline linked_stack<elem>::linked_stack (next n) : Next {n} {}
    
    template <Copyable elem>
    constexpr inline linked_stack<elem>::linked_stack () : Next {nullptr} {}
    
    template <Copyable elem>
    inline linked_stack<elem>::linked_stack (inserted<elem> e, const linked_stack &l) : linked_stack {std::make_shared<node> (e, l)} {}
    
    template <Copyable elem>
    inline linked_stack<elem>::linked_stack (inserted<elem> e) : linked_stack {e, linked_stack {}} {}
    
    // if the list is empty, then this function
    // will dereference a nullptr. It is your
    // responsibility to check. 
    template <Copyable elem>
    const elem inline &linked_stack<elem>::first () const {
        if (Next == nullptr) throw empty_sequence_exception {};
        return Next->First;
    }
    
    template <Copyable elem>
    elem inline &linked_stack<elem>::first () {
        if (Next == nullptr) throw empty_sequence_exception {};
        return Next->First;
    }
    
    template <Copyable elem>
    bool inline linked_stack<elem>::empty () const {
        return Next == nullptr;
    }
    
    template <Copyable elem>
    linked_stack<elem> inline linked_stack<elem>::rest () const {
        if (empty ()) return {};
        
        return Next->rest ();
    }
    
    template <Copyable elem>
    inline bool linked_stack<elem>::valid () const {
        if (empty ()) return true;
        if (!data::valid (first ())) return false;
        return rest ().valid ();
    }
    
    template <Copyable elem>
    bool inline linked_stack<elem>::contains (elem x) const {
        if (empty ()) return false;
            
        return Next->contains (x);
    }
    
    template <Copyable elem>
    inline size_t linked_stack<elem>::size () const {
        if (empty ()) return 0;
            
        return Next->size ();
    }
    
    template <Copyable elem>
    linked_stack<elem> inline linked_stack<elem>::operator >> (inserted<elem> x) const {
        return linked_stack {x, *this};
    }
    
    template <Copyable elem>
    linked_stack<elem> inline linked_stack<elem>::prepend (inserted<elem> x) const {
        return linked_stack {x, *this};
    }
    
    template <Copyable elem>
    linked_stack<elem> inline &linked_stack<elem>::operator >>= (inserted<elem> x) {
        return *this = (prepend (x));
    }

    template <Copyable elem>
    template <typename X, typename Y, typename ... P>
    linked_stack<elem> inline linked_stack<elem>::prepend (X x, Y y, P ... p) const {
        return prepend (x).prepend (y, p...);
    }
    
    template <Copyable elem>
    linked_stack<elem> inline linked_stack<elem>::operator ^ (linked_stack l) const {
        return prepend (l);
    }
    
    template <Copyable elem>
    linked_stack<elem> linked_stack<elem>::from (uint32 n) const {
        if (empty ()) return {};
        if (n == 0) return *this;
        return rest ().from (n - 1);
    }
    
    template <Copyable elem>
    const elem inline &linked_stack<elem>::operator [] (size_t n) const {
        return drop (*this, n).first ();
    }
    
    template <Copyable elem>
    elem inline &linked_stack<elem>::operator [] (size_t n) {
        return drop (*this, n).first ();
    }

    template <Copyable elem>
    linked_stack<elem>::const_iterator inline linked_stack<elem>::begin () const {
        return const_iterator {this, Next};
    }

    template <Copyable elem>
    linked_stack<elem>::const_iterator inline linked_stack<elem>::end () const {
        return const_iterator {this, nullptr};
    }

    template <Copyable elem>
    linked_stack<elem>::iterator inline linked_stack<elem>::begin () {
        return iterator {this, Next};
    }

    template <Copyable elem>
    linked_stack<elem>::iterator inline linked_stack<elem>::end () {
        return iterator {this, nullptr};
    }

    template <Copyable elem>
    template <Sequence X> requires std::equality_comparable_with<elem, decltype (std::declval<X> ().first ())>
    bool inline linked_stack<elem>::operator == (const X &x) const {
        return sequence_equal (*this, x);
    }

    template <Copyable elem>
    template <typename X> requires ImplicitlyConvertible<elem, X>
    inline linked_stack<elem>::operator linked_stack<X> () const {
        if (size () == 0) return linked_stack<X> {};
        return linked_stack<X> {rest ()}.prepend (X (first ()));
    }

    template <Copyable elem>
    template <typename X> requires ExplicitlyConvertible<elem, X>
    inline linked_stack<elem>::operator linked_stack<X> () const {
        if (size () == 0) return linked_stack<X> {};
        return linked_stack<X> {rest ()}.prepend (X (first ()));
    }

    template <Copyable elem>
    template <typename L, typename V>
    bool inline linked_stack<elem>::it<L, V>::operator == (const it &i) const {
        return Stack == i.Stack && Next == i.Next;
    }

    template <Copyable elem>
    template <typename L, typename V>
    linked_stack<elem>::it<L, V>::reference inline linked_stack<elem>::it<L, V>::operator * () const {
        return Next->First;
    }

    template <Copyable elem>
    template <typename L, typename V>
    linked_stack<elem>::it<L, V>::pointer inline linked_stack<elem>::it<L, V>::operator -> () const {
        return &Next->First;
    }

    template <Copyable elem>
    template <typename L, typename V>
    linked_stack<elem>::it<L, V> inline &linked_stack<elem>::it<L, V>::operator ++ () {
        if (Next != nullptr) Next = Next->Rest.Next;
        return *this;
    }

    template <Copyable elem>
    template <typename L, typename V>
    linked_stack<elem>::it<L, V> inline linked_stack<elem>::it<L, V>::operator ++ (int) {
        it n = *this;
        ++(*this);
        return n;
    }

}

#endif
