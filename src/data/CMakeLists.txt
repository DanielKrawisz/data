# everything
add_library (data STATIC)

# functional data structures
add_library (core INTERFACE)

# arrays, strings, bits etc
add_library (arr STATIC)

# this is not ready yet because we would like
# to remove the dependency on GMP.
# add_library (hash STATIC)

# numbers, like it says
add_library (numbers STATIC)

# networking
add_library (net STATIC)

target_include_directories (
  core

  INTERFACE

  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated/>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# arr stands for array.
target_sources (
  arr

  PRIVATE

  random.cpp

  encoding/hex.cpp
  encoding/utf8.cpp
  string.cpp
  encoding/base64.cpp
)

target_compile_features (arr PUBLIC cxx_std_23)
set_target_properties (arr PROPERTIES CXX_EXTENSIONS ON)

target_link_libraries (
  arr

  PUBLIC

  Boost::boost
  ctre
  core
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

  arr
  Boost::boost
  GMP::GMP
  rotella
  gmpxx
  ctre
)

target_sources (
  net

  PRIVATE

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
  net/email.cpp
  
  tools/rate_limiter.cpp
)

target_compile_features (net PUBLIC cxx_std_23)
set_target_properties (net PROPERTIES CXX_EXTENSIONS ON)

target_link_libraries (
  net
  PUBLIC
  arr
  Boost::boost
  nlohmann_json::nlohmann_json
  taocpp::pegtl
)

target_sources (
  data

  PRIVATE

  crypto/secret_share.cpp
  crypto/block.cpp

  #io/run.cpp
  io/wait_for_enter.cpp
  io/log.cpp
)

configure_file (include/data/version.hpp.in generated/data/version.hpp)

target_link_libraries (
  data

  PUBLIC

  net
  numbers
  arr

  Boost::boost
  Boost::chrono
  Boost::thread
  Boost::log_setup
  Boost::log
  OpenSSL::SSL
  OpenSSL::Crypto
  PkgConfig::cryptopp
  nlohmann_json::nlohmann_json
  GMP::GMP
  taocpp::pegtl
  argh
  rotella
  gmpxx
  sv
  ctre
)

get_target_property (OUT data LINK_LIBRARIES)

# Set C++ version

target_compile_features (data PUBLIC cxx_std_23)
set_target_properties (data PROPERTIES CXX_EXTENSIONS ON)

include (GNUInstallDirs)

# copy include directory into install destination
install (
  DIRECTORY include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install (
  TARGETS data numbers net arr core nlohmann_json argh rotella milewski gmpxx sv
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
