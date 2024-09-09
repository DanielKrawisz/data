add_library(sv OBJECT
utiltime.cpp
fs.cpp
crypto/chacha20.cpp
crypto/hmac_sha512.cpp
crypto/ripemd160.cpp
crypto/sha1.cpp
crypto/sha256.cpp
crypto/sha512.cpp
support/cleanse.cpp
support/lockedpool.cpp
)

target_include_directories(sv PUBLIC include)