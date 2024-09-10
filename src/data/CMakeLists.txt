add_library (data STATIC

  types.cpp
  string.cpp

  math/number/gmp/mpq.cpp
  math/number/gmp/N.cpp
  math/number/gmp/aks.cpp
  math/number/gmp/sqrt.cpp

  encoding/hex.cpp
  encoding/utf8.cpp
  encoding/base64.cpp
  encoding/base58.cpp
  encoding/integer.cpp

  net/URL.cpp
  net/beast/beast.cpp
  net/HTTP.cpp
  net/REST.cpp
  net/JSON.cpp
  net/TCP.cpp
  net/websocket.cpp
  net/HTTP_client.cpp
  net/email.cpp

  #io/run.cpp

  tools/channel.cpp
  crypto/random.cpp
  crypto/secret_share.cpp
  crypto/AES.cpp
  tools/circular_queue.cpp
  tools/rate_limiter.cpp
  log/log.cpp
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
  rotella
  milewski
  gmpxx
  sv
  ctre
  # PkgConfig::LIBSECP256K1 
)

get_target_property (OUT data LINK_LIBRARIES)

# Set C++ version

target_compile_features (data PUBLIC cxx_std_23)
set_target_properties (data PROPERTIES CXX_EXTENSIONS ON)
target_compile_options (data PUBLIC "-fconcepts")

install(TARGETS data EXPORT dataTargets
ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

# install (EXPORT dataTargets
#         NAMESPACE data::
#         DESTINATION lib/cmake/data
# )