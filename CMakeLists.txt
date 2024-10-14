cmake_minimum_required (VERSION 3.16)

# Back compatibility for VERSION range
if (${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy (VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif ()
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds are not allowed")
endif ()
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake" CACHE STRING "Modules for CMake" FORCE)

project (
  data
  VERSION 0.0.27
  DESCRIPTION "wrappers and high-level programming constructs in c++"
  LANGUAGES CXX
)

set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

find_package (Boost COMPONENTS thread chrono log_setup log REQUIRED)
find_package (OpenSSL REQUIRED Crypto SSL)
find_package (GMP REQUIRED)
find_package (SECP256K1 REQUIRED)
find_package (pegtl REQUIRED)

include (FetchContent)
FetchContent_Declare (json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable (json)

FetchContent_Declare (argh URL https://github.com/adishavit/argh/archive/refs/tags/v1.3.2.zip)
FetchContent_MakeAvailable (argh)

FetchContent_Declare (ctre URL https://github.com/hanickadot/compile-time-regular-expressions/archive/refs/tags/v3.9.0.zip)
FetchContent_MakeAvailable (ctre)


find_package (PkgConfig REQUIRED)
pkg_check_modules (Cryptopp REQUIRED IMPORTED_TARGET libcrypto++)

# Check if GTests is installed. If not, install it

option (PACKAGE_TESTS "Build the tests" ON)

add_definitions ("-DHAS_BOOST")

if (PACKAGE_TESTS)
  include (CTest)
  FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)

set (gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable (googletest)
  enable_testing ()
  add_subdirectory (test)

endif ()

# Add Library source files here

add_subdirectory (src bin)
