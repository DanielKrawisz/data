#ifndef DATA_TOOLS_ITERATOR_LIST
#define DATA_TOOLS_ITERATOR_LIST

#include <type_traits>
#include <iterator>
#include <data/list.hpp>

namespace data {

    template <typename it, typename elem>
    struct iterator_list {

        it Next;
        it End;

        iterator_list(it n, it e) : Next{n}, End{e} {}
        constexpr static const list::definition::list<iterator_list<it, elem>, elem> is_list{};

        bool empty() const {
            return Next == End;
        }

        // if the list is empty, then this function
        // will dereference a nullptr. It is your
        // responsibility to check. 
        const elem& first() const {
            return *Next;
        }

        const iterator_list rest() const {
            if (empty()) return *this;
            auto i = iterator_list{End, Next};
            i.Next++;
            return i;
        }

        bool contains(elem x) const {
            if (empty()) return false;
                
            Next->contains(x);
        }

        bool operator==(const iterator_list<it, elem>& l) {
            if (this == &l) return true;
            return Next == l.Next && End == l.End;
        }
        
        iterator_list<it, elem> from(uint n) const {
            if (empty()) return {};
            if (n == 0) return this;
            return rest().from(n - 1);
        }

        it begin() {
            return Next;
        }
            
        it end() {
            return End;
        }
            
    };

    template <typename it, typename elem>
    inline bool empty(const data::iterator_list<it, elem> l) {
        return l.empty();
    }

    template <typename it, typename elem>
    inline const elem& first(const data::iterator_list<it, elem> l) {
        return l.first();
    }

    template <typename it, typename elem>
    inline const iterator_list<it, elem> rest(const iterator_list<it, elem> l) {
        return l.rest();
    }
    
    template <typename iterable>
    inline iterator_list<typename std::invoke_result<decltype(&iterable::begin), iterable>::type,
        typename std::iterator_traits<
            typename std::invoke_result<decltype(&iterable::begin), iterable>::type>::value_type>
        make_iterator_list(iterable i) {
        return {std::begin(i), std::end(i)};
    }

}

#endif
