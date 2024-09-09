add_library(rotella OBJECT
aks.cpp
sieve.cpp
akslib.cpp
)

target_include_directories(rotella PUBLIC include)