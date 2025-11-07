cmake_minimum_required (VERSION 3.16)

# Back compatibility for VERSION range
if (${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy (VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif ()

option (OPTIONAL_TESTS "Build optional Tests" OFF)
include (GoogleTest)

add_executable (
    unit_tests
    # These tests only require data_core for the components being tested. 
    # other components may be used for the tests themselves. 
    maybe_either.cpp
    store.cpp
    exception.cpp
    linked_stack.cpp             # TODO: contains commented tests
    list.cpp                     # TODO: contains commented tests
    ordered_sequence.cpp         # TODO: contains commented tests
    circular_queue.cpp           # TODO: contains commented tests
    tree.cpp                     # TODO: contains commented tests
    functional_interfaces.cpp
    empty.cpp                    # TODO: Contains commented test cases
    take.cpp
    reverse.cpp
    contains.cpp                 # TODO: Contains commented test cases
    sort.cpp
    set.cpp                      # TODO: Need to test each function more precisely, like in list
    map.cpp                      # TODO: Need to test each function more precisely, like in list
    priority_queue.cpp           # TODO: Contains commented test cases
    testForEach.cpp              # TODO: contains commented test cases.
    map_schema.cpp
    permutation.cpp              # TODO: Need to test each function more precisely, like in list
    combinatorics.cpp

    # string 
    integer_format.cpp
    bytes.cpp
    hex.cpp
    base64.cpp
    ASCII.cpp
    stream.cpp
    endian.cpp
    bounded.cpp                  # TODO: should be typed test, has commented sections

    # hash
    hash.cpp

    # numbers
    numbers.cpp                  # TODO: need a typed test suite.
    bit_ops.cpp                  # TODO: Contains commented test cases
    decimal.cpp                  # TODO: need to include bounded numbers.
    hexidecimal.cpp
    base58.cpp
    N.cpp
    Z.cpp
    carry.cpp
    N_bytes.cpp
    Z_bytes.cpp
    string_numbers.cpp
    infinite.cpp                 # TODO: should be a typed test.
    division.cpp
    Bitcoin_numbers.cpp          # TODO: should be a typed test.
    power.cpp                    # TODO: should be a typed test.
    extended_euclidian.cpp
    Eratosthenes.cpp
    prime_factor.cpp
    number_theory.cpp            # TODO: make typed tests

    # math
    rational.cpp                 # TODO: all fractions should be NumberComparable
    Caylay_Dickson.cpp           # TODO: contains commented test cases.
    sign.cpp                     # TODO: Contains commented test cases and should be a typed test
    abs.cpp                      # TODO: should be a typed test.
    roots.cpp                    # TODO: should be a typed test.
    modular.cpp                  # TODO: cases are commented out.
    limits.cpp                   # TODO: cases are commented out.
    polynomial.cpp               # TODO: cases are commented out and should be a typed test.
    finite_field.cpp             # TODO: make typed test.
    algebra.cpp

    # crypto 
    # random_primes.cpp
    geometry.cpp
    symmetric_crypto.cpp
    secret_share.cpp
    testEllipticCurve.cpp

    # net
    IP.cpp
    email.cpp

    # io
    testLog.cpp
    args_parser.cpp
)

target_link_libraries (unit_tests
  PRIVATE data gtest_main gmock)
gtest_discover_tests (unit_tests)

if (OPTIONAL_TESTS)
    add_subdirectory (optional)
endif ()
