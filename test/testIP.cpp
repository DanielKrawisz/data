// Copyright (c) 2019-2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/net/URL.hpp"
#include "data/encoding/hex.hpp"
#include "gtest/gtest.h"
#include <regex>

namespace data::net::IP {

    TEST (IPTest, TestIPFormats) {

        EXPECT_EQ (protocol {"FTP"}, protocol {"ftp"});
        EXPECT_EQ (domain_name {"news.cat.web"}, domain_name {"NEWS.cAt.WEB"});

        EXPECT_TRUE (protocol {"lgbtq++"}.valid ());
        EXPECT_FALSE (protocol {"122"}.valid ());
        EXPECT_FALSE (protocol {"abc&@"}.valid ());

    }

    TEST (IPTest, TestDomainNameFormat) {

        struct test_case {
            domain_name DomainName;
            bool ExpectValid;
        };

        for (const test_case &t : list<test_case> {
            {"example.com", true},  // simple domain
            {"www.example.com", true},  // with subdomain
            {"subdomain.example.co.uk", true},  // multiple subdomains
            {"example123.com", true},  // numeric characters in domain
            {"example-name.com", true},  // hyphen in domain
            {"example_name.org", false},  // underscore in domain
            {"a.b.c.example.net", true},  // multiple subdomains

            {"example..com", false},  // consecutive periods
            {"-example.com", false},  // starts with a hyphen
            {"example-.com", true},  // ends with a hyphen
            {"example..co.uk", false},  // consecutive periods
            {"example._com", false},  // starts with an underscore
            {"example_.com", false},  // ends with an underscore
            {".example.com", false},  // starts with a period
            {"example.com.", false},  // ends with a period
        }) {
            if (t.ExpectValid) {
                EXPECT_TRUE (t.DomainName.valid ()) << "expected " << t.DomainName << " to be a valid domain, but it is not.";
            } else {
                EXPECT_FALSE (t.DomainName.valid ()) << "expected " << t.DomainName << " to be an invalid domain, but it is valid.";
            }
        }

    }

    std::regex ip_v4_regex {"(((25[0-5]|(2[0-4]|1\\d|[1-9]|)\\d)\\.?\\b){4})"};
    std::regex ip_v6_regex {"((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:)))(%.+)?"};

