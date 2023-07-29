// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_FOR_EACH
#define DATA_FOR_EACH

#include <data/tools.hpp>
#include <data/function.hpp>

namespace data {
    
    template <typename fun, typename input, 
        typename element = std::remove_const_t<std::remove_reference_t<decltype (std::declval<input> ().first ())>>, 
        typename output = std::remove_const_t<std::remove_reference_t<decltype (std::declval<fun> () (std::declval<element> ()))>>>
    requires functional::function<fun, output, element> && sequence<input, element>
    list<output> for_each (const fun& f, const input& i) {
        return fold ([&f] (list<output> q, element x) -> list<output> {
            return append (q, f (x));
        }, list<output> {}, i);
    }
    
    template <typename fun, typename input, 
        typename key = std::remove_reference_t<decltype (std::declval<input> ().values ().first ().key ())>, 
        typename value = std::remove_reference_t<decltype (std::declval<input> ().values ().first ().value ())>, 
        typename output = std::remove_reference_t<decltype (std::declval<fun> () (std::declval<value> ()))>>
    requires functional::function<fun, output, value> && functional::map<input, key, value>
    map<key, output> inline for_each (const fun& f, const input& i) {
        return fold ([&f] (map<key, output> m, const entry<key, value>& e) -> map<key, output> {
            return m.insert (e.Key, f (e.Value));
        }, map<key, output> {}, i.values ());
    }
    
    template <typename fun, typename input, 
        typename element = std::remove_reference_t<decltype(std::declval<input>().values().first())>, 
        typename output = std::remove_reference_t<decltype(std::declval<fun>()(std::declval<element>()))>>
    requires functional::function<fun, output, element> && functional::ordered_set<input, element>
    list<output> inline for_each (const fun &f, const input &i) {
        return fold ([&f] (list<output> q, element x) -> list<output> {
            return append(q, f (x));
        }, list<output> {}, i.values ());
    }
    /*
    template <typename fun, typename input, 
        typename element = std::remove_reference_t<decltype(std::declval<input>().values().first())>, 
        typename output = std::remove_reference_t<decltype(std::declval<fun>()(std::declval<element>()))>>
    requires functional::function<fun, output, element> && functional::tree<input, element>
    tree<output> inline for_each(const fun& f, const input& i) {
        if (empty(i)) return {};
        return {f(root(i)), for_each(f, left(i)), for_each(f, right(i))};
    }*/
    
    template <typename fun, typename element, 
        typename output = std::remove_reference_t<decltype (std::declval<fun> () (std::declval<element> ()))>>
    requires functional::function<fun, output, element>
    inline tree<output> for_each (const fun &f, const tree<element> &t) {
        if (empty (t)) return {};
        
        return {f (root (t)), for_each (f, left (t)), for_each (f, right (t))};
    }
    
    template <typename fun, typename element, 
        typename output = std::remove_reference_t<decltype (std::declval<fun> () (std::declval<element> ()))>>
    requires functional::function<fun, output, element>
    inline cross<output> for_each (const fun &f, const cross<element> &i) {
        cross<output> z;
        z.resize (i.size ());
        auto a = i.begin ();
        auto b = z.begin ();
        
        while (a != i.end ()) {
            *b = f(*a);
            a++;
            b++;
        }
        
        return z;
    }
    
}

#endif
