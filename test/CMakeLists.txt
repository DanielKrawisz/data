cmake_minimum_required (VERSION 3.1...3.14)

# Back compatibility for VERSION range
if (${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy (VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif ()
option (OPTIONAL_TESTS "Build optional Tests" OFF)
include (GoogleTest)
macro (package_add_test TESTNAME)
    # create an exectuable in which the tests will be stored
    add_executable (${TESTNAME} ${ARGN})
    # link the Google test infrastructure, mocking library, and a default main fuction to
    # the test executable.  Remove g_test_main if writing your own main function.
    target_include_directories (${TESTNAME} PUBLIC ${CMAKE_SOURCE_DIR}/include
    boost::boost 
    openssl::openssl 
    cryptopp::cryptopp 
    nlohmann_json::nlohmann_json 
    gmp::gmp 
    secp256k1::secp256k1)
    target_link_libraries (${TESTNAME} gtest::gtest data)
    # gtest_discover_tests replaces gtest_add_tests,
    # see https://cmake.org/cmake/help/v3.10/module/GoogleTest.html for more options to pass to it

    gtest_discover_tests (${TESTNAME}
            # set a working directory so your project root so that you can find test data via paths relative to the project root
            SOURCES ${ARGN}
            WORKING_DIRECTORY ${PROJECT_DIR}
            )
    set_target_properties (${TESTNAME} PROPERTIES FOLDER tests)
endmacro ()

package_add_test (TestException testException.cpp)
package_add_test (TestEndian testEndian.cpp)
package_add_test (TestHex testHex.cpp)
package_add_test (TestAscii testAscii.cpp)
package_add_test (TestStream testStream.cpp)
package_add_test (TestHash testHash.cpp)
package_add_test (TestIntegerFormat testIntegerFormat.cpp)
package_add_test (TestFunctionalInterfaces testFunctionalInterfaces.cpp)
package_add_test (TestLinkedStack testLinkedStack.cpp)
package_add_test (TestList testList.cpp)
package_add_test (TestCircularQueue testCircularQueue.cpp)
package_add_test (TestReverse testReverse.cpp)
package_add_test (TestContains testContains.cpp)
package_add_test (TestTake testTake.cpp)
package_add_test (TestOrderedList testOrderedList.cpp)
package_add_test (TestSort testSort.cpp)
package_add_test (TestLinkedTree testLinkedTree.cpp)
package_add_test (TestMap testMap.cpp)
package_add_test (TestPriorityQueue testPriorityQueue.cpp)
package_add_test (TestForEach testForEach.cpp)
package_add_test (TestPermutation testPermutation.cpp)
package_add_test (TestBytes testBytes.cpp)
package_add_test (TestSign testSign.cpp)
package_add_test (TestBitShift testBitShift.cpp)
package_add_test (TestN testN.cpp)
package_add_test (TestZ testZ.cpp)
package_add_test (TestNBytes testNBytes.cpp)
package_add_test (TestRoots testRoot.cpp)
package_add_test (TestZBytes testZBytes.cpp)
package_add_test (TestAbs testAbs.cpp)
package_add_test (TestBounded testBounded.cpp)
package_add_test (TestBase58 testBase58.cpp)
package_add_test (TestNumbers testNumbers.cpp)
package_add_test (TestStringNumbers testStringNumbers.cpp)
package_add_test (TestDecimal testDecimal.cpp)
package_add_test (TestHexidecimal testHexidecimal.cpp)
package_add_test (TestDivision testDivision.cpp)
package_add_test (TestTwosComplement testTwosComplement.cpp)
package_add_test (TestExtendedEuclidian testExtendedEuclidian.cpp)
package_add_test (TestEratosthenes testEratosthenes.cpp)
package_add_test (TestPrimeFactor testPrimeFactor.cpp)
package_add_test (TestFiniteField testFiniteField.cpp)
package_add_test (TestFraction testFraction.cpp)
package_add_test (TestCaylayDickson testCaylayDickson.cpp)
package_add_test (TestPolynomial testPolynomial.cpp)
package_add_test (TestSecretShare testSecretShare.cpp)
package_add_test (TestLog testLog.cpp)
package_add_test (TestArgsParser testArgsParser.cpp)
package_add_test (TestIP testIP.cpp)
package_add_test (TestEmail testEmail.cpp)
package_add_test (TestTCP testTCP.cpp)
#package_add_test (TestRateLimiter testRateLimiter.cpp)
#package_add_test (TestNetworking testNetworking.cpp)

if (OPTIONAL_TESTS)
    add_subdirectory (optional)
endif ()
