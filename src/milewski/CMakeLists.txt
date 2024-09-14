add_library(milewski INTERFACE)
target_include_directories(milewski INTERFACE 
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
$<INSTALL_INTERFACE:include/milewski>)
include(GNUInstallDirs)
install(DIRECTORY
milewski
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(TARGETS milewski
EXPORT milewskiTargets ARCHIVE 
)