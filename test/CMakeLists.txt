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
    linked_stack.cpp             # TODO: ensure we check the interface by argument lookup and in data::
                                 #       ensure that we can use this type in a pure functional way.
    list.cpp                     # TODO: contains commented tests
                                 #       ensure that we can use this type in a pure functional way.
    ordered_sequence.cpp         # TODO: contains commented tests
                                 #       ensure that we can use this type in a pure functional way.
    tree.cpp                     # TODO: contains commented tests
                                 #       ensure that we can use this type in a pure functional way.
    set.cpp                      # TODO: Need to test each function more precisely, like in list
                                 #       ensure that we can use this type in a pure functional way.
    map.cpp                      # TODO: Need to test each function more precisely, like in list
                                 #       ensure that we can use this type in a pure functional way.
    priority_queue.cpp           # TODO: Contains commented test cases
                                 #       ensure that we can use this type in a pure functional way.
    functional_interfaces.cpp    # TODO: contains commented tests
                                 #       We can eventually get rid of this file entirely.
    empty.cpp                    # TODO: Contains notes
    take.cpp                     # TODO: ensure we check the interface by argument lookup and in data::
    reverse.cpp                  # TODO: Contains commented test cases
    contains.cpp                 # TODO: Contains commented test cases
    sort.cpp
    get.cpp
    for_each.cpp                 # TODO: could be more thorough
    replace.cpp
    select.cpp
    lift.cpp                     # TODO: could be more thorough
    transpose.cpp
    flatten.cpp
    permutation.cpp              # TODO: Need to test each function more precisely, like in list
    transpose.cpp
    combinatorics.cpp
    map_schema.cpp

    # string
    integer_format.cpp
    bytes.cpp
    write.cpp
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
    modular.cpp                  # TODO: A condition is commented out.
    limits.cpp                   # TODO: cases are commented out.
    polynomial.cpp               # TODO: cases are commented out and should be a typed test.
    finite_field.cpp             # TODO: make typed test.
    matrix.cpp
    algebra.cpp
    exterior.cpp
    elliptic_curve.cpp

    # crypto
    # random_primes.cpp
    symmetric_crypto.cpp
    NIST_DRBG.cpp
    secret_share.cpp

    # net
    IP.cpp
    circular_queue.cpp
    email.cpp

    # io
    log.cpp
    args_parser.cpp
)

target_link_libraries (unit_tests
  PRIVATE io net crypto data gtest_main gmock)
gtest_discover_tests (unit_tests)

if (OPTIONAL_TESTS)
    add_subdirectory (optional)
endif ()

target_compile_options (unit_tests PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-ftemplate-depth=2048>
    $<$<CXX_COMPILER_ID:Clang>:-ftemplate-depth=2048>
    $<$<CXX_COMPILER_ID:AppleClang>:-ftemplate-depth=2048>
    $<$<CXX_COMPILER_ID:MSVC>:/constexpr:depth2048>
)
