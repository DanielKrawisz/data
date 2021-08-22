cmake_minimum_required(VERSION 3.1...3.14)

# Back compatibility for VERSION range
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(data 	VERSION 1.0
        DESCRIPTION "wrappers and high-level programming constructs in c++"
        LANGUAGES CXX)

# Set cmake as import path for Find*.cmake files
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Setup paths to outputs of conan's "cmake_find_package" generator
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR} ${CMAKE_PREFIX_PATH})

## Check if GTests is installed. If not, install it

option(PACKAGE_TESTS "Build the tests" ON)
if(NOT TARGET gtest_main AND PACKAGE_TESTS)
    # Download and unpack googletest at configure time
    configure_file(cmake/gtests.txt.in googletest-download/CMakeLists.txt)
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
            ${CMAKE_CURRENT_BINARY_DIR}/googletest-build)
    include(GoogleTest)
endif()


# Include libraries

# Boost settings
set(Boost_LIB_PREFIX            "lib"       CACHE STRING "")
set(Boost_USE_MULTITHREADED     ON          CACHE BOOL "") # '-mt' flag
set(Boost_USE_STATIC_LIBS       ON         CACHE BOOL "")
set(Boost_USE_STATIC_RUNTIME    OFF         CACHE BOOL "") # '-s' tag
set(Boost_USE_DEBUG_RUNTIME     OFF         CACHE BOOL "") # '-g' tag
set(Boost_COMPILER              "-mgw49"    CACHE STRING "")

# Include Boost
find_package(Boost 1.76.0 COMPONENTS system log log_setup regex REQUIRED)

if(Boost_FOUND)

    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})
    add_definitions("-DHAS_BOOST")

endif()


# Find Crypto++
find_package(CryptoPP REQUIRED)
if(cryptopp_INCLUDE_DIRS)
    set(CRYPTOPP_INCLUDE_DIRS ${cryptopp_INCLUDE_DIRS})
    set(CRYPTOPP_LIBRARIES ${cryptopp_LIBRARIES})
endif()
if(CRYPTOPP_INCLUDE_DIRS)

    message(STATUS "Crypto++ INCLUIDE DIRS: ${CRYPTOPP_INCLUDE_DIRS}")
    MESSAGE(STATUS "Found Crypto++: ${CRYPTOPP_LIBRARIES}")
    include_directories(${CRYPTOPP_INCLUDE_DIRS})
endif()


#find_package(ICU 60.2 COMPONENTS uc i18n REQUIRED)
find_package(OpenSSL REQUIRED)
message("OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
message("OpenSSL libraries: ${OPENSSL_LIBRARIES}")
include_directories(${OPENSSL_INCLUDE_DIR})

find_package(PkgConfig REQUIRED)
#pkg_check_modules(LIBSECP256K1 REQUIRED IMPORTED_TARGET libsecp256k1)
find_package(SECP256K1 REQUIRED)
# Find GMP
find_package(GMP REQUIRED)
if(GMP_FOUND)
    include_directories(${GMP_INCLUDE_DIR})
endif()

## Enable testing
include(CTest)

if(PACKAGE_TESTS)
    enable_testing()
    add_subdirectory(test)


endif()


#  Add Library source files here

ADD_LIBRARY(data STATIC
    src/sv/fs.cpp
    src/sv/support/cleanse.cpp
    src/sv/support/lockedpool.cpp
    src/sv/crypto/chacha20.cpp
    src/sv/crypto/hmac_sha512.cpp
    src/sv/crypto/ripemd160.cpp
    src/sv/crypto/sha1.cpp
    src/sv/crypto/sha256.cpp
    src/sv/crypto/sha512.cpp
    
    include/rotella/aks.cpp
    include/rotella/sieve.cpp
    include/rotella/akslib.cpp
    
    src/data/types.cpp
    src/data/encoding/hex.cpp
    src/data/encoding/ascii.cpp
    src/data/encoding/base58.cpp
    src/data/encoding/base64.cpp
    src/data/encoding/integer.cpp
    src/data/encoding/utf8.cpp
    src/data/networking/http.cpp
    src/data/tools/channel.cpp
    src/data/crypto/secret_share.cpp
    src/data/math/number/gmp/mpq.cpp
    src/data/math/number/gmp/N.cpp
    src/data/math/number/gmp/aks.cpp
    src/data/math/number/gmp/sqrt.cpp
    src/data/crypto/AES.cpp
    src/data/tools/circular_queue.cpp
    src/data/tools/rate_limiter.cpp
    src/data/log/log.cpp
)

target_include_directories(data PUBLIC include)

target_link_libraries(data ${SECP256K1_LIBRARY} ${CRYPTOPP_LIBRARIES} Boost::regex Boost::system Boost::log Boost::log_setup ${Boost_LIBRARIES} ${GMP_LIBRARY} ${GMPXX_LIBRARY} ${OPENSSL_LIBRARIES}
#PkgConfig::LIBSECP256K1
)
get_target_property(OUT data LINK_LIBRARIES)
message(STATUS ${OUT})
# Set C++ version
target_compile_features(data PUBLIC cxx_std_17)
set_target_properties(data PROPERTIES CXX_EXTENSIONS OFF)