    // https://en.wikipedia.org/wiki/Module:IPAddress/testcases
    TEST (IPTest, TestIPAddressFormat) {

        struct test_case {
            address Address;
            int32 Version;
            string Bytes;

            void test () const {

                if (Version == 4) EXPECT_TRUE (std::regex_match (Address, ip_v4_regex));
                else if (Version == 6) EXPECT_TRUE (std::regex_match (Address, ip_v6_regex));

                EXPECT_EQ (Address.valid (), Version >= 0)
                    << "Address " << Address << " is" << (Version >= 0 ? " " : " not ") << "expected to be valid; version = " << Version;

                int32 version = Address.version ();
                EXPECT_EQ (version, Version) << "expected " << Address << " to be version " << Version;

                uint16 port = 666;
                
                std::stringstream v6_format;
                std::stringstream v4_format;
                
                v6_format << "tcp://[" << static_cast<std::string> (Address) << "]:" << port;
                v4_format << "tcp://" << static_cast<std::string> (Address) << ":" << port;

                TCP::endpoint v6 {v6_format.str ()};
                TCP::endpoint v4 {v4_format.str ()};
                
                EXPECT_EQ (v6.valid (), version == 6) << v6;
                EXPECT_EQ (v4.valid (), version == 4) << v4;

                EXPECT_EQ (bytes (Address), *encoding::hex::read (Bytes));

            }
        };

        test_case test_cases[] {
            {"200.200.200.200", 4, "c8c8c8c8"},
            // whitespace not currently allowed
            {" 200.200.200.200", -1, ""},
            // whitespace not currently allowed
            {"200.200.200.200 ", -1, ""},
            {"200.200.256.200", -1, ""},
            {"200.200.200.200.", -1, ""},
            {"200.200.200", -1, ""},
            {"200.200.200.2d0", -1, ""},
            {"0.0.0.0", 4, "00000000"},
            // according to talkpage, leading zeroes unacceptable.
            {"00.00.00.00", -1, ""},
            // according to talkpage, leading zeroes unacceptable.
            {"100.100.020.100", -1, ""},
            {"255.255.255.255", 4, "ffffffff"},
            {"-1.0.0.0", -1, ""},
            {"200000000000000000000000000000000000000000000000000000000000000000000000000000.200.200.200", -1, ""},
            {"00000000000005.10.10.10", -1, ""},
            // full length
            {"00AB:0002:3008:8CFD:00AB:0002:3008:8CFD", 6, "00AB000230088CFD00AB000230088CFD"},
            // lowercase
            {"00ab:0002:3008:8cfd:00ab:0002:3008:8cfd", 6, "00AB000230088CFD00AB000230088CFD"},
            // mixed case
            {"00aB:0002:3008:8cFd:00Ab:0002:3008:8cfD", 6, "00AB000230088CFD00AB000230088CFD"},
            // at most 4 digits per segment
            {"00AB:00002:3008:8CFD:00AB:0002:3008:8CFD", -1, ""},
            // can"t remove all 0s from first segment unless using ::
            {":0002:3008:8CFD:00AB:0002:3008:8CFD", -1, ""},
            // can"t remove all 0s from last segment unless using ::
            {"00AB:0002:3008:8CFD:00AB:0002:3008:", -1, ""},
            // abbreviated
            {"AB:02:3008:8CFD:AB:02:3008:8CFD", 6, "00AB000230088CFD00AB000230088CFD"},
            // too long
            {"AB:02:3008:8CFD:AB:02:3008:8CFD:02", -1, ""},
            // correct use of ::
            {"AB:02:3008:8CFD::02:3008:8CFD", 6, "00AB000230088CFD0000000230088CFD"},
            // too long
            {"AB:02:3008:8CFD::02:3008:8CFD:02", -1, ""},
            // can"t have two ::s
            {"AB:02:3008:8CFD::02::8CFD", -1, ""},
            // Invalid character G
            {"GB:02:3008:8CFD:AB:02:3008:8CFD", -1, ""},
            // unassigned IPv6 address
            {"::", 6, "00000000000000000000000000000000"},
            // loopback IPv6 address
            // could be 0 to 7.
            {"::1", 6, "00000000000000000000000000000001"},
            // another name for unassigned IPv6 address
            {"0::", 6, "00000000000000000000000000000000"},
            // another name for unassigned IPv6 address
            {"0::0", 6, "00000000000000000000000000000000"},
            // illegal: three colons
            {"2:::3", -1, ""},
            // full form of IPv6
            {"fe80:0000:0000:0000:0204:61ff:fe9d:f156", 6, "fe80000000000000020461fffe9df156"},
            // drop leading zeroes
            {"fe80:0:0:0:204:61ff:fe9d:f156", 6,"fe80000000000000020461fffe9df156"},
            // collapse multiple zeroes to :: in the IPv6 address
            {"fe80::204:61ff:fe9d:f156", 6, "fe80000000000000020461fffe9df156"},
            // IPv4 dotted quad at the end"fe80000000000000020461fffe9df156"
            {"fe80:0000:0000:0000:0204:61ff:254.157.241.86", 6, "fe80000000000000020461fffe9df156"},
            // drop leading zeroes, IPv4 dotted quad at the end
            {"fe80:0:0:0:0204:61ff:254.157.241.86", 6, "fe80000000000000020461fffe9df156"},
            // dotted quad at the end, multiple zeroes collapsed
            {"fe80::204:61ff:254.157.241.86", 6, "fe80000000000000020461fffe9df156"},
            // link-local prefix
            {"fe80::", 6, "fe800000000000000000000000000000"},
            // global unicast prefix
            {"2001::", 6, "20010000000000000000000000000000"}
        };
        
        for (const test_case &t : test_cases) t.test ();
    }
}

