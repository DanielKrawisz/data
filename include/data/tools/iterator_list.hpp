#ifndef DATA_TOOLS_ITERATOR_LIST_HPP
#define DATA_TOOLS_ITERATOR_LIST_HPP

#include <type_traits>
#include <data/list.hpp>

namespace data {
        
    template <typename it>
    class iterator_list {
        
        it Next;
        it End;

        iterator_list(it n, it e) : Next{n}, End{e} {}
        
    public:
        using Element = typename it::value_type;
        constexpr static const list::definition::list<iterator_list<it>, Element> is_list{};
            
        bool empty() const {
            return Next == End;
        }
            
        // if the list is empty, then this function
        // will dereference a nullptr. It is your
        // responsibility to check. 
        const Element& first() const {
            return *Next;
        }
            
        const iterator_list rest() const {
            if (empty()) return *this;
                
            return iterator_list{End, Next++};
        }
                
        bool contains(Element x) const {
            if (empty()) return false;
                
            Next->contains(x);
        }
            
        bool operator==(const iterator_list<it>& l) {
            if (this == &l) return true;
            return Next == l.Next && End == l.End;
        }
        
        iterator_list<it> from(uint n) const {
            if (empty()) return {};
            if (n == 0) return this;
            return rest().from(n - 1);
        }
        
        template <typename container>
        explicit iterator_list(container c) : iterator_list(std::begin(c), std::end(c)) {}
            
        it begin() {
            return Next;
        }
            
        it end() {
            return End;
        }
            
    };
    
    template <typename container>
    struct is_iterable {
        using iterator = typename std::invoke_result<decltype(&container::begin), container>::type;
        using element = typename std::remove_reference<typename std::invoke_result<decltype(&iterator::operator*), iterator>::type>::type;
        using list = iterator_list<iterator>;
    };
    
    template <typename container>
    inline typename is_iterable<container>::iterator_list make_iterator_list(container c) {
        return iterator_list<typename is_iterable<container>::iterator>{c};
    }

    template <typename it>
    inline bool empty(const data::iterator_list<it> l) {
        return l.empty();
    }

    template <typename it>
    inline const typename it::value_type& first(const data::iterator_list<it> l) {
        return l.first();
    }

    template <typename it>
    inline const data::iterator_list<it> rest(const data::iterator_list<it> l) {
        return l.rest();
    }

}

#endif
