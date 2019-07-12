cmake_minimum_required(VERSION 3.1...3.14)

# Back compatibility for VERSION range
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()


ADD_EXECUTABLE(testData testHex.cpp testAscii.cpp testLib.cpp )
target_link_libraries(testData data ${CRYPTOPP_LIBRARIES} ${Boost_LIBRARIES} gmock_main)

add_test(NAME HexToArray COMMAND testData)
add_test(NAME ValidHexString COMMAND testData)
add_test(NAME InvalidExceptionOnError COMMAND testData)
add_test(NAME WriteBytes COMMAND testData)
add_test(NAME InvalidAscii COMMAND testData)
add_test(NAME ValidAscii COMMAND testData)
add_test(NAME CastToBytes COMMAND testData)