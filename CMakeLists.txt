cmake_minimum_required (VERSION 3.16)

# Back compatibility for VERSION range
if (${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy (VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif ()

if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds are not allowed")
endif ()

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake" CACHE STRING "Modules for CMake" FORCE)

project (
  data
  VERSION 0.0.29
  DESCRIPTION "wrappers and high-level programming constructs in c++"
  LANGUAGES CXX
)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

find_package (Boost 1.86 COMPONENTS thread chrono log_setup log REQUIRED)
# if we do not have boost
#   * linux:
#     * go to the boost website, download and extract the latest version.
#     * `./bootstrap.sh`
#     * `./b2`
#     * `sudo ./b2 install`

add_definitions ("-DHAS_BOOST")

find_package (OpenSSL REQUIRED Crypto SSL)
# if we do not have openssl
#   * on linux, `sudo apt-get install libssl-dev`

find_package (GMP REQUIRED)
# if we do not have GMP
#   * on linux, `sudo apt-get install libgmp3-dev`

find_package (SECP256K1 REQUIRED)
# if we do not have SECP256K1
#   * on linux
#     * go to `https://github.com/bitcoin-core/secp256k1`, download the repo, and follow the instructions.
#     * you mighht need autoconf: `sudo apt install autoconf`
#     * and libtool: `sudo apt-get install libtool-bin`

find_package (pegtl REQUIRED)
# If we do not have PegTL
#   * on linux, `sudo apt-get install tao-pegtl-dev`

IF (WIN32)
  pkg_check_modules (cryptopp REQUIRED IMPORTED_TARGET cryptopp)
ELSE ()
  pkg_check_modules (cryptopp REQUIRED IMPORTED_TARGET libcrypto++)
  # If we do not have crypto++, go to `https://github.com/weidai11/cryptopp` and follow the instructions.
ENDIF ()

find_package (PkgConfig REQUIRED)
# If we do not have PkgConfig
#   * on linux, `sudo apt-get install pkg-config`

include (FetchContent)
FetchContent_Declare (json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable (json)

FetchContent_Declare (argh URL https://github.com/adishavit/argh/archive/refs/tags/v1.3.2.zip)
FetchContent_MakeAvailable (argh)

FetchContent_Declare (ctre URL https://github.com/hanickadot/compile-time-regular-expressions/archive/refs/tags/v3.9.0.zip)
FetchContent_MakeAvailable (ctre)

# You can configure this for your need, presumbably you want speificy a git tag here instead of a branch
FetchContent_Declare(sqliteOrm
  GIT_REPOSITORY https://github.com/fnc12/sqlite_orm
  GIT_TAG        origin/dev
)

FetchContent_MakeAvailable(sqliteOrm)

# Check if GTests is installed. If not, install it

option (PACKAGE_TESTS "Build the tests" ON)
include (CTest)

if (PACKAGE_TESTS)
  include (CTest)
  FetchContent_Declare (
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

if(CMAKE_C_BYTE_ORDER STREQUAL "BIG_ENDIAN")
  add_compile_definitions(WORDS_BIGENDIAN)
else()
  add_compile_definitions(WORDS_LITTLEENDIAN)
endif ()

include (CheckSymbolExists)

CHECK_INCLUDE_FILE ("endian.h" HAVE_ENDIAN_H)
CHECK_INCLUDE_FILE ("sys/endian.h" HAVE_SYS_ENDIAN_H)

set (SYMBOLS_TO_CHECK
    "htole16-endian.h"
    "be16toh-endian.h"
    "le16toh-endian.h"
    "htobe32-endian.h"
    "htole32-endian.h"
    "be32toh-endian.h"
    "le32toh-endian.h"
    "htobe64-endian.h"
    "htole64-endian.h"
    "be64toh-endian.h"
    "le64toh-endian.h"
    "htobe16-endian.h"
    "bswap_16-byteswap.h"
)

list (LENGTH SYMBOLS_TO_CHECK L_LENGTH)
message(${L_LENGTH})

CHECK_INCLUDE_FILE("endian.h" HAVE_ENDIAN_H)
CHECK_INCLUDE_FILE("sys/endian.h" HAVE_SYS_ENDIAN_H)
foreach(SYMBOL_HEADER_PATH ${SYMBOLS_TO_CHECK})
  string(REPLACE "-" ";" SYMBOL_HEADER_PAIR ${SYMBOL_HEADER_PATH})
  message(${SYMBOL_HEADER_PAIR})
  list(GET SYMBOL_HEADER_PAIR 0 SYMBOL)
  list(GET SYMBOL_HEADER_PAIR 1 HEADER)
  message("Checking ${SYMBOL} from ${HEADER}")
  set(SYMBOL_EXISTS_RESULT_VAR "SYMBOL_EXISTS_${SYMBOL}")
  CHECK_SYMBOL_EXISTS(${SYMBOL} "${HEADER}" ${SYMBOL_EXISTS_RESULT_VAR})

  if(${SYMBOL_EXISTS_RESULT_VAR})
    string(TOUPPER ${SYMBOL} SYMBOL_UPPERCASE)
    add_compile_definitions(HAVE_DECL_${SYMBOL_UPPERCASE})
  endif()
endforeach()

if(HAVE_ENDIAN_H)
  add_compile_definitions(HAVE_ENDIAN_H)
endif()
if(HAVE_SYS_ENDIAN_H)
  add_compile_definitions(HAVE_SYS_ENDIAN_H)
endif()

add_subdirectory (src bin)
