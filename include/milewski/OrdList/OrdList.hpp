#ifndef MILEWSKI_OKASAKI_ORDLIST
#define MILEWSKI_OKASAKI_ORDLIST

#include <cassert>
#include <memory>
#include <initializer_list>

namespace milewski::okasaki {
    template<class T>
    // requires Ord<T>
    struct OrdList
    {
        struct Item
        {
            Item(T v, std::shared_ptr<const Item> const & tail) : _val(v), _next(tail) {}
            T _val;
            std::shared_ptr<const Item> _next;
        };
        friend Item;
        explicit OrdList(std::shared_ptr<const Item> const & items) : _head(items) {}
        
        // Empty list
        OrdList() : _head{nullptr} {}
        // Cons
        OrdList(T v, OrdList const & tail) : _head(std::make_shared<Item>(v, tail._head))
        {
            assert(tail.isEmpty() || v <= tail.front());
        }
        bool isEmpty() const { return !_head; } // conversion to bool
        const T& front() const
        {
            assert(!isEmpty());
            return _head->_val;
        }
        OrdList popped_front() const
        {
            assert(!isEmpty());
            return OrdList(_head->_next);
        }
        // Additional utilities
        OrdList inserted(T v) const
        {
            if (isEmpty() || v <= front())
                return OrdList(v, OrdList(_head));
            else {
                return OrdList<T>(front(), popped_front().inserted(v));
            }
        }
        // For debugging
        int headCount() const { return _head.use_count(); }
        
        std::shared_ptr<const Item> _head;
        
        bool operator==(const OrdList o) const { return _head == o._head; }
        
        bool operator!=(const OrdList o) const { return _head != o._head; }
    };


    template<class T>
    OrdList<T> merged(OrdList<T> const & a, OrdList<T> const & b)
    {
        if (a.isEmpty())
            return b;
        if (b.isEmpty())
            return a;
        if (a.front() <= b.front())
            return OrdList<T>(a.front(), merged(a.popped_front(), b));
        else
            return OrdList<T>(b.front(), merged(a, b.popped_front()));
    }
}

#endif
