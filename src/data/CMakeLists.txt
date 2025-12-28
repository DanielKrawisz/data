# everything except crypto and net.
add_library (data INTERFACE)

# functional data structures, arrays
add_library (core INTERFACE)

# strings, bits etc
add_library (string STATIC)

add_library (io STATIC)

add_library (hash STATIC)

# numbers, like it says
add_library (numbers STATIC)

# networking
add_library (net STATIC)

add_library (crypto STATIC)

target_include_directories (
  core

  INTERFACE

  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated/>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_sources (
  string

  PRIVATE

  random.cpp

  encoding/hex.cpp
  encoding/utf8.cpp
  string.cpp
  encoding/base64.cpp
)

target_compile_features (string PUBLIC cxx_std_23)
set_target_properties (string PROPERTIES CXX_EXTENSIONS ON)

target_link_libraries (
  string

  PUBLIC

  Boost::boost
  ctre
  core
)

target_sources (
  io

  PRIVATE

  #io/run.cpp
  io/wait_for_enter.cpp
  io/log.cpp
)

target_compile_features (io PUBLIC cxx_std_23)
set_target_properties (io PROPERTIES CXX_EXTENSIONS ON)

target_link_libraries (
  io

  PUBLIC

  core
  Boost::log_setup
  Boost::log
  argh
)

target_sources (
  hash

  PRIVATE

  ../sv/utiltime.cpp
  ../sv/fs.cpp
  ../sv/crypto/chacha20.cpp
  ../sv/crypto/hmac_sha512.cpp
  ../sv/crypto/ripemd160.cpp
  ../sv/crypto/sha1.cpp
  ../sv/crypto/sha256.cpp
  ../sv/crypto/sha512.cpp
  ../sv/support/cleanse.cpp
)

target_compile_features (hash PUBLIC cxx_std_23)
set_target_properties (hash PROPERTIES CXX_EXTENSIONS ON)

target_link_libraries (
  hash

  PUBLIC

  string
  OpenSSL::SSL
  OpenSSL::Crypto
  PkgConfig::cryptopp

)

target_sources (
  numbers

  PRIVATE

  math/number/gmp/mpq.cpp
  math/number/gmp/N.cpp
  math/number/gmp/aks.cpp
  math/number/gmp/sqrt.cpp
  encoding/base58.cpp
  encoding/integer.cpp

)

target_compile_features (numbers PUBLIC cxx_std_23)
set_target_properties (numbers PROPERTIES CXX_EXTENSIONS ON)

target_link_libraries (
  numbers

  PUBLIC

  string
  Boost::boost
  GMP::GMP
  rotella
  gmpxx
  ctre
)

target_sources (
  net

  PRIVATE

  # this is first because it takes a really long time.
  net/email.cpp

  net/URL.cpp

  async.cpp
  net/beast/http.cpp
  net/HTTP.cpp
  net/REST.cpp
  net/JSON.cpp
  net/TCP.cpp
  #net/websocket.cpp
  net/HTTP_client.cpp
  net/HTTP_server.cpp
  
  tools/rate_limiter.cpp
)

target_compile_features (net PUBLIC cxx_std_23)
set_target_properties (net PROPERTIES CXX_EXTENSIONS ON)

target_link_libraries (
  net

  PUBLIC

  string
  Boost::chrono
  Boost::thread
  nlohmann_json::nlohmann_json
  taocpp::pegtl
  OpenSSL::SSL
  OpenSSL::Crypto
)

target_sources (
  crypto

  PRIVATE

  crypto/secret_share.cpp
  crypto/block.cpp
)

target_compile_features (crypto PUBLIC cxx_std_23)
set_target_properties (crypto PROPERTIES CXX_EXTENSIONS ON)

target_link_libraries (
  crypto

  PUBLIC

  hash
)

target_link_libraries (
  data

  INTERFACE

  numbers
  io

)

configure_file (include/data/version.hpp.in generated/data/version.hpp)

get_target_property (OUT data LINK_LIBRARIES)

include (GNUInstallDirs)

# copy include directory into install destination
install (
  DIRECTORY include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install (
  TARGETS data crypto net numbers hash io string core nlohmann_json argh rotella
  EXPORT DataTargets
  ARCHIVE
)

install (
  FILES ${CMAKE_CURRENT_BINARY_DIR}/generated/data/version.hpp
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/data
)

install (
  EXPORT DataTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/data
  FILE dataTargets.cmake
  NAMESPACE Data::
)

include (CMakePackageConfigHelpers)

# generate the config file that includes the exports
configure_package_config_file (${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/dataConfig.cmake"
  INSTALL_DESTINATION "lib/cmake/dataFunctions"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file (
  "${CMAKE_CURRENT_BINARY_DIR}/dataConfigVersion.cmake"
  COMPATIBILITY AnyNewerVersion
)

install (FILES
  ${CMAKE_CURRENT_BINARY_DIR}/dataConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/dataConfigVersion.cmake
  DESTINATION lib/cmake/data
)

set (CPACK_PACKAGE_VENDOR "Daniel Krawisz")
set (CPACK_PACKAGE_CONTACT "email@example.com")
set (CPACK_PACKAGE_DESCRIPTION "data")
include (CPack)
