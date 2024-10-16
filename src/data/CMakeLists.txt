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
  io/wait_for_enter.cpp
  tools/channel.cpp
  crypto/random.cpp
  crypto/secret_share.cpp
  tools/circular_queue.cpp
  tools/rate_limiter.cpp
  log/log.cpp
)

configure_file (include/data/version.hpp.in generated/data/version.hpp )
target_include_directories (data PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated/> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_include_directories (data PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
target_link_libraries (
  data
  PUBLIC
  Boost::boost
  Boost::chrono
  Boost::thread
  Boost::log_setup
  Boost::log
  OpenSSL::SSL
  OpenSSL::Crypto
  PkgConfig::Cryptopp
  nlohmann_json::nlohmann_json
  GMP::GMP
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

include (GNUInstallDirs)
install (DIRECTORY
  include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install (TARGETS data nlohmann_json argh rotella milewski gmpxx sv
EXPORT DataTargets ARCHIVE 
)

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/generated/data/version.hpp
DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/data)

install (EXPORT DataTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/data
  FILE dataTargets.cmake
  NAMESPACE Data::
)

include (CMakePackageConfigHelpers)
# generate the config file that includes the exports
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/dataConfig.cmake"
  INSTALL_DESTINATION "lib/cmake/dataFunctions"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
  )

  write_basic_package_version_file(
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
