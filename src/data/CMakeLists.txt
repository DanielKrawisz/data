

# everything except crypto and net.
add_library (data INTERFACE)
add_library (Data::data ALIAS data)

# functional data structures, arrays
add_library (core INTERFACE)
add_library (Data::core ALIAS core)

# strings, bits etc
add_library (string STATIC)
add_library (Data::string ALIAS string)

# hash functions, including cryptographic
add_library (hash STATIC)
add_library (Data::hash ALIAS hash)

# numbers, like it says
add_library (numbers STATIC)
add_library (Data::numbers ALIAS numbers)

add_library (crypto STATIC)
add_library (Data::crypto ALIAS crypto)

set_target_properties (string PROPERTIES
  OUTPUT_NAME "data_string"
)

set_target_properties (numbers PROPERTIES
  OUTPUT_NAME "data_numbers"
)

set_target_properties (hash PROPERTIES
  OUTPUT_NAME "data_hash"
)

set_target_properties (crypto PROPERTIES
  OUTPUT_NAME "data_crypto"
)

target_include_directories (
  core

  INTERFACE

  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated/>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_sources (
  string

  PRIVATE

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

  Data::core
  Boost::boost
  ctre
  Boost::log_setup
  Boost::log
)

set_property(TARGET string PROPERTY CXX_MODULE_STD ON)
get_target_property(STRING_MODULE_STD string CXX_MODULE_STD)
message(STATUS "string CXX_MODULE_STD = ${STRING_MODULE_STD}")

target_sources (
  numbers

  PRIVATE

  math/number/gmp/N.cpp
  math/number/gmp/mpq.cpp
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

  Data::string
  Boost::boost
  GMP::GMP
  rotella
  gmpxx
  ctre
)

target_link_libraries (
  data

  INTERFACE

  Data::numbers

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

  Data::string
  OpenSSL::SSL
  OpenSSL::Crypto
  PkgConfig::cryptopp

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

  Data::hash
  Data::data
)

configure_file (
    ${PROJECT_SOURCE_DIR}/include/data/version.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/generated/data/version.hpp
)

get_target_property (OUT Data::data LINK_LIBRARIES)

include (GNUInstallDirs)

# copy include directory into install destination
install (
  DIRECTORY ${PROJECT_SOURCE_DIR}/include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install (
  TARGETS data crypto numbers hash string core rotella
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
set (CPACK_PACKAGE_DESCRIPTION "WhatsOnChain API")
include (CPack)

include(GNUInstallDirs)
