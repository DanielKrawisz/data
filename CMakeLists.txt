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
endif()


# Include libraries

# Boost settings
set(Boost_LIB_PREFIX            "lib"       CACHE STRING "")
set(Boost_USE_MULTITHREADED     ON          CACHE BOOL "") # '-mt' flag
set(Boost_USE_STATIC_LIBS       OFF          CACHE BOOL "")
set(Boost_USE_STATIC_RUNTIME    OFF          CACHE BOOL "") # '-s' tag
set(Boost_USE_DEBUG_RUNTIME     ON          CACHE BOOL "") # '-g' tag
set(Boost_COMPILER              "-mgw49"    CACHE STRING "")

# Include Boost
find_package(Boost 1.60.0 COMPONENTS system  REQUIRED)

if(Boost_FOUND)

    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})
    add_definitions("-DHAS_BOOST")

endif()


# Find Crypto++
find_package(CryptoPP REQUIRED)
if(CRYPTOPP_INCLUDE_DIRS)

    message(STATUS "Crypto++ INCLUIDE DIRS: ${CRYPTOPP_INCLUDE_DIRS}")
    include_directories(${CRYPTOPP_INCLUDE_DIRS})
endif()

# Find LibBitcoin
set(ENV{PKG_CONFIG_PATH} "/usr/local/lib/pkgconfig/:$ENV{PKG_CONFIG_PATH}")

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIB_BITCOIN REQUIRED libbitcoin-system)
include_directories(${LIB_BITCOIN_INCLUDE_DIRS})

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
        src/data/encoding/hex.cpp
        src/data/encoding/ascii.cpp
        src/data/encoding/base58.cpp
		src/data/encoding/base32.cpp
        src/data/tools/channel.cpp
        src/data/math/number/gmp/mpq.cpp
        src/data/math/number/gmp/Q.cpp
        src/data/crypto/digest.cpp
        src/data/crypto/ripemd160.cpp
        src/data/crypto/sha256.cpp
		src/data/crypto/digest.cpp
        src/data/crypto/sha512.cpp
        src/data/crypto/secp256k1.cpp
        src/data/stream.cpp)
		
target_include_directories(data PUBLIC include)

# Set C++ version
target_compile_features(data PUBLIC cxx_std_17)
set_target_properties(data PROPERTIES CXX_EXTENSIONS OFF)