namespace data::encoding::percent {

    TEST (IPTest, TestPercentEncoding) {

        struct positive_test_case {
            data::UTF8 Valid;
            string Additional;
            data::encoding::percent::string Expected;
        };

        struct negative_test_case {
            string Invalid;
        };

        for (const positive_test_case &tt : list<positive_test_case> {{
            "Hello, World! Привет, мир!", "!,.",
            R"(Hello%2C%20World%21%20%D0%9F%D1%80%D0%B8%D0%B2%D0%B5%D1%82%2C%20%D0%BC%D0%B8%D1%80%21)"
        }, {"example.com/path?query=value#fragment", "/?=#",
            "example.com%2Fpath%3Fquery%3Dvalue%23fragment"

        }}) {
            auto encoded = encode (tt.Valid, tt.Additional);
            EXPECT_EQ (encoded, tt.Expected) << "expected " << encoded << " == " << tt.Expected;
            EXPECT_EQ (*percent::decode (encoded), tt.Valid);
        }

        for (const negative_test_case &tt : list<negative_test_case> {
            {R"(Hello%2GWorld)"},
            {R"(Hello%)"},
            {R"(Hello )"}
        }) {
            EXPECT_FALSE (bool (percent::decode (tt.Invalid)));
        }

        struct test_equal {
            string Left;
            string Right;
            bool Expected;
        };

        for (const test_equal &tt : list<test_equal> {
            {"abc", "abc", true},       // No percent encoding
            {"abc+", "abc%2B", false},  // '+' is a reserved character
            {"abc/", "abc%2F", false},  // '/' is a reserved character
            {"abc?", "abc%3F", false},  // '?' is a reserved character
            {"abc#", "abc%23", false},  // '#' is a reserved character
            {"abc[", "abc%5B", false},  // '[' is a reserved character
            {"abc]", "abc%5D", false},  // ']' is a reserved character
            {"abc@", "abc%40", false},  // '@' is a reserved character
            {"abc!", "abc%21", false},  // '!' is a reserved character
            {"abc$", "abc%24", false},  // '$' is a reserved character
            {"abc&", "abc%26", false}   // '&' is a reserved character
        }) {
            EXPECT_EQ (tt.Left == tt.Right, tt.Expected)
                << "expected " << tt.Left << " " << (tt.Expected ? "==" : "!=") << " " << tt.Right;
        }
    }

}

namespace data::net {

