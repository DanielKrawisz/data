// Copyright (c) 2019-2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/net/TCP.hpp"
#include "gtest/gtest.h"

namespace data::net::IP {

    // https://en.wikipedia.org/wiki/Module:IPAddress/testcases
    TEST (IPTest, TestIPAddressFormat) {
            
        struct test_case {
            string Address;
            uint32 Version;
            
            void test () const {
                address ip_address (Address);
                uint32 version = ip_address.version ();
                EXPECT_EQ (version, Version) << "expected " << Address << " to be version " << Version;
                
                uint16 port = 666;
                
                std::stringstream v6_format;
                std::stringstream v4_format;
                
                v6_format << "[" << Address << "]:" << port;
                v4_format << Address << ":" << port;
                
                TCP::endpoint v6_string {v6_format.str ()};
                TCP::endpoint v4_string {v4_format.str ()};
                
                EXPECT_EQ (v6_string.valid (), version == 6) << v6_string;
                EXPECT_EQ (v4_string.valid (), version == 4) << v4_string;
            }
        };
        
        test_case test_cases[] {
            {"200.200.200.200", 4},
            // whitespace not currently allowed
            {" 200.200.200.200", 0}, 
            // whitespace not currently allowed
            {"200.200.200.200 ", 0},
            {"200.200.256.200", 0},
            {"200.200.200.200.", 0},
            {"200.200.200", 0},
            {"200.200.200.2d0", 0},
            {"0.0.0.0", 4},
            // according to talkpage, leading zeroes unacceptable.
            {"00.00.00.00", 0}, 
            // according to talkpage, leading zeroes unacceptable.
            {"100.100.020.100", 0},  
            {"255.255.255.255", 4},
            {"-1.0.0.0", 0},
            {"200000000000000000000000000000000000000000000000000000000000000000000000000000.200.200.200", 0},
            {"00000000000005.10.10.10", 0},
            // full length
            {"00AB:0002:3008:8CFD:00AB:0002:3008:8CFD", 6},
            // lowercase
            {"00ab:0002:3008:8cfd:00ab:0002:3008:8cfd", 6},
            // mixed case
            {"00aB:0002:3008:8cFd:00Ab:0002:3008:8cfD", 6},
            // at most 4 digits per segment
            {"00AB:00002:3008:8CFD:00AB:0002:3008:8CFD", 0}, 
            // can"t remove all 0s from first segment unless using ::
            {":0002:3008:8CFD:00AB:0002:3008:8CFD", 0}, 
            // can"t remove all 0s from last segment unless using ::
            {"00AB:0002:3008:8CFD:00AB:0002:3008:", 0},
            // abbreviated
            {"AB:02:3008:8CFD:AB:02:3008:8CFD", 6}, 
            // too long
            {"AB:02:3008:8CFD:AB:02:3008:8CFD:02", 0},
            // correct use of ::
            {"AB:02:3008:8CFD::02:3008:8CFD", 6},
            // too long
            {"AB:02:3008:8CFD::02:3008:8CFD:02", 0},
            // can"t have two ::s
            {"AB:02:3008:8CFD::02::8CFD", 0}, 
            // Invalid character G
            {"GB:02:3008:8CFD:AB:02:3008:8CFD", 0}, 
            // unassigned IPv6 address
            {"::", 6},                                       
            // loopback IPv6 address
            {"::1", 6}, 
            // another name for unassigned IPv6 address
            {"0::", 6},
            // another name for unassigned IPv6 address
            {"0::0", 6},
            // illegal: three colons
            {"2:::3", 0},
            // full form of IPv6
            {"fe80:0000:0000:0000:0204:61ff:fe9d:f156", 6},
            // drop leading zeroes
            {"fe80:0:0:0:204:61ff:fe9d:f156", 6}, 
            // collapse multiple zeroes to :: in the IPv6 address
            {"fe80::204:61ff:fe9d:f156", 6},
            // IPv4 dotted quad at the end
            {"fe80:0000:0000:0000:0204:61ff:254.157.241.86", 6},
            // drop leading zeroes, IPv4 dotted quad at the end
            {"fe80:0:0:0:0204:61ff:254.157.241.86", 6}, 
            // dotted quad at the end, multiple zeroes collapsed
            {"fe80::204:61ff:254.157.241.86", 6}, 
            // link-local prefix
            {"fe80::", 6}, 
            // global unicast prefix
            {"2001::", 6} 
        };
        
        for (const test_case &t : test_cases) t.test ();
    }
        
}
