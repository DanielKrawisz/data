# Locate Secp256k1
# This module defines
# SCEP256K1_LIBRARY
# SCEP256K1_FOUND, if false, do not try to link to
# SCEP256K1_INCLUDE_DIR, where to find the headers

FIND_PATH(SECP256K1_INCLUDE_DIR secp256k1.h
        HINTS
        $ENV{SECP256K1DIR}
        PATH_SUFFIXES secp256k1 include/secp256k1 include
        PATHS
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local
        /usr
        /sw # Fink
        /opt/local # DarwinPorts
        /opt/csw # Blastwave
        /opt
        )

FIND_LIBRARY(SECP256K1_LIBRARY
        NAMES secp256k1
        HINTS
        $ENV{SECP256K1DIR}
        PATH_SUFFIXES lib64 lib libs64 libs libs/Win32 libs/Win64
        PATHS
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local
        /usr
        /sw
        /opt/local
        /opt/csw
        /opt
        )


# handle the QUIETLY and REQUIRED arguments and set NTL_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SECP256K1 DEFAULT_MSG  SECP256K1_LIBRARY SECP256K1_INCLUDE_DIR)

MARK_AS_ADVANCED(SECP256K1_LIBRARY SECP256K1_INCLUDE_DIR)
