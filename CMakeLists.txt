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
        src/data/encoding/hex.cpp)

ADD_EXECUTABLE(testData test/testLib.cpp)
target_link_libraries(testData data ${CRYPTOPP_LIBRARIES} ${Boost_LIBRARIES})
target_compile_features(data PUBLIC cxx_std_17)
set_target_properties(data PROPERTIES CXX_EXTENSIONS ON)