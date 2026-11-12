cmake_minimum_required (VERSION 3.16)

if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message (FATAL_ERROR "In-source builds are not allowed")
endif ()

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake" CACHE STRING "Modules for CMake" FORCE)

# ---------------------------------------------------------
# Enable modules
# ---------------------------------------------------------

set (CMAKE_EXPERIMENTAL_CXX_IMPORT_STD
    "d0edc3af-4c50-42ea-a356-e2862fe7a444"
)

# ---------------------------------------------------------
#  Project info
# ---------------------------------------------------------

project (
  data
  VERSION 0.0.36
  DESCRIPTION "functinal data structures, mathematics, cryptography, networking, wrappers, and other high-level programming constructs in c++"
  LANGUAGES CXX
)

set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# We tried earlier to upgrade to gcc 15 and 16 to use modules
# But that didn't work. These versions of gcc were too unstable.
# Uncomment the next 3 lines to re-enable modules.

#set (CMAKE_CXX_MODULE_STD ON)
#set (CMAKE_CXX_SCAN_FOR_MODULES ON)
#set (CMAKE_CXX_MODULE_STD ON)

# ---------------------------------------------------------
#  Build type setup (default to Debug for single-config)
# ---------------------------------------------------------
if (NOT CMAKE_CONFIGURATION_TYPES)   # True for Make/Ninja
    if (NOT CMAKE_BUILD_TYPE)
        set (CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the build type." FORCE)
    endif ()
endif ()

# List of supported build types
set (_supported_build_types
    "Debug"
    "Release"
    "RelWithDebInfo"
    "MinSizeRel"
)

set (_build_type_descriptions
    "Debug          - For development. Enables debug symbols, sanitizers, and minimal optimization."
    "Release        - For distribution. Full optimization, no debug symbols, no assertions."
    "RelWithDebInfo - Optimized but retains debug info. Good for profiling and crash investigation."
    "MinSizeRel     - Focuses on shrinking the binary. Useful for constrained environments."
)

# ---------------------------------------------------------
#  User message explaining usage
# ---------------------------------------------------------
message (STATUS "")
message (STATUS "Build type selected: ${CMAKE_BUILD_TYPE}")
message (STATUS "To choose a different build type, configure with:")
message (STATUS "    cmake -DCMAKE_BUILD_TYPE=<type> ..")
message (STATUS "Available build types:")
foreach (line IN LISTS _build_type_descriptions)
    message (STATUS "  • ${line}")
endforeach ()
message (STATUS "")

# ---------------------------------------------------------
#  Compiler flags
# ---------------------------------------------------------

# Base flags for each type (C and C++)
set (CMAKE_C_FLAGS_RELEASE        "-O3 -DNDEBUG")
set (CMAKE_CXX_FLAGS_RELEASE      "-O3 -DNDEBUG")

set (CMAKE_C_FLAGS_RELWITHDEBINFO   "-O2 -g -DNDEBUG")
set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG")

set (CMAKE_C_FLAGS_DEBUG        "-O1 -g3 -fno-omit-frame-pointer")
set (CMAKE_CXX_FLAGS_DEBUG      "-O1 -g3 -fno-omit-frame-pointer")

# ---------------------------------------------------------
#  Sanitizers (enabled only in Debug)
# ---------------------------------------------------------
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message (STATUS "Enabling sanitizers for Debug build.")

    set (_san_flags "-fsanitize=address,undefined")

    add_compile_options (${_san_flags})
    add_link_options (${_san_flags})
endif ()

# ---------------------------------------------------------
# User option: build tests
# ---------------------------------------------------------

option (PACKAGE_TESTS "Build the unit tests for this project" ON)

if (PACKAGE_TESTS)
    message (STATUS "Tests: enabled")
else ()
    message (STATUS "Tests: disabled")
endif ()

message (STATUS "To change this setting, reconfigure with:")
message (STATUS "    cmake -DPACKAGE_TESTS=ON  ..   # build tests")
message (STATUS "    cmake -DPACKAGE_TESTS=OFF ..   # skip building tests")
message (STATUS "")

# ---------------------------------------------------------
# Concepts diagnostics depth (GCC/Clang only)
# ---------------------------------------------------------

include (CheckCXXCompilerFlag)

check_cxx_compiler_flag ("-fconcepts-diagnostics-depth=20" HAS_CONCEPTS_DEPTH)

if (HAS_CONCEPTS_DEPTH)
    add_compile_options ("-fconcepts-diagnostics-depth=20")
endif ()
# ---------------------------------------------------------
# Dependencies
# ---------------------------------------------------------

find_package (GMP REQUIRED)
# If we do not have GMP:
#   * on Linux, sudo apt-get install libgmp-dev

include (FetchContent)
cmake_policy (SET CMP0135 NEW)

FetchContent_Declare (ctre
    URL https://github.com/hanickadot/compile-time-regular-expressions/archive/refs/tags/v3.10.0.zip
)

FetchContent_MakeAvailable (ctre)

set (NTL_PREFIX "/opt/ntl" CACHE PATH
    "Installation prefix for NTL and GF2X")

find_path (NTL_INCLUDE_DIR
    NAMES NTL/ZZ.h
    PATHS "${NTL_PREFIX}/include"
    NO_DEFAULT_PATH
    REQUIRED
)

find_library (NTL_LIBRARY
    NAMES ntl
    PATHS "${NTL_PREFIX}/lib" "${NTL_PREFIX}/lib64"
    NO_DEFAULT_PATH
    REQUIRED
)

find_library (GF2X_LIBRARY
    NAMES gf2x
    PATHS "${NTL_PREFIX}/lib" "${NTL_PREFIX}/lib64"
    NO_DEFAULT_PATH
    REQUIRED
)

add_library (NTL::NTL UNKNOWN IMPORTED)

set_target_properties (NTL::NTL PROPERTIES
    IMPORTED_LOCATION "${NTL_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${NTL_INCLUDE_DIR}"
    INTERFACE_LINK_LIBRARIES "${GF2X_LIBRARY}"
)

# ---------------------------------------------------------
# setup build
# ---------------------------------------------------------

add_subdirectory (src bin)

# ---------------------------------------------------------
# Tests
# ---------------------------------------------------------

if (PACKAGE_TESTS)
  include (CTest)

  set (gtest_force_shared_crt ON CACHE BOOL "" FORCE)

  FetchContent_Declare (
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
  )

  FetchContent_MakeAvailable (googletest)

  enable_testing ()
  add_subdirectory (test)
endif ()
