cmake_minimum_required(VERSION 3.1...3.14)

# Back compatibility for VERSION range
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()


ADD_EXECUTABLE(testData testHex.cpp testAscii.cpp testSlice.cpp testDigest.cpp testBase58.cpp testBase32.cpp testEratosthenes.cpp testLib.cpp )
link_directories(${Boost_LIBRARY_DIR} ${LIB_BITCOIN_LIBRARY_DIR})
target_link_libraries(testData data ${CRYPTOPP_LIBRARIES} ${Boost_LIBRARIES} ${GMP_LIBRARY} ${LIB_BITCOIN_LIBRARIES} gmock_main)

# Test Hex
add_test(NAME HexHexToArray COMMAND testData)
add_test(NAME HexValidHexString COMMAND testData)
add_test(NAME HexInvalidExceptionOnError COMMAND testData)
add_test(NAME HexWriteBytes COMMAND testData)

# Test Ascii
add_test(NAME AsciiInvalidAscii COMMAND testData)
add_test(NAME AsciiValidAscii COMMAND testData)
add_test(NAME AsciiCastToBytes COMMAND testData)

# Test Base58
add_test(NAME Base58HexToArray COMMAND testData)
add_test(NAME Base58ValidHexString COMMAND testData)
add_test(NAME Base58InvalidExceptionOnError COMMAND testData)

# Test Slice
add_test(NAME SliceWriteUint16Big COMMAND testData)
add_test(NAME SliceWriteUint16Little COMMAND testData)
add_test(NAME SliceWriteUint32Big COMMAND testData)
add_test(NAME SliceWriteUint32Little COMMAND testData)
add_test(NAME SliceWriteUint64Big COMMAND testData)
add_test(NAME SliceWriteUint64Little COMMAND testData)
add_test(NAME SliceWriteBytes COMMAND testData)
add_test(NAME SliceReadUint16Big COMMAND testData)
add_test(NAME SliceReadUint16Little COMMAND testData)
add_test(NAME SliceReadUint32Big COMMAND testData)
add_test(NAME SliceReadUint32Little COMMAND testData)
add_test(NAME SliceReadUint64Big COMMAND testData)
add_test(NAME SliceReadUint64Little COMMAND testData)
add_test(NAME SliceReadBytes COMMAND testData)

#Test Eratosthenes
add_test(name First100Primes command testData)
