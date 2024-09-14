add_library(gmpxx INTERFACE)
target_include_directories(gmpxx INTERFACE 
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
$<INSTALL_INTERFACE:include>
)
target_link_libraries(gmpxx INTERFACE GMP::GMP)

include(GNUInstallDirs)
install(DIRECTORY
gmp
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(TARGETS gmpxx
EXPORT gmpxxTargets ARCHIVE 
)