// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_FOR_EACH
#define DATA_FOR_EACH

#include <data/list/for_each.hpp>
#include <data/queue/for_each.hpp>
#include <data/tree/for_each.hpp>
#include <data/map.hpp>
#include <data/meta/which.hpp>
#include <data/meta/equal.hpp>
#include <data/tools/linked_stack.hpp>
#include <data/tools/linked_tree.hpp>
#include <data/tools/rb_map.hpp>
#include <data/tools/functional_queue.hpp>
#include <data/tools/entry_function.hpp>
#include <data/tools/iterator_list.hpp>

namespace data::meta::functional {
    using namespace data::functional;
    
    // we always know how to construct linked lists,
    // so we can always form a list::for_each
    // type using a linked list as the return type. 
    /*template <typename function, typename input> 
    struct for_each_list : interface::sequence<input> {
        using input_element = typename interface::sequence<input>::element;
        using output_element = typename std::invoke_result<function, input_element>::type;
        using output = functional::stack::linked<output_element>;
        
        output operator()(const function f, const input l) const {
            return functional::list::for_each<function, input, output>{}(f, l);
        }
    };*/
    
    template <typename function, typename input> 
    struct for_each_container {
        using input_element = typename input::value_type;
        using output_element = typename std::invoke_result<function, input_element>::type;
        using output = tool::linked_stack<output_element>;
        
        output operator()(const function f, const input i) const {
            return list::for_each<function, iterator_list<decltype(i.begin()), input_element>, output>{}(f, iterator_list<decltype(i.begin()), input_element>{i.begin(), i.end()});
        }
    };
    
    template <typename function, typename input, typename proof = interface::list<input>> 
    struct for_each_queue  {
        using input_element = decltype(std::declval<input>().first());
        using output_element = typename std::invoke_result<function, input_element>::type;
        using output = tool::functional_queue<tool::linked_stack<output_element>>;
        
        output operator()(const function f, const input l) const {
            return queue::for_each<function, input, output>{}(f, l);
        }
    };
    
    template <typename function, typename input> 
    struct for_each_tree {
        using input_element = typename interface::tree<input>::element;
        using output_element = typename std::invoke_result<function, input_element>::type;
        using output = data::tool::linked_tree<output_element>;
        
        output operator()(const function f, const input l) const {
            return functional::tree::for_each<function, input, output>{}(f, l);
        }
    };
    
    template <typename function, typename input, typename key, typename value> 
    struct for_each_value {
        using input_element = typename interface::list<input>::element;
        constexpr static Equal<input_element, entry<key, value>> required{};
        using inner_function = entry_function<key, function, value>;
        using output_element = typename std::invoke_result<inner_function, input_element>::type;
        
        tool::linked_stack<output_element> operator()(const function f, const input l) const {
            return for_each_queue<inner_function, input>{}(inner_function{f}, l);
        }
    };
    
    // we know how to make rb_maps, so we can
    // do for_each on a map. 
    template <typename function, typename input> 
    struct for_each_map {
        using requirement = interface::map<input>;
        constexpr static requirement Satisfied{};
        using entry = typename requirement::entry;
        using key = typename requirement::key;
        using value = typename requirement::value;
        using list = typename requirement::list;
        using outer_function = typename Which<for_each_queue<entry_function<key, function, value>, list>, for_each_queue<value_function<key, function, value>, list>>::result;
        using inner_function = decltype(outer_function::Function);
        using output_value = typename inner_function::output_element::value;
        using output = tool::rb_map<key, output_value>;
        
        output operator()(const function f, const input m) const {
            return {outer_function{}(inner_function{f}, values(m))};
        }
    };
    
}

namespace data::meta {
    
    template <typename function, typename input> 
    struct for_each {
        using inner_function = typename Which<
            functional::for_each_queue<function, input>,
            functional::for_each_map<function, input>, 
            functional::for_each_container<function, input>, 
            functional::for_each_tree<function, input>>::result;
        using output = typename inner_function::output;
        
        output operator()(const function f, const input m) const {
            return {inner_function{}(f, m)};
        }
    };
    
}

namespace data {

    template <typename f, typename d>
    inline auto for_each(const f fun, const d data) -> decltype(meta::for_each<f, d>{}(fun, data)) {
        return meta::for_each<f, d>{}(fun, data);
    }
    
}

// Examples that are also test cases. 
namespace data::meta::documentation {
    
    template <typename A, typename B, typename f, typename List>
    struct for_each_list :
        public meta::is_function<A, f, B>,
        public interface::stack<List>, 
        public meta::Equal<typename interface::stack<List>::element, A> {
        
        tool::linked_stack<B> use_case(f fun, List a) {
            return for_each(fun, a);
        }
        
    };
    
    template <typename A, typename B, typename f, typename Queue>
    struct for_each_queue :
        public meta::is_function<A, f, B>,
        public interface::list<Queue>, 
        public meta::Equal<typename interface::list<Queue>::element, A> {
        
        tool::functional_queue<tool::linked_stack<B>> use_case(f fun, Queue a) {
            return for_each(fun, a);
        }
        
    };
    
    template <typename A, typename B, typename f, typename Key, typename Map>
    struct for_each_map_1 :
        public meta::is_function<A, f, B>,
        public interface::map<Map>, 
        public meta::Equal<typename interface::map<Map>::key, Key>, 
        public meta::Equal<typename interface::map<Map>::value, A> {
        
        tool::rb_map<Key, B> use_case(f fun, Map ka) {
            return for_each(fun, ka);
        }
        
    };
    
    template <typename A, typename B, typename f, typename Key, typename Map>
    struct for_each_map_2 :
        public meta::is_function<entry<Key, A>, f, B>,
        public interface::map<Map>, 
        public meta::Equal<typename interface::map<Map>::key, Key>, 
        public meta::Equal<typename interface::map<Map>::value, A> {
        
        tool::rb_map<Key, B> use_case(f fun, Map ka) {
            return for_each(fun, ka);
        }
        
    };
    
    template <typename A, typename B, typename f, typename T>
    struct for_each_tree :
        public meta::is_function<A, f, B>,
        public interface::tree<T> {
        
        data::tool::linked_tree<B> use_case(f fun, T t) {
            return  for_each(fun, t);
        }
        
    };
    
}

#endif
