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

void constructability_tests() {
    
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

