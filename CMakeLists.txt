cmake_minimum_required(VERSION 3.14)
project(data)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


include_directories(include)
include_directories(include/data)
include_directories(include/data/crypto)
include_directories(include/data/crypto/nist)

include_directories(include/data/encoding)
include_directories(include/data/io)

include_directories(include/data/list)
include_directories(include/data/map)
include_directories(include/data/math)
include_directories(include/data/math/number)
include_directories(include/data/math/number/gmp)
include_directories(src/data/encoding)
include_directories(src/data/encoding/utf8)
set(Boost_LIB_PREFIX            "lib"       CACHE STRING "")
set(Boost_USE_MULTITHREADED     ON          CACHE BOOL "") # '-mt' flag
set(Boost_USE_STATIC_LIBS       ON          CACHE BOOL "")
set(Boost_USE_STATIC_RUNTIME    ON          CACHE BOOL "") # '-s' tag
set(Boost_USE_DEBUG_RUNTIME     ON          CACHE BOOL "") # '-g' tag
set(Boost_COMPILER              "-mgw49"    CACHE STRING "")

find_package(Boost 1.60.0 COMPONENTS system  REQUIRED)

if(Boost_FOUND)

    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})
    add_definitions("-DHAS_BOOST")

endif()
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/contrib/cmake")
find_package(CryptoPP REQUIRED)
if(CRYPTOPP_INCLUDE_DIRS)

    message(STATUS "Crypto++ INCLUIDE DIRS: ${CRYPTOPP_INCLUDE_DIRS}")
    include_directories(${CRYPTOPP_INCLUDE_DIRS})
endif()

set(ENV{PKG_CONFIG_PATH} "/usr/local/lib/pkgconfig/:$ENV{PKG_CONFIG_PATH}")

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIB_BITCOIN REQUIRED libbitcoin-system)
include_directories(${LIB_BITCOIN_INCLUDE_DIRS})

find_package(GMP REQUIRED)
if(GMP_FOUND)
    include_directories(${GMP_INCLUDE_DIR})
endif()


# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

ADD_LIBRARY(data STATIC
        include/data/crypto/sha256.hpp
        include/data/encoding/ascii.hpp
        include/data/encoding/hex.hpp
        include/data/io/main.hpp
        include/data/io/unimplemented.hpp
        include/data/list/linked_list.hpp
        include/data/map/rb.hpp
        include/data/math/number/gmp/N.hpp
        src/data/crypto/sha256.cpp
        src/data/math/number/gmp/N.cpp
        src/data/encoding/hex.cpp
        src/data/encoding/ascii.cpp)

ADD_EXECUTABLE(testData test/testHex.cpp test/testAscii.cpp test/testLib.cpp )
target_link_libraries(testData data ${CRYPTOPP_LIBRARIES} ${Boost_LIBRARIES} gmock_main)
enable_testing()
target_compile_features(data PUBLIC cxx_std_17)
set_target_properties(data PROPERTIES CXX_EXTENSIONS ON)
add_test(NAME HexToArray COMMAND testData)
add_test(NAME ValidHexString COMMAND testData)
add_test(NAME InvalidExceptionOnError COMMAND testData)
add_test(NAME WriteBytes COMMAND testData)
add_test(NAME InvalidAscii COMMAND testData)
add_test(NAME ValidAscii COMMAND testData)
add_test(NAME CastToBytes COMMAND testData)