    TEST (IPTest, TestURLFormat) {

        struct positive_test_case {

            net::URL URL;

            data::ASCII Scheme;
            maybe<data::ASCII> Authority;
            data::ASCII Path;
            maybe<data::ASCII> Query;
            maybe<data::ASCII> Fragment;

            maybe<data::ASCII> UserInfo;
            maybe<data::ASCII> Host;
            maybe<data::ASCII> Port;
            maybe<list<entry<data::UTF8, data::UTF8>>> QueryMap;

            maybe<uint16> PortNumber;
            data::ASCII PortDNS;
            maybe<domain_name> HostDNS;
            maybe<IP::address> HostAddress;

        };

        // valid URLs.
        for (const positive_test_case &tt : list<positive_test_case> {{
                "ftp://ftp.is.co.za/rfc/rfc1808.txt",
                "ftp", {"ftp.is.co.za"}, "/rfc/rfc1808.txt", {}, {},
                {}, "ftp.is.co.za", {}, {},
                {}, "ftp", {"ftp.is.co.za"}, {}
            }, {
                "http://www.ietf.org/rfc/rfc2396.txt",
                "http", {"www.ietf.org"}, "/rfc/rfc2396.txt", {}, {},
                {}, "www.ietf.org", {}, {},
                {}, "http", {"www.ietf.org"}, {}
            }, {
                "ldap://[2001:db8::7]/c=GB?objectClass?one",
                "ldap", "[2001:db8::7]", "/c=GB", "objectClass?one", {},
                {}, "[2001:db8::7]", {}, {},
                {}, "ldap", {}, {"2001:db8::7"}
            }, {
                "mailto:John.Doe@example.com",
                "mailto", {}, "John.Doe@example.com", {}, {},
                {}, {}, {}, {},
                {}, "mailto", {}, {}
            }, {
                "news:comp.infosystems.www.servers.unix",
                "news", {}, "comp.infosystems.www.servers.unix", {}, {},
                {}, {}, {}, {},
                {}, "news", {}, {}
            }, {
                "tel:+1-816-555-1212",
                "tel", {}, "+1-816-555-1212", {}, {}, {}, {}, {}, {},
                {}, "tel", {}, {}
            }, {
                "telnet://192.0.2.16:80/",
                "telnet", "192.0.2.16:80", "/", {}, {},
                {}, "192.0.2.16", "80", {},
                {80}, "80", {}, {"192.0.2.16"}
            }, {
                "urn:oasis:names:specification:docbook:dtd:xml:4.1.2",
                "urn", {}, "oasis:names:specification:docbook:dtd:xml:4.1.2", {}, {},
                {}, {}, {}, {},
                {}, "urn", {}, {}
            }, {
                "foo://example.com:8042/over/there?name=ferret&size=long#nose",
                "foo", "example.com:8042", "/over/there", "name=ferret&size=long", "nose",
                {}, "example.com", "8042", {{{"name", "ferret"}, {"size", "long"}}},
                {8042}, "8042", {"example.com"}, {}
            }, {
                "urn:example:animal:ferret:nose",
                "urn", {}, "example:animal:ferret:nose", {}, {},
                {}, {}, {}, {},
                {}, "urn", {}, {}
            }, {
                "http://www.example.com",
                "http", "www.example.com", "", {}, {},
                {}, "www.example.com", {}, {},
                {}, "http", {"www.example.com"}, {}
            }, {
                "https://example.org:8080/path/to/resource",
                "https", "example.org:8080", "/path/to/resource", {}, {},
                {}, "example.org", "8080", {},
                {8080}, "8080", {"example.org"}, {}
            }, {
                "ftp://example.com/resource?param1=value1&param2=value2",
                "ftp", "example.com", "/resource", "param1=value1&param2=value2", {},
                {}, "example.com", {}, {{{"param1", "value1"}, {"param2", "value2"}}},
                {}, "ftp", {"example.com"}, {}
            }, {
                "mailto:user@example.com",
                "mailto", {}, "user@example.com", {}, {},
                {}, {}, {}, {},
                {}, "mailto", {}, {}
            }, {
                "https://example.com/path/to/resource#fragment",
                "https", "example.com", "/path/to/resource", {}, "fragment",
                {}, "example.com", {}, {},
                {}, "https", {"example.com"}, {}
            }, {
                "https://example.com/path?query#", "https", "example.com", "/path", "query", {""},
                {}, {"example.com"}, {}, {},
                {}, "https", {"example.com"}, {}
            }, {
                "http://", "http", {""}, "", {}, {},
                {}, {""}, {}, {},
                {}, "http", {}, {}
            }, {
                "http://www.ex_ample.com", "http", {"www.ex_ample.com"}, "", {}, {},
                {}, {"www.ex_ample.com"}, {}, {},
                {}, "http", {}, {}
            }
        }) {

            EXPECT_TRUE (tt.URL.valid ()) << "expected " << tt.URL << " to be a valid URL.";

            EXPECT_EQ (tt.URL.scheme (), tt.Scheme);
            EXPECT_EQ (tt.URL.authority (), tt.Authority) << "incorrect authority retrieved for " << tt.URL;
            EXPECT_EQ (tt.URL.path (), tt.Path);
            EXPECT_EQ (tt.URL.query (), tt.Query) << "incorrect query retrieved for " << tt.URL;
            EXPECT_EQ (tt.URL.fragment (), tt.Fragment) << "incorrect fragment retrieved for " << tt.URL;

            auto make_url_1 = URL::make {}.scheme (tt.Scheme).path (tt.Path);
            if (tt.Fragment) make_url_1 = make_url_1.fragment (*tt.Fragment);
            if (tt.Query) make_url_1 = make_url_1.query (*tt.Query);
            if (tt.Authority) make_url_1 = make_url_1.authority (*tt.Authority);

            auto url_1 = URL (make_url_1);
            EXPECT_EQ (tt.URL, url_1) << "expected " << tt.URL << " == " << url_1 << std::endl;

            EXPECT_EQ (tt.URL.user_info (), tt.UserInfo) << "incorrect user_info retrieved for " << tt.URL;
            EXPECT_EQ (tt.URL.host (), tt.Host) << "incorrect host retrieved for " << tt.URL;
            EXPECT_EQ (tt.URL.port (), tt.Port) << "incorrect port retrieved for " << tt.URL;

            EXPECT_EQ (tt.URL.port_number (), tt.PortNumber);
            EXPECT_EQ (tt.URL.port_DNS (), tt.PortDNS);
            EXPECT_EQ (tt.URL.domain_name (), tt.HostDNS);
            EXPECT_EQ (tt.URL.address (), tt.HostAddress) << "incorrect host address retrieved for " << tt.URL;

            auto make_url_2 = URL::make {}.scheme (tt.Scheme).path (tt.Path);
            if (tt.Fragment) make_url_2 = make_url_2.fragment (*tt.Fragment);
            if (tt.Query) make_url_2 = make_url_2.query (*tt.Query);

            if (tt.UserInfo) make_url_2 = make_url_2.user_info (*tt.UserInfo);
            if (tt.HostAddress) make_url_2 = make_url_2.address (*tt.HostAddress);
            else if (tt.HostDNS) make_url_2 = make_url_2.domain_name (*tt.HostDNS);
            else if (tt.Host) make_url_2 = make_url_2.registered_name (*tt.Host);
            if (tt.PortNumber) make_url_2 = make_url_2.port (*tt.PortNumber);

            auto url_2 = URL (make_url_2);
            EXPECT_EQ (tt.URL, url_2);

            if (tt.QueryMap) {
                EXPECT_EQ (tt.URL.query_map (), tt.QueryMap);

                auto make_url_3 = URL::make {}.scheme (tt.Scheme).path (tt.Path);
                if (tt.Fragment) make_url_3 = make_url_3.fragment (*tt.Fragment);
                if (tt.Query) make_url_3 = make_url_3.query_map (*tt.QueryMap);
                if (tt.Authority) make_url_3 = make_url_3.authority (*tt.Authority);
            }

        }

        struct negative_test_case {
            net::URL URL;
        };

        for (const negative_test_case &tt : list<negative_test_case> {
            {"https//example.org"},                 // (Malformed scheme)
            {"https&:/example.org"},                 // (Malformed scheme)
          //{"http://example.com:65536"},           // (Invalid port number)
        }) {
            EXPECT_FALSE (tt.URL.valid ()) << "expected " << tt.URL << " to be an invalid URL.";
        }

    }

