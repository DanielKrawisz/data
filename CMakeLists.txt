cmake_minimum_required (VERSION 3.16)

# Back compatibility for VERSION range
if (${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy (VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif ()

project (
  data
  VERSION 0.0.24
  DESCRIPTION "wrappers and high-level programming constructs in c++"
  LANGUAGES CXX
)

find_package (Boost CONFIG REQUIRED)
find_package (OpenSSL CONFIG REQUIRED)
find_package (cryptopp CONFIG REQUIRED)
find_package (nlohmann_json CONFIG REQUIRED)
find_package (gmp CONFIG REQUIRED)
find_package (secp256k1 CONFIG REQUIRED)
find_package (pegtl CONFIG REQUIRED)
find_package (argh CONFIG REQUIRED)

# Check if GTests is installed. If not, install it

option (PACKAGE_TESTS "Build the tests" ON)

add_definitions ("-DHAS_BOOST")

if (PACKAGE_TESTS)
  include (CTest)
  find_package (GTest REQUIRED)
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
  src/data/tools.cpp
  src/data/encoding/hex.cpp
  src/data/encoding/ascii.cpp
  src/data/encoding/utf8.cpp
  src/data/encoding/base64.cpp
  src/data/encoding/base58.cpp
  src/data/encoding/integer.cpp
  src/data/net/URL.cpp
  src/data/net/HTTP.cpp
  src/data/net/REST.cpp
  src/data/net/JSON.cpp
  src/data/net/TCP.cpp
  src/data/net/websocket.cpp
  src/data/net/HTTP_client.cpp
  #src/data/io/run.cpp
  src/data/tools/channel.cpp
  src/data/crypto/random.cpp
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

target_include_directories (data PUBLIC include)

target_link_libraries (
  data
  boost::boost
  openssl::openssl
  cryptopp::cryptopp
  nlohmann_json::nlohmann_json
  gmp::gmp
  secp256k1::secp256k1
  taocpp::pegtl
  argh
  # PkgConfig::LIBSECP256K1
)

get_target_property (OUT data LINK_LIBRARIES)
message (STATUS ${OUT})

# Set C++ version
target_compile_features (data PUBLIC cxx_std_20)
set_target_properties (data PROPERTIES CXX_EXTENSIONS ON)
target_compile_options (data PUBLIC "-fconcepts")

install (DIRECTORY include/ DESTINATION include)
install (TARGETS data)
