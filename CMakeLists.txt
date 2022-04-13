cmake_minimum_required(VERSION 3.1...3.14)

# Back compatibility for VERSION range
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()
set(CMAKE_C_COMPILER "gcc-10")
set(CMAKE_CXX_COMPILER "g++-10")

project(data 	VERSION 1.0
        DESCRIPTION "wrappers and high-level programming constructs in c++"
        LANGUAGES CXX)



# Set cmake as import path for Find*.cmake files
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

### Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

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



    add_definitions("-DHAS_BOOST")








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
    src/sv/utiltime.cpp
    
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

target_link_libraries(data CONAN_PKG::boost CONAN_PKG::openssl CONAN_PKG::cryptopp CONAN_PKG::nlohmann_json CONAN_PKG::gmp CONAN_PKG::SECP256K1 CONAN_PKG::uriparser
#PkgConfig::LIBSECP256K1
)
get_target_property(OUT data LINK_LIBRARIES)
message(STATUS ${OUT})
# Set C++ version
target_compile_features(data PUBLIC cxx_std_20)
set_target_properties(data PROPERTIES CXX_EXTENSIONS ON)
target_compile_options(data PUBLIC "-fconcepts")