find_package(PkgConfig)
pkg_check_modules(PC_DATA QUIET Data)

find_path(DATA_INCLUDE_DIR
NAMES data.hpp
HINTS ENV DATA_INC_DIR
      ENV DATA_DIR
      $ENV{DATA_DIR}/include
PATH_SUFFIXES include/data
  DOC "The directory containing the Data header files"
)

find_library(DATA_LIBRARY NAMES data
HINTS ENV DATA_LIB_DIR
ENV DATA_DIR
$ENV{DATA_DIR}/lib
PATH_SUFFIXES lib/data
DOC "Path to the Data library"
)
message(STATUS "${DATA_INCLUDE_DIR}")
if(DATA_INCLUDE_DIR)
    file(READ "${DATA_INCLUDE_DIR}/data/version.hpp" ver)
    string(REGEX MATCH "#define DATA_VERSION \"([0-9*.]*)\"" _ ${ver})
    set(DATA_VERSION ${CMAKE_MATCH_1})
endif()
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Data
  FOUND_VAR DATA_FOUND
  REQUIRED_VARS
    DATA_LIBRARY
    DATA_INCLUDE_DIR
  VERSION_VAR DATA_VERSION
)

if(DATA_FOUND)
  set(DATA_LIBRARIES ${DATA_LIBRARY})
  set(DATA_INCLUDE_DIRS ${DATA_INCLUDE_DIR})
endif()

if(DATA_FOUND AND NOT TARGET Data::Data)
  add_library(Data::Data UNKNOWN IMPORTED)
  set_target_properties(Data::Data PROPERTIES
    IMPORTED_LOCATION "${DATA_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${DATA_INCLUDE_DIR}"
  )
endif()

mark_as_advanced(
  DATA_INCLUDE_DIR
  DATA_LIBRARY
)