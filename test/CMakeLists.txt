cmake_minimum_required (VERSION 3.16)

# Back compatibility for VERSION range
if (${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy (VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif ()

option (OPTIONAL_TESTS "Build optional Tests" OFF)
include (GoogleTest)

add_executable (
    unit_tests
    # These tests only require data_core. 
    maybe_either.cpp
    testStore.cpp
    testException.cpp
    integer_format.cpp
    linked_stack.cpp
    list.cpp
    ordered_sequence.cpp
    testCircularQueue.cpp
    tree.cpp
    functional_interfaces.cpp
    empty.cpp
    take.cpp
    reverse.cpp
    contains.cpp
    sort.cpp
    testSet.cpp
    map.cpp
    priority_queue.cpp
    testForEach.cpp

    testBytes.cpp
    testHex.cpp
    testBase64.cpp
    testAscii.cpp
    testStream.cpp

    testPermutation.cpp
    combinatorics.cpp
    testEndian.cpp
    testSign.cpp
    testBitShift.cpp
    testN.cpp
    testZ.cpp
    testCarry.cpp
    testNBytes.cpp
    testRoot.cpp
    Z_bytes.cpp
    testAbs.cpp
    testBounded.cpp
    testBase58.cpp
    testNumbers.cpp
    testStringNumbers.cpp
    testDecimal.cpp
    testHexidecimal.cpp
    testInfinite.cpp
    testDivision.cpp
    testBitcoinNumbers.cpp
    testExtendedEuclidian.cpp
    testEratosthenes.cpp
    power.cpp
    testCaylayDickson.cpp
    testModular.cpp
    testAlgebra.cpp
    testPolynomial.cpp
    number_theory.cpp
    testGeometry.cpp
    testHash.cpp
    testSymmetricCrypto.cpp
    testSecretShare.cpp
    testEllipticCurve.cpp
    testLog.cpp
    testArgsParser.cpp
    IP.cpp
    testEmail.cpp
)

target_link_libraries (unit_tests
  PRIVATE data gtest_main gmock)
gtest_discover_tests (unit_tests)

if (OPTIONAL_TESTS)
    add_subdirectory (optional)
endif ()
