cmake_minimum_required (VERSION 3.16)

# Back compatibility for VERSION range
if (${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy (VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif ()

option (OPTIONAL_TESTS "Build optional Tests" OFF)
include (GoogleTest)

add_executable (unit_tests
    testMaybeEither.cpp
    testStore.cpp
    testException.cpp
    testEndian.cpp
    testHex.cpp
    testBase64.cpp
    testAscii.cpp
    testStream.cpp
    testIntegerFormat.cpp
    testFunctionalInterfaces.cpp
    testLinkedStack.cpp
    testList.cpp
    testCircularQueue.cpp
    testReverse.cpp
    testContains.cpp
    testTake.cpp
    testOrderedList.cpp
    testSort.cpp
    testTree.cpp
    testMap.cpp
    testPriorityQueue.cpp
    testForEach.cpp
    testPermutation.cpp
    testCombinatorics.cpp
    testBytes.cpp
    testSign.cpp
    testBitShift.cpp
    testN.cpp
    testZ.cpp
    testCarry.cpp
    testNBytes.cpp
    testRoot.cpp
    testZBytes.cpp
    testAbs.cpp
    testBounded.cpp
    testBase58.cpp
    testNumbers.cpp
    testStringNumbers.cpp
    testDecimal.cpp
    testHexidecimal.cpp
    testInfinite.cpp
    testDivision.cpp
    testTwosComplement.cpp
    testExtendedEuclidian.cpp
    testCaylayDickson.cpp
    testModular.cpp
    testAlgebra.cpp
    testPolynomial.cpp
    testNumberTheory.cpp
    testGeometry.cpp
    testHash.cpp
    testSymmetricCrypto.cpp
    testSecretShare.cpp
    testEllipticCurve.cpp
    testLog.cpp
    testArgsParser.cpp
    testIP.cpp
    testEmail.cpp
)

target_link_libraries (unit_tests
  PRIVATE data gtest_main gmock)
gtest_discover_tests (unit_tests)

if (OPTIONAL_TESTS)
    add_subdirectory (optional)
endif ()
