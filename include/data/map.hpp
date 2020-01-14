#ifndef DATA_MAP_HPP
#define DATA_MAP_HPP

#include "types.hpp"
#include "list.hpp"
#include "set.hpp"
    
namespace data {

    template <typename K, typename V>
    struct entry {
        const K Key;
        V Value;
        
        entry(const K k, V v) : Key(k), Value(v) {}
        
        // an invalid instance must exist for type value.
        entry(const K k) : Key(k), Value{} {}
        
        bool valid() const {
            return Value != V{};
        }
        
        const K key() const {
            return Key;
        }
        
        V value() const {
            return Value;
        }
        
        V operator[](const K k) {
            if (Key == k) return Value;
            return V{};
        }
        
        // Sorting is done with keys only. 
        bool operator<(const entry e) {
            return Key < e.Key;
        }
        
        bool operator==(const entry e) {
            return Key == e.Key;
        }
        
        bool operator<(const entry e) const {
            return Key < e.Key;
        }
        
        bool operator>(const entry e) const {
            return Key > e.Key;
        }
        
        bool operator<=(const entry e) const {
            return Key < e.Key;
        }
        
        bool operator>=(const entry e) const {
            return Key > e.Key;
        }
    };
    
    namespace meta {
        
        template <typename map, typename seq>
        class has_keys_method {
            template <typename X> static auto test(int) -> typename 
                std::enable_if<std::is_same<decltype(std::declval<X>().keys()), seq>::value, yes>::type;
            template <typename> static no test(...);
        public:
            static constexpr bool value = std::is_same<decltype(test<map>(0)), yes>::value;
        };
        
    }
    
    namespace interface {
        
        template <typename M>
        struct map {
            using values = typename container<M>::values;
            using keys = decltype(std::declval<M>().keys());
            using key = decltype(std::declval<keys>().first());
            using value = typename indexed<M, key>::value;
            using element = typename sequence<values>::element;
            // TODO ensure element is entry<key, value>
        private:
            using require_default_constructable = typename std::enable_if<std::is_default_constructible<M>::value, void>::type;
            using require_insert_method = typename std::enable_if<
                meta::has_insert_method<M, key, value>::value, void>::type;
            using require_remove_method = typename std::enable_if<
                meta::has_remove_method<M, key>::value, void>::type;
        }; 
    }
    
}

#endif
