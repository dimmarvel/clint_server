cmake_minimum_required(VERSION 3.26)

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.24.2.tar.gz"
    SHA1 "17fd3b9f0b8c271eac9b71e30a6aa1b9fecab299"
)

project(cli_srv)

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

hunter_add_package(Boost COMPONENTS system thread context)
hunter_add_package(spdlog)

find_package(Boost          CONFIG REQUIRED system thread context)
find_package(spdlog         CONFIG REQUIRED)

include_directories("src")

add_subdirectory(src)