    TEST (IPTest, TestMakeURL) {

        EXPECT_EQ (URL {"http://example.com"}, URL (URL::make {"http://example.com"}));
        EXPECT_EQ (URL {"http://example.com"}, URL (URL::make {}.protocol ("http").domain_name ("example.com")));
        EXPECT_EQ (URL {"http://example.com"}, URL (URL::make {}.scheme ("http").authority ("example.com")));

        EXPECT_EQ (URL {"http://example.com/test?query"},
            URL (URL::make {}.protocol ("http").path ("/test").domain_name ("example.com").query ("query")));

        EXPECT_EQ (URL {"http://example.com/test?query"}, URL (URL::make {"http://example.com"}.query ("query").path ("/test")));

        EXPECT_EQ (URL {"zoom://moop:zoop@something.nothing:4321?weem=peen&zap=bap#floop"},
            URL (URL::make {}.user_name_pass ("moop", "zoop").query_map ({{"weem", "peen"}, {"zap", "bap"}}).protocol
                ("zoom").fragment ("floop").domain_name ("something.nothing").port (4321)));

    }

    TEST (IPTest, TestURLEqual) {
        struct test_equal {
            URL Left;
            URL Right;
            bool Expected;
        };

        for (const test_equal &tt : list<test_equal> {
            {"http://example.com", "http://example.com", true},  // Identical URLs
            {"http://example.com", "HTTP://example.com", true},  // Case-insensitive scheme
             // Hex encoding of non-reserved characters
            {"http://example.com", "http://%65%78%61%6D%70%6C%65.com", true},
            {"http://example.com/test", "http://example.com/test", true},  // Identical path
            {"http://example.com/test", "http://example.com/TEST", false},  // Case-sensitive path
            {"http://example.com/test?query", "http://example.com/test?QUERY", false},  // Case-sensitive query
             // Case-sensitive fragment
            {"http://example.com/test#fragment", "http://example.com/test#FRAGMENT", false},
             // Mixed case-sensitivity
            {"http://example.com/test?query#fragment", "http://example.com/test?query#FRAGMENT", false},
             // All case-sensitive parts
            {"http://example.com/test?query#fragment", "http://example.com/test?QUERY#FRAGMENT", false},
            {"http://example.com:80", "http://example.com:80", true},  // Identical port
            {"http://example.com:80", "http://example.com:8080", false},  // Different port
            {"http://www.example.com", "http://WWW.EXAMPLE.COM", true}, // Domain is case-insensitive
            {"http://www.example.com", "http://www.example.com:80", false}, // Default port for HTTP is 80
            {"https://www.example.com", "https://www.example.com:443", false}, // Default port for HTTPS is 443
            {"http://www.example.com", "http://www.example.com:81", false}, // Different ports
            {"http://www.example.com/path", "http://www.example.com/PATH", false}, // Path is case-sensitive
            {"http://www.example.com?query=test", "http://www.example.com?QUERY=TEST", false}, // Query is case-sensitive
            {"http://www.example.com#fragment", "http://www.example.com#FRAGMENT", false}, // Fragment is case-sensitive
            {"http://www.example.com/%7efoo", "http://www.example.com/~foo", true}, // Non-reserved character percent-encoded
            {"http://www.example.com?query=%3D", "http://www.example.com?query==", false}, // Reserved character percent-encoded
            {"http://www.example.com", "https://www.example.com", false}, // Different schemes
            {"http://www.example.com", "http://www.example.org", false}, // Different domain names
            {"http://www.example.com/path", "http://www.example.com/another-path", false}, // Different paths
            {"http://www.example.com?query=test", "http://www.example.com?query=another-test", false}, // Different queries
            {"http://www.example.com#fragment", "http://www.example.com#another-fragment", false} // Different fragments
        }) {
            EXPECT_EQ (tt.Left == tt.Right, tt.Expected) << "expected \"" << tt.Left << "\" == \"" << tt.Right << "\"" << std::endl;
        }

    }

    TEST (IPTest, TestURLParameters) {
        URL yes {"fnorp://zeep:zoop@moop.pim.pom?weep=warp&beep=boop"};
        URL no {"fnorp://zeep:zoop:zap@moop.pim.pom?weep&boop"};

        EXPECT_TRUE (yes.user_name_pass ());
        EXPECT_TRUE (yes.query_map ());

        EXPECT_FALSE (no.user_name_pass ());
        EXPECT_FALSE (no.query_map ());

    }

}
