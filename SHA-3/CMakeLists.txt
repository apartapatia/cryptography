cmake_minimum_required(VERSION 3.28)
project(SHA_3)

set(CMAKE_CXX_STANDARD 17)

add_executable(SHA_3 main.cpp
        util/math_u.cpp
        util/math_u.h
        lib/arrays_l.cpp
        lib/arrays_l.h
        hash/sha3.cpp
        hash/sha3.h
        hash/sha.cpp
        hash/sha.h)
