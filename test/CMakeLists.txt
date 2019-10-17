cmake_minimum_required(VERSION 3.1...3.14)

# Back compatibility for VERSION range
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()


ADD_EXECUTABLE(testData testHex.cpp testAscii.cpp testStream.cpp testIntegerFormat.cpp testN.cpp testBounded.cpp testDigest.cpp testBase58.cpp  testBase64.cpp testEratosthenes.cpp testLinkedList.cpp testPolynomial.cpp testLib.cpp )
link_directories(${Boost_LIBRARY_DIR} ${LIB_BITCOIN_LIBRARY_DIR})
target_link_libraries(testData data ${CRYPTOPP_LIBRARIES} ${Boost_LIBRARIES} ${GMP_LIBRARY} ${GMPXX_LIBRARY} ${LIB_BITCOIN_LIBRARIES} gmock_main)

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
add_test(NAME Base58Invert COMMAND testData)
add_test(NAME Base58HexToArray COMMAND testData)
add_test(NAME Base58ValidHexString COMMAND testData)
add_test(NAME Base58InvalidExceptionOnError COMMAND testData)

# Test Base64
add_test(NAME Base64HexToArray COMMAND testData)
add_test(NAME Base64ValidHexString COMMAND testData)
add_test(NAME Base64InvalidExceptionOnError COMMAND testData)

#Test Integer Format
add_test(NAME TestIntegerFormat COMMAND testData)

#Test N
add_test(NAME TestN COMMAND testData)
add_test(NAME TestMultiply COMMAND testData)
add_test(NAME TestDivision COMMAND testData)
add_test(NAME TestStringToN COMMAND testData)

#Test LinkedList
add_test(NAME TestLinkedList1 COMMAND testData)
add_test(NAME TestLinkedList2 COMMAND testData)
add_test(NAME TestLinkedListSort COMMAND testData)

# Test Digest
add_test(NAME DigestEqual COMMAND testData)
add_test(NAME DigestNotEqual COMMAND testData)
add_test(NAME DigestOrdering COMMAND testData)
add_test(NAME SHA256 COMMAND testData)

# Test Bounded
add_test(NAME BoundedEqual COMMAND testData)
add_test(NAME BoundedNotEqual COMMAND testData)

# Test Stream
add_test(NAME StreamWriteUint16Big COMMAND testData)
add_test(NAME StreamWriteUint16Little COMMAND testData)
add_test(NAME StreamWriteUint32Big COMMAND testData)
add_test(NAME StreamWriteUint32Little COMMAND testData)
add_test(NAME StreamWriteUint64Big COMMAND testData)
add_test(NAME StreamWriteUint64Little COMMAND testData)
add_test(NAME StreamWriteBytes COMMAND testData)
add_test(NAME StreamReadUint16Big COMMAND testData)
add_test(NAME StreamReadUint16Little COMMAND testData)
add_test(NAME StreamReadUint32Big COMMAND testData)
add_test(NAME StreamReadUint32Little COMMAND testData)
add_test(NAME StreamReadUint64Big COMMAND testData)
add_test(NAME StreamReadUint64Little COMMAND testData)
add_test(NAME StreamReadBytes COMMAND testData)

#Test Polynomial
add_test(NAME TestPolynomial COMMAND testData)

#Test Eratosthenes
add_test(NAME TestEratosthenes COMMAND testData)
