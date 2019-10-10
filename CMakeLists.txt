cmake_minimum_required(VERSION 3.10)

project(Riichi_mahjong)

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /utf-8")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
endif()

file(GLOB_RECURSE SOURCE ${CMAKE_SOURCE_DIR}/src/*.*)

add_executable(mahjong ${SOURCE})
target_include_directories(mahjong PRIVATE src)