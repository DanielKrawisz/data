# - Try to find the SECP256k1 libraries
# This module defines:
#  SECP256K1_FOUND             - system has SECP256K1 lib
#  SECP256K1_INCLUDE_DIR       - the SECP256K1 include directory
#  SECP256K1_LIBRARIES_DIR     - directory where the SECP256K1 libraries are located
#  SECP256K1_LIBRARIES         - Link these to use SECP256K1

# TODO: support MacOSX

include(FindPackageHandleStandardArgs)

if(SECP256K1_INCLUDE_DIR)
  set(SECP256K1_in_cache TRUE)
else()
  set(SECP256K1_in_cache FALSE)
endif()
if(NOT SECP256K1_LIBRARIES)
  set(SECP256K1_in_cache FALSE)
endif()

# Is it already configured?
if( NOT SECP256K1_in_cache )

  find_path(SECP256K1_INCLUDE_DIR
            NAMES secp256k1.h
            HINTS ENV SECP256K1_INC_DIR
                  ENV SECP256K1_DIR
                  $ENV{SECP256K1_DIR}/include
            PATHS
              /usr/include
              /usr/local/include
              /opt/local/include
              $ENV{ProgramFiles}/libsecp256k1/include
              $ENV{ProgramFiles\(x86\)}/libsecp256k1/include
              "C:/Program Files/libsecp256k1/include"
              "C:/Program Files (x86)/libsecp256k1/include"
            PATH_SUFFIXES include
  	        DOC "The directory containing the SECP256K1 header files"
           )

  find_library(SECP256K1_LIBRARY_RELEASE NAMES secp256k1
    HINTS ENV SECP256K1_LIB_DIR
          ENV SECP256K1_DIR
          $ENV{SECP256K1_DIR}/lib
    PATHS
            /usr/lib
            /usr/local/lib
            /opt/local/lib
            $ENV{ProgramFiles}/libsecp256k1/lib
            $ENV{ProgramFiles\(x86\)}/libsecp256k1/lib
            "C:/Program Files/libsecp256k1/lib"
            "C:/Program Files (x86)/libsecp256k1/lib"
    PATH_SUFFIXES lib
    DOC "Path to the Release SECP256K1 library"
    )

set(SECP256K1_LIBRARIES ${SECP256K1_LIBRARY_RELEASE})

if(WIN32)
        find_file(SECP256K1_DLL
            NAMES libsecp256k1-5.dll
            HINTS
                ENV SECP256K1_BIN_DIR
                ENV SECP256K1_DIR
                ${SECP256K1_DIR}/bin
            PATHS
                $ENV{ProgramFiles}/libsecp256k1/bin
                $ENV{ProgramFiles\(x86\)}/libsecp256k1/bin
                "C:/Program Files/libsecp256k1/bin"
                "C:/Program Files (x86)/libsecp256k1/bin"
            PATH_SUFFIXES bin
            DOC "Path to the SECP256K1 DLL (Windows only)"
        )
    endif()

    set(SECP256K1_DLLS ${SECP256K1_DLL})

  # Attempt to load a user-defined configuration for SECP256K1 if couldn't be found
  if ( NOT SECP256K1_INCLUDE_DIR OR NOT SECP256K1_LIBRARIES)
    include( SECP256K1Config OPTIONAL )
  endif()

endif()

find_package_handle_standard_args(SECP256K1 "DEFAULT_MSG" SECP256K1_LIBRARIES SECP256K1_INCLUDE_DIR)
mark_as_advanced(SECP256K1_INCLUDE_DIR SECP256K1_LIBRARIES SECP256K1_DLLS)
