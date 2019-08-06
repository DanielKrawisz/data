#ifndef DATA_MAP_HPP
#define DATA_MAP_HPP

#include "types.hpp"
#include "list.hpp"
#include "set.hpp"
    
namespace data {

    namespace map {

        template <typename key, typename value>
        struct entry {
            key Key;
            value Value;

            entry(key k, value v) : Key(k), Value(v) {}

            // an invalid instance must exist for type value.
            entry(key k) : Key(k), Value{} {}
            
            bool valid() const {
                return Value != value{};
            }

            // Sorting is done with keys only. 
            bool operator<(entry<key, value> e) {
                return Key < e.Key;
            }

            bool operator==(entry<key, value> e) {
                return Key == e.Key;
            }

            value operator[](key k) {
                if (Key == k) return Value;
                return value{};
            }
            
            bool operator<(entry<key, value> e) const {
                return Key < e.Key;
            }
            
            bool operator>(entry<key, value> e) const {
                return Key > e.Key;
            }
            
            bool operator<=(entry<key, value> e) const {
                return Key < e.Key;
            }
            
            bool operator>=(entry<key, value> e) const {
                return Key > e.Key;
            }

        };

        namespace definition {
            
            template <typename M, typename key, typename value>
            struct map : public set::definition::set<M, key> {
                value get(const M m, const key k) const {
                    return m[k];
                }

                M insert(const M m, const key k, const value v) {
                    return m.insert(k, v);
                }

                M plus(const M m, const entry<key, value> e) {
                    return m + e;
                }

                M default_constructor() const {
                    return M{};
                }

                M initializer_list_constructor(const std::initializer_list<std::pair<key, value> > init) const {
                    return M{init};
                }
            }; 

            template <typename M, typename key, typename value>
            struct map<M*, key, value> : public set::definition::set<M*, key> {
                value get(const M* m, const key k) const {
                    if (m == nullptr) return value{};
                    return m->get(k);
                }
                        
                map insert(const M* m, const key k, const value v) const {
                    if (m == nullptr) return new map{{k, v}};
                    return m->insert(k, v);
                }

                M* initializer_list_constructor(const std::initializer_list<std::pair<key, value> > init) const {
                    return new M{init};
                }
            }; 

            template <typename M, typename key, typename value>
            struct map<ptr<M>, key, value> : public set::definition::set<ptr<M>, key>, public map<M*, key, value> {}; 

            // For some kinds of maps, it's possible to count over
            // all the elements, in which case the map can be treated
            // as a list of entries. 
            template <typename M, typename key, typename value, typename L>
            struct countable : public map<M, key, value> {
                L entries(const M m) const {
                    static const list::definition::list<L, entry<key, value>> requirement{};
                    return m.entries();
                }
            }; 

            template <typename M, typename key, typename value, typename L>
            struct countable<M*, key, value, L> : public map<M*, key, value> {
                L entries(const M* m) const {
                    static const list::definition::list<L, entry<key, value>> requirement{};
                    return m->entries();
                }
            }; 

            template <typename M, typename key, typename value, typename L>
            struct countable<ptr<M>, key, value, L> : public map<ptr<M>, key, value>, public countable<M*, key, value, L> {}; 

            template <typename M, typename key>
            struct removable : public data::set::definition::removable<M, key> {};

        }
        
        template <typename M>
        struct is_map {
            using entries = typename std::invoke_result<decltype(&M::entries), M>::type;
            using required = list::is_list<entries>;
            constexpr static required Pass{};
            
            using entry = typename required::element;
            using key = decltype(entry::Key);
            using value = decltype(entry::Value);
        };
        
        template <typename M>
        struct is_map<M*> {
            using entries = typename std::invoke_result<decltype(&M::entries), M>::type;
            using required = list::is_list<entries>;
            constexpr static required Pass{};
            
            using entry = typename required::element;
            using key = decltype(entry::Key);
            using value = decltype(entry::Value);
        };
        
        template <typename M>
        struct is_map<ptr<M>> : public is_map<M*> {};
        
        // functions that can be satisfied by maps. 
        template <typename M>
        inline bool empty(M m) {
            return set::definition::existence<M>{}.empty(m);
        }

        template <typename M, typename K, typename V> 
        inline V get(M m, K k) {
            return definition::map<M, K, V>{}.get(m, k);
        }

        template <typename M, typename K, typename V> 
        inline M insert(M m, entry<K, V> e) {
            return definition::map<M, K, V>{}.insert(m, e.Key, e.Value);
        }

        template <typename M, typename K, typename V> 
        inline M insert(M m, K k, V v) {
            return definition::map<M, K, V>{}.insert(m, k, v);
        }

        template <typename M, typename K> 
        inline bool contains(M m, K k) {
            return set::definition::set<M, K>{}.contains(m, k);
        }

        template <typename M, typename K> 
        inline M remove(M m, K k) {
            return definition::removable<M, K>{}.remove(m, k);
        }
            
        template <typename M, typename K, typename V, typename L> 
        inline L entries(M m) {
            return map::definition::countable<M, K, V, L>{}.entries(m);
        }
        
    }
    
}

#endif
