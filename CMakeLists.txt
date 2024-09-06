cmake_minimum_required (VERSION 3.16)

# Back compatibility for VERSION range
if (${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy (VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif ()
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake" CACHE STRING "Modules for CMake" FORCE)

project (
  data
  VERSION 0.0.24
  DESCRIPTION "wrappers and high-level programming constructs in c++"
  LANGUAGES CXX
)

find_package (Boost COMPONENTS thread chrono log_setup log REQUIRED)
find_package (OpenSSL REQUIRED Crypto SSL)
find_package (GMP REQUIRED)
find_package (SECP256K1 REQUIRED)
find_package (pegtl REQUIRED)

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(argh URL https://github.com/adishavit/argh/archive/refs/tags/v1.3.2.zip)
FetchContent_MakeAvailable(argh)



find_package(PkgConfig REQUIRED)
pkg_check_modules(Cryptopp REQUIRED IMPORTED_TARGET libcrypto++)

# Check if GTests is installed. If not, install it

option (PACKAGE_TESTS "Build the tests" ON)

add_definitions ("-DHAS_BOOST")

if (PACKAGE_TESTS)
  include (CTest)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
  enable_testing ()
  add_subdirectory (test)

endif ()

# Add Library source files here

add_library (data STATIC
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
  src/data/string.cpp
  src/data/io/wait_for_enter.cpp

  src/data/math/number/gmp/mpq.cpp
  src/data/math/number/gmp/N.cpp
  src/data/math/number/gmp/aks.cpp
  src/data/math/number/gmp/sqrt.cpp

  src/data/encoding/hex.cpp
  src/data/encoding/utf8.cpp
  src/data/encoding/base64.cpp
  src/data/encoding/base58.cpp
  src/data/encoding/integer.cpp

  src/data/net/URL.cpp
  src/data/net/beast/beast.cpp
  src/data/net/HTTP.cpp
  src/data/net/REST.cpp
  src/data/net/JSON.cpp
  src/data/net/TCP.cpp
  src/data/net/websocket.cpp
  src/data/net/HTTP_client.cpp
  src/data/net/email.cpp

  #src/data/io/run.cpp

  src/data/tools/channel.cpp
  src/data/crypto/random.cpp
  src/data/crypto/secret_share.cpp
  src/data/crypto/AES.cpp
  src/data/tools/circular_queue.cpp
  src/data/tools/rate_limiter.cpp
  src/data/log/log.cpp
)

target_include_directories (data PUBLIC include)

target_link_libraries (
  data
  
  Boost::boost
  Boost::chrono
  Boost::thread
  Boost::log_setup
  Boost::log
  OpenSSL::SSL
  OpenSSL::Crypto
  PkgConfig::Cryptopp
  nlohmann_json::nlohmann_json
  gmp
  secp256k1
  taocpp::pegtl
  argh
  # PkgConfig::LIBSECP256K1
)

get_target_property (OUT data LINK_LIBRARIES)
message (STATUS ${OUT})

# Set C++ version
target_compile_features (data PUBLIC cxx_std_23)
set_target_properties (data PROPERTIES CXX_EXTENSIONS ON)
target_compile_options (data PUBLIC "-fconcepts")

install (DIRECTORY include/ DESTINATION include)
install (TARGETS data)
