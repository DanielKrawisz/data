add_library (rotella OBJECT
  aks.cpp
  sieve.cpp
  akslib.cpp)

target_include_directories (rotella PUBLIC 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/rotella>)

target_link_libraries (
  rotella
  PUBLIC
  GMP::GMP)

include (GNUInstallDirs)

install (DIRECTORY
  include/rotella
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install (TARGETS rotella EXPORT rotellaTargets ARCHIVE)
