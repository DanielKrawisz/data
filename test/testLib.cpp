// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <gtest/gtest.h>
#include <data/data.hpp>

void constructability_tests() {
    using namespace data::exported;
    using string = std::string;
    
    ordered_list<string> test_ordered_list{};
    ordered_list<const string&> test_ordered_list_reference{};
    
    data::interface::sequence<ordered_list<string>> test_ordered_list_is_sequence{};
    data::interface::sequence<ordered_list<const string&>> test_ordered_list_ref_is_sequence{};
    
    priority_queue<string> test_priority_queue{};
    priority_queue<const string*> test_priority_queue_reference{};
    
    data::interface::sequence<priority_queue<string>> test_priority_queue_is_sequence{};
    data::interface::sequence<priority_queue<const string*>> test_priority_queue_ref_is_sequence{};
    
    list<string> test_linked_list{};
    list<const string&> test_linked_list_reference{};
    
    data::interface::list<list<string>> linked_list_is_list{};
    data::interface::const_iterable<list<string>> linked_list_is_iterable{};
    data::interface::list<list<const string&>> linked_list_ref_is_list{};
    
    queue<string> test_functional_queue{};
    queue<const string&> test_functional_queue_reference{};
    
    data::interface::queue<queue<string>> functional_queue_is_queue{};
    data::interface::queue<queue<const string&>> functional_queue_ref_is_queue{};
    
    tree<string> test_linked_tree{};
    tree<const string&> test_linked_tree_reference{};
    
    data::interface::tree<tree<string>> linked_tree_is_tree{};
    data::interface::tree<tree<const string&>> linked_tree_ref_is_tree{};
    
    /*map<string, string> test_rb_map{};
    map<const string&, string> test_rb_map_reference{};
    
    data::interface::map<map<string, string>> rb_map_is_map{};
    data::interface::map<map<const string&, string>> rb_map_ref_is_map{};
    
    set<string> test_map_set{};
    set<const string&> test_map_set_reference{};
    
    data::interface::set<set<string>> map_set_is_map{};
    data::interface::set<set<const string&>> map_set_ref_is_map{};*/
    
    // iterater_list must satisfy interface sequence. 
    
    // prove that array is array.
    // prove that bounded is array.
    // prove that N_bytes and Z_bytes is array. 
    
    // prove that bounded, N/Z_bytes, and N/Z are numbers.
    
}

int main(int argc, char *argv[]){
    constructability_tests();
    
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
