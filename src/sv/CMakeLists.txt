add_library (
  sv

  STATIC

  utiltime.cpp
  fs.cpp
  crypto/chacha20.cpp
  crypto/hmac_sha512.cpp
  crypto/ripemd160.cpp
  crypto/sha1.cpp
  crypto/sha256.cpp
  crypto/sha512.cpp
  support/cleanse.cpp
)

target_include_directories (
  sv
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)


target_link_libraries (
  sv

  PUBLIC

  Boost::boost
  Boost::chrono
)

include (GNUInstallDirs)

install (DIRECTORY
  include/sv
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install (TARGETS sv
  EXPORT svTargets ARCHIVE 
)
