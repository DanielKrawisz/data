cmake_minimum_required (VERSION 3.1...3.14)

# Back compatibility for VERSION range
if (${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
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

package_add_test (testEndian testEndian.cpp)
package_add_test (testHex testHex.cpp)
package_add_test (testAscii testAscii.cpp)
package_add_test (testStream testStream.cpp)
package_add_test (testHash testHash.cpp)
package_add_test (testIntegerFormat testIntegerFormat.cpp)
package_add_test (testFunctionalInterfaces testFunctionalInterfaces.cpp)
package_add_test (testLinkedStack testLinkedStack.cpp)
package_add_test (testCircularQueue testCircularQueue.cpp)
package_add_test (testReverse testReverse.cpp)
package_add_test (testContains testContains.cpp)
package_add_test (testTake testTake.cpp)
package_add_test (testSort testSort.cpp)
package_add_test (testLinkedTree testLinkedTree.cpp)
package_add_test (testMap testMap.cpp)
package_add_test (testPriorityQueue testPriorityQueue.cpp)
package_add_test (testForEach testForEach.cpp)
package_add_test (testPermutation testPermutation.cpp)
package_add_test (testBytes testBytes.cpp)
package_add_test (testSign testSign.cpp)
package_add_test (testBitShift testBitShift.cpp)
package_add_test (testN testN.cpp)
package_add_test (testZ testZ.cpp)
package_add_test (testNBytes testNBytes.cpp)
package_add_test (testRoots testRoot.cpp)
package_add_test (testZBytes testZBytes.cpp)
package_add_test (testAbs testAbs.cpp)
package_add_test (testBounded testBounded.cpp)
package_add_test (testBase58 testBase58.cpp)
package_add_test (testNumbers testNumbers.cpp)
package_add_test (testStringNumbers testStringNumbers.cpp)
package_add_test (testDecimal testDecimal.cpp)
package_add_test (testHexidecimal testHexidecimal.cpp)
package_add_test (testDivision testDivision.cpp)
package_add_test (testTwosComplement testTwosComplement.cpp)
package_add_test (testExtendedEuclidian testExtendedEuclidian.cpp)
package_add_test (testEratosthenes testEratosthenes.cpp)
package_add_test (testPrimeFactor testPrimeFactor.cpp)
package_add_test (testFiniteField testFiniteField.cpp)
package_add_test (testFraction testFraction.cpp)
package_add_test (testCaylayDickson testCaylayDickson.cpp)
package_add_test (testPolynomial testPolynomial.cpp)
package_add_test (testSecretShare testSecretShare.cpp)
package_add_test (testLog testLog.cpp)
package_add_test (testArgsParser testArgsParser.cpp)
package_add_test (testIP testIP.cpp)
package_add_test (testException testException.cpp)
package_add_test (testTCP testTCP.cpp)
#package_add_test (testRateLimiter testRateLimiter.cpp)
#package_add_test (testNetworking testNetworking.cpp)

if (OPTIONAL_TESTS)
    add_subdirectory (optional)
endif ()
