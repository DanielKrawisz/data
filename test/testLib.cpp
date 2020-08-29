// Copyright (c) 2019 Katrina Swales
// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/data.hpp>
#include <data/math/number/natural.hpp>
#include <data/math/number/integer.hpp>
#include <gtest/gtest.h>
#include "gmock/gmock.h"
#include "gmock/gmock-matchers.h"
namespace data {
    
template <sequence X>
void test_sequence(const X&) {}

    TEST(LibTest,constructability_tests) {
    
        ordered_list<string> test_ordered_list;
        ordered_list<const string&> test_ordered_list_reference;
        
        test_sequence(test_ordered_list);
        test_sequence(test_ordered_list_reference);
        
        priority_queue<string> test_priority_queue;
        priority_queue<const string*> test_priority_queue_reference;
        
        test_sequence(test_priority_queue);
        test_sequence(test_priority_queue_reference);
        
        // TODO iterater_list must satisfy interface sequence. 
        
        stack<string> test_linked_list;
        stack<const string&> test_linked_list_reference;
        
        data::interface::stack<stack<string>> linked_list_is_list;
        //data::interface::const_iterable<stack<string>> linked_list_is_iterable;
        data::interface::stack<stack<const string&>> linked_list_ref_is_list;
        
        list<string> test_functional_queue;
        list<const string&> test_functional_queue_reference;
        
        data::interface::list<list<string>> functional_queue_is_list;
        data::interface::list<list<const string&>> functional_queue_ref_is_list;
        
        tree<string> test_linked_tree;
        tree<const string&> test_linked_tree_reference;
        
        data::interface::tree<tree<string>> linked_tree_is_tree;
        data::interface::tree<tree<const string&>> linked_tree_ref_is_tree;
        
        // TODO prove that vector is linear. 
        
        //data::interface::natural<N> N_is_natural;
        data::interface::integer<Z> Z_is_integer;
        
        //data::interface::field<Q> Q_is_field;
        
        //data::interface::normed_division_algebra<Q, Q> Q_is_normed_division_algebra_over_Q;
        
        //data::interface::field<G> G_is_field;
        
        //data::interface::normed_division_algebra<G, Q> Q_is_normed_division_algebra_over_Q;
        
        //data::interface::normed_division_algebra<H, G> H_is_normed_division_algebra_over_Q;
        
        /*
        data::interface::natural<N_bytes<data::endian::little>> N_bytes_little_is_natural;
        data::interface::natural<N_bytes<data::endian::big>> N_bytes_big_is_natural;
        
        data::interface::integer<Z_bytes<data::endian::little>> Z_bytes_little_is_integer;
        data::interface::integer<Z_bytes<data::endian::big>> Z_bytes_big_is_integer;
        
        // sizes 0 through 7 covers every pattern.
        data::interface::natural<uint<0>> uint_0_is_natural;
        data::interface::integer<integer<0>> integer_0_is_integer;
        
        data::interface::natural<uint<1>> uint_1_is_natural;
        data::interface::integer<integer<1>> integer_1_is_integer;
        
        data::interface::natural<uint<2>> uint_2_is_natural;
        data::interface::integer<integer<2>> integer_2_is_integer;
        
        data::interface::natural<uint<3>> uint_3_is_natural;
        data::interface::integer<integer<3>> integer_3_is_integer;
        
        data::interface::natural<uint<4>> uint_4_is_natural;
        data::interface::integer<integer<4>> integer_4_is_integer;
        
        data::interface::natural<uint<5>> uint_5_is_natural;
        data::interface::integer<integer<5>> integer_5_is_integer;
        
        data::interface::natural<uint<6>> uint_6_is_natural;
        data::interface::integer<integer<6>> integer_6_is_integer;
        
        data::interface::natural<uint<7>> uint_7_is_natural;
        data::interface::integer<integer<7>> integer_7_is_integer;
        */
        //data::interface::field<Q, data::plus<Q>, data::times<Q>> Q_is_field;
        //data::interface::field<G, data::plus<G>, data::times<G>> G_is_field;
        
        // TODO prove that complex is linear over R. 
        
        // TODO prove that quaternions are linear over R. 
        
        // TODO prove that octonions are linear over R. 
        
        //data::interface::ring<polynomial<Z>, data::plus<polynomial<Z>>, data::times<polynomial<Z>>> polynomial_Z_is_ring;
        //data::interface::ring<polynomial<Q>, data::plus<polynomial<Q>>, data::times<polynomial<Q>>> polynomial_Q_is_ring;
        
        // TODO prove that polynomial is linear. 
        // TODO prove that polynomail is a function. 
        
        // TODO prove that prime fields are a fields. 
        
        // TODO prove that exterior algebra is linear. 

    }


}
